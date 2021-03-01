#pragma once

#if 1

#include <atomic>
#include <jsi/jsi.h>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <WeakReference.h>
#include <winrt/Windows.Foundation.h>

// Common helpers/types
namespace jswinrt
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

            new (data() + m_size) T(value);
            ++m_size;
        }

        void push_back(T&& value)
        {
            if (m_size == m_capacity)
            {
                reserve(m_capacity * 2);
            }

            new (data() + m_size) T(std::move(value));
            ++m_size;
        }

        template <typename... Args>
        void emplace_back(Args&&... args)
        {
            if (m_size == m_capacity)
            {
                reserve(m_capacity * 2);
            }

            new (data() + m_size) T(std::forward<Args>(args)...);
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
                other.m_capacity = OhterBufferSize;
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
}

// Generic helpers
namespace jswinrt
{
    namespace jsi = facebook::jsi;
    using namespace std::string_literals;

    using call_function_t = jsi::Value (*)(jsi::Runtime&, const jsi::Value&, const jsi::Value*, size_t);

    using static_get_property_t = jsi::Value (*)(jsi::Runtime&);
    using static_set_property_t = void (*)(jsi::Runtime&, const jsi::Value&);
    using static_add_event_t = winrt::event_token (*)(jsi::Runtime&, const jsi::Value&);
    using static_remove_event_t = void (*)(jsi::Runtime&, winrt::event_token);

    using instance_get_property_t = jsi::Value (*)(jsi::Runtime&, const winrt::Windows::Foundation::IInspectable&);
    using instance_set_property_t = void (*)(
        jsi::Runtime&, const winrt::Windows::Foundation::IInspectable&, const jsi::Value&);
    using instance_add_event_t = winrt::event_token (*)(
        jsi::Runtime&, const winrt::Windows::Foundation::IInspectable&, const jsi::Value&);
    using instance_remove_event_t = void (*)(
        jsi::Runtime&, const winrt::Windows::Foundation::IInspectable&, const jsi::Value&, winrt::event_token);
    using instance_call_function_t = jsi::Value (*)(
        jsi::Runtime&, const winrt::Windows::Foundation::IInspectable&, const jsi::Value*);

    inline constexpr std::string_view add_event_name = "addEventListener"sv;
    inline constexpr std::string_view remove_event_name = "removeEventListener"sv;

    inline jsi::String make_string(jsi::Runtime& runtime, std::string_view str)
    {
        return jsi::String::createFromUtf8(runtime, reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    jsi::String make_string(jsi::Runtime& runtime, std::wstring_view str);

    inline jsi::PropNameID make_propid(jsi::Runtime& runtime, std::string_view str)
    {
        return jsi::PropNameID::forUtf8(runtime, reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    inline jsi::Value make_error(jsi::Runtime& runtime, std::string_view message)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "message", make_string(runtime, message));
        return jsi::Value(runtime, result);
    }

    inline jsi::Value make_error(jsi::Runtime& runtime, const std::exception& exception)
    {
        return make_error(runtime, exception.what());
    }

    inline jsi::Value make_error(jsi::Runtime& runtime, const winrt::hresult_error& error)
    {
        jsi::Object result(runtime);
        result.setProperty(runtime, "message", make_string(runtime, error.message()));
        result.setProperty(runtime, "number", static_cast<int32_t>(error.code()));
        return jsi::Value(runtime, result);
    }

    template <typename T>
    auto bind_host_function(jsi::Value (T::*fn)(jsi::Runtime&, const jsi::Value*, size_t))
    {
        return [fn](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) {
            auto strongThis = thisValue.asObject(runtime).asHostObject<T>(runtime);
            return (strongThis->*fn)(runtime, args, count);
        }
    }

    template <typename T>
    auto bind_host_function(jsi::Value (T::*fn)(jsi::Runtime&, const jsi::Value*, size_t) const)
    {
        return [fn](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) {
            auto strongThis = thisValue.asObject(runtime).asHostObject<T>(runtime);
            return (strongThis->*fn)(runtime, args, count);
        }
    }

    template <typename T, typename Enable = void>
    struct projected_value_traits
    {
        [[noreturn]] static jsi::Value as_value(jsi::Runtime&, const T&)
        {
            throw jsi::JSError(runtime, "TypeError: Conversion from native to JS not implemented for type '"s +
                                            typeid(T).name() +
                                            "'. This is likely caused by the type being in a non-projected namespace");
        }

        [[noreturn]] static T as_native(jsi::Runtime&, const jsi::Value&)
        {
            throw jsi::JSError(runtime, "TypeError: Conversion from JS to native not implemented for type '"s +
                                            typeid(T).name() +
                                            "'. This is likely caused by the type being in a non-projected namespace");
        }
    };

    template <typename T>
    jsi::Value convert_native_to_value(jsi::Runtime& runtime, T&& value)
    {
        return projected_value_traits<T>::as_value(runtime, std::forward<T>(value));
    }

    template <typename T>
    T convert_value_to_native(jsi::Runtime& runtime, const jsi::Value& value)
    {
        return projected_value_traits<T>::as_native(runtime, value);
    }

    namespace details
    {
        inline void fill_return_struct(jsi::Runtime&, jsi::Object&)
        {
        }

        template <typename T, typename... Args>
        void fill_return_struct(
            jsi::Runtime& runtime, jsi::Object& target, std::string_view name, const T& value, const Args&... args)
        {
            target.setProperty(runtime, make_propid(name), convert_native_to_value(runtime, value));
            fill_return_struct(runtime, target, args...);
        }
    }

    template <typename T, typename... Args>
    jsi::Value make_void_return_struct(jsi::Runtime& runtime, const Args&... args)
    {
        jsi::Object result(runtime);
        details::fill_return_struct(runtime, result, args...);
        return jsi::Value(runtime, result);
    }

    template <typename T, typename... Args>
    jsi::Value make_return_struct(jsi::Runtime& runtime, const Args&... args)
    {
        return make_void_return_struct(runtime, "returnValue", args...);
    }

    struct pinterface_traits_base
    {
        static constexpr bool is_async_with_progress = false;
        static constexpr bool is_async_with_result = false;
    };

    template <typename T>
    struct pinterface_traits : pinterface_traits_base
    {
    };

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

    struct promise_wrapper
    {
        static promise_wrapper create(jsi::Runtime& runtime)
        {
            // NOTE: The promise callback is called immediately, hence the capture by reference
            std::optional<jsi::Function> resolve, reject;
            auto callback = jsi::Function::createFromHostFunction(runtime, make_propid(runtime, "callback"), 2,
                [&](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count < 2)
                    {
                        throw jsi::JSError(runtime, "Promise callback unexpectedly called with insufficient arguments");
                    }

                    resolve = args[0].asObject(runtime).asFunction(runtime);
                    reject = args[1].asObject(runtime).asFunction(runtime);
                    return jsi::Value::undefined();
                });

            auto promise =
                runtime.global().getPropertyAsFunction(runtime, "Promise").callAsConstructor(runtime, callback);
            return promise_wrapper(std::move(promise), std::move(*resolve), std::move(*reject));
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
        promise_wrapper(jsi::Value promise, jsi::Function resolve, jsi::Function reject) :
            m_promise(std::move(promise)), m_resolve(std::move(resolve)), m_reject(std::move(reject))
        {
        }

        jsi::Value m_promise;
        bool m_completed = false;
        jsi::Function m_resolve;
        jsi::Function m_reject;
    };
}

// JS thread context data
namespace jswinrt
{
    struct runtime_context;

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

    struct object_instance_cache
    {
        // Maps an IInspectable pointer to the HostObject that represents that object. Note that it is possible for a
        // WinRT object to get destroyed and have its memory location re-used for a later object. This is okay since the
        // HostObject holds a strong reference to the WinRT object and therefore the WinRT object getting destroyed
        // would therefore imply that the JS object also got destroyed, meaning we won't accidentally re-use the same
        // HostObject after its underlying object got destroyed
        std::unordered_map<void*, std::variant<jsi::WeakObject, std::weak_ptr<jsi::HostObject>>> instances;

        // TODO: This is kind of a hack/workaround for V8, which does not appear to have WeakObject support per
        // V8Runtime::createWeakObject/V8Runtime::lockWeakObject
        // (https://github.com/microsoft/v8-jsi/blob/master/src/V8JsiRuntime.cpp)
        bool supports_weak_object = true;

        jsi::Value get_instance(jsi::Runtime& runtime, const winrt::Windows::Foundation::IInspectable& value);
    };

    // A reusable mapping of (delegate, event name) -> event_token for a single object
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

        void add(const winrt::Windows::Foundation::IInspectable& instance, jsi::Object object, const char* eventName,
            winrt::event_token token)
        {
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
                return itr->second.registrations.remove(runtime, object, eventName);
            }

            return {};
        }
    };

    struct runtime_context
    {
        jsi::Runtime& runtime;
        std::thread::id thread_id = std::this_thread::get_id();
        std::function<void(std::function<void()>)> call_invoker;

        // TODO: Clean up maps periodically
        object_instance_cache instance_cache;
        object_event_cache event_cache;

        runtime_context(jsi::Runtime& runtime, std::function<void(std::function<void()>)> callInvoker) :
            runtime(runtime), call_invoker(callInvoker)
        {
        }

        void call(std::function<void()> fn)
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

        void call_async(std::function<void()> fn)
        {
            call_invoker(std::move(fn));
        }

        void call_sync(std::function<void()> fn)
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

                // TODO: Propagate exceptions?
                call_invoker([&]() {
                    fn();
                    ::SetEvent(event.get());
                });

                if (::WaitForSingleObject(event.get(), INFINITE) != WAIT_OBJECT_0)
                {
                    // TODO: Too harsh? We could release/leak the event & capture by value
                    winrt::terminate();
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
}

// Types used to store static data
namespace jswinrt
{
    // NOTE: All of these instances are intended to go into the .text section and hold no state that needs to be free'd,
    // hence the lack of a virtual destructor
    struct static_projection_data
    {
        constexpr static_projection_data(std::string_view name) : name(name)
        {
        }

        virtual jsi::Value create(jsi::Runtime& runtime) const = 0;

        std::string_view name; // E.g. 'Baz' for the type/namespace/etc. 'Foo.Bar.Baz'
    };

    struct static_namespace_data final : static_projection_data
    {
        constexpr static_namespace_data(std::string_view name, span<const static_projection_data* const> children) :
            static_projection_data(name), children(children)
        {
        }

        virtual jsi::Value create(jsi::Runtime& runtime) const override;

        span<const static_projection_data* const> children;
    };

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

        virtual jsi::Value create(jsi::Runtime& runtime) const override;

        jsi::Value get_value(std::string_view valueName) const;

        span<const value_mapping> values;
    };

    extern const span<const static_namespace_data* const> root_namespaces;

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

        virtual jsi::Value create(jsi::Runtime& runtime) const override;

        span<const property_mapping> properties;
        span<const event_mapping> events;
        span<const function_mapping> functions;
    };

    struct static_activatable_class_data final : static_class_data
    {
        constexpr static_activatable_class_data(std::string_view name, call_function_t constructor,
            span<const property_mapping> properties, span<const event_mapping> events,
            span<const function_mapping> functions) :
            static_class_data(name, properties, events, functions),
            constructor(constructor)
        {
        }

        virtual jsi::Value create(jsi::Runtime& runtime) const override;

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
            size_t arity;
            bool is_default_overload;
        };

        constexpr static_interface_data(const winrt::guid& guid, span<const property_mapping> properties,
            span<const event_mapping> events, span<const function_mapping> functions) :
            guid(guid),
            properties(properties), events(events), functions(functions)
        {
        }

        winrt::guid guid;
        span<const property_mapping> properties;
        span<const event_mapping> events;
        span<const function_mapping> functions;
    };

    extern const span<const std::pair<winrt::guid, const static_interface_data*>> global_interface_map;
}

// Types used for object instances, etc.
namespace jswinrt
{
    struct projected_namespace final : public jsi::HostObject
    {
        projected_namespace(const static_namespace_data* data) : m_data(data)
        {
            m_children.resize(data->children.size());
        }

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        const static_namespace_data* m_data;
        std::vector<jsi::Value> m_children;
    };

    struct projected_enum final : public jsi::HostObject
    {
        projected_enum(const static_enum_data* data) : m_data(data)
        {
        }

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        const static_enum_data* m_data;
    };

    struct projected_statics_class final : public jsi::HostObject
    {
        projected_statics_class(const static_class_data* data) : m_data(data)
        {
        }

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

    private:
        jsi::Value add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);
        jsi::Value remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);

        const static_class_data* m_data;
        std::unordered_map<std::string_view, jsi::Value> m_functions;
        event_registration_array m_events;
    };

    struct projected_function;
    struct projected_overloaded_function;

    struct projected_object_instance : public jsi::HostObject
    {
        friend struct projected_function;
        friend struct projected_overloaded_function;

        projected_object_instance(const winrt::Windows::Foundation::IInspectable& instance);

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override;
        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override;
        virtual std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime& runtime) override;

        const winrt::Windows::Foundation::IInspectable& instance() const noexcept
        {
            return m_instance;
        }

    private:
        jsi::Value add_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);
        jsi::Value remove_event_listener(jsi::Runtime& runtime, const jsi::Value* args, size_t count);

        winrt::Windows::Foundation::IInspectable m_instance;
        sso_vector<const static_interface_data*> m_interfaces;
        std::unordered_map<std::string_view, jsi::Value> m_functions;
    };

    template <typename IFace>
    struct projected_async_instance :
        public jsi::HostObject,
        public std::enable_shared_from_this<projected_async_instance>
    {
        using traits = pinterface_traits<IFace>;

        projected_async_instance(IFace instance) : m_instance(std::move(instance))
        {
            initialize(m_instance);

            if constexpr (traits::is_async_with_progress) // IAsync*WithProgress
            {
                m_instance.Progress([weakThis = weak_from_this(), ctxt = current_runtime_context()->add_reference()](
                                        const auto&, const auto& progress) {
                    ctxt->call([progress, weakThis]() {
                        if (auto strongThis = weakThis.lock())
                        {
                            strongThis->on_progress(
                                current_runtime_context()->runtime, convert_native_to_value(progress));
                        }
                    });
                });
            }
        }

        // HostObject functions
        virtual jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& id) override
        {
            auto name = id.utf8(runtime);
            if (name == "cancel"sv)
            {
                return jsi::Value(
                    runtime, jsi::Function::createFromHostFunction(runtime, id, 0, bind_host_function(on_cancel)));
            }
            else if (name == "catch"sv)
            {
                return jsi::Value(
                    runtime, jsi::Function::createFromHostFunction(runtime, id, 1, bind_host_function(on_catch)));
            }
            else if (name == "done"sv)
            {
                return jsi::Value(
                    runtime, jsi::Function::createFromHostFunction(runtime, id, 3, bind_host_function(on_done)));
            }
            else if (name == "finally"sv)
            {
                return jsi::Value(
                    runtime, jsi::Function::createFromHostFunction(runtime, id, 3, bind_host_function(on_finally)));
            }
            else if (name == "then"sv)
            {
                return jsi::Value(
                    runtime, jsi::Function::createFromHostFunction(runtime, id, 3, bind_host_function(on_then)));
            }
            else if (name == "operation"sv)
            {
                return current_runtime_context()->instance_cache.get_instance(runtime, m_instance);
            }

            return jsi::Value::undefined();
        }

        virtual void set(jsi::Runtime& runtime, const jsi::PropNameID& name, const jsi::Value& value) override
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
        winrt::fire_and_forget initialize(IFace inst)
        {
            auto weakThis = weak_from_this();
            auto ctxt = current_runtime_context()->add_reference();
            auto& runtime = ctxt->runtime;
            try
            {
                if constexpr (!traits::is_async_with_result) // IAsyncAction*
                {
                    co_await inst;
                    ctxt->call([weakThis, &runtime]() {
                        if (auto strongThis = weakThis.lock())
                        {
                            strongThis->on_completed(runtime, jsi::Value::undefined(), true);
                        }
                    });
                }
                else // IAsyncOperation*
                {
                    auto result = co_await inst;
                    ctxt->call([weakThis, &runtime, result = std::move(result)]() {
                        if (auto strongThis = weakThis.lock())
                        {
                            strongThis->on_completed(runtime, convert_native_to_value(result), true);
                        }
                    });
                }
            }
            catch (winrt::hresult_error& err)
            {
                ctxt->call([weakThis, &runtime, err = std::move(err)]() {
                    if (auto strongThis = weakThis.lock())
                    {
                        strongThis->on_completed(runtime, make_error(runtime, err), false);
                    }
                });
            }
            catch (std::exception& err)
            {
                ctxt->call([weakThis, &runtime, err = std::move(err)]() {
                    if (auto strongThis = weakThis.lock())
                    {
                        strongThis->on_completed(runtime, make_error(runtime, err), false);
                    }
                });
            }
        }

        jsi::Value on_cancel(jsi::Runtime& runtime, const jsi::Value*, size_t)
        {
            m_instance.Cancel();
            return jsi::Value::undefined();
        }

        jsi::Value on_catch(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
        {
            // Prototype: catch(rejectCallback) -> Promise
            // NOTE: Equivalent to 'then(null, rejectCallback)'
            return handle_then(runtime, jsi::Value::undefined(), count >= 1 ? args[0] : jsi::Value::undefined());
        }

        jsi::Value on_done(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
        {
            // Prototype: done(resolveCallback, rejectCallback, progressCallback)
            continuation c{ continuation::type::done, {},
                { count >= 1 ? jsi::Value(runtime, args[0]) : jsi::Value::undefined(),
                    count >= 2 ? jsi::Value(runtime, args[1]) : jsi::Value::undefined(),
                    count >= 3 ? jsi::Value(runtime, args[2]) : jsi::Value::undefined() } };
            handle_continuation(runtime, std::move(c));

            return jsi::Value::undefined();
        }

        jsi::Value on_finally(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
        {
            // Prototype: finally(callback) -> Promise
            continuation c{ continuation::type::finally, promise_wrapper::create(runtime),
                { count >= 1 ? jsi::Value(runtime, args[0]) : jsi::Value::undefined() } };

            auto result = jsi::Value(runtime, c.get());
            handle_continuation(runtime, std::move(c));

            return result;
        }

        jsi::Value on_then(jsi::Runtime& runtime, const jsi::Value* args, size_t count)
        {
            // Prototype: then(resolvedCallback, rejectedCallback)
            return handle_then(runtime, count >= 1 ? args[0] : jsi::Value::undefined(),
                count >= 2 ? args[1] : jsi::Value::undefined());
        }

        jsi::Value handle_then(
            jsi::Runtime& runtime, const jsi::Value& resolvedHandler, const jsi::Value& rejectedHandler)
        {
            continuation c{ continuation::type::then, promise_wrapper::create(runtime),
                { jsi::Value(runtime, resolvedHandler), jsi::Value(runtime, rejectedHandler) } };

            auto result = jsi::Value(runtime, c.get());
            handle_continuation(runtime, std::move(c));

            return result;
        }

        void handle_continuation(jsi::Runtime& runtime, continuation&& c)
        {
            if (m_state != state::pending)
            {
                // NOTE: Still expected to be async
                // NOTE: The callback will occur on the same thread, implying that the 'Runtime' instance will still be
                // alive and valid, hence the ref is safe
                current_runtime_context()->call_async(
                    [&runtime, strongThis = shared_from_this(), cont = std::move(c)]() {
                        strongThis->dispatch_continuation(runtime, cont);
                    });
            }
            else
            {
                m_continuations.push_back(std::move(c));
            }
        }

        void on_progress(jsi::Runtime& runtime, const jsi::Value& progress)
        {
            for (auto& cont : m_continuations)
            {
                if ((cont.kind == continuation::type::done) && cont.data[2].isFunction(runtime))
                {
                    cont.data[2].getFunction(runtime).call(runtime, progress);
                }
            }
        }

        void on_completed(jsi::Runtime& runtime, const jsi::Value& value, bool resolved)
        {
            assert(m_state == state::pending);
            m_state = resolved ? state::resolved : state::rejected;

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
            case continuation::type::then:
            case continuation::type::done: // Effectively 'then', but with progress and no promise
                if (resolved)
                {
                    if (cont.data[0].isFunction(runtime))
                    {
                        continuationResult = cont.data[0].getFunction(runtime).call(runtime, m_result);
                    }
                }
                else // rejected
                {
                    if (cont.data[1].isFunction(runtime))
                    {
                        continuationResult = cont.data[1].getFunction(runtime).call(runtime, m_result);
                        resolved = true;
                    }
                }
                break;

            case continuation::type::finally:
                // NOTE: Finally does not change the value, nor the resolved state
                if (cont.data[0].isFunction(runtime))
                {
                    cont.data[0].getFunction(runtime).call(runtime, m_result);
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

        struct continuation
        {
            enum class type
            {
                then, // Covers both .then and .catch
                finally,
                done,
            };

            type kind;
            std::optional<promise_wrapper> promise;
            jsi::Value data[3]; // Contents depend on 'kind'
        };

        enum class state
        {
            pending,
            resolved,
            rejected,
        };

        IFace m_instance;
        sso_vector<continuation, 1> m_continuations;
        state m_state = state::pending;
        jsi::Value m_result;
    };
}

// Collections wrappers
namespace jswinrt
{
    // Implementations for passing arrays as various iterable types
    template <typename D, typename T>
    struct array_iterator :
        winrt::implements<array_iterator<D, T>, winrt::Windows::Foundation::Collections::IIterator<T>>
    {
        array_iterator(D* target) : target(target)
        {
        }

        T Current()
        {
            target->CheckThread();
            throw winrt::hresult_not_implemented(); // TODO
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
        array_vector_base(std::shared_ptr<ProjectionsContext> context, jsi::Array array) :
            context(std::move(context)), array(std::move(array)), thread_id(::GetCurrentThreadId())
        {
        }

        void CheckThread()
        {
            if (thread_id != ::GetCurrentThreadId())
            {
                throw winrt::hresult_wrong_thread{};
            }
        }

        // IIterator functions
        winrt::Windows::Foundation::Collections::IIterator<T> First()
        {
            return winrt::make<array_iterator<D, T>>(static_cast<D*>(this));
        }

        // IVectorView functions
        std::uint32_t Size()
        {
            CheckThread();
            return static_cast<std::uint32_t>(array.size(context->Runtime));
        }

        T GetAt(std::uint32_t index)
        {
            CheckThread();
            // TODO
            throw winrt::hresult_not_implemented{};
        }

        std::uint32_t GetMany(std::uint32_t startIndex, winrt::array_view<T> const& items)
        {
            CheckThread();
            // TODO
            throw winrt::hresult_not_implemented{};
        }

        bool IndexOf(T const& value, std::uint32_t& index)
        {
            CheckThread();
            // TODO
            throw winrt::hresult_not_implemented{};
        }

        // IVector functions, with the exception of 'GetView'
        void Append(T const& value)
        {
            CheckThread();
            // TODO: Currently, jsi::Array does not allow modification of size. We might be able to get around this by
            // invoking javascript methods directly? E.g. call 'push'
            throw winrt::hresult_not_implemented{};
        }

        void Clear()
        {
            CheckThread();
            // TODO: Currently, jsi::Array does not allow modification of size. We might be able to get around this by
            // invoking javascript methods directly?
            throw winrt::hresult_not_implemented{};
        }

        void InsertAt(std::uint32_t index, T const& value)
        {
            CheckThread();
            // TODO: Currently, jsi::Array does not allow modification of size. We might be able to get around this by
            // invoking javascript methods directly? E.g. call 'splice'
            throw winrt::hresult_not_implemented{};
        }

        void RemoveAt(std::uint32_t index)
        {
            CheckThread();
            // TODO: Currently, jsi::Array does not allow modification of size. We might be able to get around this by
            // invoking javascript methods directly? E.g. call 'splice'
            throw winrt::hresult_not_implemented{};
        }

        void RemoveAtEnd()
        {
            CheckThread();
            // TODO: Currently, jsi::Array does not allow modification of size. We might be able to get around this by
            // invoking javascript methods directly? E.g. call 'splice'
            throw winrt::hresult_not_implemented{};
        }

        void ReplaceAll(winrt::array_view<const T> const& items)
        {
            CheckThread();
            // TODO: Currently, jsi::Array does not allow modification of size. We might be able to get around this by
            // invoking javascript methods directly?
            throw winrt::hresult_not_implemented{};
        }

        void SetAt(std::uint32_t index, T const& value)
        {
            CheckThread();
            // TODO
        }

        std::shared_ptr<ProjectionsContext> context;
        jsi::Array array;
        DWORD thread_id;
    };

    template <typename T>
    struct array_iterable :
        winrt::implements<array_iterable<T>, winrt::Windows::Foundation::Collections::IIterable<T>>,
        array_vector_base<array_iterable<T>, T>
    {
        using array_vector_base::array_vector_base;
    };

    template <typename T>
    struct array_vector_view :
        winrt::implements<array_vector_view<T>, winrt::Windows::Foundation::Collections::IVectorView<T>>,
        array_vector_base<array_vector_view<T>, T>
    {
        using array_vector_base::array_vector_base;
    };

    template <typename T>
    struct array_vector :
        winrt::implements<array_vector<T>, winrt::Windows::Foundation::Collections::IVector<T>>,
        array_vector_base<array_vector<T>, T>
    {
        using array_vector_base::array_vector_base;

        winrt::Windows::Foundation::Collections::IVectorView<T> GetView()
        {
            // TODO: Is there a better way to "copy" the array reference?
            return winrt::make<array_vector_view<T>>(context, array.getArray(context->Runtime));
        }
    };
}

// Value converters
namespace jswinrt
{
    template <>
    struct projected_value_traits<bool>
    {
        static jsi::Value as_value(jsi::Runtime&, bool value) noexcept
        {
            return jsi::Value(value);
        }

        static bool as_native(jsi::Runtime&, const jsi::Value& value) noexcept;
    };

    template <typename T>
    struct projected_value_traits<T, std::enable_if_t<std::is_arithmetic_v<T>>>
    {
        static jsi::Value as_value(jsi::Runtime&, T value) noexcept
        {
            return jsi::Value(static_cast<double>(value));
        }

        static T as_native(jsi::Runtime&, const jsi::Value& value)
        {
            return static_cast<T>(value.asNumber());
        }
    };

    template <typename T>
    struct projected_value_traits<T, std::enable_if_t<std::is_enum_v<T>>>
    {
        static jsi::Value as_value(jsi::Runtime&, T value) noexcept
        {
            return jsi::Value(static_cast<double>(static_cast<std::underlying_type_t<T>>(value)));
        }

        static T as_native(jsi::Runtime&, const jsi::Value& value)
        {
            return static_cast<T>(static_cast<std::underlying_type_t<T>>(value.asNumber()));
        }
    };

    template <>
    struct projected_value_traits<char16_t>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, char16_t value);
        static char16_t as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::hstring>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::hstring& value)
        {
            return make_string(runtime, static_cast<std::wstring_view>(value));
        }

        static winrt::hstring as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::guid>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::guid& value);
        static winrt::guid as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::hresult>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, winrt::hresult value)
        {
            return projected_value_traits<int32_t>::as_value(runtime, static_cast<int32_t>(value));
        }

        static winrt::hresult as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            return winrt::hresult(projected_value_traits<int32_t>::as_native(runtime, value));
        }
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::DateTime>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, winrt::Windows::Foundation::DateTime value);
        static winrt::Windows::Foundation::DateTime as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::TimeSpan>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, winrt::Windows::Foundation::TimeSpan value);
        static winrt::Windows::Foundation::TimeSpan as_native(jsi::Runtime& runtime, const jsi::Value& value);
    };

    // TODO: Arrays

    jsi::Value convert_from_property_value(
        jsi::Runtime& runtime, const winrt::Windows::Foundation::IPropertyValue& value);
    winrt::Windows::Foundation::IInspectable convert_to_property_value(jsi::Runtime& runtime, const jsi::Value& value);

    template <typename T>
    struct projected_value_traits<T, std::enable_if_t<std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>>>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const T& value)
        {
            if (!value)
            {
                return jsi::Value::null();
            }

            if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IPropertyValue>)
            {
                if (auto result = convert_from_property_value(runtime, value); !result.isUndefined())
                {
                    return result;
                }
            }
            else if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
            {
                if (auto propVal = value.try_as<winrt::Windows::Foundation::IPropertyValue>())
                {
                    if (auto result = convert_from_property_value(runtime, propVal); !result.isUndefined())
                    {
                        return result;
                    }
                }
            }

            return current_runtime_context()->instance_cache.get_instance(runtime, value);
        }

        static T as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            if (value.isNull() || value.isUndefined())
            {
                return nullptr;
            }

            if (value.isObject())
            {
                auto obj = value.getObject(runtime);
                if (obj.isHostObject<projected_object_instance>(runtime))
                {
                    const auto& result = obj.getHostObject<projected_object_instance>(runtime)->instance();
                    if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable>)
                    {
                        return result;
                    }
                    else
                    {
                        return result.as<T>();
                    }
                }
            }

            if constexpr (std::is_same_v<T, winrt::Windows::Foundation::IInspectable> ||
                          std::is_same_v<T, winrt::Windows::Foundation::IPropertyValue>)
            {
                if (auto result = convert_to_property_value(runtime, value))
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

            // TODO: Array as Collections

            throw jsi::JSError(
                runtime, "TypeError: Cannot derive a WinRT interface for the JS value. Expecting: " + typeid(T).name());
        }
    };

    template <typename T>
    struct projected_value_traits<winrt::Windows::Foundation::IReference<T>>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::IReference<T>& value)
        {
            if (!value)
            {
                return jsi::Value::null();
            }

            return convert_native_to_value(runtime, value.Value());
        }

        static winrt::Windows::Foundation::IReference<T> as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            if (value.isNull() || value.isUndefined())
            {
                return nullptr;
            }

            return winrt::box_value(convert_value_to_native<T>(runtime, value))
                .as<winrt::Windows::Foundation::IReference<T>>();
        }
    };

    template <typename T>
    struct projected_value_traits<winrt::Windows::Foundation::IReferenceArray<T>>
    {
        static jsi::Value as_value(jsi::Runtime&, const winrt::Windows::Foundation::IReferenceArray<T>& value)
        {
            if (!value)
            {
                return jsi::Value(nullptr);
            }

            // TODO: Array return
            // return ConvertComArrayToValue<decltype(std::declval(I).Value()), CTV>(context, value.Value());
            throw winrt::hresult_not_implemented();
        }

        static winrt::Windows::Foundation::IReferenceArray<T> from_value(jsi::Runtime&, const jsi::Value& value)
        {
            if (value.isNull() || value.isUndefined)
            {
                return nullptr;
            }

            // It doesn't seem like C++/WinRT actually provides an implementation of IReferenceArray<T> like it does for
            // IReference<T> nor does it even map the basic array types supported by Windows::Foundation::PropertyValue.
            // It can be done but since the public SDK doesn't actually make use of it, perhaps it it is not necessary
            // to implement.
            throw jsi::JSError(context->Runtime,
                "TypeError: Conversion to native reference array to JS not implemented for "s + typeid(T).name());
        }
    };

    template <>
    struct projected_value_traits<winrt::Windows::Foundation::IAsyncAction>
    {
        using interface_type = winrt::Windows::Foundation::IAsyncAction;
        static jsi::Value as_value(jsi::Runtime& runtime, const interface_type& value)
        {
            return jsi::Value(runtime, jsi::Object::createFromHostObject(
                                           runtime, std::make_shared<projected_async_instance<interface_type>>(value)));
        }

        static interface_type as_native(jsi::Runtime& runtime, const jsi::Value&)
        {
            // TODO: Any chance this will ever be necessary?
            throw jsi::JSError(runtime, "TypeError: Conversion from JS value to IAsyncAction is not supported");
        }
    };

    template <typename TProgress>
    struct projected_value_traits<winrt::Windows::Foundation::IAsyncActionWithProgress<TProgress>>
    {
        using interface_type = winrt::Windows::Foundation::IAsyncActionWithProgress<TProgress>;
        static jsi::Value as_value(jsi::Runtime& runtime, const interface_type& value)
        {
            return jsi::Value(runtime, jsi::Object::createFromHostObject(
                                           runtime, std::make_shared<projected_async_instance<interface_type>>(value)));
        }

        static interface_type as_native(jsi::Runtime& runtime, const jsi::Value&)
        {
            // TODO: Any chance this will ever be necessary?
            throw jsi::JSError(
                runtime, "TypeError: Conversion from JS value to IAsyncActionWithProgress is not supported");
        }
    };

    template <typename TResult>
    struct projected_value_traits<winrt::Windows::Foundation::IAsyncOperation<TResult>>
    {
        using interface_type = winrt::Windows::Foundation::IAsyncOperation<TResult>;
        static jsi::Value as_value(jsi::Runtime& runtime, const interface_type& value)
        {
            return jsi::Value(runtime, jsi::Object::createFromHostObject(
                                           runtime, std::make_shared<projected_async_instance<interface_type>>(value)));
        }

        static interface_type as_native(jsi::Runtime& runtime, const jsi::Value&)
        {
            // TODO: Any chance this will ever be necessary?
            throw jsi::JSError(runtime, "TypeError: Conversion from JS value to IAsyncOperation is not supported");
        }
    };

    template <typename TResult, typename TProgress>
    struct projected_value_traits<winrt::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>>
    {
        using interface_type = winrt::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>;
        static jsi::Value as_value(jsi::Runtime& runtime, const interface_type& value)
        {
            return jsi::Value(runtime, jsi::Object::createFromHostObject(
                                           runtime, std::make_shared<projected_async_instance<interface_type>>(value)));
        }

        static interface_type as_native(jsi::Runtime& runtime, const jsi::Value&)
        {
            // TODO: Any chance this will ever be necessary?
            throw jsi::JSError(
                runtime, "TypeError: Conversion from JS value to IAsyncOperationWithProgress is not supported");
        }
    };

    template <typename T>
    struct projected_value_traits<winrt::Windows::Foundation::EventHandler<T>>
    {
        static jsi::Value as_value(jsi::Runtime& runtime, const winrt::Windows::Foundation::EventHandler<T>& value)
        {
            return jsi::Function::createFromHostFunction(runtime, make_propid("EventHandler"), 2,
                [value](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count != 2)
                    {
                        throw jsi::JSError(runtime, "TypeError: Invalid number of arguments to EventHandler");
                    }

                    auto arg0 = convert_value_to_native<winrt::Windows::Foundation::IInspectable>(runtime, args[0]);
                    auto arg1 = convert_value_to_native<T>(runtime, args[1]);
                    value(arg0, arg1);
                    return jsi::Value::undefined();
                });
        }

        static winrt::Windows::Foundation::EventHandler<T> as_native(jsi::Runtime& runtime, const jsi::Value& value)
        {
            return [fn = value.asObject(runtime).asFunction(runtime)](const IInspectable& sender, const T& args) {
                // TODO: Need to be able to call back to the JS thread.
                // TODO: Is this guaranteed to be safe? What if the JS thread is waiting on this thread? Can we assume
                // that will never happen since the JS thread in theory should never block? But since we're dealing with
                // WinRT, perhaps that's something outside of our control? That said, it's certainly not safe for us to
                // assume that the call is happening on the JS thread, and it is definitely not safe to make the call on
                // a different thread...
                // fn.call(runtime, convert_native_to_value(sender), convert_native_to_value(args));
            };
        }
    };

    template <typename TSender, typename TResult>
    struct projected_value_traits<winrt::Windows::Foundation::TypedEventHandler<TSender, TResult>>
    {
        static jsi::Value as_value(
            jsi::Runtime& runtime, const winrt::Windows::Foundation::TypedEventHandler<TSender, TResult>& value)
        {
            return jsi::Function::createFromHostFunction(runtime, make_propid("TypedEventHandler"), 2,
                [value](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count != 2)
                    {
                        throw jsi::JSError(runtime, "TypeError: Invalid number of arguments to TypedEventHandler");
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
            return [fn = value.asObject(runtime).asFunction(runtime)](const TSender& sender, const TResult& args) {
                // TODO: Need to be able to call back to the JS thread.
                // fn.call(runtime, convert_native_to_value(sender), convert_native_to_value(args));
            };
        }
    };

    template <typename K, typename V>
    struct projected_value_traits<winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>
    {
        static jsi::Value as_value(
            jsi::Runtime& runtime, const winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>& value)
        {
            return jsi::Function::createFromHostFunction(runtime, make_propid("MapChangedEventHandler"), 2,
                [value](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count != 2)
                    {
                        throw jsi::JSError(runtime, "TypeError: Invalid number of arguments to MapChangedEventHandler");
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
            return [fn = value.asObject(runtime).asFunction(runtime)](
                       const winrt::Windows::Foundation::Collections::IObservableMap<K, V>& sender,
                       const winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K, V>& args) {
                // TODO: Need to be able to call back to the JS thread.
                // fn.call(runtime, convert_native_to_value(sender), convert_native_to_value(args));
            };
        }
    };

    template <typename T>
    struct projected_value_traits<winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>
    {
        static jsi::Value as_value(
            jsi::Runtime& runtime, const winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>& value)
        {
            return jsi::Function::createFromHostFunction(runtime, make_propid("VectorChangedEventHandler"), 2,
                [value](jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
                    if (count != 2)
                    {
                        throw jsi::JSError(
                            runtime, "TypeError: Invalid number of arguments to VectorChangedEventHandler");
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
            return [fn = value.asObject(runtime).asFunction(runtime)](
                       const winrt::Windows::Foundation::Collections::IObservableVector<T>& sender,
                       const winrt::Windows::Foundation::Collections::IVectorChangedEventArgs<T>& args) {
                // TODO: Need to be able to call back to the JS thread.
                // fn.call(runtime, convert_native_to_value(sender), convert_native_to_value(args));
            };
        }
    };
}

// Static data for generic types
namespace jswinrt
{
    namespace Windows::Foundation
    {
        using winrt::Windows::Foundation::IInspectable;

        namespace IAsyncActionWithProgress
        {
            template <typename TProgress>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::IAsyncActionWithProgress<TProgress>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "completed",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Completed());
                        },
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& value) {
                            thisValue.as<native_type>().Completed(convert_value_to_native<
                                winrt::Windows::Foundation::AsyncActionWithProgressCompletedHandler<TProgress>>(value));
                        } },
                    { "progress",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Progress());
                        },
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& value) {
                            thisValue.as<native_type>().Progress(convert_value_to_native<
                                winrt::Windows::Foundation::AsyncActionProgressHandler<TProgress>>(runtime, value));
                        } },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "getResults",
                        [](jsi::Runtime&, const IInspectable& thisValue, const jsi::Value*) {
                            thisValue.as<native_type>().GetResults();
                            return jsi::Value::undefined();
                        },
                        0, false },
                };
            };

            template <typename TProgress>
            struct data_t
            {
                using iface = interface_data<TProgress>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, iface::functions);
            };

            template <typename TProgress>
            constexpr const static_interface_data* data = &data_t<TProgress>::value;
        }

        namespace IAsyncOperation
        {
            template <typename TResult>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::IAsyncOperation<TResult>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "completed",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Completed());
                        },
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& value) {
                            thisValue.as<native_type>().Completed(convert_value_to_native<
                                winrt::Windows::Foundation::AsyncOperationCompletedHandler<TResult>>(runtime, value));
                        } },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "getResults",
                        [](jsi::Runtime& runtime, const IInspectable thisValue, const jsi::Value*) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().GetResults());
                        },
                        0, false },
                }
            };

            template <typename TResult>
            struct data_t
            {
                using iface = interface_data<TResult>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, iface::functions);
            };

            template <typename TResult>
            constexpr const static_interface_data* data = &data_t<TResult>::value;
        }

        namespace IAsyncOperationWithProgress
        {
            template <typename TResult, typename TProgress>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "completed",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Completed());
                        },
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& value) {
                            thisValue.as<native_type>().Completed(convert_value_to_native<winrt::Windows::Foundation::
                                    AsyncOperationWithProgressCompletedHandler<TResult, TProgress>>(value));
                        } },
                    { "progress",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Progress());
                        },
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& value) {
                            thisValue.as<native_type>().Progress(convert_value_to_native<
                                winrt::Windows::Foundation::AsyncOperationProgressHandler<TResult, TProgress>>(
                                runtime, value));
                        } },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "getResults",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value*) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().GetResults());
                        },
                        0, false },
                };
            };

            template <typename TResult, typename TProgress>
            struct data_t
            {
                using iface = interface_data<TResult, TProgress>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, iface::functions);
            };

            template <typename TResult, typename TProgress>
            constexpr const static_interface_data* data = &data_t<TResult, TProgress>::value;
        }
    }

    namespace Windows::Foundation::Collections
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
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value*) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().First());
                        } },
                };
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), {}, {}, iface::functions);
            };

            template <typename T>
            constexpr const static_interface_data* data = &data_t<T>::value;
        }

        namespace IIterator
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IIterator<T>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "current",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Current());
                        },
                        nullptr },
                    { "hasCurrent",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().HasCurrent());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "getMany",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            // TODO: Need to convert args[0] to native array of T
                            // return convert_native_to_value(runtime, thisValue.as<native_type>().GetMany(items));
                            return jsi::Value::undefined();
                        },
                        1, false },
                    { "moveNext",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value*) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().MoveNext());
                        },
                        0, false },
                };
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, properties::functions);
            };

            template <typename T>
            constexpr const static_interface_data* data = &data_t<T>::value;
        }

        namespace IKeyValuePair
        {
            template <typename K, typename V>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "key",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Key());
                        },
                        nullptr },
                    { "value",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Value());
                        },
                        nullptr },
                };
            };

            template <typename K, typename V>
            struct data_t
            {
                using iface = interface_type<Key, V>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, {});
            };

            template <typename K, typename V>
            constexpr const static_interface_data* data = &data_t<K, V>::value;
        }

        namespace IMap
        {
            template <typename K, typename V>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IMap<K, V>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "size",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_value>().Size());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "clear",
                        [](jsi::Runtime&, const IInspectable& thisValue, const jsi::Value*) {
                            thisValue.as<native_type>().Clear();
                            return jsi::Value::undefined();
                        },
                        0, false },
                    { "getView",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value*) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().GetView());
                        },
                        0, false },
                    { "hasKey",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto key = convert_value_to_native<K>(runtime, args[0]);
                            return convert_native_to_value(runtime, thisValue.as<native_type>().HasKey(key));
                        },
                        1, false },
                    { "insert",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto key = convert_value_to_native<K>(runtime, args[0]);
                            auto value = convert_value_to_native<V>(runtime, args[1]);
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Insert(key, value));
                        },
                        2, false },
                    { "lookup",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto key = convert_value_to_native<K>(args[0]);
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Lookup(key));
                        },
                        1, false },
                    { "remove",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto key = convert_value_to_native<K>(args[0]);
                            thisValue.as<native_type>().Remove(key);
                            return jsi::Value::undefined();
                        },
                        1, false },
                };
            };

            template <typename K, typename V>
            struct data_t
            {
                using iface = interface_data<K, V>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, iface::functions);
            };

            template <typename K, typename V>
            constexpr const static_interface_data* data = &data_t<K, V>::value;
        }

        namespace IMapChangedEventArgs
        {
            template <typename K>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IMapChangedEventArgs<K>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "collectionChange",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().CollectionChange());
                        },
                        nullptr },
                    { "key",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Key());
                        },
                        nullptr },
                };
            };

            template <typename K>
            struct data_t
            {
                using iface = interface_type<K>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, {});
            };

            template <typename K>
            constexpr const static_interface_data* data = &data_t<K>::value;
        }

        namespace IMapView
        {
            template <typename K, typename V>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IMapView<K, V>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "size",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_value>().Size());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "hasKey",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto key = convert_value_to_native<K>(runtime, args[0]);
                            return convert_native_to_value(runtime, thisValue.as<native_type>().HasKey(key));
                        },
                        1, false },
                    { "lookup",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto key = convert_value_to_native<K>(args[0]);
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Lookup(key));
                        },
                        1, false },
                    { "split",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value*) {
                            winrt::Windows::Foundation::Collections::IMapView<K, V> first;
                            winrt::Windows::Foundation::Collections::IMapView<K, V> second;
                            thisValue.as<native_type>().Split(first, second);
                            return make_void_return_struct(runtime, "first", first, "second", second);
                        },
                        0, false },
                };
            };

            template <typename K, typename V>
            struct data_t
            {
                using iface = interface_data<K, V>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, iface::functions);
            };

            template <typename K, typename V>
            constexpr const static_interface_data* data = &data_t<K, V>::value;
        }

        namespace IObservableMap
        {
            template <typename K, typename V>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IObservableMap<K, V>;

                static constexpr const static_interface_data::event_mapping events[] = {
                    { "mapchanged",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& callback) {
                            auto handler = convert_value_to_native<
                                winrt::Windows::Foundation::Collections::MapChangedEventHandler<K, V>>(
                                runtime, callback);
                            return thisValue.as<native_type>().MapChanged(handler);
                        },
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& callback,
                            winrt::event_token token) { thisValue.as<native_type>().MapChanged(token); } },
                }
            };

            template <typename K, typename V>
            struct data_t
            {
                using iface = interface_data<K, V>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), {}, iface::events, {});
            };

            template <typename K, typename V>
            constexpr const static_interface_data* data = &data_t<K, V>::value;
        }

        namespace IObservableVector
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IObservableVector<T>;

                static constexpr const static_interface_data::event_mapping events[] = {
                    { "vectorchanged",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& callback) {
                            auto handler = convert_value_to_native<
                                winrt::Windows::Foundation::Collections::VectorChangedEventHandler<T>>(
                                runtime, callback);
                            return thisValue.as<native_type>().VectorChanged(handler);
                        },
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value& callback,
                            winrt::event_token token) { thisValue.as<native_type>().VectorChanged(token); } },
                };
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), {}, iface::events, {});
            };

            template <typename T>
            constexpr const static_interface_data* data = &data_t<T>::value;
        }

        namespace IVector
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IVector<T>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "size",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Size());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "append",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto value = convert_value_to_native<T>(runtime, args[0]);
                            thisValue.as<native_type>().Append(value);
                            return jsi::Value::undefined();
                        },
                        1, false },
                    { "clear",
                        [](jsi::Runtime&, const IInspectable& thisValue, const jsi::Value*) {
                            thisValue.as<native_type>().Clear();
                            return jsi::Value::undefined();
                        },
                        0, false },
                    { "getAt",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto index = convert_value_to_native<uint32_t>(runtime, args[0]);
                            return convert_native_to_value(runtime, thisValue.as<native_type>().GetAt(index));
                        },
                        1, false },
                    { "getMany",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto startIndex = convert_value_to_native<uint32_t>(runtime, args[0]);
                            // TODO: Need to allocate array for fill array & copy back
                            return jsi::Value::undefined();
                        },
                        2, false },
                    { "getView",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value*) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().GetView());
                        },
                        0, false },
                    { "indexOf",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto value = convert_value_to_native<T>(args[0]);
                            uint32_t index;
                            auto returnValue = thisValue.as<native_type>().IndexOf(value, index);
                            return make_return_struct(runtime, returnValue, "index", index);
                        },
                        1, false },
                    { "insertAt",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto index = convert_value_to_native<uint32_t>(runtime, args[0]);
                            auto value = convert_value_to_native<T>(runtime, args[1]);
                            thisValue.as<native_type>().InsertAt(index, value);
                            return jsi::Value::undefined();
                        },
                        2, false },
                    { "removeAt",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto index = convert_value_to_native<uint32_t>(runtime, args[0]);
                            thisValue.as<native_type>().RemoveAt(index);
                            return jsi::Value::undefined();
                        },
                        1, false },
                    { "removeAtEnd",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value*) {
                            thisValue.as<native_type>().RemoveAtEnd();
                            return jsi::Value::undefined();
                        },
                        0, false },
                    { "replaceAll",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            // TODO: Convert to read-only array
                            return jsi::Value::undefined();
                        },
                        1, false },
                    { "setAt",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto index = convert_value_to_native<uint32_t>(runtime, args[0]);
                            auto value = convert_value_to_native<T>(runtime, args[1]);
                            thisValue.as<native_type>().SetAt(index, value);
                            return jsi::Value::undefined();
                        },
                        2, false },
                };
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, iface::functions);
            };

            template <typename T>
            constexpr const static_interface_data* data = &data_t<T>::value;
        }

        namespace IVectorView
        {
            template <typename T>
            struct interface_data
            {
                using native_type = winrt::Windows::Foundation::Collections::IVectorView<T>;

                static constexpr const static_interface_data::property_mapping properties[] = {
                    { "size",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue) {
                            return convert_native_to_value(runtime, thisValue.as<native_type>().Size());
                        },
                        nullptr },
                };

                static constexpr const static_interface_data::function_mapping functions[] = {
                    { "getAt",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto index = convert_value_to_native<uint32_t>(runtime, args[0]);
                            return convert_native_to_value(runtime, thisValue.as<native_type>().GetAt(index));
                        },
                        1, false },
                    { "getMany",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto startIndex = convert_value_to_native<uint32_t>(runtime, args[0]);
                            // TODO: Need to allocate array for fill array & copy back
                            return jsi::Value::undefined();
                        },
                        2, false },
                    { "indexOf",
                        [](jsi::Runtime& runtime, const IInspectable& thisValue, const jsi::Value* args) {
                            auto value = convert_value_to_native<T>(args[0]);
                            uint32_t index;
                            auto returnValue = thisValue.as<native_type>().IndexOf(value, index);
                            return make_return_struct(runtime, returnValue, "index", index);
                        },
                        1, false },
                };
            };

            template <typename T>
            struct data_t
            {
                using iface = interface_data<T>;
                static constexpr const static_interface_data value(
                    winrt::guid_of<typename iface::native_type>(), iface::properties, {}, iface::functions);
            };

            template <typename T>
            constexpr const static_interface_data* data = &data_t<T>::value;
        }
    }
}

#else

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

namespace WinRTTurboModule
{
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
}

#endif
