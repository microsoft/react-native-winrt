#pragma once
#include "JsiHelpers.h"

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
        virtual void Remove(I& instance, const winrt::event_token& token) const = 0;
    };

    template <typename I, typename A, typename R, typename C>
    class ProjectedEventWrapper final : public IProjectedEvent<I>
    {
    public:
        ProjectedEventWrapper(const std::string_view& name, A add, R remove, C convertValueToDelegate) :
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

        virtual void Remove(I& instance, const winrt::event_token& token) const override
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
        static std::shared_ptr<IProjectedEventBase> Create(const std::string_view& name,
            winrt::event_token (J::*add)(const D&) const, void (J::*remove)(const winrt::event_token&) const,
            ValueToNativeConverter<D> convertValueToDelegate)
        {
            return std::shared_ptr<IProjectedEventBase>(
                new ProjectedEventWrapper<I, decltype(add), decltype(remove), decltype(convertValueToDelegate)>(
                    name, add, remove, convertValueToDelegate));
        }
    };

    class ProjectedEventInstance final
    {
    public:
        ProjectedEventInstance(const std::shared_ptr<IProjectedEventBase>& event);
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