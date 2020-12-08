#pragma once
#define NOMINMAX

#include <set>
#include <variant>

#include <inspectable.h>
#include <unknwn.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>

#include <TurboModule/Core/TurboModule.h> // TODO: Move react-specific code to separate files
#include <jsi/jsi.h>

#include <Windows.h>

#include <wil/cppwinrt.h>
#include <wil/resource.h>
#include <wil/win32_helpers.h>
#include <wil/winrt.h>

#define EXPAND_SEQUENCE1(x, y, d) x##1##y
#define EXPAND_SEQUENCE2(x, y, d) EXPAND_SEQUENCE1(x, y, d)##d x##2##y
#define EXPAND_SEQUENCE3(x, y, d) EXPAND_SEQUENCE2(x, y, d)##d x##3##y
#define EXPAND_SEQUENCE4(x, y, d) EXPAND_SEQUENCE3(x, y, d)##d x##4##y
#define EXPAND_SEQUENCE5(x, y, d) EXPAND_SEQUENCE4(x, y, d)##d x##5##y
#define EXPAND_SEQUENCE6(x, y, d) EXPAND_SEQUENCE5(x, y, d)##d x##6##y
#define EXPAND_SEQUENCE7(x, y, d) EXPAND_SEQUENCE6(x, y, d)##d x##7##y
#define EXPAND_SEQUENCE8(x, y, d) EXPAND_SEQUENCE7(x, y, d)##d x##8##y
#define EXPAND_SEQUENCE9(x, y, d) EXPAND_SEQUENCE8(x, y, d)##d x##9##y
#define EXPAND_SEQUENCE10(x, y, d) EXPAND_SEQUENCE9(x, y, d)##d x##10##y
#define EXPAND_SEQUENCE11(x, y, d) EXPAND_SEQUENCE10(x, y, d)##d x##11##y
#define EXPAND_SEQUENCE12(x, y, d) EXPAND_SEQUENCE11(x, y, d)##d x##12##y
#define EXPAND_SEQUENCE13(x, y, d) EXPAND_SEQUENCE12(x, y, d)##d x##13##y
#define EXPAND_SEQUENCE14(x, y, d) EXPAND_SEQUENCE13(x, y, d)##d x##14##y
#define EXPAND_SEQUENCE15(x, y, d) EXPAND_SEQUENCE14(x, y, d)##d x##15##y
#define EXPAND_SEQUENCE16(x, y, d) EXPAND_SEQUENCE15(x, y, d)##d x##16##y
#define EXPAND_SEQUENCE17(x, y, d) EXPAND_SEQUENCE16(x, y, d)##d x##17##y
#define EXPAND_SEQUENCE18(x, y, d) EXPAND_SEQUENCE17(x, y, d)##d x##18##y
#define EXPAND_SEQUENCE19(x, y, d) EXPAND_SEQUENCE18(x, y, d)##d x##19##y
#define EXPAND_SEQUENCE20(x, y, d) EXPAND_SEQUENCE19(x, y, d)##d x##20##y
#define EXPAND_SEQUENCE21(x, y, d) EXPAND_SEQUENCE20(x, y, d)##d x##21##y
#define EXPAND_SEQUENCE22(x, y, d) EXPAND_SEQUENCE21(x, y, d)##d x##22##y
#define EXPAND_SEQUENCE23(x, y, d) EXPAND_SEQUENCE22(x, y, d)##d x##23##y
#define EXPAND_SEQUENCE24(x, y, d) EXPAND_SEQUENCE23(x, y, d)##d x##24##y
#define EXPAND_SEQUENCE25(x, y, d) EXPAND_SEQUENCE24(x, y, d)##d x##25##y
#define EXPAND_SEQUENCE26(x, y, d) EXPAND_SEQUENCE25(x, y, d)##d x##26##y
#define EXPAND_SEQUENCE27(x, y, d) EXPAND_SEQUENCE26(x, y, d)##d x##27##y
#define EXPAND_SEQUENCE28(x, y, d) EXPAND_SEQUENCE27(x, y, d)##d x##28##y
#define EXPAND_SEQUENCE29(x, y, d) EXPAND_SEQUENCE27(x, y, d)##d x##29##y
#define EXPAND_SEQUENCE30(x, y, d) EXPAND_SEQUENCE29(x, y, d)##d x##30##y
#define EXPAND_SEQUENCE(x, y, i, d) EXPAND_SEQUENCE##i(x, y, d)

#define EXPAND_SEQUENCE_PAIR1(x, y, z, d) x##1##y##1##z
#define EXPAND_SEQUENCE_PAIR2(x, y, z, d) EXPAND_SEQUENCE_PAIR1(x, y, z, d)##d x##2##y##2##z
#define EXPAND_SEQUENCE_PAIR3(x, y, z, d) EXPAND_SEQUENCE_PAIR2(x, y, z, d)##d x##3##y##3##z
#define EXPAND_SEQUENCE_PAIR4(x, y, z, d) EXPAND_SEQUENCE_PAIR3(x, y, z, d)##d x##4##y##4##z
#define EXPAND_SEQUENCE_PAIR5(x, y, z, d) EXPAND_SEQUENCE_PAIR4(x, y, z, d)##d x##5##y##5##z
#define EXPAND_SEQUENCE_PAIR6(x, y, z, d) EXPAND_SEQUENCE_PAIR5(x, y, z, d)##d x##6##y##6##z
#define EXPAND_SEQUENCE_PAIR7(x, y, z, d) EXPAND_SEQUENCE_PAIR6(x, y, z, d)##d x##7##y##7##z
#define EXPAND_SEQUENCE_PAIR8(x, y, z, d) EXPAND_SEQUENCE_PAIR7(x, y, z, d)##d x##8##y##8##z
#define EXPAND_SEQUENCE_PAIR9(x, y, z, d) EXPAND_SEQUENCE_PAIR8(x, y, z, d)##d x##9##y##9##z
#define EXPAND_SEQUENCE_PAIR10(x, y, z, d) EXPAND_SEQUENCE_PAIR9(x, y, z, d)##d x##10##y##10##z
#define EXPAND_SEQUENCE_PAIR11(x, y, z, d) EXPAND_SEQUENCE_PAIR10(x, y, z, d)##d x##11##y##11##z
#define EXPAND_SEQUENCE_PAIR12(x, y, z, d) EXPAND_SEQUENCE_PAIR11(x, y, z, d)##d x##12##y##12##z
#define EXPAND_SEQUENCE_PAIR13(x, y, z, d) EXPAND_SEQUENCE_PAIR12(x, y, z, d)##d x##13##y##13##z
#define EXPAND_SEQUENCE_PAIR14(x, y, z, d) EXPAND_SEQUENCE_PAIR13(x, y, z, d)##d x##14##y##14##z
#define EXPAND_SEQUENCE_PAIR15(x, y, z, d) EXPAND_SEQUENCE_PAIR14(x, y, z, d)##d x##15##y##15##z
#define EXPAND_SEQUENCE_PAIR16(x, y, z, d) EXPAND_SEQUENCE_PAIR15(x, y, z, d)##d x##16##y##16##z
#define EXPAND_SEQUENCE_PAIR17(x, y, z, d) EXPAND_SEQUENCE_PAIR16(x, y, z, d)##d x##17##y##17##z
#define EXPAND_SEQUENCE_PAIR18(x, y, z, d) EXPAND_SEQUENCE_PAIR17(x, y, z, d)##d x##18##y##18##z
#define EXPAND_SEQUENCE_PAIR19(x, y, z, d) EXPAND_SEQUENCE_PAIR18(x, y, z, d)##d x##19##y##19##z
#define EXPAND_SEQUENCE_PAIR20(x, y, z, d) EXPAND_SEQUENCE_PAIR19(x, y, z, d)##d x##20##y##20##z
#define EXPAND_SEQUENCE_PAIR21(x, y, z, d) EXPAND_SEQUENCE_PAIR20(x, y, z, d)##d x##21##y##21##z
#define EXPAND_SEQUENCE_PAIR22(x, y, z, d) EXPAND_SEQUENCE_PAIR21(x, y, z, d)##d x##22##y##22##z
#define EXPAND_SEQUENCE_PAIR23(x, y, z, d) EXPAND_SEQUENCE_PAIR22(x, y, z, d)##d x##23##y##23##z
#define EXPAND_SEQUENCE_PAIR24(x, y, z, d) EXPAND_SEQUENCE_PAIR23(x, y, z, d)##d x##24##y##24##z
#define EXPAND_SEQUENCE_PAIR25(x, y, z, d) EXPAND_SEQUENCE_PAIR24(x, y, z, d)##d x##25##y##25##z
#define EXPAND_SEQUENCE_PAIR26(x, y, z, d) EXPAND_SEQUENCE_PAIR25(x, y, z, d)##d x##26##y##26##z
#define EXPAND_SEQUENCE_PAIR27(x, y, z, d) EXPAND_SEQUENCE_PAIR26(x, y, z, d)##d x##27##y##27##z
#define EXPAND_SEQUENCE_PAIR28(x, y, z, d) EXPAND_SEQUENCE_PAIR27(x, y, z, d)##d x##28##y##28##z
#define EXPAND_SEQUENCE_PAIR29(x, y, z, d) EXPAND_SEQUENCE_PAIR27(x, y, z, d)##d x##29##y##29##z
#define EXPAND_SEQUENCE_PAIR30(x, y, z, d) EXPAND_SEQUENCE_PAIR29(x, y, z, d)##d x##30##y##30##z
#define EXPAND_SEQUENCE_PAIR(x, y, z, i, d) EXPAND_SEQUENCE_PAIR##i(x, y, z, d)

#define EXPAND_DEFINITIONS1(x) x##(1)
#define EXPAND_DEFINITIONS2(x)                                                                                         \
    EXPAND_DEFINITIONS1(x)                                                                                             \
    x##(2)
#define EXPAND_DEFINITIONS3(x)                                                                                         \
    EXPAND_DEFINITIONS2(x)                                                                                             \
    x##(3)
#define EXPAND_DEFINITIONS4(x)                                                                                         \
    EXPAND_DEFINITIONS3(x)                                                                                             \
    x##(4)
#define EXPAND_DEFINITIONS5(x)                                                                                         \
    EXPAND_DEFINITIONS4(x)                                                                                             \
    x##(5)
#define EXPAND_DEFINITIONS6(x)                                                                                         \
    EXPAND_DEFINITIONS5(x)                                                                                             \
    x##(6)
#define EXPAND_DEFINITIONS7(x)                                                                                         \
    EXPAND_DEFINITIONS6(x)                                                                                             \
    x##(7)
#define EXPAND_DEFINITIONS8(x)                                                                                         \
    EXPAND_DEFINITIONS7(x)                                                                                             \
    x##(8)
#define EXPAND_DEFINITIONS9(x)                                                                                         \
    EXPAND_DEFINITIONS8(x)                                                                                             \
    x##(9)
#define EXPAND_DEFINITIONS10(x)                                                                                        \
    EXPAND_DEFINITIONS9(x)                                                                                             \
    x##(10)
#define EXPAND_DEFINITIONS11(x)                                                                                        \
    EXPAND_DEFINITIONS10(x)                                                                                            \
    x##(11)
#define EXPAND_DEFINITIONS12(x)                                                                                        \
    EXPAND_DEFINITIONS11(x)                                                                                            \
    x##(12)
#define EXPAND_DEFINITIONS13(x)                                                                                        \
    EXPAND_DEFINITIONS12(x)                                                                                            \
    x##(13)
#define EXPAND_DEFINITIONS14(x)                                                                                        \
    EXPAND_DEFINITIONS13(x)                                                                                            \
    x##(14)
#define EXPAND_DEFINITIONS15(x)                                                                                        \
    EXPAND_DEFINITIONS14(x)                                                                                            \
    x##(15)
#define EXPAND_DEFINITIONS16(x)                                                                                        \
    EXPAND_DEFINITIONS15(x)                                                                                            \
    x##(16)
#define EXPAND_DEFINITIONS17(x)                                                                                        \
    EXPAND_DEFINITIONS16(x)                                                                                            \
    x##(17)
#define EXPAND_DEFINITIONS18(x)                                                                                        \
    EXPAND_DEFINITIONS17(x)                                                                                            \
    x##(18)
#define EXPAND_DEFINITIONS19(x)                                                                                        \
    EXPAND_DEFINITIONS18(x)                                                                                            \
    x##(19)
#define EXPAND_DEFINITIONS20(x)                                                                                        \
    EXPAND_DEFINITIONS19(x)                                                                                            \
    x##(20)
#define EXPAND_DEFINITIONS21(x)                                                                                        \
    EXPAND_DEFINITIONS20(x)                                                                                            \
    x##(21)
#define EXPAND_DEFINITIONS22(x)                                                                                        \
    EXPAND_DEFINITIONS21(x)                                                                                            \
    x##(22)
#define EXPAND_DEFINITIONS23(x)                                                                                        \
    EXPAND_DEFINITIONS22(x)                                                                                            \
    x##(23)
#define EXPAND_DEFINITIONS24(x)                                                                                        \
    EXPAND_DEFINITIONS23(x)                                                                                            \
    x##(24)
#define EXPAND_DEFINITIONS25(x)                                                                                        \
    EXPAND_DEFINITIONS24(x)                                                                                            \
    x##(25)
#define EXPAND_DEFINITIONS26(x)                                                                                        \
    EXPAND_DEFINITIONS25(x)                                                                                            \
    x##(26)
#define EXPAND_DEFINITIONS27(x)                                                                                        \
    EXPAND_DEFINITIONS26(x)                                                                                            \
    x##(27)
#define EXPAND_DEFINITIONS28(x)                                                                                        \
    EXPAND_DEFINITIONS27(x)                                                                                            \
    x##(28)
#define EXPAND_DEFINITIONS29(x)                                                                                        \
    EXPAND_DEFINITIONS28(x)                                                                                            \
    x##(29)
#define EXPAND_DEFINITIONS30(x)                                                                                        \
    EXPAND_DEFINITIONS29(x)                                                                                            \
    x##(30)
#define EXPAND_DEFINITIONS(x, i) EXPAND_DEFINITIONS##i(x)

#define COMMA ,
#define PARENTHESIS_OPEN (
#define PARENTHESIS_CLOSE )

namespace jsi
{
    using namespace facebook::jsi;
}

namespace react
{
    using namespace facebook::react;
}

namespace std
{
    template <>
    struct less<winrt::guid>
    {
        bool operator()(const winrt::guid& left, const winrt::guid& right) const
        {
            return memcmp(&left, &right, sizeof(winrt::guid)) < 0;
        }
    };

#if 0
    template<> struct hash<winrt::guid>
    {
        size_t operator()(const winrt::guid& guid) const
        {
            const uint64_t* const guidAsUint64 = reinterpret_cast<uint64_t const*>(&guid);
            hash<uint64_t> uint64Hash;
            return static_cast<size_t>(uint64Hash(guidAsUint64[0]) ^ uint64Hash(guidAsUint64[1]));
        }
    };
#endif
}

using namespace std::literals;

namespace WinRTTurboModule
{
    struct ProjectionsContext;

    inline jsi::PropNameID CreatePropNameID(jsi::Runtime& runtime, std::string_view value)
    {
        return jsi::PropNameID::forUtf8(runtime, reinterpret_cast<const uint8_t*>(value.data()), value.size());
    }

    inline jsi::String CreateString(jsi::Runtime& runtime, std::string_view value)
    {
        return jsi::String::createFromUtf8(runtime, reinterpret_cast<const uint8_t*>(value.data()), value.size());
    }

    jsi::String CreateString(jsi::Runtime& runtime, std::wstring_view value);

    jsi::Value CreateError(jsi::Runtime& runtime, std::string_view message);
    jsi::Value CreateError(jsi::Runtime& runtime, const std::exception& exception);
    jsi::Value CreateError(jsi::Runtime& runtime, const winrt::hresult_error& error);

    struct PromiseWrapper final
    {
        static jsi::Value Create(jsi::Runtime& runtime, std::shared_ptr<PromiseWrapper>& wrapper);
        PromiseWrapper(jsi::Function resolve, jsi::Function reject);

        void Resolve(jsi::Runtime& runtime, const jsi::Value& value);
        void Reject(jsi::Runtime& runtime, const jsi::Value& error);

    private:
        jsi::Function m_resolve;
        jsi::Function m_reject;
        bool m_completed{};
    };

    struct CallInvokerWrapper final
    {
        CallInvokerWrapper(std::shared_ptr<react::CallInvoker> callInvokerWrapper);

        // Run the function synchronously if on the JS thread or asynchronously if not.
        void Call(std::function<void()> func);

        // Always queue the function to run asynchronously.
        void CallAsync(std::function<void()> func);

        // Run the function synchronously if on the JS thread or asynchronously and block until complete if not.
        void CallSync(std::function<void()> func);

        const DWORD ThreadId;

    private:
        const std::shared_ptr<react::CallInvoker> m_callInvokerWrapper;
    };

    template <typename T>
    using NativeToValueConverter = jsi::Value (*)(
        const std::shared_ptr<WinRTTurboModule::ProjectionsContext>&, const T&);

    template <typename T>
    using ValueToNativeConverter = T (*)(
        const std::shared_ptr<WinRTTurboModule::ProjectionsContext>&, const jsi::Value&);
}

namespace WinRTTurboModule
{
    struct DECLSPEC_NOVTABLE IProjectedValueProvider
    {
        virtual std::string_view Name() const = 0;
        virtual jsi::Value Value(jsi::Runtime& runtime) = 0;
    };

    struct ProjectedHostObjectProvider final : public IProjectedValueProvider
    {
        ProjectedHostObjectProvider(std::string_view name, std::shared_ptr<jsi::HostObject> hostObject);
        virtual ~ProjectedHostObjectProvider() = default;

        virtual std::string_view Name() const override;
        virtual jsi::Value Value(jsi::Runtime& runtime) override;

    private:
        const std::string_view m_name;
        const std::shared_ptr<jsi::HostObject> m_hostObject;
        std::optional<jsi::Object> m_object;
    };
}

namespace WinRTTurboModule
{
    struct ProjectedRuntimeClassInstanceFactory
    {
        explicit ProjectedRuntimeClassInstanceFactory(ProjectionsContext& context);

        jsi::Value Get(const winrt::Windows::Foundation::IInspectable& instance);
        bool Remove(const winrt::Windows::Foundation::IInspectable& instance);

    private:
        ProjectionsContext& m_context;
        std::unordered_map<void*, std::variant<jsi::WeakObject, std::weak_ptr<jsi::HostObject>>> m_instanceCache;
        bool m_weakObjectSupported{ true };
    };
}

namespace WinRTTurboModule
{
    struct IProjectedEventBase;
    struct ProjectedEventInstance;

    // The purpose of this component is to ensure that event registrations can out-live a given instance of
    // ProjectedRuntimeClassInstance or ProjectedInterfaceInstance but can still be unregistered later. That is, a WinRT
    // object might outlive the HostObject that we return to JS as its lifetime may be preserved by a native component
    // and the API usage assumes this. So if one queries an object from a static activation factory, lets the garbage
    // collector free the ProjectedRuntimeClassInstance (ProjectedRuntimeClassInstanceFactory only holds weak
    // references) and then queries again later obtaining a new ProjectedRuntimeClassInstance corresponding to the same
    // WinRT object, it should be able to unregister any extant event handlers all the same. If we automatically
    // unregistered for events whenever the HostObject is freed, it would impose a requirement to preserve the object
    // unlike what exists in EdgeHTML.

    struct ProjectedEventRegistrar
    {
        ProjectedEventRegistrar(ProjectionsContext& context);

        // WARNING: Use default IInspectable implementation (i.e., that returned by QueryInterface) rather than static
        //          cast on another interface derived from IInspectable as we use the ABI pointer to key off of. This
        //          key should ideally match that used for caching with ProjectedRuntimeClassInstanceFactory::Get.
        std::shared_ptr<ProjectedEventInstance> Get(const winrt::Windows::Foundation::IInspectable& inspectable,
            const std::shared_ptr<IProjectedEventBase>& event);

    private:
        // TODO: How should we garbage collect the map? Should we store wint::weak_ref here and periodically sweap like
        //       the code tries to do below opportunistically? Another option might be to try releasing on
        //       ProjectedRuntimeClassInstance releasing pointers as it will often be the last holder of strong
        //       references, but there is no guarantee since per above we are using something like
        //       ProjectedEventRegistrar because WinRT objects and outstanding event handlers may outlive the
        //       ProjectedRuntimeClassInstance.
        void CleanupMapIfApplicable();

        struct Entry
        {
            winrt::weak_ref<winrt::Windows::Foundation::IInspectable> WeakRef;
            std::vector<std::shared_ptr<ProjectedEventInstance>> EventInstances;
        };

        ProjectionsContext& m_context;
        std::unordered_map<void*, Entry> m_map;
        uint64_t m_lastCleanUp{ GetTickCount64() };
        uint32_t m_requestsSinceCleanup{};
    };
}

namespace WinRTTurboModule
{
    struct ProjectionsContext final : public std::enable_shared_from_this<ProjectionsContext>
    {
        static std::shared_ptr<ProjectionsContext> Create(
            jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> invoker);
        static std::shared_ptr<ProjectionsContext> TryGetForCurrentThread();

        ProjectionsContext(jsi::Runtime& runtime, std::shared_ptr<react::CallInvoker> invoker);
        ~ProjectionsContext();

        // TODO: Is there actually a safe way to hold runtime such that we can determine when it is no longer valid?
        jsi::Runtime& Runtime;
        const std::shared_ptr<CallInvokerWrapper> Invoker;
        ProjectedRuntimeClassInstanceFactory InstanceFactory;
        ProjectedEventRegistrar EventRegistrar;

    private:
        std::unordered_map<void*, std::variant<jsi::WeakObject, std::weak_ptr<jsi::HostObject>>>
            RuntimeClassInstanceCache;

        static std::map<DWORD, std::weak_ptr<ProjectionsContext>> s_instanceMap;
        static wil::srwlock s_lock;
    };
}

namespace WinRTTurboModule
{
    struct ProjectedNamespace final : public jsi::HostObject
    {
        using InitializerFunction = std::vector<std::shared_ptr<IProjectedValueProvider>> (*)(
            const std::shared_ptr<ProjectionsContext>&);
        static std::shared_ptr<IProjectedValueProvider> Create(
            std::string_view name, std::shared_ptr<ProjectionsContext> context, InitializerFunction initializer);

        ProjectedNamespace(
            std::string_view name, std::shared_ptr<ProjectionsContext> context, InitializerFunction initializer);
        virtual ~ProjectedNamespace() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        void EnsureInitialized();

        const std::string_view m_name;
        const std::shared_ptr<ProjectionsContext> m_context;
        std::unordered_map<std::string_view, std::shared_ptr<IProjectedValueProvider>> m_propertyMap;
        const InitializerFunction m_initializer;
        bool m_isInitialized{};
    };
}

namespace WinRTTurboModule
{
    struct ProjectedEnum final : public jsi::HostObject
    {
        using InitializerFunction = std::unordered_map<std::string_view, int64_t> (*)();
        static std::shared_ptr<IProjectedValueProvider> Create(std::string_view name, InitializerFunction initializer);

        ProjectedEnum(std::string_view name, InitializerFunction initializer);
        virtual ~ProjectedEnum() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        void EnsureInitialized();

        const std::string_view m_name;
        const InitializerFunction m_initializer;
        std::unordered_map<std::string_view, int64_t> m_propertyMap;
        bool m_isInitialized{};
    };
}

namespace WinRTTurboModule
{
    class ProjectedActivationFactory final : public IProjectedValueProvider
    {
    public:
        using GetActivationFactory = winrt::Windows::Foundation::IInspectable (*)();

        template <typename T>
        static std::shared_ptr<IProjectedValueProvider> Create(std::string_view name,
            std::shared_ptr<ProjectionsContext> context, std::set<winrt::guid> factoryInterfaceIds)
        {
            return std::make_shared<ProjectedActivationFactory>(name, std::move(context),
                &winrt::get_activation_factory<T, winrt::Windows::Foundation::IInspectable>,
                std::move(factoryInterfaceIds));
        }

        ProjectedActivationFactory(std::string_view name, std::shared_ptr<ProjectionsContext> context,
            GetActivationFactory getActivationFactory, std::set<winrt::guid> m_factoryInterfaceIds);
        virtual ~ProjectedActivationFactory() = default;

        virtual std::string_view Name() const override;
        virtual jsi::Value Value(jsi::Runtime& runtime) override;

    private:
        const std::string_view m_name;
        const std::shared_ptr<ProjectionsContext> m_context;
        const GetActivationFactory m_getActivationFactory;
        const std::set<winrt::guid> m_factoryInterfaceIds;

        std::optional<jsi::Object> m_object;
    };
}

namespace WinRTTurboModule
{
    struct ProjectedFunctionVoidRetVal
    {
    };

    extern __declspec(selectany) struct ProjectedFunctionInParamKind
    {
    } ProjectedFunctionInParam{};

    extern __declspec(selectany) struct ProjectedFunctionOutParamKind
    {
    } ProjectedFunctionOutParam{};

    template <typename T, typename K>
    class ProjectedFunctionParamAdapter;

    template <typename T, typename K>
    struct ProjectedFunctionParamInfo final
    {
        using NativeType = T;
        using ParamKind = K;

        ProjectedFunctionParamInfo(K, ValueToNativeConverter<T> inParamConverter) :
            InParamConverter(inParamConverter), OutParamConverter(nullptr), Name(nullptr)
        {
        }

        ProjectedFunctionParamInfo(K, NativeToValueConverter<T> outParamConverter, PCSTR name) :
            InParamConverter(nullptr), OutParamConverter(outParamConverter), Name(name)
        {
        }

        ProjectedFunctionParamInfo(const ProjectedFunctionParamInfo&) = delete;
        ProjectedFunctionParamInfo(ProjectedFunctionParamInfo&&) = default;

        ProjectedFunctionParamAdapter<T, K> CreateAdapter() const
        {
            return ProjectedFunctionParamAdapter<T, K>(*this);
        }

        const ValueToNativeConverter<T> InParamConverter;
        const NativeToValueConverter<T> OutParamConverter;
        const PCSTR Name;
    };

    template <typename T, typename K>
    class ProjectedFunctionParamAdapter final
    {
    public:
        // C++/WinRT types may attempt to activate the underlying class with the default constructor (deleted or not) if
        // initialized with an empty intializer list rather than nullptr.
        template <typename T, typename K,
            std::enable_if_t<std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>, int> = 0>
        ProjectedFunctionParamAdapter(const ProjectedFunctionParamInfo<T, K>& info) : m_info(info), NativeValue(nullptr)
        {
        }

        template <typename T, typename K,
            std::enable_if_t<!std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>, int> = 0>
        ProjectedFunctionParamAdapter(const ProjectedFunctionParamInfo<T, K>& info) : m_info(info), NativeValue{}
        {
        }

        ProjectedFunctionParamAdapter(const ProjectedFunctionParamAdapter&) = delete;
        ProjectedFunctionParamAdapter(ProjectedFunctionParamAdapter&&) = default;

        virtual ~ProjectedFunctionParamAdapter() = default;

        void InitValueFromArgs(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value* args,
            const uint16_t numArgs, uint16_t& argIndex)
        {
            if constexpr (std::is_same_v<K, ProjectedFunctionInParamKind>)
            {
                if (argIndex >= numArgs)
                {
                    throw jsi::JSError(context->Runtime,
                        "TypeError: Received too few args: " + std::to_string(numArgs) + "for function call");
                }
                NativeValue = m_info.InParamConverter(context, args[argIndex++]);
            }
            // else ProjectedFunctionOutParamKind should just left as init'd to '{}' or 'nullptr' by the constructor.
        }

        void AddOutParam(const std::shared_ptr<ProjectionsContext>& context, jsi::Object& result) const
        {
            if constexpr (std::is_same_v<K, ProjectedFunctionOutParamKind>)
            {
                result.setProperty(context->Runtime, m_info.Name, m_info.OutParamConverter(context, NativeValue));
            }
        }

        T NativeValue;

    private:
        const ProjectedFunctionParamInfo<T, K>& m_info;
    };

    struct DECLSPEC_NOVTABLE IProjectedFunctionOverloadBase
    {
        virtual uint16_t Arity() const noexcept = 0;
    };

    template <typename I>
    struct DECLSPEC_NOVTABLE IProjectedFunctionOverload : IProjectedFunctionOverloadBase
    {
        virtual jsi::Value Invoke(I& instance, const std::shared_ptr<ProjectionsContext>& context,
            const jsi::Value* args, const uint16_t numArgs) const = 0;
    };

    template <typename I, typename F, typename R, typename... A>
    class ProjectedFunctionOverloadOnlyInParams final : public IProjectedFunctionOverload<I>
    {
    public:
        ProjectedFunctionOverloadOnlyInParams(
            F function, NativeToValueConverter<R> resultConverter, ValueToNativeConverter<A>... params) :
            m_function(function),
            m_resultConverter(resultConverter), m_params(params...)
        {
        }

        virtual jsi::Value Invoke(I& instance, const std::shared_ptr<ProjectionsContext>& context,
            const jsi::Value* args, uint16_t numArgs) const override
        {
            if (numArgs != Arity())
            {
                throw jsi::JSError(context->Runtime,
                    "TypeError: Received " + std::to_string(numArgs) + " but expecting " + std::to_string(Arity()));
            }

            return InvokeInternal(instance, context, args, std::index_sequence_for<A...>{});
        }

        virtual uint16_t Arity() const noexcept override
        {
            return sizeof...(A);
        }

    private:
        template <std::size_t... Is>
        inline jsi::Value InvokeInternal(I& instance, const std::shared_ptr<ProjectionsContext>& context,
            const jsi::Value* args, std::index_sequence<Is...>) const
        {
            // NOTE: It is necessary for the converters to be called inline to the function call because we require that
            //       the result of the converter be kept in-scope in the event that it is converting from value to
            //       std::array_view<const T> or std::array_view<T> as otherwise tearing via the cast operator on the
            //       wrapper objects can occur if the wrapper that allocated heap memory for the native array is freed
            //       prematurely In the case of ProjectedFunctionOverloadWithOutParams, ProjectedFunctionParamAdapter
            //       holds the wrapper object.
            if constexpr (std::is_same_v<R, ProjectedFunctionVoidRetVal>)
            {
                (instance.*m_function)(std::get<Is>(m_params)(context, args[Is])...);
                return jsi::Value::undefined();
            }
            else
            {
                return m_resultConverter(context, (instance.*m_function)(std::get<Is>(m_params)(context, args[Is])...));
            }
        }

        const F m_function;
        const std::tuple<ValueToNativeConverter<A>...> m_params;
        const NativeToValueConverter<R> m_resultConverter;
    };

    template <typename I, typename F, typename R, typename... P>
    class ProjectedFunctionOverloadWithOutParams final : public IProjectedFunctionOverload<I>
    {
    public:
        ProjectedFunctionOverloadWithOutParams(F function, NativeToValueConverter<R> resultConverter, P&&... params) :
            m_function(function), m_resultConverter(resultConverter), m_params(std::forward<P>(params)...)
        {
        }

        virtual jsi::Value Invoke(I& instance, const std::shared_ptr<ProjectionsContext>& context,
            const jsi::Value* args, uint16_t numArgs) const override
        {
            return InvokeInternal(instance, context, args, numArgs, std::index_sequence_for<P...>{});
        }

        virtual uint16_t Arity() const noexcept override
        {
            constexpr uint16_t c_arity =
                (static_cast<uint16_t>(std::is_same_v<typename P::ParamKind, ProjectedFunctionInParamKind>) + ... + 0);
            return c_arity;
        }

    private:
        template <std::size_t... Is>
        jsi::Value InvokeInternal(I& instance, const std::shared_ptr<ProjectionsContext>& context,
            const jsi::Value* args, uint16_t numArgs, std::index_sequence<Is...>) const
        {
            auto& runtime = context->Runtime;
            if (numArgs != Arity())
            {
                throw jsi::JSError(runtime,
                    "TypeError: Received " + std::to_string(numArgs) + " but expecting " + std::to_string(Arity()));
            }

            // Do not intialize the values when creating the tuple, as C++ does not guarantee the order of parameter
            // evaluation.
            auto adapters = std::make_tuple(std::get<Is>(m_params).CreateAdapter()...);

            // Hopefully it at least does so in unfolding comma-operator scenarios like the following.
            uint16_t argIndex{};
            (std::get<Is>(adapters).InitValueFromArgs(context, args, numArgs, argIndex), ...);

            // 'argIndex' is not referenced when there are zero arguments
            argIndex;

            auto returnValue = jsi::Value::undefined();
            if constexpr (std::is_same_v<R, ProjectedFunctionVoidRetVal>)
            {
                (instance.*m_function)(std::get<Is>(adapters).NativeValue...);
            }
            else
            {
                returnValue = m_resultConverter(context, (instance.*m_function)(std::get<Is>(adapters).NativeValue...));
            }

            jsi::Object result(runtime);
            result.setProperty(runtime, "returnValue", std::move(returnValue));
            (std::get<Is>(adapters).AddOutParam(context, result), ...);
            return jsi::Value(runtime, result);
        }

        const F m_function;
        const std::tuple<P...> m_params;
        const NativeToValueConverter<R> m_resultConverter;
    };

    struct ProjectedFunctionOverload final
    {
        // "typename J" is because member function pointers to C++/WinRT types reference the generated "consume_.*" for
        // the method and the compiler complains. ProjectedProperty has a similar issue. It's actually beneficial to
        // have the code generation explicitly reference the consume implementation of interfaces requiring (essentially
        // inheriting) other interfaces due to ambiguity given how C++/WinRT defines the interfaces. In most cases, J =
        // winrt::impl::consume_t<I>

        // "new" is used instead of std::make_shared<> to avoid specializing std::shared_ptr<> for the specialized class
        // which reduces the size of the object file signicantly for certain generated namespaces. Like this we only
        // specialize it for IProjectedFunctionOverloadBase and even skip it for IProjectedFunctionOverloadBase<I> by
        // static_cast instead of std::static_pointer_cast.

        template <typename I, typename J>
        static std::shared_ptr<IProjectedFunctionOverloadBase> Create(void (J::*function)() const)
        {
            return std::shared_ptr<IProjectedFunctionOverloadBase>(
                new ProjectedFunctionOverloadOnlyInParams<I, decltype(function), ProjectedFunctionVoidRetVal>(
                    function, nullptr));
        }

        template <typename I, typename R, typename J>
        static std::shared_ptr<IProjectedFunctionOverloadBase> Create(
            R (J::*function)() const, NativeToValueConverter<R> resultConverter)
        {
            return std::shared_ptr<IProjectedFunctionOverloadBase>(
                new ProjectedFunctionOverloadOnlyInParams<I, decltype(function), R>(function, resultConverter));
        }

// Unfortunately C++ template folding and inference limitations make it untenable to implicitly disambiguate overloads.
// That is, the compiler wouldn't be able to infer from a the number of folded arguments of A or P to determine which
// overload to reference for function. Hence, the the following explicitly defines overloads with different numbers of
// arguments.
#define DefineProjectedFunctionOverloadCreate(n)                                                                       \
    template <typename I, EXPAND_SEQUENCE(typename A, , n, COMMA), typename J,                                         \
        EXPAND_SEQUENCE(typename B, , n, COMMA)>                                                                       \
    static std::shared_ptr<IProjectedFunctionOverloadBase> Create(void (J::*function)(EXPAND_SEQUENCE(B, , n, COMMA))  \
                                                                      const,                                           \
        EXPAND_SEQUENCE_PAIR(ValueToNativeConverter<A, > a, , n, COMMA))                                               \
    {                                                                                                                  \
        return std::shared_ptr<IProjectedFunctionOverloadBase>(                                                        \
            new ProjectedFunctionOverloadOnlyInParams<I, decltype(function), ProjectedFunctionVoidRetVal,              \
                EXPAND_SEQUENCE(A, , n, COMMA)>(function, nullptr, EXPAND_SEQUENCE(a, , n, COMMA)));                   \
    }                                                                                                                  \
    template <typename I, typename R, EXPAND_SEQUENCE(typename A, , n, COMMA), typename J,                             \
        EXPAND_SEQUENCE(typename B, , n, COMMA)>                                                                       \
    static std::shared_ptr<IProjectedFunctionOverloadBase> Create(R (J::*function)(EXPAND_SEQUENCE(B, , n, COMMA))     \
                                                                      const,                                           \
        NativeToValueConverter<R> resultConverter, EXPAND_SEQUENCE_PAIR(ValueToNativeConverter<A, > a, , n, COMMA))    \
    {                                                                                                                  \
        return std::shared_ptr<IProjectedFunctionOverloadBase>(                                                        \
            new ProjectedFunctionOverloadOnlyInParams<I, decltype(function), R, EXPAND_SEQUENCE(A, , n, COMMA)>(       \
                function, resultConverter, EXPAND_SEQUENCE(a, , n, COMMA)));                                           \
    }                                                                                                                  \
    template <typename I, EXPAND_SEQUENCE(typename P, , n, COMMA), typename J,                                         \
        EXPAND_SEQUENCE(typename B, , n, COMMA)>                                                                       \
    static std::shared_ptr<IProjectedFunctionOverloadBase> Create(                                                     \
        void (J::*function)(EXPAND_SEQUENCE(B, , n, COMMA)) const, EXPAND_SEQUENCE_PAIR(P, &&p, , n, COMMA))           \
    {                                                                                                                  \
        return std::shared_ptr<IProjectedFunctionOverloadBase>(new ProjectedFunctionOverloadWithOutParams<I,           \
            decltype(function), ProjectedFunctionVoidRetVal, EXPAND_SEQUENCE(P, , n, COMMA)>(function, nullptr,        \
            EXPAND_SEQUENCE_PAIR(std::forward<P, > PARENTHESIS_OPEN p, PARENTHESIS_CLOSE, n, COMMA)));                 \
    }                                                                                                                  \
    template <typename I, typename R, EXPAND_SEQUENCE(typename P, , n, COMMA), typename J,                             \
        EXPAND_SEQUENCE(typename B, , n, COMMA)>                                                                       \
    static std::shared_ptr<IProjectedFunctionOverloadBase> Create(R (J::*function)(EXPAND_SEQUENCE(B, , n, COMMA))     \
                                                                      const,                                           \
        NativeToValueConverter<R> resultConverter, EXPAND_SEQUENCE_PAIR(P, &&p, , n, COMMA))                           \
    {                                                                                                                  \
        return std::shared_ptr<IProjectedFunctionOverloadBase>(                                                        \
            new ProjectedFunctionOverloadWithOutParams<I, decltype(function), R, EXPAND_SEQUENCE(P, , n, COMMA)>(      \
                function, resultConverter,                                                                             \
                EXPAND_SEQUENCE_PAIR(std::forward<P, > PARENTHESIS_OPEN p, PARENTHESIS_CLOSE, n, COMMA)));             \
    }

        // IDisplayScanout2::SetPlaneParameters2 has the largest number of arguments in the public SDK at 10 so 15 seems
        // like it should be enough.
        EXPAND_DEFINITIONS(DefineProjectedFunctionOverloadCreate, 15);
    };

    // The definition above for ProjectedFunctionOverload::Create is able to disambiguate based on the number of
    // parameters in "function" but not based on argument types. That is, the compiler cannot reconile the
    // std::is_convertible_v checks against the ParamInfo::NativeType values passed with inferring the correct overload
    // to reference. The arguments need to be specified explicitly to disambiguate, but this cannot be done by passing
    // template parameters without changing ParamInfo to take const or const& params and decay the type in its
    // implementation. A workaround is to wrap the function pointer in a compile-time check to pick an overload - it
    // effectively operates like a static_cast<> but without having to fully specify the template params explicitly. The
    // argument types are explicit but the interface and result can be inferred.
    template <typename... A, typename I, typename R>
    constexpr auto ConstrainFunctionOverload(R (I::*fn)(A...))
    {
        return fn;
    }

    // ProjectedFunction is not templated by the winrt interface to reduce the amount of specialized code. Avoiding
    // specializing STL contains greatly reduces the size of object files and static libs.
    class ProjectedFunction final : public std::enable_shared_from_this<ProjectedFunction>
    {
    public:
        using InitializerFunction = std::vector<std::shared_ptr<IProjectedFunctionOverloadBase>> (*)();

        static std::shared_ptr<ProjectedFunction> Create(std::string_view name, InitializerFunction initializer);
        static std::shared_ptr<ProjectedFunction> Create(
            std::string_view name, std::shared_ptr<IProjectedFunctionOverloadBase> implementation);

        ProjectedFunction(std::string_view name, InitializerFunction initializer);
        ProjectedFunction(std::string_view name, std::shared_ptr<IProjectedFunctionOverloadBase> implementation);

        std::string_view Name() const noexcept;

        template <typename I>
        jsi::Function GetFunction(I instance, const std::shared_ptr<ProjectionsContext>& context)
        {
            EnsureInitialized();
            return jsi::Function::createFromHostFunction(context->Runtime, CreatePropNameID(context->Runtime, m_name),
                m_maxArity,
                [strongThis{ shared_from_this() }, instance{ std::move(instance) }, context](
                    jsi::Runtime&, const jsi::Value&, const jsi::Value* args, size_t count) mutable {
                    return strongThis->Invoke<I>(instance, context, args, static_cast<uint16_t>(count));
                });
        }

        template <typename I>
        jsi::Value Invoke(I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value* args,
            const uint16_t numArgs)
        {
            // Use "static_cast" instead of std::static_pointer_cast to avoid specializing
            // std::shard_ptr<IProjectedProperty<I>>.
            return static_cast<IProjectedFunctionOverload<I>*>(GetOverload(context->Runtime, numArgs).get())
                ->Invoke(instance, context, args, numArgs);
        }

        bool CanInvoke(uint16_t arity);

    private:
        std::shared_ptr<IProjectedFunctionOverloadBase>& GetOverload(jsi::Runtime& runtime, uint16_t numArgs);
        void EnsureInitialized();

        std::map<uint16_t, std::shared_ptr<IProjectedFunctionOverloadBase>> m_overloads;
        const std::string_view m_name;
        const InitializerFunction m_initializer;
        uint16_t m_maxArity{};
        bool m_isInitialized{};
    };
}

namespace WinRTTurboModule
{
    struct DECLSPEC_NOVTABLE IProjectedPropertyBase
    {
        virtual std::string_view Name() const noexcept = 0;
        virtual bool IsReadonly() const noexcept = 0;
    };

    template <typename I>
    struct DECLSPEC_NOVTABLE IProjectedProperty : IProjectedPropertyBase
    {
        virtual jsi::Value Get(I& instance, const std::shared_ptr<ProjectionsContext>& context) const = 0;
        virtual void Set(
            I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) const = 0;
    };

    template <typename I, typename G, typename GC>
    class ProjectedReadonlyProperty final : public IProjectedProperty<I>
    {
    public:
        ProjectedReadonlyProperty(std::string_view name, G get, GC convertToValue) :
            m_name(name), m_get(get), m_convertToValue(convertToValue)
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

        virtual void Set(I&, const std::shared_ptr<ProjectionsContext>&, const jsi::Value&) const override
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

    // TODO: There are weird cases where the property getters/setters are actually on different interfaces implemented
    //       by the same runtime class. While this is generally bad practice, if we did want to support it, we would
    //       need to have ProjectedRuntimeClassInstance property lookups for get/set check whether a matching name it
    //       encounters on an interface is capable of the intended operation, and proceed to check other interfaces
    //       until it finds a match. We would also need to amend the RN/WinRT to allow generation of write-only
    //       properties (rather crashing).

    template <typename I, typename G, typename GC, typename S, typename SC>
    class ProjectedWriteableProperty final : public IProjectedProperty<I>
    {
    public:
        ProjectedWriteableProperty(std::string_view name, G get, GC convertToValue, S set, SC convertFromValue) :
            m_name(name), m_get(get), m_convertToValue(convertToValue), m_set(set), m_convertFromValue(convertFromValue)
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

        virtual void Set(
            I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) const override
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
        // typename J is used because something like winrt::Windows::Foundation::IUriRuntimeClass::AbsoluteUri() is not
        // convertible from from
        // winrt::impl::consume_Windows_Foundation_IUriRuntimeClass<winrt::Windows::Foundation::IUriRuntimeClass>::AbsoluteUri()
        // as returned by the compiler so J != I and is not convertable even though I <- J when there are overloads.
        // ProjectFunction is similar.

        // "new" is used instead of std::make_shared<> to avoid specializing std::shared_ptr<> for the specialized class
        // which reduces the size of the object file signicantly for certain generated namespaces. Like this we only
        // specialize it for IProjectedProperty and can even skip IProjectedProperty<I> by static_cast instead of
        // std::static_pointer_cast.

        template <typename I, typename T, typename J>
        static std::shared_ptr<IProjectedPropertyBase> Create(
            std::string_view name, T (J::*get)() const, NativeToValueConverter<T> convertToValue)
        {
            return std::shared_ptr<IProjectedPropertyBase>(
                new ProjectedReadonlyProperty<I, decltype(get), decltype(convertToValue)>(name, get, convertToValue));
        }

        // In odd cases like a Windows.Security.Cryptography.Certificates.ICertificateExtension.Value, get and set are
        // supported but T != U because of winrt::com_array != winrt::array_view and similarly for the converters
        // ConvertComArrayToValue != ReadOnlyArrayWrapper. Note that while GC=NativeToValueConverter<T> and
        // SC=ValueToNativeConverter<U> the compiler complains about constness ambiguities so templating the coverters
        // is a workaround.

        template <typename I, typename T, typename J, typename U, typename GC, typename SC>
        static std::shared_ptr<IProjectedPropertyBase> Create(
            std::string_view name, T (J::*get)() const, GC convertToValue, void (J::*set)(U) const, SC convertFromValue)
        {
            return std::shared_ptr<IProjectedPropertyBase>(
                new ProjectedWriteableProperty<I, decltype(get), GC, decltype(set), SC>(
                    name, get, convertToValue, set, convertFromValue));
        }
    };
}

namespace WinRTTurboModule
{
    struct DECLSPEC_NOVTABLE IProjectedEventBase
    {
        virtual std::string_view Name() const noexcept = 0;
    };

    template <typename I>
    struct DECLSPEC_NOVTABLE IProjectedEvent : IProjectedEventBase
    {
        virtual winrt::event_token Add(
            I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) const = 0;
        virtual void Remove(I& instance, winrt::event_token token) const = 0;
    };

    template <typename I, typename A, typename R, typename C>
    struct ProjectedEventWrapper final : public IProjectedEvent<I>
    {
        ProjectedEventWrapper(std::string_view name, A add, R remove, C convertValueToDelegate) :
            m_name(name), m_add(add), m_remove(remove), m_convertValueToDelegate(convertValueToDelegate)
        {
        }

        virtual ~ProjectedEventWrapper() = default;

        virtual std::string_view Name() const noexcept override
        {
            return m_name;
        }

        virtual winrt::event_token Add(
            I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) const override
        {
            return (instance.*m_add)(m_convertValueToDelegate(context, value));
        }

        virtual void Remove(I& instance, winrt::event_token token) const override
        {
            (instance.*m_remove)(token);
        }

    private:
        const std::string_view m_name;
        const A m_add;
        const R m_remove;
        const C m_convertValueToDelegate;
    };

    struct ProjectedEvent
    {
        // See comment for ProjectedProperty on why J is needed here.
        template <typename I, typename D, typename J>
        static std::shared_ptr<IProjectedEventBase> Create(std::string_view name,
            winrt::event_token (J::*add)(const D&) const, void (J::*remove)(const winrt::event_token&) const,
            ValueToNativeConverter<D> convertValueToDelegate)
        {
            return std::shared_ptr<IProjectedEventBase>(
                new ProjectedEventWrapper<I, decltype(add), decltype(remove), decltype(convertValueToDelegate)>(
                    name, add, remove, convertValueToDelegate));
        }
    };

    struct ProjectedEventInstance final
    {
        ProjectedEventInstance(std::shared_ptr<IProjectedEventBase> event);
        virtual ~ProjectedEventInstance();

        const std::shared_ptr<IProjectedEventBase>& Event() const noexcept
        {
            return m_event;
        }

        template <typename I>
        void Add(I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
        {
            auto object = value.asObject(context->Runtime);
            if (!HasEntry(context->Runtime, object))
            {
                AddEntry(
                    std::move(object), static_cast<IProjectedEvent<I>*>(m_event.get())->Add(instance, context, value));
            }
        }

        template <typename I>
        void Remove(I& instance, const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
        {
            if (const auto entry = RemoveEntry(context->Runtime, value.asObject(context->Runtime)))
            {
                static_cast<IProjectedEvent<I>*>(m_event.get())->Remove(instance, entry->second);
            }
        }

        template <typename I>
        void RemoveAll(I& instance)
        {
            const auto event = static_cast<IProjectedEvent<I>*>(m_event.get());
            for (const auto& entry : m_listeners)
            {
                event->Remove(instance, entry.second);
            }
            m_listeners.clear();
        }

    private:
        std::vector<std::pair<jsi::Object, winrt::event_token>>::iterator FindEntry(
            jsi::Runtime& runtime, const jsi::Object& object);
        bool HasEntry(jsi::Runtime& runtime, const jsi::Object& object);
        void AddEntry(jsi::Object object, winrt::event_token token);
        std::optional<std::pair<jsi::Object, winrt::event_token>> RemoveEntry(
            jsi::Runtime& runtime, const jsi::Object& object);

        const std::shared_ptr<IProjectedEventBase> m_event;
        std::vector<std::pair<jsi::Object, winrt::event_token>> m_listeners;
    };
}

namespace WinRTTurboModule
{
    class ProjectedInterface;

    struct DECLSPEC_NOVTABLE IProjectedInterfaceInstance
    {
        virtual ProjectedInterface& Interface() const noexcept = 0;

        virtual jsi::Value GetMethod(jsi::Runtime& runtime, const std::shared_ptr<ProjectedFunction>& method) = 0;
        virtual jsi::Value InvokeMethod(jsi::Runtime& runtime, const std::shared_ptr<ProjectedFunction>& method,
            const jsi::Value* args, uint16_t numArgs) = 0;

        virtual jsi::Value GetProperty(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedPropertyBase>& property) = 0;
        virtual void SetProperty(jsi::Runtime& runtime, const std::shared_ptr<IProjectedPropertyBase>& property,
            const jsi::Value& value) = 0;

        virtual void AddEventListener(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedEventBase>& event, const jsi::Value& value) = 0;
        virtual void RemoveEventListener(
            jsi::Runtime& runtime, const std::shared_ptr<IProjectedEventBase>& event, const jsi::Value& value) = 0;
    };

    template <typename I>
    class ProjectedInterfaceInstance final : public IProjectedInterfaceInstance
    {
    public:
        static std::shared_ptr<IProjectedInterfaceInstance> Create(std::shared_ptr<ProjectionsContext> context,
            winrt::Windows::Foundation::IInspectable instance, std::shared_ptr<ProjectedInterface> projectedInterface)
        {
            // Use "new" instead of std::make_shared to avoid specializing
            // std::shared_ptr<ProjectedInterfaceInstance<I>> to reduce the sizes of object files.
            return std::shared_ptr<ProjectedInterfaceInstance>(new ProjectedInterfaceInstance<I>(
                std::move(context), std::move(instance), std::move(projectedInterface)));
        }

        ProjectedInterfaceInstance(std::shared_ptr<ProjectionsContext> context,
            winrt::Windows::Foundation::IInspectable instance, std::shared_ptr<ProjectedInterface> projectedInterface) :
            m_context(std::move(context)),
            m_inspectableInstance(std::move(instance)), m_projectedInterface(std::move(projectedInterface))
        {
        }

        virtual ProjectedInterface& Interface() const noexcept override
        {
            return *m_projectedInterface;
        }

        virtual jsi::Value GetMethod(jsi::Runtime&, const std::shared_ptr<ProjectedFunction>& method) override
        {
            EnsureInitialized();
            return method->GetFunction<I>(m_queriedInstance, m_context);
        }

        virtual jsi::Value InvokeMethod(jsi::Runtime&, const std::shared_ptr<ProjectedFunction>& method,
            const jsi::Value* args, uint16_t numArgs) override
        {
            EnsureInitialized();
            return method->Invoke<I>(m_queriedInstance, m_context, args, numArgs);
        }

        // Use "static_cast" instead of std::static_pointer_cast to avoid specializing
        // std::shard_ptr<IProjectedProperty<I>>.
        virtual jsi::Value GetProperty(jsi::Runtime&, const std::shared_ptr<IProjectedPropertyBase>& property) override
        {
            EnsureInitialized();
            return static_cast<IProjectedProperty<I>*>(property.get())->Get(m_queriedInstance, m_context);
        }

        virtual void SetProperty(
            jsi::Runtime&, const std::shared_ptr<IProjectedPropertyBase>& property, const jsi::Value& value) override
        {
            EnsureInitialized();
            static_cast<IProjectedProperty<I>*>(property.get())->Set(m_queriedInstance, m_context, value);
        }

        virtual void AddEventListener(
            jsi::Runtime&, const std::shared_ptr<IProjectedEventBase>& event, const jsi::Value& value) override
        {
            EnsureInitialized();
            m_context->EventRegistrar.Get(m_inspectableInstance, event)->Add(m_queriedInstance, m_context, value);
        }

        virtual void RemoveEventListener(
            jsi::Runtime&, const std::shared_ptr<IProjectedEventBase>& event, const jsi::Value& value) override
        {
            EnsureInitialized();
            m_context->EventRegistrar.Get(m_inspectableInstance, event)->Remove(m_queriedInstance, m_context, value);
        }

    private:
        void EnsureInitialized()
        {
            if (!m_queriedInstance)
            {
                m_queriedInstance = m_inspectableInstance.as<I>();
            }
        }

        const std::shared_ptr<ProjectionsContext> m_context;
        const winrt::Windows::Foundation::IInspectable m_inspectableInstance;
        const std::shared_ptr<ProjectedInterface> m_projectedInterface;
        I m_queriedInstance{ nullptr };
    };

    struct ProjectedInterfaceData
    {
        std::vector<std::shared_ptr<IProjectedPropertyBase>> Properties;
        std::vector<std::shared_ptr<ProjectedFunction>> Methods;
        std::vector<std::shared_ptr<IProjectedEventBase>> Events;
    };

    // ProjectedInterface is not templated by the winrt interface to reduce the amount of specialized code. Avoiding
    // specializing STL contains greatly reduces the size of object files and static libs. For example, even
    // std::enable_shared_from_this<ProjectedInterface<I>> is very expensive.
    class ProjectedInterface final : public std::enable_shared_from_this<ProjectedInterface>
    {
    public:
        using InitializerFunction = ProjectedInterfaceData (*)();
        using InstanceFactory = std::shared_ptr<IProjectedInterfaceInstance> (*)(std::shared_ptr<ProjectionsContext>,
            winrt::Windows::Foundation::IInspectable, std::shared_ptr<ProjectedInterface>);

        static std::shared_ptr<ProjectedInterface> Get(const winrt::guid& iid);

        template <typename I>
        static std::shared_ptr<ProjectedInterface> Create(InitializerFunction initializer)
        {
            return std::make_shared<ProjectedInterface>(
                winrt::guid_of<I>(), &ProjectedInterfaceInstance<I>::Create, initializer);
        }

        ProjectedInterface(const winrt::guid& iid, InstanceFactory factory, InitializerFunction initializer);
        virtual ~ProjectedInterface() = default;

        const winrt::guid& InterfaceId() const noexcept;

        std::shared_ptr<ProjectedFunction> FindMethod(std::string_view name);
        std::shared_ptr<ProjectedFunction> FindMethod(std::string_view name, uint16_t numArgs);
        std::shared_ptr<ProjectedFunction> FindMethod(uint16_t numArgs);
        const std::unordered_map<std::string_view, std::shared_ptr<ProjectedFunction>>& Methods();

        std::shared_ptr<IProjectedPropertyBase> FindProperty(std::string_view name);
        const std::unordered_map<std::string_view, std::shared_ptr<IProjectedPropertyBase>>& Properties();

        std::shared_ptr<IProjectedEventBase> FindEvent(std::string_view name);
        const std::unordered_map<std::string_view, std::shared_ptr<IProjectedEventBase>>& Events();

        std::shared_ptr<IProjectedInterfaceInstance> CreateInstance(
            std::shared_ptr<ProjectionsContext> context, winrt::Windows::Foundation::IInspectable instance);

    private:
        void EnsureInitialized();

        static void EnsureInterfaceMap();
        static std::unordered_map<winrt::guid, std::shared_ptr<ProjectedInterface>> s_interfaceMap;

        std::unordered_map<std::string_view, std::shared_ptr<IProjectedPropertyBase>> m_propertyMap;
        std::unordered_map<std::string_view, std::shared_ptr<ProjectedFunction>> m_methodMap;
        std::unordered_map<std::string_view, std::shared_ptr<IProjectedEventBase>> m_eventMap;

        const winrt::guid& m_iid;
        const InitializerFunction m_initializer;
        const InstanceFactory m_factory;
        bool m_isInitialized{};
    };
}

namespace WinRTTurboModule
{
    class ProjectedRuntimeClassInstance final : public jsi::HostObject
    {
    public:
        static jsi::Value Get(const std::shared_ptr<ProjectionsContext>& context,
            const winrt::Windows::Foundation::IInspectable& instance);

        ProjectedRuntimeClassInstance(
            std::shared_ptr<ProjectionsContext> context, winrt::Windows::Foundation::IInspectable instance);
        virtual ~ProjectedRuntimeClassInstance();

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

        std::optional<jsi::Value> GetEventListenerFunction(jsi::Runtime& runtime, bool isAddEventListener);

        inline auto Instance() const noexcept
        {
            return m_instance;
        }

        inline const auto& Interfaces() noexcept
        {
            EnsureInitialized();
            return m_interfaceInstances;
        }

    private:
        void EnsureInitialized();

        const std::shared_ptr<ProjectionsContext> m_context;
        const winrt::Windows::Foundation::IInspectable m_instance;
        std::vector<std::shared_ptr<IProjectedInterfaceInstance>> m_interfaceInstances;
        bool m_isInitialized{};
    };

    jsi::Function CreateCompositeConstructor(jsi::Runtime& runtime, std::string className,
        std::vector<std::shared_ptr<IProjectedInterfaceInstance>> factoryInterfaceInstances);
    jsi::Value CreateCompositeMethod(jsi::Runtime& runtime, std::string methodName,
        std::vector<std::pair<std::shared_ptr<IProjectedInterfaceInstance>, std::shared_ptr<ProjectedFunction>>>
            interfaceMethods);
}

namespace WinRTTurboModule
{
    struct ProjectedAsyncOperationBaseMethods final :
        public std::enable_shared_from_this<ProjectedAsyncOperationBaseMethods>
    {
        ProjectedAsyncOperationBaseMethods(std::shared_ptr<ProjectionsContext> context);
        virtual ~ProjectedAsyncOperationBaseMethods() = default;

        std::optional<jsi::Value> TryGetMethod(const jsi::PropNameID& propName, const std::string& propNameUtf8);

        void OnCompleted(jsi::Value result, bool succeeded);
        void OnProgress(jsi::Value progress);

        std::shared_ptr<ProjectionsContext> Context() const
        {
            return m_context;
        }

    private:
        jsi::Value HandleThen(const jsi::Value& onResolved, const jsi::Value& onRejected, const jsi::Value& onProgress,
            bool forDone = false);

        struct Continuation
        {
            std::shared_ptr<PromiseWrapper> Promise;
            std::optional<jsi::Function> OnResolved;
            std::optional<jsi::Function> OnRejected;
            std::optional<jsi::Function> OnProgress;
        };
        void DispatchContinuation(const Continuation& continuation) const;

        const std::shared_ptr<ProjectionsContext> m_context;
        std::vector<std::shared_ptr<Continuation>> m_continuations;
        std::optional<jsi::Value> m_result;
        const DWORD m_threadId;
        bool m_succeeded{};
    };

    struct ProjectedAsyncOperationBase : public jsi::HostObject
    {
        ProjectedAsyncOperationBase(std::shared_ptr<ProjectionsContext> context);
        virtual ~ProjectedAsyncOperationBase() = default;

        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& propName) override final;
        virtual void set(
            jsi::Runtime& runtime, const jsi::PropNameID& propName, const jsi::Value& value) override final;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override final;

        virtual winrt::Windows::Foundation::IInspectable Instance() = 0;

    protected:
        std::shared_ptr<ProjectedAsyncOperationBaseMethods> m_methods;

    private:
        std::optional<jsi::Value> m_operation;
    };

    struct IsAsyncAction
    {
    };
    struct NoAsyncProgress
    {
    };

    template <typename T, typename R = IsAsyncAction, typename P = NoAsyncProgress>
    struct ProjectedAsyncOperation final : public ProjectedAsyncOperationBase
    {
        ProjectedAsyncOperation(const std::shared_ptr<ProjectionsContext>& context, const T& instance,
            NativeToValueConverter<R> valueConverter = nullptr, NativeToValueConverter<P> progressConverter = nullptr) :
            ProjectedAsyncOperationBase(context),
            m_instance(instance)
        {
            AwaitCompletion(context, instance, valueConverter, m_methods);

            if constexpr (!std::is_same_v<P, NoAsyncProgress>)
            {
                // Progress(...) internally calls winrt::impl::make_agile_delegate so the lambda should be called on the
                // original thread rather than on some arbitrary background thread.
                m_instance.Progress(
                    [progressConverter, methods{ m_methods }, context](const auto&, const auto& progress) {
                        methods->OnProgress(progressConverter(context, progress));
                    });
            }
        }

        virtual ~ProjectedAsyncOperation() = default;

        virtual winrt::Windows::Foundation::IInspectable Instance() override
        {
            return m_instance;
        }

    private:
        static winrt::fire_and_forget AwaitCompletion(std::shared_ptr<ProjectionsContext> context, T instance,
            NativeToValueConverter<R> converter, std::shared_ptr<ProjectedAsyncOperationBaseMethods> methods)
        {
            auto& runtime = context->Runtime;
            const auto& invoker = context->Invoker;

            try
            {
                if constexpr (std::is_same_v<R, IsAsyncAction>)
                {
                    // "Call" is necessary because RNW uses ChakraCore or V8 with a MTA thread as opposed to an ASTA
                    // even though JS is single-threaded; therefore, we must call back into JS on the original thread
                    // to avoid trouble. Call will queue the lambda on the JS thread if the current thread is not the
                    // same as that of JS else execute it synchronously.

                    // PERF: If co_await causes code-bloat, it is reasonable to just use a Completed delegate instead.
                    co_await instance;
                    invoker->Call(
                        [&runtime, methods]() { methods->OnCompleted(jsi::Value::undefined(), true /*succeeded*/); });
                }
                else
                {
                    invoker->Call([methods, converter, context{ std::move(context) }, result{ co_await instance }]() {
                        methods->OnCompleted(converter(context, result), true /*succeeded*/);
                    });
                }
            }
            catch (winrt::hresult_error e)
            {
                invoker->Call([&runtime, methods, e{ std::move(e) }]() {
                    methods->OnCompleted(CreateError(runtime, e), false /*succeeded*/);
                });
            }
            catch (std::exception e)
            {
                invoker->Call([&runtime, methods, e{ std::move(e) }]() {
                    methods->OnCompleted(CreateError(runtime, e), false /*succeeded*/);
                });
            }
        }

        T m_instance;
    };
}

namespace WinRTTurboModule::Windows::Foundation::Collections
{
    template <typename T>
    ProjectedInterfaceData InitIIterableInterface()
    {
        return { {},
            {
                ProjectedFunction::Create("first"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IIterable<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IIterable<T>>::First,
                        &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IIterator<T>>)),
            },
            {} };
    }

    template <typename T, auto CTV>
    ProjectedInterfaceData InitIIteratorInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IIterator<T>>(
                         "current"sv, &winrt::Windows::Foundation::Collections::IIterator<T>::Current, &CTV),
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IIterator<T>>("hasCurrent"sv,
                         &winrt::Windows::Foundation::Collections::IIterator<T>::HasCurrent, &ConvertBooleanToValue),
                 },
            {
                ProjectedFunction::Create("getMany"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IIterator<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IIterator<T>>::GetMany,
                        &ConvertNumberToValue<uint32_t>, &ConvertValueToWriteOnlyArrayView<T, CTV>)),
                ProjectedFunction::Create("moveNext"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IIterator<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IIterator<T>>::MoveNext,
                        &ConvertBooleanToValue)),
            },
            {} };
    }

    template <typename K, typename T, auto CKV, auto CTV>
    ProjectedInterfaceData InitIKeyValuePairInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IKeyValuePair<K, T>>(
                         "key"sv, &winrt::Windows::Foundation::Collections::IKeyValuePair<K, T>::Key, &CKV),
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IKeyValuePair<K, T>>(
                         "value"sv, &winrt::Windows::Foundation::Collections::IKeyValuePair<K, T>::Value, &CTV),
                 },
            {}, {} };
    }

    template <typename K, auto CKV>
    ProjectedInterfaceData InitIMapChangedEventArgsInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>(
                         "collectionChange"sv,
                         &winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>::CollectionChange,
                         &ConvertEnumToValue<winrt::Windows::Foundation::Collections::CollectionChange>),
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>(
                         "key"sv, &winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>::Key, &CKV),
                 },
            {}, {} };
    }

    template <typename K, typename T, auto CVK, auto CTV>
    ProjectedInterfaceData InitIMapViewInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IMapView<K, T>>("size"sv,
                         &winrt::Windows::Foundation::Collections::IMapView<K, T>::Size,
                         &ConvertNumberToValue<uint32_t>),
                 },
            {
                ProjectedFunction::Create("hasKey"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMapView<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMapView<K, T>>::HasKey,
                        &ConvertBooleanToValue, &CVK)),
                ProjectedFunction::Create("lookup"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMapView<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMapView<K, T>>::Lookup, &CTV,
                        &CVK)),
                ProjectedFunction::Create("split"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMapView<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMapView<K, T>>::Split,
                        ProjectedFunctionParamInfo(ProjectedFunctionOutParam,
                            &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IMapView<K, T>>, "first"),
                        ProjectedFunctionParamInfo(ProjectedFunctionOutParam,
                            &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IMapView<K, T>>,
                            "second"))),
            },
            {} };
    }

    template <typename K, typename T, auto CVK, auto CTV, auto CVT>
    ProjectedInterfaceData InitIMapInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>("size"sv,
                         &winrt::Windows::Foundation::Collections::IMap<K, T>::Size, &ConvertNumberToValue<uint32_t>),
                 },
            {
                ProjectedFunction::Create("clear"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::Clear)),
                ProjectedFunction::Create("getView"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::GetView,
                        &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IMapView<K, T>>)),
                ProjectedFunction::Create("hasKey"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::HasKey,
                        &ConvertBooleanToValue, &CVK)),
                ProjectedFunction::Create("insert"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::Insert,
                        &ConvertBooleanToValue, &CVK, &CVT)),
                ProjectedFunction::Create("lookup"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::Lookup, &CTV,
                        &CVK)),
                ProjectedFunction::Create("remove"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IMap<K, T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IMap<K, T>>::Remove, &CVK)),
            },
            {} };
    }

    template <typename K, typename T>
    ProjectedInterfaceData InitIObservableMapInterface()
    {
        return { {}, {},
            {
                ProjectedEvent::Create<winrt::Windows::Foundation::Collections::IObservableMap<K, T>>("mapChanged"sv,
                    &winrt::Windows::Foundation::Collections::IObservableMap<K, T>::MapChanged,
                    &winrt::Windows::Foundation::Collections::IObservableMap<K, T>::MapChanged,
                    &ConvertValueToMapChangedEventHandler<K, T>),
            } };
    }

    template <typename T, auto CTV, auto CVT>
    ProjectedInterfaceData InitIVectorViewInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>("size"sv,
                         &winrt::Windows::Foundation::Collections::IVectorView<T>::Size,
                         &ConvertNumberToValue<uint32_t>),
                 },
            {
                ProjectedFunction::Create("getAt"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVectorView<T>>::GetAt, &CTV,
                        &ConvertValueToNumber<uint32_t>)),
                ProjectedFunction::Create("getMany"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVectorView<T>>::GetMany,
                        &ConvertNumberToValue<uint32_t>, &ConvertValueToNumber<uint32_t>,
                        &ConvertValueToWriteOnlyArrayView<T, CTV>)),
                ProjectedFunction::Create("indexOf"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVectorView<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVectorView<T>>::IndexOf,
                        &ConvertBooleanToValue, ProjectedFunctionParamInfo(ProjectedFunctionInParam, &CVT),
                        ProjectedFunctionParamInfo(
                            ProjectedFunctionOutParam, &ConvertNumberToValue<uint32_t>, "index"))),
            },
            {} };
    }

    template <typename T, auto CTV, auto CVT>
    ProjectedInterfaceData InitIVectorInterface()
    {
        return { {
                     ProjectedProperty::Create<winrt::Windows::Foundation::Collections::IVector<T>>("size"sv,
                         &winrt::Windows::Foundation::Collections::IVector<T>::Size, &ConvertNumberToValue<uint32_t>),
                 },
            {
                ProjectedFunction::Create("append"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::Append, &CVT)),
                ProjectedFunction::Create("clear"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::Clear)),
                ProjectedFunction::Create(
                    "getAt"sv, ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                                   &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::GetAt,
                                   &CTV, &ConvertValueToNumber<uint32_t>)),
                ProjectedFunction::Create("getMany"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::GetMany,
                        &ConvertNumberToValue<uint32_t>, &ConvertValueToNumber<uint32_t>,
                        &ConvertValueToWriteOnlyArrayView<T, CTV>)),
                ProjectedFunction::Create("getView"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::GetView,
                        &ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IVectorView<T>>)),
                ProjectedFunction::Create("indexOf"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::IndexOf,
                        &ConvertBooleanToValue, ProjectedFunctionParamInfo(ProjectedFunctionInParam, &CVT),
                        ProjectedFunctionParamInfo(
                            ProjectedFunctionOutParam, &ConvertNumberToValue<uint32_t>, "index"))),
                ProjectedFunction::Create("insertAt"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::InsertAt,
                        &ConvertValueToNumber<uint32_t>, &CVT)),
                ProjectedFunction::Create("replaceAll"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::ReplaceAll,
                        &ConvertValueToReadOnlyArrayView<T, CVT>)),
                ProjectedFunction::Create("removeAt"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::RemoveAt,
                        &ConvertValueToNumber<uint32_t>)),
                ProjectedFunction::Create("removeAtEnd"sv,
                    ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                        &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::RemoveAtEnd)),
                ProjectedFunction::Create(
                    "setAt"sv, ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::IVector<T>>(
                                   &winrt::impl::consume_t<winrt::Windows::Foundation::Collections::IVector<T>>::SetAt,
                                   &ConvertValueToNumber<uint32_t>, &CVT)),
            },
            {} };
    }

    template <typename T>
    ProjectedInterfaceData InitIObservableVectorInterface()
    {
        return { {}, {},
            {
                ProjectedEvent::Create<winrt::Windows::Foundation::Collections::IObservableVector<T>>("vectorChanged"sv,
                    &winrt::Windows::Foundation::Collections::IObservableVector<T>::VectorChanged,
                    &winrt::Windows::Foundation::Collections::IObservableVector<T>::VectorChanged,
                    &ConvertValueToVectorChangedEventHandler<T>),
            } };
    }
}

namespace WinRTTurboModule
{
    // Boolean
    inline jsi::Value ConvertBooleanToValue(const std::shared_ptr<ProjectionsContext>&, const bool& value)
    {
        return jsi::Value(value);
    }

    bool ConvertValueToBoolean(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // Integer/Float
    template <typename T>
    inline jsi::Value ConvertNumberToValue(const std::shared_ptr<ProjectionsContext>&, const T& value)
    {
        return jsi::Value(static_cast<double>(value));
    }

    template <typename T>
    inline T ConvertValueToNumber(const std::shared_ptr<ProjectionsContext>&, const jsi::Value& value)
    {
        return static_cast<T>(value.asNumber());
    }

    // Enum
    template <typename T>
    inline jsi::Value ConvertEnumToValue(const std::shared_ptr<ProjectionsContext>&, const T& value)
    {
        return jsi::Value(static_cast<double>(static_cast<int64_t>(value)));
    }

    template <typename T>
    inline T ConvertValueToEnum(const std::shared_ptr<ProjectionsContext>&, const jsi::Value& value)
    {
        return static_cast<T>(static_cast<int64_t>(value.asNumber()));
    }

    // char16_t
    jsi::Value ConvertCharToValue(const std::shared_ptr<ProjectionsContext>& context, const char16_t& value);
    char16_t ConvertValueToChar(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // winrt::hstring
    inline jsi::Value ConvertStringToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::hstring& value)
    {
        return CreateString(context->Runtime, static_cast<std::wstring_view>(value));
    }
    winrt::hstring ConvertValueToString(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // winrt::guid
    jsi::Value ConvertGuidToValue(const std::shared_ptr<ProjectionsContext>& context, const winrt::guid& value);
    winrt::guid ConvertValueToGuid(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // Structs: Converters should generally by code-gen'd aside from a few special types but it is possible that some
    // are missing. For example, there are IDL issues like Windows.UI.Core.CorePhysicalKeyStatus being WebHostHidden but
    // by types that are not such as Windows.Web.UI.Interop.IWebViewControlAcceleratorKeyPressedEventArgs (which
    // probably should also be WebHostHidden).
    template <typename T>
    jsi::Value ConvertStructToValue(const std::shared_ptr<ProjectionsContext>& context, const T&)
    {
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion from native struct to JS not implemented for "sv) + typeid(T).name());
    }

    template <typename T>
    T ConvertValueToStruct(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value&)
    {
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion from JS to native struct not implemented for "sv) + typeid(T).name());
    }

    // winrt::hresult
    template <>
    inline jsi::Value ConvertStructToValue<winrt::hresult>(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::hresult& value)
    {
        return ConvertNumberToValue<int32_t>(context, static_cast<int32_t>(value));
    }

    template <>
    inline winrt::hresult ConvertValueToStruct<winrt::hresult>(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return winrt::hresult(ConvertValueToNumber<int32_t>(context, value));
    }

    // winrt::Windows::Foundation::DateTime <=> std::chrono::time_point<clock, TimeSpan>;
    template <>
    jsi::Value ConvertStructToValue<winrt::Windows::Foundation::DateTime>(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::DateTime& value);
    template <>
    winrt::Windows::Foundation::DateTime ConvertValueToStruct<winrt::Windows::Foundation::DateTime>(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    // winrt::Windows::Foundation::TimeSpan <=> std::chrono::duration<int64_t, impl::filetime_period>;
    template <>
    jsi::Value ConvertStructToValue<winrt::Windows::Foundation::TimeSpan>(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::TimeSpan& value);
    template <>
    winrt::Windows::Foundation::TimeSpan ConvertValueToStruct<winrt::Windows::Foundation::TimeSpan>(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    template <typename T, auto CTV>
    jsi::Value ConvertComArrayToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::com_array<T>& value)
    {
        const auto size = value.size();
        auto jsArray = jsi::Array(context->Runtime, size);
        for (uint32_t i = 0; i < size; ++i)
        {
            jsArray.setValueAtIndex(context->Runtime, i, CTV(context, value[i]));
        }
        return jsArray;
    }

    template <typename T, auto CVT>
    struct ReadOnlyArrayWrapper final
    {
        ReadOnlyArrayWrapper() = default;

        ReadOnlyArrayWrapper(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
        {
            auto& runtime = context->Runtime;
            const auto array = value.asObject(runtime).asArray(runtime);
            const auto size = array.size(runtime);

            if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>)
            {
                m_nativeArray = winrt::com_array<T>(static_cast<uint32_t>(size), nullptr);
            }
            else
            {
                m_nativeArray = winrt::com_array<T>(static_cast<uint32_t>(size));
            }

            for (uint32_t i = 0; i < size; ++i)
            {
                m_nativeArray[i] = CVT(context, array.getValueAtIndex(runtime, i));
            }
        }

        ReadOnlyArrayWrapper(const ReadOnlyArrayWrapper&) = delete;
        ReadOnlyArrayWrapper(ReadOnlyArrayWrapper&&) = default;

        ~ReadOnlyArrayWrapper() = default;

        operator winrt::array_view<const T>()
        {
            return winrt::array_view<const T>(static_cast<const T*>(m_nativeArray.data()),
                static_cast<const T*>(m_nativeArray.data() + m_nativeArray.size()));
        }

    private:
        winrt::com_array<T> m_nativeArray;
    };

    template <typename T, auto CVT>
    ReadOnlyArrayWrapper<T, CVT> ConvertValueToReadOnlyArrayView(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return { context, value };
    }

    template <typename T, auto CTV>
    struct WriteOnlyArrayWrapper final
    {
        WriteOnlyArrayWrapper() = default;

        WriteOnlyArrayWrapper(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value) :
            m_context(context), m_jsArray(value.asObject(m_context->Runtime).asArray(m_context->Runtime))
        {
            const auto size = m_jsArray->size(m_context->Runtime);
            if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>)
            {
                m_nativeArray = winrt::com_array<T>(static_cast<uint32_t>(size), nullptr);
            }
            else
            {
                m_nativeArray = winrt::com_array<T>(static_cast<uint32_t>(size));
            }
        }

        WriteOnlyArrayWrapper(const WriteOnlyArrayWrapper&) = delete;
        WriteOnlyArrayWrapper(WriteOnlyArrayWrapper&&) = default;

        // TODO: Should an exception here be allowed to propagate, potentially fatally?
        ~WriteOnlyArrayWrapper() noexcept
        try
        {
            const auto size = m_nativeArray.size();
            for (uint32_t i = 0; i < size; ++i)
            {
                m_jsArray->setValueAtIndex(m_context->Runtime, i, CTV(m_context, m_nativeArray[i]));
            }
        }
        CATCH_LOG_RETURN()

        operator winrt::array_view<T>&()
        {
            return m_nativeArray;
        }

    private:
        const std::shared_ptr<ProjectionsContext> m_context;
        std::optional<jsi::Array> m_jsArray;
        winrt::com_array<T> m_nativeArray;
    };

    template <typename T, auto CTV>
    WriteOnlyArrayWrapper<T, CTV> ConvertValueToWriteOnlyArrayView(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return { context, value };
    }

    // Interfaces
    std::optional<jsi::Value> TryConvertPropertyValueToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IPropertyValue& value);
    winrt::Windows::Foundation::IInspectable TryConvertValueToPropertyValue(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value);

    template <typename T>
    jsi::Value ConvertInterfaceToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        if (!value)
        {
            return jsi::Value(nullptr);
        }

        if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IPropertyValue>)
        {
            if (auto convertedValue = TryConvertPropertyValueToValue(context, value))
            {
                return std::move(*convertedValue);
            }
        }
        else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
        {
            if (const auto propertyValue = value.try_as<winrt::Windows::Foundation::IPropertyValue>())
            {
                if (auto convertedValue = TryConvertPropertyValueToValue(context, propertyValue))
                {
                    return std::move(*convertedValue);
                }
            }
        }

        // The purpose of the explicit QI as IInspectable is to reduce the probability of aliasing where we end up
        // instantiatin multiple instances of ProjectedRuntimeClassInstance rather than reusing cached instances. The
        // lookup is based on the raw ABI pointer as void* but since by definition WinRT COM interface inherit
        // IInspectable (and IUnknown) it might a different vtable entry and a different pointer. So even if T ==
        // winrt::Windows::Foundation::IInspectable we should QI just to be sure because a static_cast of a WinRT COM
        // interface as IInspectable may have been performed pointing to a different vtable entry and giving a different
        // pointer. In most cases a class will always return a particular implementation of IInspectable given use of a
        // single implementation of IUnknown::QueryInterface.
        return ProjectedRuntimeClassInstance::Get(context, value.as<winrt::Windows::Foundation::IInspectable>());
    }

    winrt::Windows::Foundation::IInspectable TryGetInterfaceFromHostObject(
        jsi::Runtime& runtime, const jsi::Value& value);

    template <typename T>
    T ConvertValueToInterface(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        if (value.isNull() || value.isUndefined())
        {
            return nullptr;
        }

        if (const auto inspectable = TryGetInterfaceFromHostObject(context->Runtime, value))
        {
            if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
            {
                return inspectable;
            }
            else if (const auto queried = inspectable.try_as<T>())
            {
                return queried;
            }
            else
            {
                throw jsi::JSError(
                    context->Runtime, std::string("TypeError: Could not convert COM interface to ") + typeid(T).name());
            }
        }

        if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable> ||
                      std::is_same_v<T, winrt::Windows::Foundation::IPropertyValue>)
        {
            if (auto inspectable = TryConvertValueToPropertyValue(context, value))
            {
                if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
                {
                    return inspectable;
                }
                else
                {
                    return inspectable.as<winrt::Windows::Foundation::IPropertyValue>();
                }
            }
        }

        // TODO: Special case allowing JS to pass types like Array for IVector/IVectorView and Map for IMap/IMapView.

        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Cannot derive a WinRT interface for the JS value. Expecting: ") + typeid(T).name());
    }

    // ProjectedAsyncOperation is an imitation of Promise except it also supports some special features like Chakra such
    // as a progress callback and exposing the underlying IAsyncOperation as an "operation" property. It is better than
    // Chakra's as it implements Promise rather than just PromiseLike. That said, to avoid code bloat, the interfaces
    // for the underlying operation are not specialized (exception IAsyncAction since it isn't generic anyway) and
    // exposing them would give the same functionality as the ProjectedAsyncOperation Proimise wrapper. If there is
    // strong reasons for their projection it could be done.
    jsi::Value ConvertAsyncActionToValue(
        const std::shared_ptr<ProjectionsContext>& context, const winrt::Windows::Foundation::IAsyncAction& value);

    template <typename A, auto CPV>
    jsi::Value ConvertAsyncActionWithProgressToValue(const std::shared_ptr<ProjectionsContext>& context, const A& value)
    {
        return jsi::Value(context->Runtime, jsi::Object::createFromHostObject(context->Runtime,
                                                std::shared_ptr<jsi::HostObject>(new ProjectedAsyncOperation(context,
                                                    value, NativeToValueConverter<IsAsyncAction>(nullptr), &CPV))));
    }

    template <typename A, auto CTV>
    jsi::Value ConvertAsyncOperationToValue(const std::shared_ptr<ProjectionsContext>& context, const A& value)
    {
        return jsi::Value(
            context->Runtime, jsi::Object::createFromHostObject(context->Runtime,
                                  std::shared_ptr<jsi::HostObject>(new ProjectedAsyncOperation(context, value, &CTV))));
    }

    template <typename A, auto CTV, auto CPV>
    jsi::Value ConvertAsyncOperationWithProgressToValue(
        const std::shared_ptr<ProjectionsContext>& context, const A& value)
    {
        return jsi::Value(context->Runtime,
            jsi::Object::createFromHostObject(context->Runtime,
                std::shared_ptr<jsi::HostObject>(new ProjectedAsyncOperation(context, value, &CTV, &CPV))));
    }

    template <typename I, auto CTV>
    jsi::Value ConvertReferenceToValue(const std::shared_ptr<ProjectionsContext>& context, const I& value)
    {
        if (value)
        {
            return CTV(context, value.Value());
        }
        return jsi::Value(nullptr);
    }

    template <typename I, auto CVT>
    I ConvertValueToReference(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        if (value.isNull() || value.isUndefined())
        {
            return nullptr;
        }
        return winrt::box_value(CVT(context, value)).as<I>();
    }

    template <typename I, auto CTV>
    jsi::Value ConvertReferenceArrayToValue(const std::shared_ptr<ProjectionsContext>& context, const I& value)
    {
        if (value)
        {
            return ConvertComArrayToValue<decltype(std::declval(I).Value()), CTV>(context, value.Value());
        }
        return jsi::Value(nullptr);
    }

    template <typename I, auto CVT>
    I ConvertValueToReferenceArray(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        if (value.isNull() || value.isUndefined())
        {
            return nullptr;
        }

        // It doesn't seem like C++/WinRT actually provides an implementation of IReferenceArray<T> like it does for
        // IReference<T> nor does it even map the basic array types supported by Windows::Foundation::PropertyValue. It
        // can be done but since the public SDK doesn't actually make use of it, perhaps it it is not necessary to
        // implement.
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion to native reference array to JS not implemented for "sv) +
                typeid(I).name());
    }

    // Delegates
    template <typename T>
    jsi::Value ConvertDelegateToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion from native delegate to JS not implemented for "sv) + typeid(T).name());
    }

    template <typename T>
    T ConvertValueToDelegate(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        throw jsi::JSError(context->Runtime,
            std::string("TypeError: Conversion from JS to native delegate not implemented for "sv) + typeid(T).name());
    }

    // Generic Delegates. These are special-cased because MidlRT special-cases generics so we don't need to generally
    // support code-gen for generic delegates. Some are explicitly omitted below because there is no purpose in
    // implementing the generic async-related delegates when the corresponding generic interfaces are not specialized
    // and included in the interface map.

    template <typename T, auto RC>
    jsi::Value ConvertEventHandlerToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        static auto s_function = ProjectedFunction::Create(
            "invoke"sv, ProjectedFunctionOverload::Create<T>(
                            &T::operator(), &ConvertValueToInterface<winrt::Windows::Foundation::IInspectable>, &RC));
        return jsi::Value(context->Runtime, s_function->GetFunction<T>(value, context));
    }

    template <typename T, auto RC>
    T ConvertValueToEventHandler(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return T([context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](
                     const winrt::Windows::Foundation::IInspectable& sender, const auto& args) {
            context->Invoker->CallSync([&]() {
                function.call(context->Runtime,
                    ConvertInterfaceToValue<winrt::Windows::Foundation::IInspectable>(context, sender),
                    RC(context, args));
            });
        });
    }

    template <typename T, auto SC, auto RC>
    jsi::Value ConvertTypedEventHandlerToValue(const std::shared_ptr<ProjectionsContext>& context, const T& value)
    {
        static auto s_function =
            ProjectedFunction::Create("invoke"sv, ProjectedFunctionOverload::Create<T>(&T::operator(), &SC, &RC));
        return jsi::Value(context->Runtime, s_function->GetFunction<T>(value, context));
    }

    template <typename T, auto SC, auto RC>
    T ConvertValueToTypedEventHandler(const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return T([context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](
                     const auto& sender, const auto& args) {
            context->Invoker->CallSync(
                [&]() { function.call(context->Runtime, SC(context, sender), RC(context, args)); });
        });
    }

    template <typename K, typename V>
    jsi::Value ConvertMapChangedEventHandlerToValue(const std::shared_ptr<ProjectionsContext>& context,
        const winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>& value)
    {
        static auto s_function = ProjectedFunction::Create("invoke"sv,
            ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>(
                &winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>::operator(),
                &ConvertValueToInterface<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>,
                &ConvertValueToInterface<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>));
        return jsi::Value(context->Runtime,
            s_function->GetFunction<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>(
                value, context));
    }

    template <typename K, typename V>
    winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V> ConvertValueToMapChangedEventHandler(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>(
            [context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](
                const winrt::Windows::Foundation::Collections::IObservableMap<K, V>& sender,
                const winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>& event) {
                context->Invoker->CallSync([&]() {
                    function.call(context->Runtime,
                        ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>(
                            context, sender),
                        ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>>(
                            context, event));
                });
            });
    }

    template <typename T>
    jsi::Value ConvertVectorChangedEventHandlerToValue(const std::shared_ptr<ProjectionsContext>& context,
        const winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>& value)
    {
        static auto s_function = ProjectedFunction::Create("invoke"sv,
            ProjectedFunctionOverload::Create<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>(
                &winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>::operator(),
                &ConvertValueToInterface<winrt::Windows::Foundation::Collections::IObservableVector<T>>,
                &ConvertValueToInterface<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>));
        return jsi::Value(context->Runtime,
            s_function->GetFunction<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>(
                value, context));
    }

    template <typename T>
    winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T> ConvertValueToVectorChangedEventHandler(
        const std::shared_ptr<ProjectionsContext>& context, const jsi::Value& value)
    {
        return winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>(
            [context, function{ value.asObject(context->Runtime).asFunction(context->Runtime) }](
                const winrt::Windows::Foundation::Collections::IObservableVector<T>& sender,
                const winrt::Windows::Foundation::Collections::IVectorChangedEventArgs& event) {
                context->Invoker->CallSync([&]() {
                    function.call(context->Runtime,
                        ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IObservableVector<T>>(
                            context, sender),
                        ConvertInterfaceToValue<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs>(
                            context, event));
                });
            });
    }
}
