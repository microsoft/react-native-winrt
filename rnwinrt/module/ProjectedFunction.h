#pragma once
#include "MacroHelpers.h"
#include "ProjectedValueProvider.h"
#include "ProjectionsContext.h"

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
            const jsi::Value* args, const uint16_t numArgs) const override
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
            const jsi::Value* args, const uint16_t numArgs) const override
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
            const jsi::Value* args, const uint16_t numArgs, std::index_sequence<Is...>) const
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

        static std::shared_ptr<ProjectedFunction> Create(const std::string_view& name, InitializerFunction initializer);
        static std::shared_ptr<ProjectedFunction> Create(
            const std::string_view& name, std::shared_ptr<IProjectedFunctionOverloadBase>&& implementation);

        ProjectedFunction(const std::string_view& name, InitializerFunction initializer);
        ProjectedFunction(
            const std::string_view& name, std::shared_ptr<IProjectedFunctionOverloadBase>&& implementation);

        std::string_view Name() const noexcept;

        template <typename I>
        jsi::Function GetFunction(I instance, const std::shared_ptr<ProjectionsContext>& context)
        {
            EnsureInitialized();
            return jsi::Function::createFromHostFunction(context->Runtime, CreatePropNameID(context->Runtime, m_name),
                m_maxArity,
                [strongThis{ shared_from_this() }, instance{ std::move(instance) }, context](
                    jsi::Runtime& runtime, const jsi::Value& thisVal, const jsi::Value* args, size_t count) mutable {
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
        std::shared_ptr<IProjectedFunctionOverloadBase>& GetOverload(jsi::Runtime& runtime, const uint16_t numArgs);
        void EnsureInitialized();

        std::map<uint16_t, std::shared_ptr<IProjectedFunctionOverloadBase>> m_overloads;
        const std::string_view m_name;
        const InitializerFunction m_initializer;
        uint16_t m_maxArity{};
        bool m_isInitialized{};
    };
}