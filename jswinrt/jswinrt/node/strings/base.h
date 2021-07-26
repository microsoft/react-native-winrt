#pragma once

// #ifndef NOMINMAX
// #define NOMINMAX 1
// #endif

// #ifndef WIN32_LEAN_AND_MEAN
// #define WIN32_LEAN_AND_MEAN 1
// #endif

// #include <WeakReference.h>
#include <atomic>
// #include <charconv>
#include <chrono>
#include <node.h>
#include <string_view>
#include <thread>
// #include <unordered_map>
// #include <variant>
#include <v8.h>
#include <vector>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>

// Common helpers/types
namespace nodewinrt
{
    // TODO: Switch to std::span when available. For now, this is just a subset that we need
    template <typename T>
    struct span
    {
        constexpr span() = default;

        constexpr span(T* data, std::size_t size) noexcept : m_data(data), m_size(size)
        {
        }

        template <std::size_t Size>
        constexpr span(T (&arr)[Size]) noexcept : m_data(arr), m_size(Size)
        {
        }

        constexpr T* data() const noexcept
        {
            return m_data;
        }

        constexpr std::size_t size() const noexcept
        {
            return m_size;
        }

        constexpr bool empty() const noexcept
        {
            return m_size == 0;
        }

        constexpr T& operator[](std::size_t index) const noexcept
        {
            assert(index < m_size);
            return m_data[index];
        }

        constexpr T* begin() const noexcept
        {
            return m_data;
        }

        constexpr T* end() const noexcept
        {
            return m_data + m_size;
        }

    private:
        T* m_data = nullptr;
        std::size_t m_size = 0;
    };

    // NOTE: This is a std::vector-like type that uses a small-size buffer optimization
    template <typename T, size_t BufferSize = 8>
    struct sso_vector
    {
        template <typename U, size_t OtherBufferSize>
        friend struct sso_vector;

        sso_vector() = default;

        sso_vector(const sso_vector& other)
        {
            copy_construct(other);
        }

        template <size_t OtherBufferSize>
        sso_vector(const sso_vector<T, OtherBufferSize>& other)
        {
            copy_construct(other);
        }

        sso_vector(sso_vector&& other)
        {
            move_construct(other);
        }

        template <size_t OtherBufferSize>
        sso_vector(sso_vector<T, OtherBufferSize>&& other)
        {
            move_construct(other);
        }

        sso_vector& operator=(const sso_vector& other)
        {
            if (&other != this)
            {
                copy_assign(other);
            }

            return *this;
        }

        template <size_t OtherBufferSize>
        sso_vector& operator=(const sso_vector<T, OtherBufferSize>& other)
        {
            copy_assign(other);
            return *this;
        }

        sso_vector& operator=(sso_vector&& other)
        {
            if (&other != this)
            {
                move_assign(other);
            }
            return *this;
        }

        template <size_t OtherBufferSize>
        sso_vector& operator=(sso_vector<T, OtherBufferSize>&& other)
        {
            move_assign(other);
            return *this;
        }

        ~sso_vector()
        {
            destroy_buffer();
        }

        size_t size() const noexcept
        {
            return m_size;
        }

        bool empty() const noexcept
        {
            return m_size == 0;
        }

        T& operator[](size_t index) noexcept
        {
            assert(index < m_size);
            return data()[index];
        }

        const T& operator[](size_t index) const noexcept
        {
            assert(index < m_size);
            return data()[index];
        }

        T& back() noexcept
        {
            assert(!empty());
            return data()[m_size - 1];
        }

        const T& back() const noexcept
        {
            assert(!empty());
            return data()[m_size - 1];
        }

        T* data() noexcept
        {
            return is_locally_allocated() ? local_data() : m_data.pointer;
        }

        const T* data() const noexcept
        {
            return is_locally_allocated() ? local_data() : m_data.pointer;
        }

        void reserve(size_t size)
        {
            // NOTE: Capacity will never fall below SSO buffer size, so this is always an allocate
            if (size > m_capacity)
            {
                auto ptr = static_cast<T*>(::operator new(size * sizeof(T)));
                try
                {
                    std::uninitialized_move(begin(), end(), ptr);
                }
                catch (...)
                {
                    ::operator delete(ptr);
                    throw;
                }

                destroy_buffer();
                m_capacity = size;
                m_data.pointer = ptr;
            }
        }

        void resize(size_t size)
        {
            reserve(size);
            auto ptr = data() + m_size;
            while (m_size < size)
            {
                ::new (ptr) T();
                ++m_size;
                ++ptr;
            }
        }

        void resize(size_t size, const T& value)
        {
            reserve(size);
            auto ptr = data() + m_size;
            while (m_size < size)
            {
                ::new (ptr) T(value);
                ++m_size;
                ++ptr;
            }
        }

        void clear() noexcept
        {
            destroy_buffer();
            m_size = 0;
            m_capacity = BufferSize;
        }

        void push_back(const T& value)
        {
            if (m_size == m_capacity)
            {
                reserve(m_capacity * 2);
            }

            ::new (data() + m_size) T(value);
            ++m_size;
        }

        void push_back(T&& value)
        {
            if (m_size == m_capacity)
            {
                reserve(m_capacity * 2);
            }

            ::new (data() + m_size) T(std::move(value));
            ++m_size;
        }

        template <typename... Args>
        void emplace_back(Args&&... args)
        {
            if (m_size == m_capacity)
            {
                reserve(m_capacity * 2);
            }

            ::new (data() + m_size) T(std::forward<Args>(args)...);
            ++m_size;
        }

        void pop_back() noexcept
        {
            --m_size;
            data()[m_size].~T();
        }

        T* begin() noexcept
        {
            return data();
        }

        const T* begin() const noexcept
        {
            return data();
        }

        T* end() noexcept
        {
            return begin() + m_size;
        }

        const T* end() const noexcept
        {
            return begin() + m_size;
        }

        const T* cbegin() const noexcept
        {
            return begin();
        }

        const T* cend() const noexcept
        {
            return end();
        }

    private:
        template <size_t OtherBufferSize>
        void copy_construct(const sso_vector<T, OtherBufferSize>& other)
        {
            assert(m_size == 0);
            reserve(other.m_size);
            std::uninitialized_copy(other.begin(), other.end(), data());
            m_size = other.m_size;
        }

        template <size_t OtherBufferSize>
        void copy_assign(const sso_vector<T, OtherBufferSize>& other)
        {
            if (other.m_size > m_capacity)
            {
                destroy_buffer();
                m_size = 0;
                m_capacity = BufferSize;
                copy_construct(other);
            }
            else
            {
                // We either need to copy and construct new elements or copy and destroy old elements
                auto assignCount = std::min(m_size, other.m_size);
                std::copy(other.begin(), other.begin() + assignCount, begin());

                if (m_size > other.m_size)
                {
                    std::destroy(begin() + assignCount, end());
                }
                else
                {
                    std::uninitialized_copy(other.begin() + assignCount, other.end(), begin() + assignCount);
                }

                m_size = other.m_size;
            }
        }

        template <size_t OtherBufferSize>
        void move_construct(sso_vector<T, OtherBufferSize>& other)
        {
            // TODO: We can "steal" the buffer in more scenarios
            assert(m_size == 0);
            if (other.m_size <= m_capacity)
            {
                std::uninitialized_move(other.begin(), other.end(), data());
                m_size = other.m_size;
                other.destroy_buffer();
                other.m_size = 0;
                other.m_capacity = OtherBufferSize; // NOTE: Could be less than our buffer size, so can't assume
            }
            else if (!other.is_locally_allocated())
            {
                m_data.pointer = other.m_data.pointer;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                other.m_size = 0;
                other.m_capacity = OtherBufferSize;
            }
            else
            {
                reserve(other.m_size);
                std::uninitialized_move(other.begin(), other.end(), data());
                m_size = other.m_size;
                other.destroy_buffer();
                other.m_size = 0; // NOTE: Capacity already correct since it's locally allocated
            }
        }

        template <size_t OtherBufferSize>
        void move_assign(sso_vector<T, OtherBufferSize>& other)
        {
            if (other.is_locally_allocated())
            {
                // Scenario 1: Other is locally allocated. We need to move elements to our own buffer
                if (other.m_size <= m_capacity)
                {
                    // Scenario 1.1: Current buffer is large enough. We need to move assign and then either construct or
                    // destroy extra elements
                    auto assignCount = std::min(m_size, other.m_size);
                    std::move(other.begin(), other.begin() + assignCount, begin());

                    if (m_size > other.m_size)
                    {
                        std::destroy(begin() + assignCount, end());
                    }
                    else
                    {
                        std::uninitialized_move(other.begin() + assignCount, other.end(), begin() + assignCount);
                    }
                }
                else
                {
                    // Scenario 1.2: Current buffer is too small. We need to move elements into a new buffer
                    destroy_buffer();
                    m_size = 0;
                    m_capacity = BufferSize;
                    reserve(other.m_size);
                    std::uninitialized_move(other.begin(), other.end(), data());
                }

                m_size = other.m_size;
                other.destroy_buffer();
                other.m_size = 0; // NOTE: Capacity already set to buffer size
            }
            else if (other.m_capacity <= BufferSize)
            {
                // Scenario 2: Other is heap allocated, but its capacity is not greater than our buffer size, so
                // assuming ownership of the pointer would break assumptions. Keep our own buffer (we know it must be
                // large enough), so that if it's already heap allocated, we may save the need to do a future allocation
                auto assignCount = std::min(m_size, other.m_size);
                std::move(other.begin(), other.begin() + assignCount, begin());

                if (m_size > other.m_size)
                {
                    std::destroy(begin() + assignCount, end());
                }
                else
                {
                    std::uninitialized_move(other.begin() + assignCount, other.end(), begin() + assignCount);
                }

                m_size = other.m_size;
                other.destroy_buffer();
                other.m_size = 0;
                other.m_capacity = OtherBufferSize;
            }
            else
            {
                // Scenario 3: Other is heap allocated and we can assume ownership of the buffer
                destroy_buffer();
                m_data.pointer = other.m_data.pointer;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
                other.m_size = 0;
                other.m_capacity = OtherBufferSize;
            }
        }

        bool is_locally_allocated() const noexcept
        {
            return m_capacity == BufferSize;
        }

        T* local_data() noexcept
        {
            return reinterpret_cast<T*>(&m_data.buffer);
        }

        const T* local_data() const noexcept
        {
            return reinterpret_cast<const T*>(&m_data.buffer);
        }

        // NOTE: Caller is in charge of setting size/capacity correctly
        void destroy_buffer()
        {
            static_assert(std::is_nothrow_destructible_v<T>);
            if (is_locally_allocated())
            {
                auto ptr = local_data();
                std::destroy(ptr, ptr + m_size);
            }
            else
            {
                auto ptr = m_data.pointer;
                std::destroy(ptr, ptr + m_size);
                ::operator delete(ptr);
            }
        }

        size_t m_size = 0;
        size_t m_capacity = BufferSize;
        union
        {
            std::aligned_storage_t<BufferSize * sizeof(T), alignof(T)> buffer;
            T* pointer;
        } m_data;
    };

#if 0
    // Used to capture a lambda whose capture includes non-copyable types in scenarios where a copy constructor is
    // needed (e.g. std::function), but is never used in practice.
    template <typename TLambda>
    struct move_only_lambda
    {
        move_only_lambda(TLambda lambda) : m_lambda(std::move(lambda))
        {
        }

        move_only_lambda(move_only_lambda&&) = default;
        move_only_lambda& operator=(move_only_lambda&&) = default;

        move_only_lambda(const move_only_lambda& other) : m_lambda(std::move(const_cast<TLambda&>(other.m_lambda)))
        {
            winrt::terminate();
        }

        move_only_lambda& operator=(const move_only_lambda&)
        {
            winrt::terminate();
        }

        template <typename... Args>
        auto operator()(Args&&... args)
        {
            return m_lambda(std::forward<Args>(args)...);
        }

    private:
        TLambda m_lambda;
    };

    template <typename T, typename U, typename Return, typename... Args>
    auto bind_this(Return (U::*fn)(Args...), T* pThis)
    {
        return [fn, pThis](Args... args) { return (pThis->*fn)(std::forward<Args>(args)...); };
    }

    template <typename T, typename U, typename Return, typename... Args>
    auto bind_this(Return (U::*fn)(Args...) const, const T* pThis)
    {
        return [fn, pThis](Args... args) { return (pThis->*fn)(std::forward<Args>(args)...); };
    }
#endif

    inline std::optional<uint32_t> index_from_name(std::string_view name)
    {
        auto begin = name.data();
        auto end = begin + name.size();
        uint32_t index;
        auto [ptr, ec] = std::from_chars(begin, end, index);
        if ((ptr == end) && (ec == std::errc{}))
        {
            return index;
        }

        return std::nullopt;
    }
}

// Per-context data
namespace nodewinrt
{
    struct runtime_context;

    struct object_instance_cache
    {
        // Maps an IInspectable pointer to its wrapping JS value, stored as a weak reference. Note that it is possible
        // for a WinRT object to get destroyed and have its memory location re-used for another object. This is OKAY
        // since the object we create holds a strong reference to the WinRT object and therefore the WinRT object
        // getting destroyed would imply that the JS object also got destroyed
        std::unordered_map<void*, v8::Global<v8::Value>> instances;

        // TODO: Probably want destructor to clean up instances? Need to better figure out how v8's memory management
        // works first.

        v8::Local<v8::Value> get_instance(
            runtime_context* context, const winrt::Windows::Foundation::IInspectable& value);

#if 0
        // TODO: Ideally this would be a periodic thing (on a timer), but for now we'll manually trigger cleanup
        // occasionally when we're querying for object instances
        std::chrono::steady_clock::time_point last_cleanup = std::chrono::steady_clock::now();

        void cleanup(jsi::Runtime& runtime)
        {
            last_cleanup = std::chrono::steady_clock::now();
            for (auto itr = instances.begin(); itr != instances.end();)
            {
                if (supports_weak_object)
                {
                    if (auto value = std::get<0>(itr->second).lock(runtime); !value.isUndefined()) // Object still alive
                    {
                        ++itr;
                    }
                    else // Object was destroyed
                    {
                        itr = instances.erase(itr);
                    }
                }
                else
                {
                    if (std::get<1>(itr->second).lock()) // Object still alive
                    {
                        ++itr;
                    }
                    else // Object was destroyed
                    {
                        itr = instances.erase(itr);
                    }
                }
            }
        }
#endif
    };

    struct runtime_context
    {
        v8::Isolate* isolate;
        std::thread::id thread_id = std::this_thread::get_id();
        object_instance_cache instance_cache;

        // Cache the templates for creating the various object types that we create
        v8::Global<v8::ObjectTemplate> namespace_template;
        v8::Global<v8::ObjectTemplate> enum_template;
        v8::Global<v8::ObjectTemplate> statics_class_template;
        v8::Global<v8::ObjectTemplate> activatable_class_template;
        v8::Global<v8::ObjectTemplate> object_instance_template;

        runtime_context(v8::Isolate* isolate);

#if 0
        std::function<void(std::function<void()>)> call_invoker;

        // TODO: Currently each of these objects handles its own periodic cleanup. It would be great if we could do this
        // on a more well-defined basis for both (e.g. on a timer)
        object_event_cache event_cache;

        void call(std::function<void()> fn) const
        {
            if (thread_id == std::this_thread::get_id())
            {
                fn();
            }
            else
            {
                call_invoker(std::move(fn));
            }
        }

        void call_async(std::function<void()> fn) const
        {
            call_invoker(std::move(fn));
        }

        void call_sync(std::function<void()> fn) const
        {
            if (thread_id == std::this_thread::get_id())
            {
                fn();
            }
            else
            {
                winrt::handle event(::CreateEventW(nullptr, true, false, nullptr));
                if (!event.get())
                {
                    winrt::throw_last_error();
                }

                std::exception_ptr exception;
                call_invoker([&]() {
                    try
                    {
                        fn();
                    }
                    catch (...)
                    {
                        exception = std::current_exception();
                    }
                    ::SetEvent(event.get());
                });

                if (::WaitForSingleObject(event.get(), INFINITE) != WAIT_OBJECT_0)
                {
                    winrt::terminate();
                }

                if (exception)
                {
                    std::rethrow_exception(exception);
                }
            }
        }

        // For the most part, these pointers should only be accessed from the JS thread and therefore the reference
        // count does not need to be bumped, but for things like asynchronous operations, we may need to take strong
        // references to ensure the object does not get destroyed during use
        std::atomic_uint32_t ref_count{ 1 };

        [[nodiscard]] shared_runtime_context add_reference()
        {
            assert(thread_id == std::this_thread::get_id());
            assert(ref_count.load() >= 1);
            return shared_runtime_context{ this }; // NOTE: Bumps ref count
        }

        void release()
        {
            if (--ref_count == 0)
            {
                delete this;
            }
        }
#endif
    };

#if 0
    // A shared, strong reference to the context, for (safe) use off the JS thread
    struct shared_runtime_context
    {
        runtime_context* pointer = nullptr;

        shared_runtime_context() = default;
        shared_runtime_context(runtime_context* ptr);

        shared_runtime_context(const shared_runtime_context& other) : shared_runtime_context(other.pointer)
        {
        }

        shared_runtime_context(shared_runtime_context&& other) : pointer(other.pointer)
        {
            other.pointer = nullptr;
        }

        ~shared_runtime_context();

        shared_runtime_context& operator=(const shared_runtime_context& other);

        shared_runtime_context& operator=(shared_runtime_context&& other)
        {
            std::swap(pointer, other.pointer);
            return *this;
        }

        runtime_context* operator->() noexcept
        {
            return pointer;
        }

        const runtime_context* operator->() const noexcept
        {
            return pointer;
        }

        runtime_context& operator*() noexcept
        {
            return *pointer;
        }

        const runtime_context& operator*() const noexcept
        {
            return *pointer;
        }
    };

    // TODO: Figure out a good interval for performing cleanup
    static constexpr auto cleanup_interval = 5min;

    // A reusable mapping of (delegate, event name) -> event_token for a single
    struct event_registration_array
    {
        // NOTE: It's possible to re-use the same callback for multiple events on the same object (especially since
        // we're talking about JS here), so we include the event name in the mapping here. Since event names are all
        // stored as static data, we primarily use it as an integer id and perform pointer comparison.
        struct registration_data
        {
            jsi::Object object;
            const char* event_name;
            winrt::event_token token;
        };

        sso_vector<registration_data, 4> registrations;

        bool empty() const noexcept
        {
            return registrations.empty();
        }

        void add(jsi::Object object, const char* eventName, winrt::event_token token)
        {
            registrations.push_back({ std::move(object), eventName, token });
        }

        winrt::event_token remove(jsi::Runtime& runtime, const jsi::Object& object, const char* eventName)
        {
            for (auto& data : registrations)
            {
                if ((data.event_name == eventName) && jsi::Object::strictEquals(runtime, data.object, object))
                {
                    auto result = data.token;
                    data = std::move(registrations.back());
                    registrations.pop_back();
                    return result;
                }
            }

            return {};
        }
    };

    struct object_event_cache
    {
        // NOTE: Since we currently hold strong references to the function objects being used as delegates, and just
        // cleanup in general, we hold a weak reference to the WinRT object and periodically try and clean the map up.
        // Note that some objects don't support weak references, in which case we just forgo this cleanup and instead
        // rely on the consumer to clean up registrations.
        struct instance_data
        {
            winrt::weak_ref<winrt::Windows::Foundation::IInspectable> weak_ref;
            event_registration_array registrations;
        };

        std::unordered_map<void*, instance_data> events;

        // TODO: Ideally this would be a periodic thing (on a timer), but for now we'll manually trigger cleanup
        // occasionally when adding tokens
        std::chrono::steady_clock::time_point last_cleanup = std::chrono::steady_clock::now();

        void add(const winrt::Windows::Foundation::IInspectable& instance, jsi::Object object, const char* eventName,
            winrt::event_token token)
        {
            if ((std::chrono::steady_clock::now() - last_cleanup) >= cleanup_interval)
            {
                cleanup();
            }

            auto ptr = winrt::get_abi(instance);
            auto& data = events[ptr];
            if (data.weak_ref && !data.weak_ref.get())
            {
                data = {}; // Object address was re-used; clean up the registrations
            }

            if (data.registrations.empty())
            {
                // NOTE: C++/WinRT has no 'try_make_weak_ref' or equivalent, so doing it manually here...
                assert(!data.weak_ref);
                if (auto src = instance.try_as<::IWeakReferenceSource>())
                {
                    winrt::check_hresult(
                        src->GetWeakReference(reinterpret_cast<::IWeakReference**>(data.weak_ref.put())));
                }
            }

            data.registrations.add(std::move(object), eventName, token);
        }

        winrt::event_token remove(jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& instance,
            const jsi::Object& object, const char* eventName)
        {
            auto ptr = winrt::get_abi(instance);
            if (auto itr = events.find(ptr); itr != events.end())
            {
                auto result = itr->second.registrations.remove(runtime, object, eventName);
                if (itr->second.registrations.empty())
                {
                    events.erase(itr);
                }

                return result;
            }

            return {};
        }

        void cleanup()
        {
            last_cleanup = std::chrono::steady_clock::now();
            for (auto itr = events.begin(); itr != events.end();)
            {
                if (itr->second.weak_ref.get()) // Object still alive
                {
                    ++itr;
                }
                else // Object was destroyed
                {
                    itr = events.erase(itr);
                }
            }
        }
    };

    inline shared_runtime_context::shared_runtime_context(runtime_context* ptr) : pointer(ptr)
    {
        if (pointer)
        {
            ++pointer->ref_count;
        }
    }

    inline shared_runtime_context& shared_runtime_context::operator=(const shared_runtime_context& other)
    {
        if (this != &other)
        {
            if (pointer)
            {
                pointer->release();
            }

            pointer = other.pointer;
            if (pointer)
            {
                ++pointer->ref_count;
            }
        }

        return *this;
    }

    inline shared_runtime_context::~shared_runtime_context()
    {
        if (pointer)
        {
            pointer->release();
        }
    }

    runtime_context* current_runtime_context();
#endif
}

// Generic v8 helpers
namespace nodewinrt
{
    using namespace std::literals;

    using call_function_t = void (*)(runtime_context*, const v8::FunctionCallbackInfo<v8::Value>&);

    using static_get_property_t = v8::Local<v8::Value> (*)(runtime_context*);
    using static_set_property_t = void (*)(runtime_context*, v8::Local<v8::Value>);
    using static_add_event_t = winrt::event_token (*)(runtime_context*, v8::Local<v8::Value>);
    using static_remove_event_t = void (*)(winrt::event_token);

    using instance_get_property_t = v8::Local<v8::Value> (*)(runtime_context*, const winrt::Windows::Foundation::IInspectable&);
    using instance_set_property_t = void (*)(
        runtime_context*, const winrt::Windows::Foundation::IInspectable&, v8::Local<v8::Value>);
    using instance_add_event_t = winrt::event_token (*)(
        runtime_context*, const winrt::Windows::Foundation::IInspectable&, v8::Local<v8::Value>);
    using instance_remove_event_t = void (*)(const winrt::Windows::Foundation::IInspectable&, winrt::event_token);
    using instance_call_function_t = void (*)(
        runtime_context*, const winrt::Windows::Foundation::IInspectable&, const v8::FunctionCallbackInfo<v8::Value>&);
    // TODO using instance_runtime_get_property_t = std::pair<std::optional<jsi::Value>, std::optional<jsi::Value>> (*)(
    //     jsi::Runtime&, const winrt::Windows::Foundation::IInspectable&, std::string_view);
    // TODO using instance_runtime_set_property_t = bool (*)(
    //     jsi::Runtime&, const winrt::Windows::Foundation::IInspectable&, std::string_view, const jsi::Value&);

    inline constexpr std::string_view add_event_name = "addEventListener"sv;
    inline constexpr std::string_view remove_event_name = "removeEventListener"sv;

    struct runtime_context;

    void register_exports(v8::Local<v8::Context> context, v8::Local<v8::Object> exports);

    inline v8::Local<v8::String> make_string_checked(v8::Isolate* isolate, std::string_view str)
    {
        return v8::String::NewFromUtf8(isolate, str.data(), v8::NewStringType::kNormal, static_cast<int>(str.size()))
            .ToLocalChecked();
    }

    v8::Local<v8::String> make_string_checked(v8::Isolate* isolate, std::wstring_view str);

    inline std::string string_to_utf8(v8::Isolate* isolate, v8::Local<v8::String> str)
    {
        // TODO: 'resize' will fill with zero even though we are about to write over the same memory... It would perhaps
        // be better if we just allocated our own buffer and returned a "string"-like thing
        std::string result;
        result.resize(str->Utf8Length(isolate));
        str->WriteUtf8(isolate, result.data());
        return result;
    }

#if 0
    std::u16string string_to_utf16(jsi::Runtime& runtime, const jsi::String& string);
#endif

    // v8 doesn't deal directly with exceptions, but C++/WinRT does. So, since we have to deal with them anyway, we
    // declare this exception type mostly to keep the control flow the same. This type mostly serves as a tag type. That
    // is, catching this type is an indication that we've already called Isolate::ThrowException.
    struct v8_exception
    {
        static v8_exception from_internal()
        {
            return v8_exception();
        }

        static v8_exception error(v8::Isolate* isolate, v8::Local<v8::String> msg)
        {
            return v8_exception(isolate, v8::Exception::Error(msg));
        }

        static v8_exception error(v8::Isolate* isolate, std::string_view msg)
        {
            return error(isolate, make_string_checked(isolate, msg));
        }

        static v8_exception type_error(v8::Isolate* isolate, v8::Local<v8::String> msg)
        {
            return v8_exception(isolate, v8::Exception::TypeError(msg));
        }

        static v8_exception type_error(v8::Isolate* isolate, std::string_view msg)
        {
            return type_error(isolate, make_string_checked(isolate, msg));
        }

        static v8_exception range_error(v8::Isolate* isolate, v8::Local<v8::String> msg)
        {
            return v8_exception(isolate, v8::Exception::RangeError(msg));
        }

        static v8_exception range_error(v8::Isolate* isolate, std::string_view msg)
        {
            return range_error(isolate, make_string_checked(isolate, msg));
        }

        // TODO

    private:

        v8_exception() = default;

        v8_exception(v8::Isolate* isolate, v8::Local<v8::Value> exception)
        {
            isolate->ThrowException(exception);
        }
    };

    inline void handle_error(v8::Isolate* isolate)
    {
        try
        {
            throw;
        }
        catch (v8_exception&)
        {
            // Already taken care of (TODO: Can we validate)
        }
        catch (std::exception& e)
        {
            isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, e.what()).ToLocalChecked()));
        }
        catch (...)
        {
            // Generic error
            isolate->ThrowException(v8::Exception::Error(
                v8::String::NewFromUtf8(isolate, "An unknown exception has occurred").ToLocalChecked()));
        }
    }

#define V8_CATCH(isolate) catch (...) { handle_error(isolate); }

    // Exception methods, to help reduce binary size
    [[noreturn]] __declspec(noinline) void throw_no_constructor(
        v8::Isolate* isolate, std::string_view typeNamespace, std::string_view typeName, int argCount);

    [[noreturn]] __declspec(noinline) void throw_no_function_overload(v8::Isolate* isolate,
        std::string_view typeNamespace, std::string_view typeName, std::string_view fnName, int argCount);

    [[noreturn]] __declspec(noinline) void throw_invalid_delegate_arg_count(
        v8::Isolate* isolate, std::string_view typeNamespace, std::string_view typeName);

    template <typename T>
    inline v8::Local<v8::Object> cast_object(v8::Isolate* isolate, v8::Local<T> value)
    {
        // NOTE: 'null' is ignored here and will still throw. Callers who care need to check for themselves
        if (!value->IsObject())
        {
            throw v8_exception::type_error(isolate, "Expected an object");
        }

        return value.As<v8::Object>();
    }

    template <typename T>
    inline v8::Local<v8::Array> cast_array(v8::Isolate* isolate, v8::Local<T> value)
    {
        if (!value->IsArray())
        {
            throw v8_exception::type_error(isolate, "Expected an array");
        }

        return value.As<v8::Array>();
    }

    template <typename T>
    inline v8::Local<v8::String> cast_string(v8::Isolate* isolate, v8::Local<T> value)
    {
        if (!value->IsString())
        {
            throw v8_exception::type_error(isolate, "Expected a string");
        }

        return value.As<v8::String>();
    }

    template <typename T>
    inline T check_maybe(v8::Maybe<T> maybe)
    {
        if (!maybe.IsJust())
        {
            // TODO: Is there a way to actually validate that an exception is pending?
            throw v8_exception::from_internal();
        }

        return maybe.FromJust();
    }

    template <typename T>
    inline v8::Local<T> check_maybe(v8::MaybeLocal<T> maybe)
    {
        if (maybe.IsEmpty())
        {
            // TODO: Is there a way to actually validate that an exception is pending?
            throw v8_exception::from_internal();
        }

        return maybe.ToLocalChecked();
    }

    inline bool has_property(
        v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> obj, std::string_view name)
    {
        return check_maybe(obj->Has(context, make_string_checked(isolate, name)));
    }

    template <typename T>
    inline void set_property(
        runtime_context* context, v8::Local<v8::Context> ctxt, v8::Local<v8::Object> target, uint32_t index, T&& value)
    {
        auto result = target->Set(ctxt, index, convert_native_to_value(context, std::forward<T>(value)));

        // NOTE: From documentation, result will always be true or not set (error)
        check_maybe(result);
        assert(result.FromJust());
    }

    template <typename T>
    inline void set_property(runtime_context* context, v8::Local<v8::Context> ctxt, v8::Local<v8::Object> target,
        std::string_view key, T&& value)
    {
        auto result = target->Set(
            ctxt, make_string_checked(context->isolate, key), convert_native_to_value(context, std::forward<T>(value)));

        // NOTE: From documentation, result will always be true or not set (error)
        check_maybe(result);
        assert(result.FromJust());
    }

    inline v8::Local<v8::Value> get_property(v8::Local<v8::Context> ctxt, v8::Local<v8::Object> target, uint32_t index)
    {
        return check_maybe(target->Get(ctxt, index));
    }

    inline v8::Local<v8::Value> get_property(
        v8::Isolate* isolate, v8::Local<v8::Context> ctxt, v8::Local<v8::Object> target, std::string_view key)
    {
        return check_maybe(target->Get(ctxt, make_string_checked(isolate, key)));
    }

    template <typename T>
    inline T get_property_as(
        runtime_context* context, v8::Local<v8::Context> ctxt, v8::Local<v8::Object> target, uint32_t index)
    {
        auto result = check_maybe(target->Get(ctxt, index));
        return convert_value_to_native<T>(context, result);
    }

    template <typename T>
    inline T get_property_as(
        runtime_context* context, v8::Local<v8::Context> ctxt, v8::Local<v8::Object> target, std::string_view key)
    {
        auto result = check_maybe(target->Get(ctxt, make_string_checked(context->isolate, key)));
        return convert_value_to_native<T>(context, result);
    }

    inline void* internal_field_value(v8::Local<v8::Object> obj, int index)
    {
        // NOTE: This function assumes that the caller knows that the field is valid and is an External
        assert(!obj.IsEmpty());
        assert(obj->InternalFieldCount() > index);

        auto field = obj->GetInternalField(index);
        assert(!field.IsEmpty());
        assert(field->IsExternal());

        auto external = field.As<v8::External>();
        assert(!external.IsEmpty());

        auto result = external->Value();
        assert(result != nullptr);
        return result;
    }

    template <typename T>
    inline T* internal_field_value(v8::Local<v8::Object> obj, int index)
    {
        return static_cast<T*>(internal_field_value(obj, index));
    }

#if 0
    template <typename T>
    jsi::Function bind_host_function(jsi::Runtime& runtime, const jsi::PropNameID& name, unsigned int paramCount,
        jsi::Value (T::*fn)(jsi::Runtime&, const jsi::Value*, size_t))
    {
        return jsi::Function::createFromHostFunction(runtime, name, paramCount,
            [fn](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) {
                auto strongThis = thisValue.asObject(runtime).asHostObject<T>(runtime);
                return (strongThis.get()->*fn)(runtime, args, count);
            });
    }

    template <typename T>
    jsi::Function bind_host_function(jsi::Runtime& runtime, const jsi::PropNameID& name, unsigned int paramCount,
        jsi::Value (T::*fn)(jsi::Runtime&, const jsi::Value*, size_t) const)
    {
        return jsi::Function::createFromHostFunction(runtime, name, paramCount,
            [fn](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) {
                auto strongThis = thisValue.asObject(runtime).asHostObject<T>(runtime);
                return (strongThis.get()->*fn)(runtime, args, count);
            });
    }

#endif

    // Value conversions
    template <typename T>
    v8::Local<v8::Value> convert_object_instance_to_value(runtime_context* context, const T& value);

    template <typename T>
    T convert_value_to_object_instance(runtime_context* context, v8::Local<v8::Value> value);

    template <typename T, typename Enable = void>
    struct projected_value_traits
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, const T& value)
        {
            if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>)
            {
                return convert_object_instance_to_value(context, value);
            }
            else
            {
                throw v8_exception::type_error(
                    context->isolate, "Conversion from native to JS not implemented for type '"s + typeid(T).name() +
                                          "'. This is likely caused by the type being in a non-projected namespace");
            }
        }

        static T as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>)
            {
                return convert_value_to_object_instance<T>(context, value);
            }
            else
            {
                throw v8_exception::type_error(
                    context->isolate, "Conversion from JS to native not implemented for type '"s + typeid(T).name() +
                                          "'. This is likely caused by the type being in a non-projected namespace");
            }
        }
    };

    template <typename T>
    auto convert_native_to_value(runtime_context* context, T&& value)
    {
        return projected_value_traits<std::remove_const_t<std::remove_reference_t<T>>>::as_value(
            context, std::forward<T>(value));
    }

    template <typename T>
    auto convert_value_to_native(runtime_context* context, v8::Local<v8::Value> value)
    {
        return projected_value_traits<T>::as_native(context, value);
    }

    template <typename T>
    struct array_to_native_iterator
    {
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = void;
        using reference = T;
        using iterator_category = std::random_access_iterator_tag;

        array_to_native_iterator(runtime_context* context, v8::Local<v8::Array> array, uint32_t index = 0) :
            m_context(context), m_array(array), m_index(index)
        {
        }

        array_to_native_iterator& begin() noexcept
        {
            return *this;
        }

        array_to_native_iterator end() noexcept
        {
            return array_to_native_iterator(m_context, m_array, m_array->Length());
        }

        bool operator==(const array_to_native_iterator& other) const noexcept
        {
            assert(*m_array == *other.m_array);
            return m_index == other.m_index;
        }

        bool operator!=(const array_to_native_iterator& other) const noexcept
        {
            return !(*this == other);
        }

        bool operator<(const array_to_native_iterator& other) const noexcept
        {
            assert(*m_array == *other.m_array);
            return m_index < other.m_index;
        }

        bool operator>(const array_to_native_iterator& other) const noexcept
        {
            assert(*m_array == *other.m_array);
            return m_index > other.m_index;
        }

        bool operator<=(const array_to_native_iterator& other) const noexcept
        {
            return !(*this > other);
        }

        bool operator>=(const array_to_native_iterator& other) const noexcept
        {
            return !(*this < other);
        }

        T operator*()
        {
            return get_property_as<T>(m_context, m_context->isolate->GetCurrentContext(), m_array, m_index);
        }

        T operator[](difference_type index) const
        {
            return get_property_as<T>(m_context, m_context->isolate->GetCurrentContext(), m_array, m_index + index);
        }

        array_to_native_iterator& operator++() noexcept
        {
            ++m_index;
            return *this;
        }

        array_to_native_iterator operator++(int) noexcept
        {
            auto result = *this;
            ++(*this);
            return result;
        }

        array_to_native_iterator& operator+=(difference_type count) noexcept
        {
            m_index += count;
            return *this;
        }

        array_to_native_iterator operator+(difference_type count) noexcept
        {
            auto result = *this;
            result += count;
            return result;
        }

        friend array_to_native_iterator operator+(difference_type count, array_to_native_iterator itr) noexcept
        {
            itr += count;
            return itr;
        }

        array_to_native_iterator& operator--() noexcept
        {
            --m_index;
            return *this;
        }

        array_to_native_iterator operator--(int) noexcept
        {
            auto result = *this;
            --(*this);
            return result;
        }

        array_to_native_iterator& operator-=(difference_type count) noexcept
        {
            m_index -= count;
            return *this;
        }

        array_to_native_iterator operator-(difference_type count) noexcept
        {
            auto result = *this;
            result -= count;
            return result;
        }

        difference_type operator-(const array_to_native_iterator& other) const noexcept
        {
            return static_cast<difference_type>(m_index - other.m_index);
        }

    private:
        runtime_context* m_context;
        v8::Local<v8::Array> m_array;
        uint32_t m_index;
    };

    namespace details
    {
        inline void fill_return_struct(runtime_context*, v8::Local<v8::Context>, v8::Local<v8::Object>)
        {
        }

        template <typename T, typename... Args>
        void fill_return_struct(runtime_context* context, v8::Local<v8::Context> ctxt, v8::Local<v8::Object> target,
            std::string_view name, const T& value, const Args&... args)
        {
            target->Set(ctxt, make_string_checked(context->isolate, name), convert_native_to_value(context, value));
            fill_return_struct(context, ctxt, target, args...);
        }
    }

    template <typename... Args>
    v8::Local<v8::Value> make_void_return_struct(runtime_context* context, const Args&... args)
    {
        auto result = v8::Object::New(context->isolate);
        details::fill_return_struct(context, context->isolate->GetCurrentContext(), result, args...);
        return result;
    }

    template <typename... Args>
    v8::Local<v8::Value> make_return_struct(runtime_context* context, const Args&... args)
    {
        return make_void_return_struct(context, "returnValue", args...);
    }

    struct pinterface_traits_base
    {
        static constexpr bool is_async_with_progress = false;
        static constexpr bool is_async_with_result = false;

        static constexpr bool is_array_convertible = false;
        static constexpr bool is_iterable = false;
        static constexpr bool is_vector_view = false;
        static constexpr bool is_vector = false;
    };

    template <typename T>
    struct pinterface_traits : pinterface_traits_base
    {
    };

    template <>
    struct pinterface_traits<winrt::Windows::Foundation::IAsyncAction> : pinterface_traits_base
    {
        // NOTE: Currently don't need anything to differentiate here
    };

    template <typename TProgress>
    struct pinterface_traits<winrt::Windows::Foundation::IAsyncActionWithProgress<TProgress>> : pinterface_traits_base
    {
        using progress_type = TProgress;
        static constexpr bool is_async_with_progress = true;
    };

    template <typename TResult>
    struct pinterface_traits<winrt::Windows::Foundation::IAsyncOperation<TResult>> : pinterface_traits_base
    {
        using result_type = TResult;
        static constexpr bool is_async_with_result = true;
    };

    template <typename TResult, typename TProgress>
    struct pinterface_traits<winrt::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>> :
        pinterface_traits_base
    {
        using progress_type = TProgress;
        using result_type = TResult;
        static constexpr bool is_async_with_progress = true;
        static constexpr bool is_async_with_result = true;
    };

    template <typename T>
    struct pinterface_traits<winrt::Windows::Foundation::Collections::IIterable<T>> : pinterface_traits_base
    {
        using value_type = T;
        static constexpr bool is_array_convertible = true;
        static constexpr bool is_iterable = true;
    };

    template <typename T>
    struct pinterface_traits<winrt::Windows::Foundation::Collections::IVectorView<T>> : pinterface_traits_base
    {
        using value_type = T;
        static constexpr bool is_array_convertible = true;
        static constexpr bool is_vector_view = true;
    };

    template <typename T>
    struct pinterface_traits<winrt::Windows::Foundation::Collections::IVector<T>> : pinterface_traits_base
    {
        using value_type = T;
        static constexpr bool is_array_convertible = true;
        static constexpr bool is_vector = true;
    };

#if 0
    struct promise_wrapper
    {
        static promise_wrapper create(jsi::Runtime& runtime)
        {
            // NOTE: The promise callback is called immediately, hence the capture by reference
            std::optional<jsi::Function> resolveFn, rejectFn;
            auto callback = jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "callback"), 2,
                [&](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count < 2)
                    {
                        throw jsi::JSError(runtime, "Promise callback unexpectedly called with insufficient arguments");
                    }

                    resolveFn = args[0].asObject(runtime).asFunction(runtime);
                    rejectFn = args[1].asObject(runtime).asFunction(runtime);
                    return jsi::Value::undefined();
                });

            auto promise =
                runtime.global().getPropertyAsFunction(runtime, "Promise").callAsConstructor(runtime, callback);
            assert(resolveFn && rejectFn);
            return promise_wrapper(std::move(promise), std::move(*resolveFn), std::move(*rejectFn));
        }

        const jsi::Value& get() const noexcept
        {
            return m_promise;
        }

        void resolve(jsi::Runtime& runtime, const jsi::Value& value)
        {
            assert(!m_completed); // TODO: Throw/fail fast?
            m_completed = true;
            m_resolve.call(runtime, value);
        }

        void reject(jsi::Runtime& runtime, const jsi::Value& value)
        {
            assert(!m_completed); // TODO: Throw/fail fast?
            m_completed = true;
            m_reject.call(runtime, value);
        }

    private:
        promise_wrapper(jsi::Value promise, jsi::Function resolveFn, jsi::Function rejectFn) :
            m_promise(std::move(promise)), m_resolve(std::move(resolveFn)), m_reject(std::move(rejectFn))
        {
        }

        jsi::Value m_promise;
        bool m_completed = false;
        jsi::Function m_resolve;
        jsi::Function m_reject;
    };

    inline bool to_boolean(jsi::Runtime& runtime, const jsi::Value& value)
    {
        // Common case first
        if (value.isBool())
            return value.getBool();
        if (value.isUndefined() || value.isNull())
            return false;
        if (value.isSymbol() || value.isObject())
            return true;
        if (value.isNumber())
            return value.getNumber() != 0;

        assert(value.isString());
        return !value.getString(runtime).utf8(runtime).empty();
    }

    inline double to_number(jsi::Runtime& runtime, const jsi::Value& value)
    {
        if (value.isNumber())
            return value.getNumber();
        if (value.isUndefined())
            return std::numeric_limits<double>::quiet_NaN();
        if (value.isNull())
            return 0;
        if (value.isBool())
            return value.getBool() ? 1 : 0;

        if (value.isString())
        {
            // This is probably not 100% correct, but it should be good enough
            auto str = value.getString(runtime).utf8(runtime);
            double result;
            auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
            if (ec != std::errc{})
                return std::numeric_limits<double>::quiet_NaN();

            return result;
        }

        // Symbol & Object (TODO: Object shouldn't be included here)
        throw jsi::JSError(runtime, "TypeError: Cannot convert value to number");
    }

    inline double to_integer_or_infinity(jsi::Runtime& runtime, const jsi::Value& value)
    {
        auto result = to_number(runtime, value);
        if (std::isnan(result) || (result == 0.0) || (result == -0.0))
            return 0;
        if ((result == std::numeric_limits<double>::infinity()) || (result == -std::numeric_limits<double>::infinity()))
            return result;

        auto integer = std::floor(std::abs(result));
        return (result < 0) ? -integer : integer;
    }
#endif
}

// Types used to store static data
namespace nodewinrt
{
    // NOTE: All of these instances are intended to go into the .text section and hold no state that needs to be free'd,
    // hence the lack of a virtual destructor
    struct static_projection_data
    {
        constexpr static_projection_data(std::string_view name) : name(name)
        {
        }

        virtual v8::Local<v8::Value> create(runtime_context* context) const = 0;

        std::string_view name; // E.g. 'Baz' for the type/namespace/etc. 'Foo.Bar.Baz'
    };

    struct static_namespace_data final : static_projection_data
    {
        constexpr static_namespace_data(std::string_view name, span<const static_projection_data* const> children) :
            static_projection_data(name), children(children)
        {
        }

        virtual v8::Local<v8::Value> create(runtime_context* context) const override;

        span<const static_projection_data* const> children;
    };

    extern const span<const static_namespace_data* const> root_namespaces;

    struct static_enum_data final : static_projection_data
    {
        // Enums are either signed or unsigned 32-bit values in Windows metadata. All of which are representable as
        // 64-bit floating point values (i.e. "number")
        struct value_mapping
        {
            std::string_view name;
            double value;
        };

        constexpr static_enum_data(std::string_view name, span<const value_mapping> values) :
            static_projection_data(name), values(values)
        {
        }

        virtual v8::Local<v8::Value> create(runtime_context* context) const override;

        v8::Local<v8::Value> get_value(std::string_view valueName) const;

        span<const value_mapping> values;
    };

    struct static_class_data : static_projection_data
    {
        struct property_mapping
        {
            std::string_view name;
            static_get_property_t getter;
            static_set_property_t setter;
        };

        struct event_mapping
        {
            std::string_view name;
            static_add_event_t add;
            static_remove_event_t remove;
        };

        struct function_mapping
        {
            std::string_view name;
            call_function_t function;
        };

        constexpr static_class_data(std::string_view name, span<const property_mapping> properties,
            span<const event_mapping> events, span<const function_mapping> functions) :
            static_projection_data(name),
            properties(properties), events(events), functions(functions)
        {
        }

        virtual v8::Local<v8::Value> create(runtime_context* context) const override;

        span<const property_mapping> properties;
        span<const event_mapping> events;
        span<const function_mapping> functions;
    };

    struct static_activatable_class_data final : static_class_data
    {
        constexpr static_activatable_class_data(std::string_view ns, std::string_view name, call_function_t constructor,
            span<const property_mapping> properties, span<const event_mapping> events,
            span<const function_mapping> functions) :
            static_class_data(name, properties, events, functions),
            full_namespace(ns), constructor(constructor)
        {
        }

        virtual v8::Local<v8::Value> create(runtime_context* context) const override;

        std::string_view full_namespace;
        call_function_t constructor;
    };

    // NOTE: We don't need to "create" objects from interfaces - we create objects and populate the interfaces that the
    // object supports - hence the fact that we don't derive from 'static_projection_data' here.
    struct static_interface_data
    {
        struct property_mapping
        {
            // NOTE: Unlike classes, the getter *can* be null
            std::string_view name;
            instance_get_property_t getter;
            instance_set_property_t setter;
        };

        struct event_mapping
        {
            std::string_view name;
            instance_add_event_t add;
            instance_remove_event_t remove;
        };

        struct function_mapping
        {
            std::string_view name;
            instance_call_function_t function;
            unsigned int arity;
            bool is_default_overload;
        };

        constexpr static_interface_data(const winrt::guid& guid, span<const property_mapping> properties,
            span<const event_mapping> events, span<const function_mapping> functions//,
            // TODO instance_runtime_get_property_t runtimeGetProperty = nullptr,
            // TODO instance_runtime_set_property_t runtimeSetProperty = nullptr
            ) :
            guid(guid),
            properties(properties), events(events), functions(functions)// TODO, runtime_get_property(runtimeGetProperty),
            // TODOruntime_set_property(runtimeSetProperty)
        {
        }

        winrt::guid guid;
        span<const property_mapping> properties;
        span<const event_mapping> events;
        span<const function_mapping> functions;

        // Some projected types want the ability to expose functions beyond what's marked on the WinRT interface (e.g.
        // collections types)
        // TODO instance_runtime_get_property_t runtime_get_property;
        // TODO instance_runtime_set_property_t runtime_set_property;
    };

    extern const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map;
}

// Types used for object instances, etc.
namespace nodewinrt
{
    struct projected_namespace
    {
        projected_namespace(runtime_context* context, const static_namespace_data* data) :
            m_context(context), m_data(data)
        {
            m_children.resize(data->children.size());
        }

        // Accessors
        static void property_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void property_query(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Integer>& info);
        static void property_enum(const v8::PropertyCallbackInfo<v8::Array>& info);
        // TODO?
        // static void property_desc(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    private:
        runtime_context* m_context;
        const static_namespace_data* m_data;
        std::vector<v8::Global<v8::Value>> m_children;
    };

    struct projected_enum
    {
        projected_enum(const static_enum_data* data) : m_data(data)
        {
        }

        // Accessors
        static void property_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void property_query(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Integer>& info);
        static void property_enum(const v8::PropertyCallbackInfo<v8::Array>& info);
        // TODO?
        // static void property_desc(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    private:
        const static_enum_data* m_data;
    };

    struct projected_class
    {
        projected_class(runtime_context* context, const static_class_data* data) :
            m_context(context), m_data(data)
        {
            auto size = m_data->functions.size();
            if (!m_data->events.empty())
            {
                size += 2;
            }

            m_functions.resize(size);
        }

        // Constructor, if applicable
        static void construct(const v8::FunctionCallbackInfo<v8::Value>& info);

        // Accessors
        static void property_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void property_setter(
            v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void property_query(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Integer>& info);
        static void property_enum(const v8::PropertyCallbackInfo<v8::Array>& info);
        // TODO?
        // static void property_desc(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

    private:
        static void call_function(const v8::FunctionCallbackInfo<v8::Value>& info);
#if 0
        jsi::Value add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);
        jsi::Value remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);
#endif

        runtime_context* m_context;
        const static_class_data* m_data;
        std::vector<v8::Global<v8::Value>> m_functions;
#if 0
        event_registration_array m_events;
#endif
    };

    struct overloaded_function
    {
        void operator()(runtime_context* context, const winrt::Windows::Foundation::IInspectable& instance,
            const v8::FunctionCallbackInfo<v8::Value>& info);

        // TODO: Figure out a good SSO size (4 might be larger than we need most of the time. Perhaps 2?)
        sso_vector<const static_interface_data::function_mapping*, 4> overloads;
    };

    struct projected_object_instance
    {
        projected_object_instance(runtime_context* context, const winrt::Windows::Foundation::IInspectable& instance);

        // Accessors
        static void property_getter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void property_setter(
            v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void property_query(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Integer>& info);
        static void property_enum(const v8::PropertyCallbackInfo<v8::Array>& info);
        // TODO?
        // static void property_desc(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info);

        const winrt::Windows::Foundation::IInspectable& instance() const noexcept
        {
            return m_instance;
        }

    private:
        static void call_function(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void call_overloaded_function(const v8::FunctionCallbackInfo<v8::Value>& info);
#if 0
        jsi::Value add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);
        jsi::Value remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);
#endif

        runtime_context* m_context;
        winrt::Windows::Foundation::IInspectable m_instance;
        sso_vector<const static_interface_data*> m_interfaces;
        std::unordered_map<std::string_view, v8::Global<v8::Value>> m_functions;

        // Since we can only associate a JS function object with a single void* pointer, it's easiest to just save the
        // data on the 'this' object and use an index into that data for the void* pointer.
        // TODO: Would this be better off being an sso_vector? Since there's relatively few uses of function overloads,
        // a normal std::vector is probably actually the better option here since it's smaller in size.
        std::vector<overloaded_function> m_overloads;
    };

    bool is_projected_object(runtime_context* context, v8::Local<v8::Object> obj);

#if 0
    template <typename IFace>
    struct projected_async_instance :
        public jsi::HostObject,
        public std::enable_shared_from_this<projected_async_instance<IFace>>
    {
    private:
        using traits = pinterface_traits<IFace>;

        struct tag_t
        {
        };

    public:
        static std::shared_ptr<projected_async_instance> create(IFace instance)
        {
            auto result = std::make_shared<projected_async_instance>(std::move(instance), tag_t{});
            result->initialize();

            if constexpr (traits::is_async_with_progress) // IAsync*WithProgress
            {
                result->m_instance.Progress(
                    [weakThis = std::weak_ptr{ result }, ctxt = current_runtime_context()->add_reference()](
                        const auto&, const auto& progress) {
                        ctxt->call([progress, weakThis]() {
                            if (auto strongThis = weakThis.lock())
                            {
                                auto& runtime = current_runtime_context()->runtime;
                                strongThis->on_progress(runtime, convert_native_to_value(runtime, progress));
                            }
                        });
                    });
            }

            return result;
        }

        projected_async_instance(IFace instance, tag_t) : m_instance(std::move(instance))
        {
        }

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& id) override
        {
            static constexpr const struct get_function_t get_functions[] = {
                { "cancel"sv, 0, &projected_async_instance::on_cancel },
                { "catch"sv, 1, &projected_async_instance::on_catch },
                { "done"sv, 3, &projected_async_instance::on_done },
                { "finally"sv, 3, &projected_async_instance::on_finally },
                { "then"sv, 3, &projected_async_instance::on_then },
            };

            return get_impl(runtime, id, get_functions);
        }

        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value&) override
        {
            throw jsi::JSError(runtime, "TypeError: Cannot assign to property '" + name.utf8(runtime) +
                                            "' of a projected WinRT AsyncOperation");
        }

        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override
        {
            std::vector<jsi::PropNameID> result;
            result.reserve(6);

            result.push_back(make_propid(runtime, "cancel"sv));
            result.push_back(make_propid(runtime, "catch"sv));
            result.push_back(make_propid(runtime, "done"sv));
            result.push_back(make_propid(runtime, "finally"sv));
            result.push_back(make_propid(runtime, "then"sv));
            result.push_back(make_propid(runtime, "operation"sv));

            return result;
        }

    private:
        struct get_function_t
        {
            std::string_view name;
            int arg_count;
            jsi::Value (projected_async_instance::*function)(jsi::Runtime&, const jsi::Value*, size_t);
        };

        jsi::Value get_impl(jsi::Runtime& runtime, const jsi::PropNameID& id, span<const get_function_t> getFns)
        {
            auto name = id.utf8(runtime);
            auto itr = std::find_if(getFns.begin(), getFns.end(), [&](auto&& pair) { return pair.name == name; });
            if (itr != getFns.end())
            {
                return jsi::Value(runtime, bind_host_function(runtime, id, itr->arg_count, itr->function));
            }
            else if (name == "operation"sv)
            {
                return current_runtime_context()->instance_cache.get_instance(runtime, m_instance);
            }

            return jsi::Value::undefined();
        }

        struct continuation
        {
            enum
            {
                then_type, // Covers both .then and .catch
                finally_type,
                done_type,
            } kind;
            std::optional<promise_wrapper> promise;
            jsi::Value data[3]; // Contents depend on 'kind'
        };

        enum class state
        {
            pending,
            resolved,
            rejected,
        };

        winrt::fire_and_forget initialize()
        {
            auto strongThis = this->shared_from_this();
            auto ctxt = current_runtime_context()->add_reference();
            auto& runtime = ctxt->runtime;
            auto inst = m_instance;
            try
            {
                if constexpr (!traits::is_async_with_result) // IAsyncAction*
                {
                    co_await inst;
                    ctxt->call(
                        [strongThis, &runtime]() { strongThis->on_completed(runtime, jsi::Value::undefined(), true); });
                }
                else // IAsyncOperation*
                {
                    auto result = co_await inst;
                    ctxt->call([strongThis, &runtime, result = std::move(result)]() {
                        strongThis->on_completed(runtime, convert_native_to_value(runtime, result), true);
                    });
                }
            }
            catch (winrt::hresult_error& err)
            {
                ctxt->call([strongThis, &runtime, err = std::move(err)]() {
                    strongThis->on_completed(runtime, make_error(runtime, err), false);
                });
            }
            catch (std::exception& err)
            {
                ctxt->call([strongThis, &runtime, err = std::move(err)]() {
                    strongThis->on_completed(runtime, make_error(runtime, err), false);
                });
            }
        }

        jsi::Value on_cancel(jsi::Runtime&, const jsi::Value*, size_t)
        {
            m_instance.Cancel();
            return jsi::Value::undefined();
        }

        jsi::Value on_catch(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
        {
            // Prototype: catch(rejectCallback) -> Promise
            // NOTE: Equivalent to 'then(null, rejectCallback)'
            auto undefined = jsi::Value::undefined();
            return handle_then(runtime, undefined, count >= 1 ? args[0] : undefined);
        }

        using handle_continuation_t = void (projected_async_instance::*)(jsi::Runtime&, continuation&&);

        __declspec(noinline) jsi::Value on_done_impl(
            jsi::Runtime& runtime, const jsi::Value* args, size_t count, handle_continuation_t handleContinuation)
        {
            // Prototype: done(resolveCallback, rejectCallback, progressCallback)
            continuation c{ continuation::done_type, {},
                { count >= 1 ? jsi::Value(runtime, args[0]) : jsi::Value::undefined(),
                    count >= 2 ? jsi::Value(runtime, args[1]) : jsi::Value::undefined(),
                    count >= 3 ? jsi::Value(runtime, args[2]) : jsi::Value::undefined() } };
            (this->*handleContinuation)(runtime, std::move(c));

            return jsi::Value::undefined();
        }

        jsi::Value on_done(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
        {
            return on_done_impl(runtime, args, count, &projected_async_instance::handle_continuation);
        }

        __declspec(noinline) jsi::Value on_finally_impl(
            jsi::Runtime& runtime, const jsi::Value* args, size_t count, handle_continuation_t handleContinuation)
        {
            // Prototype: finally(callback) -> Promise
            continuation c{ continuation::finally_type, promise_wrapper::create(runtime),
                { count >= 1 ? jsi::Value(runtime, args[0]) : jsi::Value::undefined() } };

            auto result = jsi::Value(runtime, c.promise->get());
            (this->*handleContinuation)(runtime, std::move(c));

            return result;
        }

        jsi::Value on_finally(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
        {
            return on_finally_impl(runtime, args, count, &projected_async_instance::handle_continuation);
        }

        jsi::Value on_then(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
        {
            // Prototype: then(resolvedCallback, rejectedCallback)
            auto undefined = jsi::Value::undefined();
            return handle_then(runtime, count >= 1 ? args[0] : undefined, count >= 2 ? args[1] : undefined);
        }

        __declspec(noinline) jsi::Value handle_then_impl(jsi::Runtime& runtime, const jsi::Value& resolvedHandler,
            const jsi::Value& rejectedHandler, handle_continuation_t handleContinuation)
        {
            continuation c{ continuation::then_type, promise_wrapper::create(runtime),
                { jsi::Value(runtime, resolvedHandler), jsi::Value(runtime, rejectedHandler) } };

            auto result = jsi::Value(runtime, c.promise->get());
            (this->*handleContinuation)(runtime, std::move(c));

            return result;
        }

        jsi::Value handle_then(
            jsi::Runtime& runtime, const jsi::Value& resolvedHandler, const jsi::Value& rejectedHandler)
        {
            return handle_then_impl(
                runtime, resolvedHandler, rejectedHandler, &projected_async_instance::handle_continuation);
        }

        __declspec(noinline) void handle_continuation_impl(jsi::Runtime& runtime, continuation&& c,
            std::function<void()> (*makeCallback)(projected_async_instance*, jsi::Runtime&, continuation&&))
        {
            if (m_state != state::pending)
            {
                // NOTE: Still expected to be async
                // NOTE: The callback will occur on the same thread, implying that the 'Runtime' instance will still be
                // alive and valid, hence the ref is safe
                current_runtime_context()->call_async(makeCallback(this, runtime, std::move(c)));
            }
            else
            {
                m_continuations.push_back(std::move(c));
            }
        }

        void handle_continuation(jsi::Runtime& runtime, continuation&& c)
        {
            handle_continuation_impl(runtime, std::move(c),
                [](projected_async_instance* pThis, jsi::Runtime& runtime, continuation&& c) -> std::function<void()> {
                    return move_only_lambda(
                        [&runtime, strongThis = pThis->shared_from_this(), cont = std::move(c)]() mutable {
                            strongThis->dispatch_continuation(runtime, cont);
                        });
                });
        }

        void on_progress(jsi::Runtime& runtime, const jsi::Value& progress)
        {
            for (auto&& cont : m_continuations)
            {
                if (cont.kind != continuation::done_type)
                    continue;

                if (!cont.data[2].isObject())
                    continue;

                cont.data[2].getObject(runtime).asFunction(runtime).call(runtime, progress);
            }
        }

        void on_completed(jsi::Runtime& runtime, jsi::Value&& value, bool resolved)
        {
            assert(m_state == state::pending);
            m_state = resolved ? state::resolved : state::rejected;
            m_result = std::move(value);

            for (auto& cont : m_continuations)
            {
                dispatch_continuation(runtime, cont);
            }
        }

        void dispatch_continuation(jsi::Runtime& runtime, continuation& cont)
        {
            assert(m_state != state::pending);
            std::optional<jsi::Value> continuationResult;
            bool resolved = m_state == state::resolved;

            switch (cont.kind)
            {
            case continuation::then_type:
            case continuation::done_type: // Effectively 'then', but with progress and no promise
                if (resolved)
                {
                    if (cont.data[0].isObject())
                    {
                        try
                        {
                            continuationResult =
                                cont.data[0].getObject(runtime).asFunction(runtime).call(runtime, m_result);
                        }
                        catch (const jsi::JSError& err)
                        {
                            resolved = false;
                            continuationResult = jsi::Value(runtime, err.value());
                        }
                    }
                }
                // NOTE: If the resolve handler of a 'Promise.then' throws, it is *not* passed on to the reject handler
                // that was passed in the same '.then' call. We mimic that behavior here, hence the 'else' instead of
                // re-inspecting the value of 'resolved'
                else // rejected
                {
                    if (cont.data[1].isObject())
                    {
                        try
                        {
                            continuationResult =
                                cont.data[1].getObject(runtime).asFunction(runtime).call(runtime, m_result);
                            resolved = true;
                        }
                        catch (const jsi::JSError& err)
                        {
                            // NOTE: 'resolved' already false...
                            continuationResult = jsi::Value(runtime, err.value());
                        }
                    }
                }
                break;

            case continuation::finally_type:
                // NOTE: Finally does not change the value, nor the resolved state, unless there's an exception
                if (cont.data[0].isObject())
                {
                    try
                    {
                        cont.data[0].getObject(runtime).asFunction(runtime).call(runtime, m_result);
                    }
                    catch (const jsi::JSError& err)
                    {
                        resolved = false;
                        continuationResult = jsi::Value(runtime, err.value());
                    }
                }
                break;
            }

            auto& effectiveResult = continuationResult ? *continuationResult : m_result;
            if (cont.promise)
            {
                if (resolved)
                {
                    cont.promise->resolve(runtime, effectiveResult);
                }
                else
                {
                    cont.promise->reject(runtime, effectiveResult);
                }
            }
            else if (!resolved)
            {
                // Failure in 'done' scenario - throw unhandled errors instead of swallowing
                throw jsi::JSError(runtime, jsi::Value(runtime, effectiveResult));
            }
        }

        IFace m_instance;
        sso_vector<continuation, 1> m_continuations;
        state m_state = state::pending;
        jsi::Value m_result;
    };
#endif
}

// Collections wrappers
namespace nodewinrt
{
#if 0
    // Implementations for passing arrays as various iterable types
    template <typename D, typename T>
    struct array_iterator :
        winrt::implements<array_iterator<D, T>, winrt::Windows::Foundation::Collections::IIterator<T>>
    {
        array_iterator(D* target) : target(target->get_strong())
        {
        }

        winrt::hstring GetRuntimeClassName() const
        {
            return L"JsArrayIterator";
        }

        T Current()
        {
            return target->GetAt(index);
        }

        bool HasCurrent()
        {
            return index < target->Size();
        }

        std::uint32_t GetMany(winrt::array_view<T> const& items)
        {
            auto count = target->GetMany(index, items);
            index += count;
            return count;
        }

        bool MoveNext()
        {
            auto size = target->Size();
            if (index < size)
            {
                ++index;
            }

            return index < size;
        }

        winrt::com_ptr<D> target;
        std::uint32_t index = 0;
    };

    template <typename D, typename T>
    struct array_vector_base
    {
        array_vector_base(jsi::Runtime& runtime, jsi::Array array) : runtime(runtime), array(std::move(array))
        {
        }

        void CheckThread()
        {
            if (thread_id != std::this_thread::get_id())
            {
                throw winrt::hresult_wrong_thread{};
            }
        }

        // IIterable functions
        winrt::Windows::Foundation::Collections::IIterator<T> First()
        {
            return winrt::make<array_iterator<D, T>>(static_cast<D*>(this));
        }

        // IVectorView functions
        std::uint32_t Size()
        {
            CheckThread();
            return static_cast<std::uint32_t>(array.size(runtime));
        }

        T GetAt(std::uint32_t index)
        {
            auto size = Size(); // NOTE: Checks thread access
            if (index >= size)
            {
                throw winrt::hresult_out_of_bounds();
            }

            return convert_value_to_native<T>(runtime, array.getValueAtIndex(runtime, index));
        }

        std::uint32_t GetMany(std::uint32_t startIndex, winrt::array_view<T> items)
        {
            auto size = Size(); // NOTE: Checks thread access
            auto count = std::min(size - startIndex, items.size());
            for (uint32_t i = 0; i < count; ++i)
            {
                items[i] = convert_value_to_native<T>(runtime, array.getValueAtIndex(runtime, startIndex + i));
            }

            return count;
        }

        bool IndexOf(T const& value, std::uint32_t& index)
        {
            auto size = Size(); // NOTE: Checks thread access
            for (uint32_t i = 0; i < size; ++i)
            {
                if (value == convert_value_to_native<T>(runtime, array.getValueAtIndex(runtime, i)))
                {
                    index = i;
                    return true;
                }
            }

            return false;
        }

        // IVector functions, with the exception of 'GetView'
        void Append(T const& value)
        {
            // NOTE: JSI doesn't currently seem to allow modification of arrays from native
            CheckThread();
            auto pushFn = array.getPropertyAsFunction(runtime, "push");
            pushFn.callWithThis(runtime, array, convert_native_to_value(runtime, value));
        }

        void Clear()
        {
            // NOTE: JSI doesn't currently seem to allow modification of arrays from native
            CheckThread();
            array.setProperty(runtime, "length", 0);
        }

        void InsertAt(std::uint32_t index, T const& value)
        {
            // NOTE: JSI doesn't currently seem to allow modification of arrays from native
            CheckThread();
            auto spliceFn = array.getPropertyAsFunction(runtime, "splice");
            spliceFn.callWithThis(
                runtime, array, static_cast<double>(index), 0, convert_native_to_value(runtime, value));
        }

        void RemoveAt(std::uint32_t index)
        {
            // NOTE: JSI doesn't currently seem to allow modification of arrays from native
            CheckThread();
            auto spliceFn = array.getPropertyAsFunction(runtime, "splice");
            spliceFn.callWithThis(runtime, array, static_cast<double>(index), 1);
        }

        void RemoveAtEnd()
        {
            // NOTE: JSI doesn't currently seem to allow modification of arrays from native
            CheckThread();
            auto popFn = array.getPropertyAsFunction(runtime, "pop");
            popFn.callWithThis(runtime, array);
        }

        void ReplaceAll(winrt::array_view<const T> const& items)
        {
            // NOTE: JSI doesn't currently seem to allow modification of arrays from native
            CheckThread();
            array.setProperty(runtime, "length", static_cast<double>(items.size()));

            for (uint32_t i = 0; i < items.size(); ++i)
            {
                array.setValueAtIndex(runtime, i, convert_native_to_value(runtime, items[i]));
            }
        }

        void SetAt(std::uint32_t index, T const& value)
        {
            auto size = Size(); // NOTE: Checks thread access
            if (index >= size)
            {
                throw winrt::hresult_out_of_bounds();
            }

            array.setValueAtIndex(runtime, index, convert_native_to_value(runtime, value));
        }

        jsi::Runtime& runtime;
        jsi::Array array;
        std::thread::id thread_id = std::this_thread::get_id();
    };

    template <typename T>
    struct array_iterable :
        winrt::implements<array_iterable<T>, winrt::Windows::Foundation::Collections::IIterable<T>>,
        array_vector_base<array_iterable<T>, T>
    {
        using array_vector_base<array_iterable<T>, T>::array_vector_base;

        winrt::hstring GetRuntimeClassName() const
        {
            return L"JsArrayIterable";
        }
    };

    template <typename T>
    struct array_vector_view :
        winrt::implements<array_vector_view<T>, winrt::Windows::Foundation::Collections::IVectorView<T>,
            winrt::Windows::Foundation::Collections::IIterable<T>>,
        array_vector_base<array_vector_view<T>, T>
    {
        using array_vector_base<array_vector_view<T>, T>::array_vector_base;

        winrt::hstring GetRuntimeClassName() const
        {
            return L"JsArrayVectorView";
        }
    };

    template <typename T>
    struct array_vector :
        winrt::implements<array_vector<T>, winrt::Windows::Foundation::Collections::IVector<T>,
            winrt::Windows::Foundation::Collections::IIterable<T>>,
        array_vector_base<array_vector<T>, T>
    {
        using array_vector_base<array_vector<T>, T>::array_vector_base;

        winrt::hstring GetRuntimeClassName() const
        {
            return L"JsArrayVector";
        }

        winrt::Windows::Foundation::Collections::IVectorView<T> GetView()
        {
            return winrt::make<array_vector_view<T>>(this->runtime, this->array.getArray(this->runtime));
        }
    };
#endif
}

// Value converters
namespace nodewinrt
{
    template <>
    struct projected_value_traits<bool>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, bool value) noexcept
        {
            return v8::Boolean::New(context->isolate, value);
        }

        static bool as_native(runtime_context* context, v8::Local<v8::Value> value) noexcept
        {
            return value->BooleanValue(context->isolate);
        }
    };

    template <typename T>
    struct projected_value_traits<T, std::enable_if_t<std::is_arithmetic_v<T>>>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, T value) noexcept
        {
            return v8::Number::New(context->isolate, static_cast<double>(value));
        }

        static T as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            return check_maybe(value->NumberValue(context->isolate->GetCurrentContext()));
        }
    };

    template <typename T>
    struct projected_value_traits<T, std::enable_if_t<std::is_enum_v<T>>>
    {
        using int_type = std::underlying_type_t<T>;

        static v8::Local<v8::Value> as_value(runtime_context* context, T value) noexcept
        {
            return convert_native_to_value(context, static_cast<int_type>(value));
        }

        static T as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            return static_cast<T>(convert_value_to_native<int_type>(context, value));
        }
    };

    template <>
    struct projected_value_traits<char16_t>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, char16_t value);
        static char16_t as_native(runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::hstring>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, const winrt::hstring& value)
        {
            return make_string_checked(context->isolate, static_cast<std::wstring_view>(value));
        }

        static winrt::hstring as_native(runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::guid>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, const winrt::guid& value);
        static winrt::guid as_native(runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::hresult>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, winrt::hresult value)
        {
            return convert_native_to_value(context, static_cast<int32_t>(value));
        }

        static winrt::hresult as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            return winrt::hresult(convert_value_to_native<int32_t>(context, value));
        }
    };

    template <>
    struct projected_value_traits<winrt::event_token>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, winrt::event_token value)
        {
            auto result = v8::Object::New(context->isolate);
            set_property(context, context->isolate->GetCurrentContext(), result, "value"sv, value.value);
            return result;
        }

        static winrt::event_token as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            auto isolate = context->isolate;
            auto obj = cast_object(isolate, value);
            return winrt::event_token{ get_property_as<int64_t>(
                context, isolate->GetCurrentContext(), obj, "value"sv) };
        }
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::DateTime>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, winrt::Windows::Foundation::DateTime value);
        static winrt::Windows::Foundation::DateTime as_native(runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::TimeSpan>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, winrt::Windows::Foundation::TimeSpan value);
        static winrt::Windows::Foundation::TimeSpan as_native(runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Numerics::float3x2>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, winrt::Windows::Foundation::Numerics::float3x2 value);
        static winrt::Windows::Foundation::Numerics::float3x2 as_native(
            runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Numerics::float4x4>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, winrt::Windows::Foundation::Numerics::float4x4 value);
        static winrt::Windows::Foundation::Numerics::float4x4 as_native(
            runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Numerics::plane>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, winrt::Windows::Foundation::Numerics::plane value);
        static winrt::Windows::Foundation::Numerics::plane as_native(
            runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Numerics::quaternion>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, winrt::Windows::Foundation::Numerics::quaternion value);
        static winrt::Windows::Foundation::Numerics::quaternion as_native(
            runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Numerics::float2>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, winrt::Windows::Foundation::Numerics::float2 value);
        static winrt::Windows::Foundation::Numerics::float2 as_native(
            runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Numerics::float3>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, winrt::Windows::Foundation::Numerics::float3 value);
        static winrt::Windows::Foundation::Numerics::float3 as_native(
            runtime_context* context, v8::Local<v8::Value> value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::Numerics::float4>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, winrt::Windows::Foundation::Numerics::float4 value);
        static winrt::Windows::Foundation::Numerics::float4 as_native(
            runtime_context* context, v8::Local<v8::Value> value);
    };

    template <typename T>
    struct value_fill_array_wrapper
    {
        value_fill_array_wrapper(runtime_context* context, const winrt::array_view<T>& array) :
            m_context(context), m_nativeArray(array),
            m_jsArray(v8::Array::New(context->isolate, static_cast<int>(array.size())))
        {
        }

        ~value_fill_array_wrapper() noexcept(false)
        {
            // TODO: In theory the JS code could have expanded the size of the array. Should we throw then?
            auto ctxt = m_context->isolate->GetCurrentContext();
            for (uint32_t i = 0; i < m_nativeArray.size(); ++i)
            {
                if (auto value = get_property(ctxt, m_jsArray, i); !value->IsUndefined())
                {
                    m_nativeArray[i] = convert_value_to_native<T>(m_context, value);
                }
            }
        }

        // TODO: Can we just use some 'operator v8::Local<v8::Value>' here?
        v8::Local<v8::Value> value()
        {
            return m_jsArray;
        }

    private:
        runtime_context* m_context;
        winrt::array_view<T> m_nativeArray;
        v8::Local<v8::Array> m_jsArray; // NOTE: Stack object; okay to keep as Local
    };

    template <typename T>
    struct native_fill_array_wrapper
    {
        native_fill_array_wrapper(runtime_context* context, v8::Local<v8::Value> value) :
            m_context(context), m_jsArray(cast_array(context->isolate, value))
        {
            auto size = m_jsArray->Length();
            if constexpr (std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>)
            {
                m_nativeArray.resize(size, nullptr);
            }
            else
            {
                m_nativeArray.resize(size);
            }
        }

        ~native_fill_array_wrapper() noexcept(false)
        {
            auto ctxt = m_context->isolate->GetCurrentContext();
            for (std::size_t i = 0; i < m_nativeArray.size(); ++i)
            {
                set_property(m_context, ctxt, m_jsArray, static_cast<uint32_t>(i), m_nativeArray[i]);
            }
        }

        operator winrt::array_view<T>()
        {
            return winrt::array_view<T>(m_nativeArray.data(), static_cast<std::uint32_t>(m_nativeArray.size()));
        }

    private:
        runtime_context* m_context;
        v8::Local<v8::Array> m_jsArray; // NOTE: Stack object; okay to keep as Local
        sso_vector<T, 4> m_nativeArray; // NOTE: Because std::vector<bool> makes everyone sad...
    };

    template <typename T>
    struct projected_value_traits<winrt::array_view<T>>
    {
        // NOTE: Non-const 'T' - this is specific to 'fill array' scenarios
        static value_fill_array_wrapper<T> as_value(runtime_context* context, const winrt::array_view<T>& value)
        {
            return value_fill_array_wrapper<T>(context, value);
        }

        static native_fill_array_wrapper<T> as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            return native_fill_array_wrapper<T>(context, value);
        }
    };

    template <typename T>
    struct pass_array_wrapper
    {
        pass_array_wrapper(runtime_context* context, v8::Local<v8::Value> value)
        {
            auto ctxt = context->isolate->GetCurrentContext();
            auto array = cast_array(context->isolate, value);
            auto size = array->Length();
            m_data.reserve(size);
            for (uint32_t i = 0; i < size; ++i)
            {
                m_data.push_back(get_property_as<T>(context, ctxt, array, i));
            }
        }

        operator winrt::array_view<const T>()
        {
            return winrt::array_view<const T>(m_data.data(), static_cast<std::uint32_t>(m_data.size()));
        }

    private:
        sso_vector<T, 4> m_data; // NOTE: Because std::vector<bool> makes everyone sad...
    };

    template <typename T>
    struct projected_value_traits<winrt::array_view<const T>>
    {
        // NOTE: Const 'T' - this is specific to 'pass array' scenarios
        static v8::Local<v8::Value> as_value(runtime_context* context, const winrt::array_view<const T>& value)
        {
            // Array is immutable; we can convert to a JS array and call it a day
            auto result = v8::Array::New(context->isolate, static_cast<int>(value.size()));
            auto ctxt = context->isolate->GetCurrentContext();
            for (uint32_t i = 0; i < value.size(); ++i)
            {
                set_property(context, ctxt, result, i, value[i]);
            }

            return result;
        }

        static pass_array_wrapper<T> as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            return pass_array_wrapper<T>(context, value);
        }
    };

    template <typename T>
    struct projected_value_traits<winrt::com_array<T>>
    {
        static v8::Local<v8::Value> as_value(runtime_context* context, const winrt::com_array<T>& value)
        {
            auto result = v8::Array::New(context->isolate, static_cast<int>(value.size()));
            auto ctxt = context->isolate->GetCurrentContext();
            for (uint32_t i = 0; i < value.size(); ++i)
            {
                set_property(context, ctxt, result, i, value[i]);
            }

            return result;
        }

        static winrt::com_array<T> as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            auto array = cast_array(context->isolate, value);
            array_to_native_iterator<T> range(context, array);
            return winrt::com_array<T>(range.begin(), range.end());
        }
    };

    v8::Local<v8::Value> convert_from_property_value(
        runtime_context* context, const winrt::Windows::Foundation::IPropertyValue& value);
    winrt::Windows::Foundation::IInspectable convert_to_property_value(
        runtime_context* context, v8::Local<v8::Value> value);

    template <typename T>
    v8::Local<v8::Value> convert_object_instance_to_value(runtime_context* context, const T& value)
    {
        if (!value)
        {
            return v8::Null(context->isolate);
        }

        if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IPropertyValue>)
        {
            if (auto result = convert_from_property_value(context, value); !result.IsEmpty())
            {
                return result;
            }
        }
        else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
        {
            if (auto propVal = value.try_as<winrt::Windows::Foundation::IPropertyValue>())
            {
                if (auto result = convert_from_property_value(context, propVal); !result.IsEmpty())
                {
                    return result;
                }
            }
        }

        return context->instance_cache.get_instance(context, value);
    }

    template <typename T>
    __declspec(noinline) std::optional<T> convert_value_to_object_instance_impl(runtime_context* context,
        v8::Local<v8::Value> value, T (*asTargetType)(const winrt::Windows::Foundation::IInspectable&))
    {
        if (value->IsNull() || value->IsUndefined())
        {
            return nullptr;
        }

        if (value->IsObject())
        {
            auto obj = value.As<v8::Object>();
            if (is_projected_object(context, obj))
            {
                auto ptr = internal_field_value<projected_object_instance>(obj, 1);
                return asTargetType(ptr->instance());
            }
        }

        return std::nullopt;
    }

    template <typename T>
    T convert_value_to_object_instance(runtime_context* context, v8::Local<v8::Value> value)
    {
        if (auto result = convert_value_to_object_instance_impl<T>(
                context, value, [](const winrt::Windows::Foundation::IInspectable& insp) {
                    if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
                    {
                        return insp;
                    }
                    else
                    {
                        return insp.as<T>();
                    }
                }))
        {
            return std::move(*result);
        }

        if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable> ||
                      std::is_same_v<T, winrt::Windows::Foundation::IPropertyValue>)
        {
            if (auto result = convert_to_property_value(context, value))
            {
                if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
                {
                    return result;
                }
                else
                {
                    return result.as<winrt::Windows::Foundation::IPropertyValue>();
                }
            }
        }

        if constexpr (pinterface_traits<T>::is_array_convertible)
        {
            if (value->IsArray())
            {
                auto array = value.As<v8::Array>();
                // TODO
#if 0
                using elem_type = typename pinterface_traits<T>::value_type;
                if constexpr (pinterface_traits<T>::is_iterable)
                {
                    return winrt::make<array_iterable<elem_type>>(runtime, obj.getArray(runtime));
                }
                else if constexpr (pinterface_traits<T>::is_vector_view)
                {
                    return winrt::make<array_vector_view<elem_type>>(runtime, obj.getArray(runtime));
                }
                else // is_vector
                {
                    static_assert(pinterface_traits<T>::is_vector);
                    return winrt::make<array_vector<elem_type>>(runtime, obj.getArray(runtime));
                }
#endif
            }
        }

        // TODO: Also IMap/IMapView?

        throw v8_exception::type_error(context->isolate, "Cannot derive a WinRT interface for the JS value");
    }

    template <typename T>
    struct projected_value_traits<winrt::Windows::Foundation::IReference<T>>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, const winrt::Windows::Foundation::IReference<T>& value)
        {
            if (!value)
            {
                return v8::Null(context->isolate);
            }

            return convert_native_to_value(context, value.Value());
        }

        static winrt::Windows::Foundation::IReference<T> as_native(runtime_context* context, v8::Local<v8::Value> value)
        {
            if (value->IsNull() || value->IsUndefined())
            {
                return nullptr;
            }

            return winrt::box_value(convert_value_to_native<T>(context, value))
                .as<winrt::Windows::Foundation::IReference<T>>();
        }
    };

    template <typename T>
    struct projected_value_traits<winrt::Windows::Foundation::IReferenceArray<T>>
    {
        static v8::Local<v8::Value> as_value(
            runtime_context* context, const winrt::Windows::Foundation::IReferenceArray<T>& value)
        {
            if (!value)
            {
                return v8::Null(context->isolate);
            }

            return convert_native_to_value(context, value.Value());
        }

        static winrt::Windows::Foundation::IReferenceArray<T> from_value(
            runtime_context* context, v8::Local<v8::Value> value)
        {
            if (value->IsNull() || value->IsUndefined())
            {
                return nullptr;
            }

            // It doesn't seem like C++/WinRT actually provides an implementation of IReferenceArray<T> like it does for
            // IReference<T> nor does it even map the basic array types supported by Windows::Foundation::PropertyValue.
            // It can be done but since the public SDK doesn't actually make use of it, perhaps it it is not necessary
            // to implement.
            throw v8_exception::type_error(context->isolate,
                "Conversion to native reference array to JS not implemented for "s + typeid(T).name());
        }
    };

    // TODO
#if 0
    template <>
    struct projected_value_traits<winrt::Windows::Foundation::IAsyncAction>
    {
        using interface_type = winrt::Windows::Foundation::IAsyncAction;
        static jsi::Value as_value(jsi::Runtime& runtime, const interface_type& value)
        {
            return jsi::Value(runtime,
                jsi::Object::createFromHostObject(runtime, projected_async_instance<interface_type>::create(value)));
        }

        static interface_type as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            // Assume that the caller got the value from calling 'operation' on the projected_async_instance
            return convert_value_to_object_instance<interface_type>(runtime, value);
        }
    };

    template <typename TProgress>
    struct projected_value_traits<winrt::Windows::Foundation::IAsyncActionWithProgress<TProgress>>
    {
        using interface_type = winrt::Windows::Foundation::IAsyncActionWithProgress<TProgress>;
        static jsi::Value as_value(jsi::Runtime& runtime, const interface_type& value)
        {
            return jsi::Value(runtime,
                jsi::Object::createFromHostObject(runtime, projected_async_instance<interface_type>::create(value)));
        }

        static interface_type as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            // Assume that the caller got the value from calling 'operation' on the projected_async_instance
            return convert_value_to_object_instance<interface_type>(runtime, value);
        }
    };

    template <typename TResult>
    struct projected_value_traits<winrt::Windows::Foundation::IAsyncOperation<TResult>>
    {
        using interface_type = winrt::Windows::Foundation::IAsyncOperation<TResult>;
        static jsi::Value as_value(jsi::Runtime& runtime, const interface_type& value)
        {
            return jsi::Value(runtime,
                jsi::Object::createFromHostObject(runtime, projected_async_instance<interface_type>::create(value)));
        }

        static interface_type as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            // Assume that the caller got the value from calling 'operation' on the projected_async_instance
            return convert_value_to_object_instance<interface_type>(runtime, value);
        }
    };

    template <typename TResult, typename TProgress>
    struct projected_value_traits<winrt::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>>
    {
        using interface_type = winrt::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>;
        static jsi::Value as_value(jsi::Runtime& runtime, const interface_type& value)
        {
            return jsi::Value(runtime,
                jsi::Object::createFromHostObject(runtime, projected_async_instance<interface_type>::create(value)));
        }

        static interface_type as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            // Assume that the caller got the value from calling 'operation' on the projected_async_instance
            return convert_value_to_object_instance<interface_type>(runtime, value);
        }
    };
#endif

    // TODO
#if 0
    template <typename T>
    struct projected_value_traits<winrt::Windows::Foundation::EventHandler<T>>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::EventHandler<T>& value)
        {
            return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "EventHandler"), 2,
                [value](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count != 2)
                    {
                        throw_invalid_delegate_arg_count(runtime, "Windows.Foundation"sv, "EventHandler");
                    }

                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IInspectable>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<T>(runtime, args[1]);
                    value(arg0, arg1);
                    return jsi::Value::undefined();
                });
        }

        static winrt::Windows::Foundation::EventHandler<T> as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            return
                [ctxt = current_runtime_context()->add_reference(), fn = value.asObject(runtime).asFunction(runtime)](
                    const winrt::Windows::Foundation::IInspectable& sender, const T& args) {
                    // TODO: Do we need to call synchronously? One reason might be to propagate errors, but typically
                    // event sources don't care about those.
                    ctxt->call_sync([&]() {
                        fn.call(ctxt->runtime, convert_native_to_value(ctxt->runtime, sender),
                            convert_native_to_value(ctxt->runtime, args));
                    });
                };
        }
    };

    template <typename TSender, typename TResult>
    struct projected_value_traits<winrt::Windows::Foundation::TypedEventHandler<TSender, TResult>>
    {
        static jsi::Value as_value(
            jsi::Runtime& runtime, const winrt::Windows::Foundation::TypedEventHandler<TSender, TResult>& value)
        {
            return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "TypedEventHandler"), 2,
                [value](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count != 2)
                    {
                        throw_invalid_delegate_arg_count(runtime, "Windows.Foundation"sv, "TypedEventHandler");
                    }

                    auto arg0 = convert_value_to_native<TSender>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<TResult>(runtime, args[1]);
                    value(arg0, arg1);
                    return jsi::Value::undefined();
                });
        }

        static winrt::Windows::Foundation::TypedEventHandler<TSender, TResult> as_native(
            jsi::Runtime& runtime, const jsi::Value& value)
        {
            return [ctxt = current_runtime_context()->add_reference(),
                       fn = value.asObject(runtime).asFunction(runtime)](const TSender& sender, const TResult& args) {
                ctxt->call_sync([&]() {
                    fn.call(ctxt->runtime, convert_native_to_value(ctxt->runtime, sender),
                        convert_native_to_value(ctxt->runtime, args));
                });
            };
        }
    };

    template <typename K, typename V>
    struct projected_value_traits<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>
    {
        static jsi::Value as_value(
            jsi::Runtime& runtime, const winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>& value)
        {
            return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "MapChangedEventHandler"), 2,
                [value](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count != 2)
                    {
                        throw_invalid_delegate_arg_count(
                            runtime, "Windows.Foundation.Collections"sv, "MapChangedEventHandler");
                    }

                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IObservableMap<K, V>>(
                        runtime, args[0]);
                    auto arg1 =
                        convert_value_to_native<winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K, V>>(
                            runtime, args[1]);
                    value(arg0, arg1);
                    return jsi::Value::undefined();
                });
        }

        static winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V> as_native(
            jsi::Runtime& runtime, const jsi::Value& value)
        {
            return
                [ctxt = current_runtime_context()->add_reference(), fn = value.asObject(runtime).asFunction(runtime)](
                    const winrt::Windows::Foundation::Collections::IObservableMap<K, V>& sender,
                    const winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>& args) {
                    ctxt->call_sync([&]() {
                        fn.call(ctxt->runtime, convert_native_to_value(ctxt->runtime, sender),
                            convert_native_to_value(ctxt->runtime, args));
                    });
                };
        }
    };

    template <typename T>
    struct projected_value_traits<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>
    {
        static jsi::Value as_value(
            jsi::Runtime& runtime, const winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>& value)
        {
            return jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "VectorChangedEventHandler"), 2,
                [value](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count != 2)
                    {
                        throw_invalid_delegate_arg_count(
                            runtime, "Windows.Foundation.Collections"sv, "VectorChangedEventHandler");
                    }

                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::Collections::IObservableVector<T>>(
                        runtime, args[0]);
                    auto arg1 =
                        convert_value_to_native<winrt::Windows::Foundation::Collections::IVectorChangedEventArgs<T>>(
                            runtime, args[1]);
                    value(arg0, arg1);
                    return jsi::Value::undefined();
                });
        }

        static winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T> as_native(
            jsi::Runtime& runtime, const jsi::Value& value)
        {
            return
                [ctxt = current_runtime_context()->add_reference(), fn = value.asObject(runtime).asFunction(runtime)](
                    const winrt::Windows::Foundation::Collections::IObservableVector<T>& sender,
                    const winrt::Windows::Foundation::Collections::IVectorChangedEventArgs& args) {
                    ctxt->call_sync([&]() {
                        fn.call(ctxt->runtime, convert_native_to_value(ctxt->runtime, sender),
                            convert_native_to_value(ctxt->runtime, args));
                    });
                };
        }
    };
#endif
}

// Static data for generic types
namespace nodewinrt
{
    namespace interfaces::Windows::Foundation::Collections
    {
        using winrt::Windows::Foundation::IInspectable;

        namespace IIterable
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IIterable<T>;

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "first",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().First()));
                        },
                        0, false },
                };
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(), {},
                    {}, iface::functions };
            };

            template <typename T>
            constexpr const static_interface_data& data = data_t<T>::value;
        }

        namespace IIterator
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IIterator<T>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "current",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Current());
                        },
                        nullptr },
                    { "hasCurrent",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().HasCurrent());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "getMany",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto items = convert_value_to_native<winrt::array_view<T>>(context, info[0]);
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().GetMany(items)));
                        },
                        1, false },
                    { "moveNext",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().MoveNext()));
                        },
                        0, false },
                };
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(),
                    iface::properties, {}, iface::functions };
            };

            template <typename T>
            constexpr const static_interface_data& data = data_t<T>::value;
        }

        namespace IKeyValuePair
        {
            template <typename K, typename V>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "key",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Key());
                        },
                        nullptr },
                    { "value",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Value());
                        },
                        nullptr },
                };
            };

            template <typename K, typename V>
            struct data_t
            {
                using iface = interface_data<K, V>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(),
                    iface::properties, {}, {} };
            };

            template <typename K, typename V>
            constexpr const static_interface_data& data = data_t<K, V>::value;
        }

        namespace IMap
        {
            template <typename K, typename V>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IMap<K, V>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "size",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Size());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "clear",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>&) { thisValue.as<native_type>().Clear(); },
                        0, false },
                    { "getView",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().GetView()));
                        },
                        0, false },
                    { "hasKey",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto key = convert_value_to_native<K>(context, info[0]);
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().HasKey(key)));
                        },
                        1, false },
                    { "insert",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto key = convert_value_to_native<K>(context, info[0]);
                            auto value = convert_value_to_native<V>(context, info[1]);
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().Insert(key, value)));
                        },
                        2, false },
                    { "lookup",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto key = convert_value_to_native<K>(context, info[0]);
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().Lookup(key)));
                        },
                        1, false },
                    { "remove",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto key = convert_value_to_native<K>(context, info[0]);
                            thisValue.as<native_type>().Remove(key);
                        },
                        1, false },
                };

#if 0 // TODO
                static std::pair<std::optional<jsi::Value>, std::optional<jsi::Value>> runtime_get_property(
                    jsi::Runtime& runtime, const IInspectable& thisValue, std::string_view name)
                {
                    // TODO: Should we also include 'Char' and 'Guid' as well?
                    if constexpr (std::is_same_v<K, winrt::hstring>)
                    {
                        // If the "property" is any other string, then that translates to a 'Lookup' call
                        auto map = thisValue.as<native_type>();
                        auto key = winrt::to_hstring(name);
                        if (map.HasKey(key))
                        {
                            return { convert_native_to_value(runtime, map.Lookup(key)), std::nullopt };
                        }
                    }

                    return { std::nullopt, std::nullopt };
                };

                static bool runtime_set_property(jsi::Runtime& runtime, const IInspectable& thisValue,
                    std::string_view name, const jsi::Value& value)
                {
                    if constexpr (std::is_same_v<K, winrt::hstring>)
                    {
                        auto map = thisValue.as<native_type>();
                        auto key = winrt::to_hstring(name);
                        map.Insert(key, convert_value_to_native<V>(runtime, value));
                        return true;
                    }
                    else
                        return false;
                }
#endif
            };

            template <typename K, typename V>
            struct data_t
            {
                using iface = interface_data<K, V>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(),
                    iface::properties, {}, iface::functions,
#if 0 // TODO
                    &iface::runtime_get_property,
                    &iface::runtime_set_property
#endif
                    };
            };

            template <typename K, typename V>
            constexpr const static_interface_data& data = data_t<K, V>::value;
        }

        namespace IMapChangedEventArgs
        {
            template <typename K>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "collectionChange",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().CollectionChange());
                        },
                        nullptr },
                    { "key",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Key());
                        },
                        nullptr },
                };
            };

            template <typename K>
            struct data_t
            {
                using iface = interface_data<K>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(),
                    iface::properties, {}, {} };
            };

            template <typename K>
            constexpr const static_interface_data& data = data_t<K>::value;
        }

        namespace IMapView
        {
            template <typename K, typename V>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IMapView<K, V>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "size",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Size());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "hasKey",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto key = convert_value_to_native<K>(context, info[0]);
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().HasKey(key)));
                        },
                        1, false },
                    { "lookup",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto key = convert_value_to_native<K>(context, info[0]);
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().Lookup(key)));
                        },
                        1, false },
                    { "split",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            winrt::Windows::Foundation::Collections::IMapView<K, V> first;
                            winrt::Windows::Foundation::Collections::IMapView<K, V> second;
                            thisValue.as<native_type>().Split(first, second);
                            info.GetReturnValue().Set(
                                make_void_return_struct(context, "first", first, "second", second));
                        },
                        0, false },
                };

#if 0 // TODO
                static std::pair<std::optional<jsi::Value>, std::optional<jsi::Value>> runtime_get_property(
                    jsi::Runtime& runtime, const IInspectable& thisValue, std::string_view name)
                {
                    // TODO: Should we also include 'Char' and 'Guid' as well?
                    if constexpr (std::is_same_v<K, winrt::hstring>)
                    {
                        // If the "property" is any other string, then that translates to a 'Lookup' call
                        auto map = thisValue.as<native_type>();
                        auto key = winrt::to_hstring(name);
                        if (map.HasKey(key))
                        {
                            return { convert_native_to_value(runtime, map.Lookup(key)), std::nullopt };
                        }
                    }

                    return { std::nullopt, std::nullopt };
                };
#endif
            };

            template <typename K, typename V>
            struct data_t
            {
                using iface = interface_data<K, V>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(),
                    iface::properties, {}, iface::functions,
#if 0
                    &iface::runtime_get_property
#endif
                    };
            };

            template <typename K, typename V>
            constexpr const static_interface_data& data = data_t<K, V>::value;
        }

        namespace IObservableMap
        {
            template <typename K, typename V>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IObservableMap<K, V>;

                static constexpr const static_interface_data::event_mapping events[] = {
                    { "mapchanged",
                        [](runtime_context* context, const IInspectable& thisValue, v8::Local<v8::Value> callback) {
                            auto handler = convert_value_to_native<
                                winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>(
                                context, callback);
                            return thisValue.as<native_type>().MapChanged(handler);
                        },
                        [](const IInspectable& thisValue, winrt::event_token token) {
                            thisValue.as<native_type>().MapChanged(token);
                        } },
                };
            };

            template <typename K, typename V>
            struct data_t
            {
                using iface = interface_data<K, V>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(), {},
                    iface::events, {} };
            };

            template <typename K, typename V>
            constexpr const static_interface_data& data = data_t<K, V>::value;
        }

        namespace IObservableVector
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IObservableVector<T>;

                static constexpr const static_interface_data::event_mapping events[] = {
                    { "vectorchanged",
                        [](runtime_context* context, const IInspectable& thisValue, v8::Local<v8::Value> callback) {
                            auto handler = convert_value_to_native<
                                winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>(
                                context, callback);
                            return thisValue.as<native_type>().VectorChanged(handler);
                        },
                        [](const IInspectable& thisValue, winrt::event_token token) {
                            thisValue.as<native_type>().VectorChanged(token);
                        } },
                };
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(), {},
                    iface::events, {} };
            };

            template <typename T>
            constexpr const static_interface_data& data = data_t<T>::value;
        }

#if 0 // TODO
        inline std::optional<jsi::Value> fwd_array_prototype(jsi::Runtime& runtime, std::string_view name)
        {
            auto arrayClass = runtime.global().getPropertyAsObject(runtime, "Array");
            auto arrayProto = arrayClass.getPropertyAsObject(runtime, "prototype");

            // NOTE: 'name' is constructed from a std::string
            assert(name.data()[name.size()] == 0);
            auto result = arrayProto.getProperty(runtime, name.data());
            if (!result.isUndefined())
                return result;

            return std::nullopt;
        }

        inline uint32_t vector_index_from_arg(jsi::Runtime& runtime, const jsi::Value* args, size_t count, size_t index,
            uint32_t size, uint32_t defaultValue = 0)
        {
            if (index >= count)
                return defaultValue;

            auto value = to_integer_or_infinity(context, info[index]);
            if (value == -std::numeric_limits<double>::infinity())
                return 0;

            if (value < 0)
            {
                value = std::max(0.0, size + value);
            }
            else
            {
                value = std::min(value, static_cast<double>(size));
            }

            assert(static_cast<double>(static_cast<uint32_t>(value)) == value);
            return static_cast<uint32_t>(value);
        }

        inline jsi::Function callback_from_arg(
            jsi::Runtime& runtime, const jsi::Value* args, size_t count, size_t index = 0)
        {
            if (index >= count)
            {
                throw jsi::JSError(runtime, "TypeError: undefined is not a function");
            }

            return args[index].asObject(runtime).asFunction(runtime);
        }

        inline std::optional<jsi::Object> callback_this_arg(
            jsi::Runtime& runtime, const jsi::Value* args, size_t count, size_t index = 1)
        {
            if ((index < count) && args[index].isObject())
            {
                return args[index].getObject(runtime);
            }

            return std::nullopt;
        }

        // NOTE: From Array.prototype.concat
        template <typename TVector, typename ValueType>
        inline __declspec(noinline) jsi::Value
            vector_concat_impl(jsi::Runtime& runtime, const TVector& vector, const jsi::Value* args, size_t count,
                winrt::Windows::Foundation::Collections::IVector<ValueType> (*vectorCast)(
                    const winrt::Windows::Foundation::IInspectable&),
                winrt::Windows::Foundation::Collections::IVectorView<ValueType> (*vectorViewCast)(
                    const winrt::Windows::Foundation::IInspectable&))
        {
            using namespace winrt::Windows::Foundation::Collections;

            // TODO: We could in theory try and pre-calculate the final size, but it's not quite clear that would be
            // worth it
            jsi::Array result(runtime, vector.Size());

            size_t i = 0;
            for (auto&& value : vector)
            {
                result.setValueAtIndex(runtime, i++, convert_native_to_value(runtime, value));
            }

            auto pushFn = result.getPropertyAsFunction(runtime, "push");
            for (size_t argIndex = 0; argIndex < count; ++argIndex)
            {
                auto& arg = args[argIndex];

                // TODO: By the JS standard, we should check to see if the argument responds to
                // 'Symbol.isConcatSpreadable', however this poses two problems. The first is that JSI does not offer us
                // the ability to check this, and the second is that JSI does not allow us to advertise 'IVector*' types
                // as 'isConcatSpreadable'. As a workaround - at least until JSI has the functionality we need - we'll
                // assume 'isConcatSpreadable' to be true if (1) the argument is an array, or (2) the argument is an
                // 'IVector*<T>'. Note that we'll be missing out on non-array/non-vector spreadable types as well as
                // 'IVector*<U>' types.
                // Also note that this problem exists in the reverse direction - when a vector is used as an argument to
                // an 'Array.prototype.concat' call since we'll be unable to use our logic below during that call.
                // Again, the ideal solution would be for JSI to give us the functionality we require
                if (!arg.isObject())
                {
                    pushFn.callWithThis(runtime, result, arg);
                }
                else if (auto obj = arg.getObject(runtime); obj.isArray(runtime))
                {
                    auto arr = obj.getArray(runtime);
                    auto arrLen = arr.length(runtime);
                    for (size_t j = 0; j < arrLen; ++j)
                    {
                        pushFn.callWithThis(runtime, result, arr.getValueAtIndex(runtime, j));
                    }
                }
                else if (obj.isHostObject<projected_object_instance>(runtime))
                {
                    auto hostObj = obj.getHostObject<projected_object_instance>(runtime);
                    if (auto v = vectorCast(hostObj->instance()))
                    {
                        for (auto&& value : v)
                        {
                            pushFn.callWithThis(runtime, result, convert_native_to_value(runtime, value));
                        }
                    }
                    else if (auto view = vectorViewCast(hostObj->instance()))
                    {
                        for (auto&& value : v)
                        {
                            pushFn.callWithThis(runtime, result, convert_native_to_value(runtime, value));
                        }
                    }
                    // TODO: IIterable?
                    else
                    {
                        pushFn.callWithThis(runtime, result, arg);
                    }
                }
                else
                {
                    pushFn.callWithThis(runtime, result, arg);
                }
            }

            return result;
        }

        template <typename TVector>
        inline jsi::Value vector_concat(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            using namespace winrt::Windows::Foundation;
            using namespace winrt::Windows::Foundation::Collections;
            using value_type = typename pinterface_traits<TVector>::value_type;
            return vector_concat_impl<TVector, value_type>(
                runtime, convert_value_to_native<TVector>(runtime, thisValue), args, count,
                [](const IInspectable& insp) { return insp.try_as<IVector<value_type>>(); },
                [](const IInspectable& insp) { return insp.try_as<IVectorView<value_type>>(); });
        }

        // NOTE: From Array.prototype.every
        template <typename TVector>
        inline __declspec(noinline) jsi::Value vector_every_impl(jsi::Runtime& runtime, const jsi::Value& thisValue,
            const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto fn = callback_from_arg(runtime, args, count);
            auto thisArg = callback_this_arg(runtime, args, count);

            double index = 0;
            for (auto&& value : vector)
            {
                jsi::Value result;
                if (thisArg)
                {
                    result =
                        fn.callWithThis(runtime, *thisArg, convert_native_to_value(runtime, value), index++, thisValue);
                }
                else
                {
                    result = fn.call(runtime, convert_native_to_value(runtime, value), index++, thisValue);
                }

                if (!to_boolean(runtime, result))
                    return false;
            }

            return true;
        }

        template <typename TVector>
        inline jsi::Value vector_every(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_every_impl(
                runtime, thisValue, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }

        // NOTE: From Array.prototype.filter
        template <typename TVector>
        inline __declspec(noinline) jsi::Value vector_filter_impl(jsi::Runtime& runtime, const jsi::Value& thisValue,
            const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto fn = callback_from_arg(runtime, args, count);
            auto thisArg = callback_this_arg(runtime, args, count);

            jsi::Array result(runtime, 0);
            auto pushFn = result.getPropertyAsFunction(runtime, "push");

            double index = 0;
            for (auto&& value : vector)
            {
                jsi::Value includeResult;
                if (thisArg)
                {
                    includeResult =
                        fn.callWithThis(runtime, *thisArg, convert_native_to_value(runtime, value), index++, thisValue);
                }
                else
                {
                    includeResult = fn.call(runtime, convert_native_to_value(runtime, value), index++, thisValue);
                }

                if (to_boolean(runtime, includeResult))
                {
                    pushFn.callWithThis(runtime, result, convert_native_to_value(runtime, value));
                }
            }

            return result;
        }

        template <typename TVector>
        inline jsi::Value vector_filter(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_filter_impl(
                runtime, thisValue, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }

        // NOTE: From Array.prototype.forEach
        template <typename TVector>
        inline __declspec(noinline) jsi::Value vector_forEach_impl(jsi::Runtime& runtime, const jsi::Value& thisValue,
            const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto fn = callback_from_arg(runtime, args, count);
            auto thisArg = callback_this_arg(runtime, args, count);

            double index = 0;
            for (auto&& value : vector)
            {
                if (thisArg)
                {
                    fn.callWithThis(runtime, *thisArg, convert_native_to_value(runtime, value), index++, thisValue);
                }
                else
                {
                    fn.call(runtime, convert_native_to_value(runtime, value), index++, thisValue);
                }
            }

            return jsi::Value::undefined();
        }

        template <typename TVector>
        inline jsi::Value vector_forEach(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_forEach_impl(
                runtime, thisValue, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }

        // NOTE: From Array.prototype.lastIndexOf
        template <typename TVector>
        inline __declspec(noinline) jsi::Value
            vector_lastIndexOf_impl(jsi::Runtime& runtime, const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto size = vector.Size();
            if ((size == 0) || (count < 1))
                return -1;

            int64_t fromIndex = size - 1;
            if (count >= 2)
            {
                auto val = to_integer_or_infinity(context, info[1]);
                if (val == -std::numeric_limits<double>::infinity())
                    return -1;

                if (val >= 0)
                {
                    fromIndex = static_cast<int64_t>(std::min(static_cast<double>(fromIndex), val));
                }
                else
                {
                    fromIndex = static_cast<int64_t>(size + val);
                }
            }

            for (; fromIndex >= 0; --fromIndex)
            {
                if (jsi::Value::strictEquals(context, info[0],
                        convert_native_to_value(runtime, vector.GetAt(static_cast<uint32_t>(fromIndex)))))
                    return static_cast<double>(fromIndex);
            }

            return -1;
        }

        template <typename TVector>
        inline jsi::Value vector_lastIndexOf(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_lastIndexOf_impl(runtime, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }

        // NOTE: From Array.prototype.map
        template <typename TVector>
        inline __declspec(noinline) jsi::Value vector_map_impl(jsi::Runtime& runtime, const jsi::Value& thisValue,
            const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto fn = callback_from_arg(runtime, args, count);
            auto thisArg = callback_this_arg(runtime, args, count);

            jsi::Array result(runtime, vector.Size());
            uint32_t index = 0;
            for (auto&& value : vector)
            {
                jsi::Value mapResult;
                if (thisArg)
                {
                    mapResult = fn.callWithThis(runtime, *thisArg, convert_native_to_value(runtime, value),
                        static_cast<double>(index), thisValue);
                }
                else
                {
                    mapResult = fn.call(
                        runtime, convert_native_to_value(runtime, value), static_cast<double>(index), thisValue);
                }

                result.setValueAtIndex(runtime, index, std::move(mapResult));
                ++index;
            }

            return result;
        }

        template <typename TVector>
        inline jsi::Value vector_map(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_map_impl(
                runtime, thisValue, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }

        // NOTE: From Array.prototype.reduce
        template <typename TVector>
        inline __declspec(noinline) jsi::Value vector_reduce_impl(jsi::Runtime& runtime, const jsi::Value& thisValue,
            const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto size = vector.Size();
            auto fn = callback_from_arg(runtime, args, count);

            if ((size == 0) && (count < 2))
            {
                throw jsi::JSError(runtime, "TypeError: Reduce of empty vector with no initial value");
            }

            jsi::Value accum;
            uint32_t i = 0;
            if (count >= 2)
            {
                accum = jsi::Value(context, info[1]);
            }
            else
            {
                accum = convert_native_to_value(runtime, vector.GetAt(i++));
            }

            for (; i < size; ++i)
            {
                accum = fn.call(runtime, accum, convert_native_to_value(runtime, vector.GetAt(i)),
                    static_cast<double>(i), thisValue);
            }

            return accum;
        }

        template <typename TVector>
        inline jsi::Value vector_reduce(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_reduce_impl(
                runtime, thisValue, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }

        // NOTE: From Array.prototype.reduceRight
        template <typename TVector>
        inline __declspec(noinline) jsi::Value vector_reduceRight_impl(jsi::Runtime& runtime,
            const jsi::Value& thisValue, const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto size = vector.Size();
            auto fn = callback_from_arg(runtime, args, count);

            if ((size == 0) && (count < 2))
            {
                throw jsi::JSError(runtime, "TypeError: Reduce of empty vector with no initial value");
            }

            jsi::Value accum;
            uint32_t i = size;
            if (count >= 2)
            {
                accum = jsi::Value(context, info[1]);
            }
            else
            {
                accum = convert_native_to_value(runtime, vector.GetAt(--i));
            }

            while (i-- > 0)
            {
                accum = fn.call(runtime, accum, convert_native_to_value(runtime, vector.GetAt(i)),
                    static_cast<double>(i), thisValue);
            }

            return accum;
        }

        template <typename TVector>
        inline jsi::Value vector_reduceRight(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_reduceRight_impl(
                runtime, thisValue, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }

        // NOTE: From Array.prototype.slice
        template <typename TVector>
        inline __declspec(noinline) jsi::Value
            vector_slice_impl(jsi::Runtime& runtime, const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto size = vector.Size();
            auto start = vector_index_from_arg(runtime, args, count, 0, size);
            auto end = vector_index_from_arg(runtime, args, count, 1, size, size);
            if (start > end)
                end = start;

            auto copySize = end - start;
            jsi::Array result(runtime, copySize);
            for (size_t i = 0; start < end; ++i, ++start)
            {
                result.setValueAtIndex(runtime, i, convert_native_to_value(runtime, vector.GetAt(start)));
            }

            return result;
        }

        template <typename TVector>
        inline jsi::Value vector_slice(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_slice_impl(runtime, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }

        // NOTE: From Array.prototype.some
        template <typename TVector>
        inline __declspec(noinline) jsi::Value vector_some_impl(jsi::Runtime& runtime, const jsi::Value& thisValue,
            const TVector& vector, const jsi::Value* args, size_t count)
        {
            auto fn = callback_from_arg(runtime, args, count);
            auto thisArg = callback_this_arg(runtime, args, count);

            double index = 0;
            for (auto&& value : vector)
            {
                jsi::Value result;
                if (thisArg)
                {
                    result =
                        fn.callWithThis(runtime, *thisArg, convert_native_to_value(runtime, value), index++, thisValue);
                }
                else
                {
                    result = fn.call(runtime, convert_native_to_value(runtime, value), index++, thisValue);
                }

                if (to_boolean(runtime, result))
                    return true;
            }

            return false;
        }

        template <typename TVector>
        inline jsi::Value vector_some(
            jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
        {
            // NOTE: This causes IVector<T> and IVectorView<T> to get COMDAT folded together along with IVector*<U> for
            // similar enough 'U' (e.g. classes/interfaces get folded together)
            return vector_some_impl(
                runtime, thisValue, convert_value_to_native<TVector>(runtime, thisValue), args, count);
        }
#endif

        namespace IVector
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IVector<T>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "length", // NOTE: From Array.prototype
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Size());
                        },
                        nullptr // TODO
#if 0
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& value) {
                            // Following Chakra's behavior, setting the length can only be used to remove elements
                            auto vector = thisValue.as<native_type>();
                            auto currLen = vector.Size();
                            auto newLen = convert_value_to_native<uint32_t>(runtime, value);
                            if (newLen > currLen)
                            {
                                throw jsi::JSError(runtime, "TypeError: Cannot assign 'length' to IVector that is "
                                                            "greater than its current length");
                            }
                            else if (newLen == 0)
                            {
                                vector.Clear();
                            }
                            else
                            {
                                for (; currLen > newLen; --currLen)
                                {
                                    vector.RemoveAtEnd();
                                }
                            }
                        }
#endif
                        },
                    { "size",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Size());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "append",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto value = convert_value_to_native<T>(context, info[0]);
                            thisValue.as<native_type>().Append(value);
                        },
                        1, false },
                    { "clear",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>&) { thisValue.as<native_type>().Clear(); },
                        0, false },
                    { "getAt",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto index = convert_value_to_native<uint32_t>(context, info[0]);
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().GetAt(index)));
                        },
                        1, false },
                    { "getMany",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto startIndex = convert_value_to_native<uint32_t>(context, info[0]);
                            auto items = convert_value_to_native<winrt::array_view<T>>(context, info[1]);
                            info.GetReturnValue().Set(convert_native_to_value(
                                context, thisValue.as<native_type>().GetMany(startIndex, items)));
                        },
                        2, false },
                    { "getView",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().GetView()));
                        },
                        0, false },
                    { "indexOf",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto value = convert_value_to_native<T>(context, info[0]);
                            uint32_t index;
                            auto returnValue = thisValue.as<native_type>().IndexOf(value, index);
                            info.GetReturnValue().Set(make_return_struct(context, returnValue, "index", index));
                        },
                        1, false },
                    { "insertAt",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto index = convert_value_to_native<uint32_t>(context, info[0]);
                            auto value = convert_value_to_native<T>(context, info[1]);
                            thisValue.as<native_type>().InsertAt(index, value);
                        },
                        2, false },
                    { "removeAt",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto index = convert_value_to_native<uint32_t>(context, info[0]);
                            thisValue.as<native_type>().RemoveAt(index);
                        },
                        1, false },
                    { "removeAtEnd",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>&) { thisValue.as<native_type>().RemoveAtEnd(); },
                        0, false },
                    { "replaceAll",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto items = convert_value_to_native<winrt::array_view<const T>>(context, info[0]);
                            thisValue.as<native_type>().ReplaceAll(items);
                        },
                        1, false },
                    { "setAt",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto index = convert_value_to_native<uint32_t>(context, info[0]);
                            auto value = convert_value_to_native<T>(context, info[1]);
                            thisValue.as<native_type>().SetAt(index, value);
                        },
                        2, false },
                };

#if 0 // TODO
                using convert_item_t = T (*)(jsi::Runtime&, const jsi::Value&);

                static convert_item_t item_converter()
                {
                    // NOTE: The optimizer seems to be very agressive at optimizing away function pointers to static
                    // member functions, but a lot less agressive at optimizing away function pointers created from
                    // lambdas, hence why we return a lambda here.
                    return [](jsi::Runtime& runtime, const jsi::Value& value) {
                        return convert_value_to_native<T>(runtime, value);
                    };
                }

                // NOTE: From Array.prototype.copyWithin
                static __declspec(noinline) jsi::Value copyWithin_impl(jsi::Runtime& runtime,
                    const jsi::Value& thisValue, const native_type& vector, const jsi::Value* args, size_t count)
                {
                    auto size = vector.Size();
                    auto target = vector_index_from_arg(runtime, args, count, 0, size);
                    auto start = vector_index_from_arg(runtime, args, count, 1, size);
                    auto end = vector_index_from_arg(runtime, args, count, 2, size, size);

                    if (end > start)
                    {
                        if (target < start) // Forward copy
                        {
                            // Because we're copying to earlier in the list, the [begin, end) pair define the length
                            while (start < end)
                            {
                                vector.SetAt(target++, vector.GetAt(start++));
                            }
                        }
                        else // Reverse copy
                        {
                            // Because we're copying later in the list, either [begin, end) or [target, size) define the
                            // length
                            auto copyCount = std::min(end - start, size - target);
                            while (copyCount-- != 0)
                            {
                                vector.SetAt(target + copyCount, vector.GetAt(start + copyCount));
                            }
                        }
                    }

                    return jsi::Value(runtime, thisValue);
                }

                static jsi::Value copyWithin(
                    jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
                {
                    // NOTE: This causes IVector<T> and IVector<U> to get COMDAT folded together for a similar enough
                    // 'U' (e.g. classes/interfaces get folded together)
                    return copyWithin_impl(
                        runtime, thisValue, convert_value_to_native<native_type>(runtime, thisValue), args, count);
                }

                // NOTE: From Array.prototype.pop
                static __declspec(noinline) jsi::Value
                    pop_impl(jsi::Runtime& runtime, const native_type& vector, const jsi::Value*, size_t)
                {
                    auto size = vector.Size();
                    if (size == 0)
                        return jsi::Value::undefined();

                    auto result = convert_native_to_value(runtime, vector.GetAt(size - 1));
                    vector.RemoveAtEnd();
                    return result;
                }

                static jsi::Value pop(
                    jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
                {
                    // NOTE: This causes IVector<T> and IVector<U> to get COMDAT folded together for a similar enough
                    // 'U' (e.g. classes/interfaces get folded together)
                    return pop_impl(runtime, convert_value_to_native<native_type>(runtime, thisValue), args, count);
                }

                // NOTE: From Array.prototype.push
                static __declspec(noinline) jsi::Value push_impl(jsi::Runtime& runtime, const native_type& vector,
                    const jsi::Value* args, size_t count, convert_item_t converter)
                {
                    // TODO: Why do we have to define this function? Based off the specification, we should satisfy all
                    // requements for Array.prototype.push to work
                    for (size_t i = 0; i < count; ++i)
                    {
                        vector.Append(converter(context, info[i]));
                    }

                    return static_cast<double>(vector.Size());
                }

                static jsi::Value push(
                    jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
                {
                    // NOTE: This causes IVector<T> and IVector<U> to get COMDAT folded together for a similar enough
                    // 'U' (e.g. classes/interfaces get folded together)
                    return push_impl(runtime, convert_value_to_native<native_type>(runtime, thisValue), args, count,
                        item_converter());
                }

                // NOTE: From Array.prototype.reverse
                static __declspec(noinline) jsi::Value reverse_impl(jsi::Runtime& runtime, const jsi::Value& thisValue,
                    const native_type& vector, const jsi::Value*, size_t)
                {
                    auto size = vector.Size();
                    uint32_t left = 0;
                    uint32_t right = (size != 0) ? size - 1 : 0;
                    for (; left < right; ++left, --right)
                    {
                        auto temp = vector.GetAt(right);
                        vector.SetAt(right, vector.GetAt(left));
                        vector.SetAt(left, temp);
                    }

                    return jsi::Value(runtime, thisValue);
                }

                static jsi::Value reverse(
                    jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
                {
                    // NOTE: This causes IVector<T> and IVector<U> to get COMDAT folded together for a similar enough
                    // 'U' (e.g. classes/interfaces get folded together)
                    return reverse_impl(
                        runtime, thisValue, convert_value_to_native<native_type>(runtime, thisValue), args, count);
                }

                // NOTE: From Array.prototype.shift
                static __declspec(noinline) jsi::Value
                    shift_impl(jsi::Runtime& runtime, const native_type& vector, const jsi::Value*, size_t)
                {
                    auto size = vector.Size();
                    if (size == 0)
                        return jsi::Value::undefined();

                    auto result = convert_native_to_value(runtime, vector.GetAt(0));
                    for (uint32_t i = 1; i < size; ++i)
                    {
                        vector.SetAt(i - 1, vector.GetAt(i));
                    }

                    vector.RemoveAtEnd();
                    return result;
                }

                static jsi::Value shift(
                    jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
                {
                    // NOTE: This causes IVector<T> and IVector<U> to get COMDAT folded together for a similar enough
                    // 'U' (e.g. classes/interfaces get folded together)
                    return shift_impl(runtime, convert_value_to_native<native_type>(runtime, thisValue), args, count);
                }

                // NOTE: From Array.prototype.sort
                static __declspec(noinline) jsi::Value sort_impl(jsi::Runtime& runtime, const jsi::Value& thisValue,
                    const native_type& vector, const jsi::Value* args, size_t count, convert_item_t converter)
                {
                    auto size = vector.Size();
                    if (count >= 1)
                    {
                        auto compareFn = args[0].asObject(runtime).asFunction(runtime);

                        std::vector<jsi::Value> values;
                        values.reserve(size);
                        for (auto&& value : vector)
                        {
                            values.push_back(convert_native_to_value(runtime, value));
                        }

                        std::stable_sort(values.begin(), values.end(), [&](auto&& lhs, auto&& rhs) {
                            return to_number(runtime, compareFn.call(runtime, lhs, rhs)) < 0;
                        });

                        for (uint32_t i = 0; i < size; ++i)
                        {
                            vector.SetAt(i, converter(runtime, values[i]));
                        }
                    }
                    else
                    {
                        // Default is to sort 'toString' representations
                        std::vector<std::pair<std::u16string, T>> values;
                        values.reserve(size);
                        for (auto value : vector)
                        {
                            values.emplace_back(
                                string_to_utf16(runtime, convert_native_to_value(runtime, value).toString(runtime)),
                                std::move(value));
                        }

                        std::stable_sort(values.begin(), values.end(),
                            [&](auto&& lhs, auto&& rhs) { return lhs.first < rhs.first; });

                        for (uint32_t i = 0; i < size; ++i)
                        {
                            vector.SetAt(i, values[i].second);
                        }
                    }

                    return jsi::Value(runtime, thisValue);
                }

                static jsi::Value sort(
                    jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
                {
                    return sort_impl(runtime, thisValue, convert_value_to_native<native_type>(runtime, thisValue), args,
                        count, item_converter());
                }

                // NOTE: From Array.prototype.splice
                static __declspec(noinline) jsi::Value splice_impl(jsi::Runtime& runtime, const native_type& vector,
                    const jsi::Value* args, size_t count, convert_item_t converter)
                {
                    auto size = vector.Size();
                    auto start = vector_index_from_arg(runtime, args, count, 0, size);

                    uint32_t deleteCount =
                        (count >= 2) ? static_cast<uint32_t>(std::clamp(to_integer_or_infinity(context, info[1]), 0.0,
                                           static_cast<double>(size - start))) :
                        (count >= 1) ? (size - start) :
                                       0;

                    uint32_t insertCount = (count >= 3) ? static_cast<uint32_t>(count - 2) : 0;
                    auto assignCount = std::min(deleteCount, insertCount);
                    deleteCount -= assignCount;
                    insertCount -= assignCount;

                    jsi::Array result(runtime, deleteCount + assignCount);
                    uint32_t i = 0;
                    while (assignCount-- > 0)
                    {
                        result.setValueAtIndex(runtime, i, convert_native_to_value(runtime, vector.GetAt(start)));
                        vector.SetAt(start, converter(context, info[2 + i]));
                        ++i;
                        ++start;
                    }

                    while (deleteCount-- > 0)
                    {
                        assert(insertCount == 0);
                        result.setValueAtIndex(runtime, i, convert_native_to_value(runtime, vector.GetAt(start)));
                        vector.RemoveAt(start);
                        ++i;
                    }
                    assert(i == result.length(runtime));

                    while (insertCount-- > 0)
                    {
                        vector.InsertAt(start, converter(context, info[2 + i]));
                        ++i;
                        ++start;
                    }

                    return result;
                }

                static jsi::Value splice(
                    jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
                {
                    // NOTE: This causes IVector<T> and IVector<U> to get COMDAT folded together for a similar enough
                    // 'U' (e.g. classes/interfaces get folded together)
                    return splice_impl(runtime, convert_value_to_native<native_type>(runtime, thisValue), args, count,
                        item_converter());
                }

                // NOTE: From Array.prototype.unshift
                static __declspec(noinline) jsi::Value unshift_impl(jsi::Runtime& runtime, const native_type& vector,
                    const jsi::Value* args, size_t count, convert_item_t converter)
                {
                    for (size_t i = count; i-- > 0;)
                    {
                        vector.InsertAt(0, converter(context, info[i]));
                    }

                    return static_cast<double>(vector.Size());
                }

                static jsi::Value unshift(
                    jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count)
                {
                    // NOTE: This causes IVector<T> and IVector<U> to get COMDAT folded together for a similar enough
                    // 'U' (e.g. classes/interfaces get folded together)
                    return unshift_impl(runtime, convert_value_to_native<native_type>(runtime, thisValue), args, count,
                        item_converter());
                }

                static constexpr const struct array_proto_functions_t
                {
                    std::string_view name;
                    call_function_t function;
                    int arg_count;
                } array_proto_functions[] = {
                    { "concat"sv, &vector_concat<native_type>, 1 }, // NOTE: Cannot use Array.prototype.concat since we
                                                                    // cannot satisfy 'IsConcatSpreadable'
                    { "copyWithin"sv, &copyWithin, 2 }, // NOTE: Cannot use Array.prototype.copyWithin since we cannot
                                                        // satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.entries since we satisfy 'CreateArrayIterator' requirements
                    { "every"sv, &vector_every<native_type>, 1 }, // NOTE: Cannot use Array.prototype.every since we
                                                                  // cannot satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.fill since we satisfy all requirements
                    { "filter"sv, &vector_filter<native_type>, 1 }, // NOTE: Cannot use Array.prototype.filter since we
                                                                    // cannot satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.find since we satisfy all requirements
                    // NOTE: Forward to Array.prototype.findIndex since we satisfy all requirements
                    // TODO: flat & flatMap
                    { "forEach"sv, &vector_forEach<native_type>, 1 }, // NOTE: Cannot use Array.prototype.forEach since
                                                                      // we cannot satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.includes since we satisfy all requirements
                    // NOTE: 'indexOf' is a function that exists on IVector, so that takes precedence
                    // NOTE: Forward to Array.prototype.join since we satisfy all requirements
                    // NOTE: Forward to Array.prototype.keys since we satisfy all requirements
                    { "lastIndexOf"sv, &vector_lastIndexOf<native_type>,
                        1 }, // NOTE: Cannot use Array.prototype.lastIndexOf since we
                             // cannot satisfy 'HasProperty'
                    { "map"sv, &vector_map<native_type>, 1 }, // NOTE: Cannot use Array.prototype.map since we cannot
                                                              // satisfy 'HasProperty'
                    { "pop"sv, &pop,
                        0 }, // NOTE: Cannot use Array.prototype.pop since we cannot satisfy 'DeletePropertyOrThrow'
                    { "push"sv, &push,
                        1 }, // TODO: It's not actually clear why we can't just forward to Array.prototype.push
                    { "reduce"sv, &vector_reduce<native_type>, 1 }, // NOTE: Cannot use Array.prototype.reduce since we
                                                                    // cannot satisfy 'HasProperty'
                    { "reduceRight"sv, &vector_reduceRight<native_type>,
                        1 }, // NOTE: Cannot use Array.prototype.reduceRight since we
                             // cannot satisfy 'HasProperty'
                    { "reverse"sv, &reverse,
                        0 }, // NOTE: Cannot use Array.prototype.reverse since we cannot satisfy 'HasProperty'
                    { "shift"sv, &shift,
                        0 }, // NOTE: Cannot use Array.prototype.shift since we cannot satisfy 'HasProperty'
                    { "slice"sv, &vector_slice<native_type>, 2 }, // NOTE: Cannot use Array.prototype.slice since we
                                                                  // cannot satisfy 'HasProperty'
                    { "some"sv, &vector_some<native_type>, 1 }, // NOTE: Cannot use Array.prototype.some since we cannot
                                                                // satisfy 'HasProperty'
                    { "sort"sv, &sort,
                        1 }, // NOTE: Cannot use Array.prototype.sort since we cannot satisfy 'HasProperty'
                    { "splice"sv, &splice,
                        2 }, // NOTE: Cannot use Array.prototype.splice since we cannot satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.toLocaleString since we satisfy all requirements
                    // NOTE: Forward to Array.prototype.toString since we satisfy all requirements
                    { "unshift"sv, &unshift,
                        1 }, // NOTE: Cannot use Array.prototype.unshift since we cannot satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.values since we satisfy 'CreateArrayIterator' requirements
                };

                static std::pair<std::optional<jsi::Value>, std::optional<jsi::Value>> runtime_get_property_impl(
                    jsi::Runtime& runtime, const IInspectable& thisValue, std::string_view name,
                    span<const array_proto_functions_t> protoFunctions, native_type (*queryThis)(const IInspectable&))
                {
                    // If the "property" is a number, then that translates to a 'GetAt' call
                    if (auto index = index_from_name(name))
                    {
                        auto vector = queryThis(thisValue);
                        if (*index >= vector.Size())
                        {
                            return { jsi::Value::undefined(), std::nullopt };
                        }

                        return { convert_native_to_value(runtime, vector.GetAt(*index)), std::nullopt };
                    }

                    auto itr = std::find_if(
                        protoFunctions.begin(), protoFunctions.end(), [&](auto& pair) { return pair.name == name; });
                    if (itr != protoFunctions.end())
                    {
                        return { jsi::Function::createFromHostFunction(
                                     runtime, make_propid(runtime, name), itr->arg_count, itr->function),
                            std::nullopt };
                    }

                    return { std::nullopt, fwd_array_prototype(runtime, name) };
                }

                static std::pair<std::optional<jsi::Value>, std::optional<jsi::Value>> runtime_get_property(
                    jsi::Runtime& runtime, const IInspectable& thisValue, std::string_view name)
                {
                    return runtime_get_property_impl(runtime, thisValue, name, array_proto_functions,
                        [](const IInspectable& thisVal) { return thisVal.as<native_type>(); });
                }

                static bool runtime_set_property(jsi::Runtime& runtime, const IInspectable& thisValue,
                    std::string_view name, const jsi::Value& value)
                {
                    // If the "property" is a number, then that translates to a 'SetAt' call
                    if (auto index = index_from_name(name))
                    {
                        auto vector = thisValue.as<native_type>();
                        if (*index == vector.Size())
                        {
                            // Following Chakra's behavior, assigning to one-past the end appends
                            vector.Append(convert_value_to_native<T>(runtime, value));
                        }
                        else
                        {
                            vector.SetAt(*index, convert_value_to_native<T>(runtime, value));
                        }

                        return true;
                    }

                    return false;
                }
#endif
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(),
                    iface::properties, {}, iface::functions,
#if 0 // TODO
                    &iface::runtime_get_property,
                    &iface::runtime_set_property
#endif
                    };
            };

            template <typename T>
            constexpr const static_interface_data& data = data_t<T>::value;
        }

        namespace IVectorView
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IVectorView<T>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "length", // NOTE: From Array.prototype
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Size());
                        },
                        nullptr },
                    { "size",
                        [](runtime_context* context, const IInspectable& thisValue) {
                            return convert_native_to_value(context, thisValue.as<native_type>().Size());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "getAt",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto index = convert_value_to_native<uint32_t>(context, info[0]);
                            info.GetReturnValue().Set(
                                convert_native_to_value(context, thisValue.as<native_type>().GetAt(index)));
                        },
                        1, false },
                    { "getMany",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto startIndex = convert_value_to_native<uint32_t>(context, info[0]);
                            auto items = convert_value_to_native<winrt::array_view<T>>(context, info[1]);
                            info.GetReturnValue().Set(convert_native_to_value(
                                context, thisValue.as<native_type>().GetMany(startIndex, items)));
                        },
                        2, false },
                    { "indexOf",
                        [](runtime_context* context, const IInspectable& thisValue,
                            const v8::FunctionCallbackInfo<v8::Value>& info) {
                            auto value = convert_value_to_native<T>(context, info[0]);
                            uint32_t index;
                            auto returnValue = thisValue.as<native_type>().IndexOf(value, index);
                            info.GetReturnValue().Set(make_return_struct(context, returnValue, "index", index));
                        },
                        1, false },
                };

#if 0 // TODO
                static constexpr const struct array_proto_functions_t
                {
                    std::string_view name;
                    call_function_t function;
                    int arg_count;
                } array_proto_functions[] = {
                    { "concat"sv, &vector_concat<native_type>, 1 }, // NOTE: Cannot use Array.prototype.concat since we
                                                                    // cannot satisfy 'IsConcatSpreadable'
                    // NOTE: Array.prototype.copyWithin is a modify operation. Let this fall through
                    // NOTE: Forward to Array.prototype.entries since we satisfy 'CreateArrayIterator' requirements
                    { "every"sv, &vector_every<native_type>, 1 }, // NOTE: Cannot use Array.prototype.every since we
                                                                  // cannot satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.fill since we satisfy all requirements
                    { "filter"sv, &vector_filter<native_type>, 1 }, // NOTE: Cannot use Array.prototype.filter since we
                                                                    // cannot satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.find since we satisfy all requirements
                    // NOTE: Forward to Array.prototype.findIndex since we satisfy all requirements
                    // TODO: flat & flatMap
                    { "forEach"sv, &vector_forEach<native_type>, 1 }, // NOTE: Cannot use Array.prototype.forEach since
                                                                      // we cannot satisfy 'HasProperty'
                    // NOTE: Forward to Array.prototype.includes since we satisfy all requirements
                    // NOTE: 'indexOf' is a function that exists on IVector, so that takes precedence
                    // NOTE: Forward to Array.prototype.join since we satisfy all requirements
                    // NOTE: Forward to Array.prototype.keys since we satisfy all requirements
                    { "lastIndexOf"sv, &vector_lastIndexOf<native_type>,
                        1 }, // NOTE: Cannot use Array.prototype.lastIndexOf since we
                             // cannot satisfy 'HasProperty'
                    { "map"sv, &vector_map<native_type>, 1 }, // NOTE: Cannot use Array.prototype.map since we cannot
                                                              // satisfy 'HasProperty'
                    // NOTE: Array.prototype.pop is a modify operation. Let this fall through
                    // NOTE: Array.prototype.push is a modify operation. Let this fall through
                    { "reduce"sv, &vector_reduce<native_type>, 1 }, // NOTE: Cannot use Array.prototype.reduce since we
                                                                    // cannot satisfy 'HasProperty'
                    { "reduceRight"sv, &vector_reduceRight<native_type>,
                        1 }, // NOTE: Cannot use Array.prototype.reduceRight since we
                             // cannot satisfy 'HasProperty'
                    // NOTE: Array.prototype.reverse is a modify operation. Let this fall through
                    // NOTE: Array.prototype.shift is a modify operation. Let this fall through
                    { "slice"sv, &vector_slice<native_type>, 2 }, // NOTE: Cannot use Array.prototype.slice since we
                                                                  // cannot satisfy 'HasProperty'
                    { "some"sv, &vector_some<native_type>, 1 }, // NOTE: Cannot use Array.prototype.some since we cannot
                                                                // satisfy 'HasProperty'
                    // NOTE: Array.prototype.sort is a modify operation. Let this fall through
                    // NOTE: Array.prototype.splic is a modify operation. Let this fall through
                    // NOTE: Forward to Array.prototype.toLocaleString since we satisfy all requirements
                    // NOTE: Forward to Array.prototype.toString since we satisfy all requirements
                    // NOTE: Array.prototype.unshift is a modify operation. Let this fall through
                    // NOTE: Forward to Array.prototype.values since we satisfy 'CreateArrayIterator' requirements
                };

                static std::pair<std::optional<jsi::Value>, std::optional<jsi::Value>> runtime_get_property_impl(
                    jsi::Runtime& runtime, const IInspectable& thisValue, std::string_view name,
                    span<const array_proto_functions_t> protoFunctions, native_type (*queryThis)(const IInspectable&))
                {
                    // If the "property" is a number, then that translates to a 'GetAt' call
                    if (auto index = index_from_name(name))
                    {
                        auto vector = queryThis(thisValue);
                        if (*index >= vector.Size())
                        {
                            return { jsi::Value::undefined(), std::nullopt };
                        }

                        return { convert_native_to_value(runtime, vector.GetAt(*index)), std::nullopt };
                    }

                    auto itr = std::find_if(
                        protoFunctions.begin(), protoFunctions.end(), [&](auto& pair) { return pair.name == name; });
                    if (itr != protoFunctions.end())
                    {
                        return { jsi::Function::createFromHostFunction(
                                     runtime, make_propid(runtime, name), itr->arg_count, itr->function),
                            std::nullopt };
                    }

                    return { std::nullopt, fwd_array_prototype(runtime, name) };
                }

                static std::pair<std::optional<jsi::Value>, std::optional<jsi::Value>> runtime_get_property(
                    jsi::Runtime& runtime, const IInspectable& thisValue, std::string_view name)
                {
                    return runtime_get_property_impl(runtime, thisValue, name, array_proto_functions,
                        [](const IInspectable& thisVal) { return thisVal.as<native_type>(); });
                }
#endif
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value{ winrt::guid_of<typename iface::native_type>(),
                    iface::properties, {}, iface::functions,
#if 0 // TODO
                    &iface::runtime_get_property
#endif
                    };
            };

            template <typename T>
            constexpr const static_interface_data& data = data_t<T>::value;
        }
    }
}

#include "ProjectedValueConverters.g.h"
