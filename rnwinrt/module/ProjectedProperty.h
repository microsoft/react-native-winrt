#pragma once

namespace WinRTTurboModule
{
    struct DECLSPEC_NOVTABLE IProjectedPropertyBase
    {
        virtual std::string_view Name() const noexcept = 0;
        virtual bool IsReadonly() const noexcept = 0;
    };

    template<typename I>
    struct DECLSPEC_NOVTABLE IProjectedProperty : IProjectedPropertyBase
    {
        virtual jsi::Value Get(I& instance, const std::shared_ptr<ProjectionsContext>& context) const = 0;
        virtual void Set(I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) const = 0;
    };

    template<typename I, typename G, typename GC>
    class ProjectedReadonlyProperty final : public IProjectedProperty<I>
    {
    public:
        ProjectedReadonlyProperty(const std::string_view& name, G get, GC convertToValue)
            : m_name(name), m_get(get), m_convertToValue(convertToValue)
        {
        }

        virtual std::string_view Name() const noexcept override
        {
            return m_name;
        }

        virtual jsi::Value Get(I& instance, const std::shared_ptr<ProjectionsContext>& context) const override
        {
            return m_convertToValue(context, (instance.*m_get)());
        }

        virtual void Set(I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) const override
        {
            FAIL_FAST();
        }

        virtual bool IsReadonly() const noexcept override
        {
            return true;
        }

    private:
        const std::string_view m_name;
        const G m_get;
        const GC m_convertToValue;
    };

    // TODO: There are weird cases like WindowsInternal.Shell.UnifiedTile.VisualCache.IStartVisualCacheEntryHeader/IStartVisualCacheEntryHeaderWriter where
    //       the property getters/setters are actually on different interfaces implemented by the same runtime class. While this is generally bad practice,
    //       if we did want to support it, we would need to have ProjectedRuntimeClassInstance property lookups for get/set check whether a matching name
    //       it encounters on an interface is capable of the intended operation, and proceed to check other interfaces until it finds a match. We would also
    //       need to amend the RN/WinRT to allow generation of write-only properties (rather crashing).

    template<typename I, typename G, typename GC, typename S, typename SC>
    class ProjectedWriteableProperty final : public IProjectedProperty<I>
    {
    public:
        ProjectedWriteableProperty(const std::string_view& name, G get, GC convertToValue, S set, SC convertFromValue)
            : m_name(name), m_get(get), m_convertToValue(convertToValue), m_set(set), m_convertFromValue(convertFromValue)
        {
        }

        virtual std::string_view Name() const noexcept override
        {
            return m_name;
        }

        virtual jsi::Value Get(I& instance, const std::shared_ptr<ProjectionsContext>& context) const override
        {
            return m_convertToValue(context, (instance.*m_get)());
        }

        virtual void Set(I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) const override
        {
            (instance.*m_set)(m_convertFromValue(context, value));
        }

        virtual bool IsReadonly() const noexcept override
        {
            return false;
        }

    private:
        const std::string_view m_name;
        const G m_get;
        const GC m_convertToValue;
        const S m_set;
        const SC m_convertFromValue;
    };

    struct ProjectedProperty final
    {
        // typename J is used because something like winrt::Windows::Foundation::IUriRuntimeClass::AbsoluteUri() is not convertible from
        // from winrt::impl::consume_Windows_Foundation_IUriRuntimeClass<winrt::Windows::Foundation::IUriRuntimeClass>::AbsoluteUri() as
        // returned by the compiler so J != I and is not convertable even though I <- J when there are overloads. ProjectFunction is similar.

        // "new" is used instead of std::make_shared<> to avoid specializing std::shared_ptr<> for the specialized class which reduces the size
        // of the object file signicantly for certain generated namespaces. Like this we only specialize it for IProjectedProperty and can
        // even skip IProjectedProperty<I> by static_cast instead of std::static_pointer_cast.

        template<typename I, typename T, typename J>
        static std::shared_ptr<IProjectedPropertyBase> Create(const std::string_view& name, T (J::* get)() const, NativeToValueConverter<T> convertToValue)
        {
            return std::shared_ptr<IProjectedPropertyBase>(new ProjectedReadonlyProperty<I, decltype(get), decltype(convertToValue)>(name, get, convertToValue));
        }

        // In odd cases like a Windows.Security.Cryptography.Certificates.ICertificateExtension.Value, get and set are supported but T != U
        // because of winrt::com_array != winrt::array_view and similarly for the converters ConvertComArrayToValue != ReadOnlyArrayWrapper.
        // Note that while GC=NativeToValueConverter<T> and SC=ValueToNativeConverter<U> the compiler complains about constness ambiguities so
        // templating the coverters is a workaround.

        template<typename I, typename T, typename J, typename U, typename GC, typename SC>
        static std::shared_ptr<IProjectedPropertyBase> Create(const std::string_view& name, T (J::* get)() const, GC convertToValue, void (J::* set)(U) const, SC convertFromValue)
        {
            return std::shared_ptr<IProjectedPropertyBase>(new ProjectedWriteableProperty<I, decltype(get), GC, decltype(set), SC>(name, get, convertToValue, set, convertFromValue));
        }
    };
}