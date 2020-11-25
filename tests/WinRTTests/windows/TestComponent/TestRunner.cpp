#include "pch.h"

#include "TestRunner.h"

#include "TestRunner.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

#define TEST_REQUIRE_N(type, number, expression)                                                                       \
    {                                                                                                                  \
        if (!(expression))                                                                                             \
            throw hresult_invalid_argument(type + to_hstring(number));                                                 \
    }

#define TEST_REQUIRE(message, expression)                                                                              \
    {                                                                                                                  \
        if (!(expression))                                                                                             \
            throw hresult_invalid_argument(message);                                                                   \
    }

#if 0 // TODO: Currently not needed
IAsyncAction SignalAsync(HANDLE event)
{
    co_await resume_on_signal(event);
}

IAsyncAction NoAsync()
{
    co_return;
}

auto auto_event()
{
    return handle{ CreateEvent(nullptr, false, false, nullptr) };
}

namespace winrt
{
    template <typename T, typename Allocator = std::allocator<T>>
    Windows::Foundation::Collections::IVectorView<T> single_threaded_vector_view(
        std::vector<T, Allocator>&& values = {})
    {
        return make<impl::input_vector_view<T, std::vector<T, Allocator>>>(std::move(values));
    }

    template <typename K, typename V, typename Allocator = std::allocator<K>>
    Windows::Foundation::Collections::IMapView<K, V> single_threaded_map_view(std::map<K, V, Allocator>&& values = {})
    {
        return make<impl::input_map_view<K, V, std::map<K, V, Allocator>>>(std::move(values));
    }
}
#endif

namespace winrt::TestComponent::implementation
{
    struct Tests : implements<Tests, ITests>
    {
    private:
        uint32_t m_counter{};
        const uint32_t m_total{ 99 };
        event<EventHandler<int32_t>> m_event1;
        event<TypedEventHandler<ITests, int32_t>> m_event2;

    public:
        void abi_enter()
        {
            ++m_counter;
        }

        uint32_t Percentage()
        {
            // We don't want calls to the Percentage property perturbing the results.
            --m_counter;

            return m_counter * 100 / m_total;
        }

        void Simple()
        {
        }

        template <size_t number, typename type>
        auto Param(type const& a, type& b)
        {
            b = a;
            return a;
        }

#define TEST_GEN(number, type)                                                                                         \
    auto Param##number(type const& a, type& b)                                                                         \
    {                                                                                                                  \
        return Param<number, type>(a, b);                                                                              \
    }

        TEST_GEN(1, bool);
        TEST_GEN(2, uint8_t);
        TEST_GEN(3, uint16_t);
        TEST_GEN(4, uint32_t);
        TEST_GEN(5, uint64_t);
        TEST_GEN(6, int16_t);
        TEST_GEN(7, int32_t);
        TEST_GEN(8, int64_t);
        TEST_GEN(9, float);
        TEST_GEN(10, double);
        TEST_GEN(11, char16_t);

#undef TEST_GEN

        // These are non-blittable, so we additionally validate that the out params (b) have default (zeroed) values.

        auto Param12(hstring const& a, hstring& b)
        {
            TEST_REQUIRE_N(L"Param", 12, b.empty());
            b = a;
            return a;
        }

        auto Param13(Blittable const& a, Blittable const& b, Blittable& c)
        {
            TEST_REQUIRE_N(L"Param", 13, a == b);
            c = a;
            return a;
        }

        auto Param14(NonBlittable const& a, NonBlittable const& b, NonBlittable& c)
        {
            TEST_REQUIRE_N(L"Param", 14, a == b);
            TEST_REQUIRE_N(L"Param", 14, c == NonBlittable{});
            c = a;
            return a;
        }

        auto Param15(Nested const& a, Nested const& b, Nested& c)
        {
            TEST_REQUIRE_N(L"Param", 14, a == b);
            TEST_REQUIRE_N(L"Param", 15, c == Nested{});
            c = a;
            return a;
        }

#if 0 // TODO: Out params on delegates are currently not supported
        template <size_t number, typename type, typename ParamHandler>
        void ParamPrimitiveCall(type const a, ParamHandler const& handler)
        {
            type b;
            type c = handler(a, b);
            TEST_REQUIRE_N(L"Param", number, a == b && a == c);
        }

#define TEST_GEN(number, type, ...)                                                                                    \
    void Param##number##Call(Param##number##Handler const& handler)                                                    \
    {                                                                                                                  \
        ParamPrimitiveCall<number, type>({ __VA_ARGS__ }, handler);                                                    \
    }

        TEST_GEN(1, bool, true);
        TEST_GEN(2, uint8_t, 1);
        TEST_GEN(3, uint16_t, 2);
        TEST_GEN(4, uint32_t, 3);
        TEST_GEN(5, uint64_t, 4);
        TEST_GEN(6, int16_t, 5);
        TEST_GEN(7, int32_t, 6);
        TEST_GEN(8, int64_t, 7);
        TEST_GEN(9, float, 8.1f);
        TEST_GEN(10, double, 9.1);
        TEST_GEN(11, char16_t, L'W');
        // TODO: do these also need non-blittable out param testing?
        TEST_GEN(12, hstring, L"WinRT");

#undef TEST_GEN

        template <size_t number, typename type, typename ParamHandler>
        void ParamStructCall(type const a, ParamHandler const& handler)
        {
            type b;
            type c = handler(a, a, b);
            TEST_REQUIRE_N(L"Param", number, a == b && a == c);
        }

#define TEST_GEN(number, type, ...)                                                                                    \
    void Param##number##Call(Param##number##Handler const& handler)                                                    \
    {                                                                                                                  \
        ParamStructCall<number, type>({ __VA_ARGS__ }, handler);                                                       \
    }

        TEST_GEN(13, Blittable, Blittable{ 1, 2, 3, 4, -5, -6, -7, 8.0f, 9.0, guid_of<ITests>() });
        TEST_GEN(14, NonBlittable, NonBlittable{ false, L'X', L"WinRT", 1234 });
        TEST_GEN(15, Nested,
            Nested{ { 1, 2, 3, 4, -5, -6, -7, 8.0f, 9.0, guid_of<ITests>() }, { true, L'X', L"WinRT", 1234 } });

#undef TEST_GEN
#endif

#if 0 // TODO: For some reason the following cause compilation errors w/ jswinrt
        template <size_t number, typename type>
        auto Array(array_view<type const> a, array_view<type> b, com_array<type>& c)
        {
            TEST_REQUIRE_N(L"Array", number, a.size() == b.size());
            TEST_REQUIRE_N(L"Array", number, c.size() == 0);
            std::copy(a.begin(), a.end(), b.begin());
            c = com_array<type>(a.begin(), a.end());
            return com_array<type>(a.begin(), a.end());
        }

#define TEST_GEN(number, type)                                                                                         \
    auto Array##number(array_view<type const> a, array_view<type> b, com_array<type>& c)                               \
    {                                                                                                                  \
        return Array<number, type>(a, b, c);                                                                           \
    }

        TEST_GEN(1, bool);
        TEST_GEN(2, uint8_t);
        TEST_GEN(3, uint16_t);
        TEST_GEN(4, uint32_t);
        TEST_GEN(5, uint64_t);
        TEST_GEN(6, int16_t);
        TEST_GEN(7, int32_t);
        TEST_GEN(8, int64_t);
        TEST_GEN(9, float);
        TEST_GEN(10, double);
        TEST_GEN(11, char16_t);
        TEST_GEN(12, hstring);
        TEST_GEN(13, Blittable);
        TEST_GEN(14, NonBlittable);
        TEST_GEN(15, Nested);
        TEST_GEN(16, IStringable);

#undef TEST_GEN
#endif

#if 0 // TODO: Out params on delegates are currently not supported
        template <size_t number, typename type, typename ArrayHandler>
        void ArrayCall(std::array<type, 3> a, ArrayHandler const& handler)
        {
            std::array<type, 3> b;
            com_array<type> c;
            com_array<type> d = handler(a, b, c);
            TEST_REQUIRE_N(L"Array", number, std::equal(a.begin(), a.end(), b.begin(), b.end()));
            TEST_REQUIRE_N(L"Array", number, std::equal(a.begin(), a.end(), c.begin(), c.end()));
            TEST_REQUIRE_N(L"Array", number, std::equal(a.begin(), a.end(), d.begin(), d.end()));
        }

#define TEST_GEN(number, type, ...)                                                                                    \
    void Array##number##Call(Array##number##Handler const& handler)                                                    \
    {                                                                                                                  \
        ArrayCall<number, type>({ __VA_ARGS__ }, handler);                                                             \
    }

        TEST_GEN(1, bool, true, false, true);
        TEST_GEN(2, uint8_t, 1, 2, 3);
        TEST_GEN(3, uint16_t, 4, 5, 7);
        TEST_GEN(4, uint32_t, 7, 8, 9);
        TEST_GEN(5, uint64_t, 1, 2, 3);
        TEST_GEN(6, int16_t, 4, 5, 7);
        TEST_GEN(7, int32_t, 7, 8, 9);
        TEST_GEN(8, int64_t, 1, 2, 3);
        TEST_GEN(9, float, 1.0f, 2.0f, 3.0f);
        TEST_GEN(10, double, 4.0f, 5.0f, 6.0f);
        TEST_GEN(11, char16_t, L'W', L'i', L'n');
        TEST_GEN(12, hstring, L"C++", L"C#", L"Rust");
        TEST_GEN(13, Blittable, Blittable{ 1, 2, 3, 4, -5, -6, -7, 8.0f, 9.0, guid_of<ITests>() },
            Blittable{ 10, 20, 30, 40, -50, -60, -70, 80.0f, 90.0, guid_of<IStringable>() },
            Blittable{ 1, 2, 3, 4, -5, -6, -7, 8.0f, 9.0, guid_of<IInspectable>() });
        TEST_GEN(14, NonBlittable, NonBlittable{ false, L'X', L"First", 123 },
            NonBlittable{ true, L'Y', L"Second", 456 }, NonBlittable{ false, L'Z', L"Third", 789 });
        TEST_GEN(15, Nested,
            Nested{ { 1, 2, 3, 4, -5, -6, -7, 8.0f, 9.0, guid_of<ITests>() }, { false, L'X', L"First", 123 } },
            Nested{ { 10, 20, 30, 40, -50, -60, -70, 80.0f, 90.0, guid_of<IStringable>() },
                { true, L'Y', L"Second", 456 } },
            Nested{ { 1, 2, 3, 4, -5, -6, -7, 8.0f, 9.0, guid_of<IInspectable>() }, { false, L'Z', L"Third", 789 } });
        TEST_GEN(16, IStringable, Uri(L"http://kennykerr.ca/one").as<IStringable>(),
            Uri(L"http://kennykerr.ca/two").as<IStringable>(), Uri(L"http://kennykerr.ca/three").as<IStringable>());

#undef TEST_GEN
#endif

#if 0 // TODO: Collections support
        auto Collection1(IIterable<hstring> const& a, IIterable<hstring>& b)
        {
            b = single_threaded_vector(std::vector<hstring>{ begin(a), end(a) });
            return b;
        }
        auto Collection2(
            IIterable<IKeyValuePair<hstring, hstring>> const& a, IIterable<IKeyValuePair<hstring, hstring>>& b)
        {
            std::map<hstring, hstring> copy;

            for (auto&& pair : a)
            {
                copy[pair.Key()] = pair.Value();
            }

            b = single_threaded_map(std::move(copy));
            return b;
        }
        auto Collection3(IMap<hstring, hstring> const& a, IMap<hstring, hstring>& b)
        {
            std::map<hstring, hstring> copy;

            for (auto&& pair : a)
            {
                copy[pair.Key()] = pair.Value();
            }

            b = single_threaded_map(std::move(copy));
            return b;
        }
        auto Collection4(IMapView<hstring, hstring> const& a, IMapView<hstring, hstring>& b)
        {
            std::map<hstring, hstring> copy;

            for (auto&& pair : a)
            {
                copy[pair.Key()] = pair.Value();
            }

            b = single_threaded_map_view(std::move(copy));
            return b;
        }
        auto Collection5(IVector<hstring> const& a, IVector<hstring>& b)
        {
            b = single_threaded_vector(std::vector<hstring>{ begin(a), end(a) });
            return b;
        }

        auto Collection6(IVectorView<hstring> const& a, IVectorView<hstring>& b)
        {
            b = single_threaded_vector_view(std::vector<hstring>{ begin(a), end(a) });
            return b;
        }

        void Collection1Call(Collection1Handler const& handler)
        {
            IIterable<hstring> a = single_threaded_vector<hstring>({ L"apples", L"oranges", L"pears" });
            IIterable<hstring> b;
            IIterable<hstring> c = handler(a, b);
            TEST_REQUIRE_N(L"Collection", 1, a != b && a != c);
            TEST_REQUIRE_N(L"Collection", 1, std::equal(begin(a), end(a), begin(b), end(b)));
            TEST_REQUIRE_N(L"Collection", 1, std::equal(begin(a), end(a), begin(c), end(c)));
        }
        void Collection2Call(Collection2Handler const& handler)
        {
            IIterable<IKeyValuePair<hstring, hstring>> a = single_threaded_map<hstring, hstring>(
                std::map<hstring, hstring>{ { L"apples", L"1" }, { L"oranges", L"2" }, { L"pears", L"3" } });
            IIterable<IKeyValuePair<hstring, hstring>> b;
            IIterable<IKeyValuePair<hstring, hstring>> c = handler(a, b);
            TEST_REQUIRE_N(L"Collection", 2, a != b && a != c);
            TEST_REQUIRE_N(L"Collection", 2, std::equal(begin(a), end(a), begin(b), end(b)));
            TEST_REQUIRE_N(L"Collection", 2, std::equal(begin(a), end(a), begin(c), end(c)));
        }
        void Collection3Call(Collection3Handler const& handler)
        {
            IMap<hstring, hstring> a = single_threaded_map<hstring, hstring>(
                std::map<hstring, hstring>{ { L"apples", L"1" }, { L"oranges", L"2" }, { L"pears", L"3" } });
            IMap<hstring, hstring> b;
            IMap<hstring, hstring> c = handler(a, b);
            TEST_REQUIRE_N(L"Collection", 3, a != b && a != c);
            TEST_REQUIRE_N(L"Collection", 3, std::equal(begin(a), end(a), begin(b), end(b)));
            TEST_REQUIRE_N(L"Collection", 3, std::equal(begin(a), end(a), begin(c), end(c)));

            TEST_REQUIRE_N(L"Collection", 3, !c.Insert(L"strawberries", L"4"));
            TEST_REQUIRE_N(L"Collection", 3, c.HasKey(L"strawberries"));
            TEST_REQUIRE_N(L"Collection", 3, c.Lookup(L"strawberries") == L"4");
            c.Remove(L"strawberries");
            TEST_REQUIRE_N(L"Collection", 3, c.Size() == 3);
            TEST_REQUIRE_N(L"Collection", 3, c.GetView().Size() == 3);
            c.Clear();
            TEST_REQUIRE_N(L"Collection", 3, c.Size() == 0);
        }
        void Collection4Call(Collection4Handler const& handler)
        {
            IMapView<hstring, hstring> a = single_threaded_map_view<hstring, hstring>(
                std::map<hstring, hstring>{ { L"apples", L"1" }, { L"oranges", L"2" }, { L"pears", L"3" } });
            IMapView<hstring, hstring> b;
            IMapView<hstring, hstring> c = handler(a, b);
            TEST_REQUIRE_N(L"Collection", 4, a != b && a != c);
            TEST_REQUIRE_N(L"Collection", 4, std::equal(begin(a), end(a), begin(b), end(b)));
            TEST_REQUIRE_N(L"Collection", 4, std::equal(begin(a), end(a), begin(c), end(c)));

            TEST_REQUIRE_N(L"Collection", 4, c.Lookup(L"apples") == L"1");
            TEST_REQUIRE_N(L"Collection", 4, c.HasKey(L"oranges"));
            IMapView<hstring, hstring> d;
            IMapView<hstring, hstring> e;
            c.Split(d, e);
            TEST_REQUIRE_N(L"Collection", 4, d.Size() && e.Size());
        }
        void Collection5Call(Collection5Handler const& handler)
        {
            IVector<hstring> a = single_threaded_vector<hstring>({ L"apples", L"oranges", L"pears" });
            IVector<hstring> b;
            IVector<hstring> c = handler(a, b);
            TEST_REQUIRE_N(L"Collection", 5, a != b && a != c);
            TEST_REQUIRE_N(L"Collection", 5, std::equal(begin(a), end(a), begin(b), end(b)));
            TEST_REQUIRE_N(L"Collection", 5, std::equal(begin(a), end(a), begin(c), end(c)));

            c.Append(L"bananas");
            TEST_REQUIRE_N(L"Collection", 5, c.GetAt(3) == L"bananas");

            c.SetAt(3, L"strawberries");
            c.RemoveAtEnd();

            TEST_REQUIRE_N(L"Collection", 5, c.Size() == 3);

            auto iterator = c.GetView().First();
            iterator.MoveNext();
            TEST_REQUIRE_N(L"Collection", 5, iterator.Current() == L"oranges");

            c.Clear();
        }
        void Collection6Call(Collection6Handler const& handler)
        {
            IVectorView<hstring> a = single_threaded_vector_view<hstring>({ L"apples", L"oranges", L"pears" });
            IVectorView<hstring> b;
            IVectorView<hstring> c = handler(a, b);
            TEST_REQUIRE_N(L"Collection", 6, a != b && a != c);
            TEST_REQUIRE_N(L"Collection", 6, std::equal(begin(a), end(a), begin(b), end(b)));
            TEST_REQUIRE_N(L"Collection", 6, std::equal(begin(a), end(a), begin(c), end(c)));
            TEST_REQUIRE_N(L"Collection", 6, c.GetAt(1) == L"oranges");
            uint32_t x;
            TEST_REQUIRE_N(L"Collection", 6, c.IndexOf(L"apples", x));
        }
#endif

#if 0 // TODO: The following require that we have an IAsyncAction in hand
        IAsyncAction Async1(IAsyncAction suspend, bool fail)
        {
            co_await suspend;

            if (fail)
            {
                throw hresult_invalid_argument(L"test");
            }
        }
        IAsyncActionWithProgress<int32_t> Async2(IAsyncAction suspend, bool fail, int32_t progress)
        {
            co_await suspend;
            auto callback = co_await get_progress_token();
            callback(progress);

            if (fail)
            {
                throw hresult_invalid_argument(L"test");
            }
        }
        IAsyncOperation<int32_t> Async3(IAsyncAction suspend, bool fail, int32_t result)
        {
            co_await suspend;

            if (fail)
            {
                throw hresult_invalid_argument(L"test");
            }

            co_return result;
        }
        IAsyncOperationWithProgress<int32_t, int32_t> Async4(
            IAsyncAction suspend, bool fail, int32_t result, int32_t progress)
        {
            co_await suspend;
            auto callback = co_await get_progress_token();
            callback(progress);

            if (fail)
            {
                throw hresult_invalid_argument(L"test");
            }

            co_return result;
        }

        void Async1Call(Async1Handler const& handler)
        {
            {
                TEST_REQUIRE(L"Async1", handler(NoAsync(), false).Status() == AsyncStatus::Completed);
            }
            {
                auto signal = auto_event();
                auto async = handler(SignalAsync(signal.get()), false);
                TEST_REQUIRE(L"Async1", async.Status() == AsyncStatus::Started);
                SetEvent(signal.get());
                async.get();
            }
            {
                auto signal = auto_event();
                auto async = handler(SignalAsync(signal.get()), true);
                TEST_REQUIRE(L"Async1", async.Status() == AsyncStatus::Started);
                SetEvent(signal.get());
                try
                {
                    async.get();
                    TEST_REQUIRE(L"Async1", false);
                }
                catch (hresult_invalid_argument const& e)
                {
                    TEST_REQUIRE(L"Async1", e.message() == L"test");
                }
            }
        }
        void Async2Call(Async2Handler const& handler)
        {
            {
                TEST_REQUIRE(L"Async2", handler(NoAsync(), false, 321).Status() == AsyncStatus::Completed);
            }
            {
                auto signal = auto_event();
                auto async = handler(SignalAsync(signal.get()), false, 321);
                TEST_REQUIRE(L"Async2", async.Status() == AsyncStatus::Started);
                int32_t progress{};
                async.Progress([&](auto&&, int32_t args) { progress = args; });
                SetEvent(signal.get());
                async.get();
                TEST_REQUIRE(L"Async2", progress == 321);
            }
            {
                auto signal = auto_event();
                auto async = handler(SignalAsync(signal.get()), true, 321);
                TEST_REQUIRE(L"Async2", async.Status() == AsyncStatus::Started);
                int32_t progress{};
                async.Progress([&](auto&&, int32_t args) { progress = args; });
                SetEvent(signal.get());
                try
                {
                    async.get();
                    TEST_REQUIRE(L"Async2", false);
                }
                catch (hresult_invalid_argument const& e)
                {
                    TEST_REQUIRE(L"Async2", e.message() == L"test");
                }
                TEST_REQUIRE(L"Async2", progress == 321);
            }
        }
        void Async3Call(Async3Handler const& handler)
        {
            {
                auto async = handler(NoAsync(), false, 123);
                TEST_REQUIRE(L"Async3", async.Status() == AsyncStatus::Completed);
                TEST_REQUIRE(L"Async3", async.get() == 123);
            }
            {
                auto signal = auto_event();
                auto async = handler(SignalAsync(signal.get()), false, 123);
                TEST_REQUIRE(L"Async3", async.Status() == AsyncStatus::Started);
                SetEvent(signal.get());
                TEST_REQUIRE(L"Async3", async.get() == 123);
            }
            {
                auto signal = auto_event();
                auto async = handler(SignalAsync(signal.get()), true, 123);
                TEST_REQUIRE(L"Async3", async.Status() == AsyncStatus::Started);
                SetEvent(signal.get());
                try
                {
                    async.get();
                    TEST_REQUIRE(L"Async3", false);
                }
                catch (hresult_invalid_argument const& e)
                {
                    TEST_REQUIRE(L"Async3", e.message() == L"test");
                }
            }
        }
        void Async4Call(Async4Handler const& handler)
        {
            {
                auto async = handler(NoAsync(), false, 123, 321);
                TEST_REQUIRE(L"Async4", async.Status() == AsyncStatus::Completed);
                TEST_REQUIRE(L"Async4", async.get() == 123);
            }
            {
                auto signal = auto_event();
                auto async = handler(SignalAsync(signal.get()), false, 123, 321);
                TEST_REQUIRE(L"Async4", async.Status() == AsyncStatus::Started);
                int32_t progress{};
                async.Progress([&](auto&&, int32_t args) { progress = args; });
                SetEvent(signal.get());
                TEST_REQUIRE(L"Async4", async.get() == 123);
                TEST_REQUIRE(L"Async4", progress == 321);
            }
            {
                auto signal = auto_event();
                auto async = handler(SignalAsync(signal.get()), true, 123, 321);
                TEST_REQUIRE(L"Async4", async.Status() == AsyncStatus::Started);
                int32_t progress{};
                async.Progress([&](auto&&, int32_t args) { progress = args; });
                SetEvent(signal.get());
                try
                {
                    async.get();
                    TEST_REQUIRE(L"Async4", false);
                }
                catch (hresult_invalid_argument const& e)
                {
                    TEST_REQUIRE(L"Async4", e.message() == L"test");
                }
                TEST_REQUIRE(L"Async4", progress == 321);
            }
        }
#endif

        event_token Event1(EventHandler<int32_t> const& handler)
        {
            return m_event1.add(handler);
        }

        void Event1(event_token token) noexcept
        {
            m_event1.remove(token);
        }

        event_token Event2(TypedEventHandler<ITests, int32_t> const& handler)
        {
            return m_event2.add(handler);
        }

        void Event2(event_token token)
        {
            m_event2.remove(token);
        }

        void Event1Call(int32_t value)
        {
            m_event1(*this, value);
        }

        void Event2Call(int32_t value)
        {
            m_event2(*this, value);
        }
    };

#if 0
    void RunTests(ITests const& tests)
    {
        tests.Simple();

#define TEST_GEN(type, number) tests.type##number##Call([&](auto&&... args) { return tests.type##number(args...); });

        TEST_GEN(Param, 1);
        TEST_GEN(Param, 2);
        TEST_GEN(Param, 3);
        TEST_GEN(Param, 4);
        TEST_GEN(Param, 5);
        TEST_GEN(Param, 6);
        TEST_GEN(Param, 7);
        TEST_GEN(Param, 8);
        TEST_GEN(Param, 9);
        TEST_GEN(Param, 10);
        TEST_GEN(Param, 11);
        TEST_GEN(Param, 12);
        TEST_GEN(Param, 13);
        TEST_GEN(Param, 14);
        TEST_GEN(Param, 15);

        TEST_GEN(Array, 1);
        TEST_GEN(Array, 2);
        TEST_GEN(Array, 3);
        TEST_GEN(Array, 4);
        TEST_GEN(Array, 5);
        TEST_GEN(Array, 6);
        TEST_GEN(Array, 7);
        TEST_GEN(Array, 8);
        TEST_GEN(Array, 9);
        TEST_GEN(Array, 10);
        TEST_GEN(Array, 11);
        TEST_GEN(Array, 12);
        TEST_GEN(Array, 13);
        TEST_GEN(Array, 14);
        TEST_GEN(Array, 15);
        TEST_GEN(Array, 16);

        TEST_GEN(Collection, 1);
        TEST_GEN(Collection, 2);
        TEST_GEN(Collection, 3);
        TEST_GEN(Collection, 4);
        TEST_GEN(Collection, 5);
        TEST_GEN(Collection, 6);

        TEST_GEN(Async, 1);
        TEST_GEN(Async, 2);
        TEST_GEN(Async, 3);
        TEST_GEN(Async, 4);

#undef TEST_GEN

        {
            int32_t counter = 0;

            auto token = tests.Event1([&](IInspectable const& sender, int32_t args) {
                TEST_REQUIRE(L"Event1", sender == tests);
                TEST_REQUIRE(L"Event1", args == 123);
                TEST_REQUIRE(L"Event1", counter == 0);
                ++counter;
            });

            tests.Event1Call(123);
            TEST_REQUIRE(L"Event1", counter == 1);
            tests.Event1(token);
            tests.Event1Call(456);
            TEST_REQUIRE(L"Event1", counter == 1);
        }

        {
            int32_t counter = 0;

            auto token = tests.Event2([&](ITests const& sender, int32_t args) {
                TEST_REQUIRE(L"Event2", sender == tests);
                TEST_REQUIRE(L"Event2", args == 123);
                TEST_REQUIRE(L"Event2", counter == 0);
                ++counter;
            });

            tests.Event2Call(123);
            TEST_REQUIRE(L"Event2", counter == 1);
            tests.Event2(token);
            tests.Event2Call(456);
            TEST_REQUIRE(L"Event2", counter == 1);
        }
    }

    void TestRunner::TestProducer(ITests const& tests)
    {
        RunTests(tests);
    }

    uint32_t TestRunner::TestConsumer(TestHandler const& caller)
    {
        auto tests = make<Tests>();
        caller(tests);
        return tests.Percentage();
    }

    void TestRunner::TestSelf()
    {
        TestRunner::TestProducer(make<Tests>());

        uint32_t percentage = TestRunner::TestConsumer([](ITests const& tests) { RunTests(tests); });

        TEST_REQUIRE(L"Test cover is " + to_hstring(percentage) + L"%", percentage == 100);
        percentage = TestRunner::TestConsumer([](auto&&) {});
        TEST_REQUIRE(L"TestSelf", percentage == 0);

        percentage = TestRunner::TestConsumer([](ITests const& tests) { tests.Simple(); });

        TEST_REQUIRE(L"TestSelf", percentage > 0 && percentage < 100);
    }
#endif

    ITests TestRunner::MakeTests()
    {
        return make<Tests>();
    }

#if 0
    IVector<int32_t> TestRunner::CreateInt32Vector()
    {
        return single_threaded_vector<int32_t>();
    }

    IVector<hstring> TestRunner::CreateStringVector()
    {
        return single_threaded_vector<hstring>();
    }

    IVector<IStringable> TestRunner::CreateStringableVector()
    {
        return single_threaded_vector<IStringable>();
    }

    TimeSpan TestRunner::CreateTimeSpan(uint32_t milliseconds)
    {
        return std::chrono::milliseconds(milliseconds);
    }

    IAsyncAction TestRunner::CreateAsyncAction(uint32_t milliseconds)
    {
        co_await resume_after(CreateTimeSpan(milliseconds));
    }

    hstring TestRunner::ExpectObject(Windows::Foundation::IInspectable const& value)
    {
        return get_class_name(value);
    }
#endif
}
