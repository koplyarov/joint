#ifndef BENCHMARKS_DESCRIPTORS_JOINT_HPP
#define BENCHMARKS_DESCRIPTORS_JOINT_HPP

#include <Benchmarks_adapters.hpp>


namespace descriptors {
namespace joint
{

    struct Desc
    {
        using String = ::joint::String;
        using Enum = benchmarks::Enum;
        using FlatStruct = benchmarks::FlatStruct;
        using NestedStruct = benchmarks::NestedStruct;
        using NestedStruct2 = benchmarks::NestedStruct2;
        using NestedStruct3 = benchmarks::NestedStruct3;
        using NestedStruct4 = benchmarks::NestedStruct4;

        class Object
        {
        public:
            using JointInterfaces = ::joint::TypeList<::joint::IObject>;
        };

        class Invokable
        {
        public:
            using JointInterfaces = ::joint::TypeList<
                benchmarks::IBasicInvokable,
                benchmarks::IEnumInvokable,
                benchmarks::IStructInvokable,
                benchmarks::IObjectInvokable
            >;

        private:
            ::joint::IObject_Ptr   _obj;

        public:
            Invokable(::joint::Context* ctx)
                : _obj(ctx->MakeComponent<::joint::IObject, Object>())
            { }

            JOINT_CPP_RET_TYPE(void) VoidToVoid() { JOINT_CPP_RETURN_VOID(); }

            JOINT_CPP_RET_TYPE(void) I32ToVoid(int32_t) { JOINT_CPP_RETURN_VOID(); }
            JOINT_CPP_RET_TYPE(int32_t) VoidToI32() { return 0; }

            JOINT_CPP_RET_TYPE(void) StringToVoid(::joint::StringRef s) { JOINT_CPP_RETURN_VOID(); }
            JOINT_CPP_RET_TYPE(String) VoidToString3() { return "abc"; }
            JOINT_CPP_RET_TYPE(String) VoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }

            JOINT_CPP_RET_TYPE(void) EnumToVoid(Enum) { JOINT_CPP_RETURN_VOID(); }
            JOINT_CPP_RET_TYPE(Enum) VoidToEnum() { return Enum(); }

            JOINT_CPP_RET_TYPE(void) ObjectToVoid(::joint::IObject_Ref) { JOINT_CPP_RETURN_VOID(); }
            JOINT_CPP_RET_TYPE(::joint::IObject_Ptr) VoidToObject() { return _obj; }
            JOINT_CPP_RET_TYPE(::joint::IObject_Ptr) VoidToNull() { return nullptr; }

            JOINT_CPP_RET_TYPE(void) NoStructToVoid(int32_t a, int32_t b, int32_t c, int32_t d) { JOINT_CPP_RETURN_VOID(); }
            JOINT_CPP_RET_TYPE(void) FlatStructToVoid(FlatStruct p) { JOINT_CPP_RETURN_VOID(); }
            JOINT_CPP_RET_TYPE(FlatStruct) VoidToFlatStruct() { return GetFlatStruct(); }
            JOINT_CPP_RET_TYPE(void) NestedStructToVoid(NestedStruct p) { JOINT_CPP_RETURN_VOID(); }
            JOINT_CPP_RET_TYPE(NestedStruct) VoidToNestedStruct() { return GetNestedStruct(); }

        private:
            static const FlatStruct& GetFlatStruct() { static FlatStruct s{1, 2, 3, 4}; return s;}
            static const NestedStruct& GetNestedStruct() { static NestedStruct s{1, NestedStruct2{2, NestedStruct3{3, NestedStruct4{4}}}}; return s; }
        };

        class CastComponent
        {
        public:
            using JointInterfaces = ::joint::TypeList<benchmarks::ICastInterface1, benchmarks::ICastInterface2>;
        };

        class Thrower
        {
        public:
            using JointInterfaces =  ::joint::TypeList<benchmarks::IThrower>;

            JOINT_CPP_RET_TYPE(void) Throw() { JOINT_CPP_THROW(::joint::Exception("Requested exception")); }
        };

        using ArrayBenchmarksPtr = benchmarks::IArrayBenchmarks_Ptr;
        using BenchmarksPtr = benchmarks::IBasicBenchmarks_Ptr;
        using CastBenchmarksPtr = benchmarks::ICastBenchmarks_Ptr;
        using ExceptionBenchmarksPtr = benchmarks::IExceptionBenchmarks_Ptr;
        using EnumBenchmarksPtr = benchmarks::IEnumBenchmarks_Ptr;
        using ObjectBenchmarksPtr = benchmarks::IObjectBenchmarks_Ptr;
        using StructBenchmarksPtr = benchmarks::IStructBenchmarks_Ptr;

        using ICastInterface1 = benchmarks::ICastInterface1_Ptr;
        using ICastInterface2 = benchmarks::ICastInterface2_Ptr;

        class BenchmarkCtx
        {
        private:
            ::joint::Context    _ctx;
            ::joint::Module     _module;

        public:
            BenchmarkCtx(const std::string& language)
                : _ctx(JOINT_CPP_RET_VALUE(::joint::Context::Create())),
                  _module(JOINT_CPP_RET_VALUE(_ctx.LoadModule(JOINT_CPP_RET_VALUE(::joint::Manifest::Create(g_benchmarksDir + "/" + language + "/Benchmarks/Benchmarks.jm")))))
            { }

            BenchmarkCtx(const BenchmarkCtx&) = delete;
            BenchmarkCtx& operator = (const BenchmarkCtx&) = delete;

            benchmarks::IArrayBenchmarks_Ptr CreateArrayBenchmarks() const
            { return CALL( _module.GetRootObject<benchmarks::IArrayBenchmarks>("GetBenchmarks") ); }

            benchmarks::IBasicBenchmarks_Ptr CreateBenchmarks() const
            { return CALL( _module.GetRootObject<benchmarks::IBasicBenchmarks>("GetBenchmarks") ); }

            benchmarks::ICastBenchmarks_Ptr CreateCastBenchmarks() const
            { return CALL( _module.GetRootObject<benchmarks::ICastBenchmarks>("GetBenchmarks") ); }

            benchmarks::IExceptionBenchmarks_Ptr CreateExceptionBenchmarks() const
            { return CALL( _module.GetRootObject<benchmarks::IExceptionBenchmarks>("GetBenchmarks") ); }

            benchmarks::IEnumBenchmarks_Ptr CreateEnumBenchmarks() const
            { return CALL( _module.GetRootObject<benchmarks::IEnumBenchmarks>("GetBenchmarks") ); }

            benchmarks::IObjectBenchmarks_Ptr CreateObjectBenchmarks() const
            { return CALL( _module.GetRootObject<benchmarks::IObjectBenchmarks>("GetBenchmarks") ); }

            benchmarks::IStructBenchmarks_Ptr CreateStructBenchmarks() const
            { return CALL( _module.GetRootObject<benchmarks::IStructBenchmarks>("GetBenchmarks") ); }

            benchmarks::IBasicInvokable_Ptr CreateLocalInvokable()
            { return _ctx.MakeComponent<benchmarks::IBasicInvokable, Invokable>(&_ctx); }

            benchmarks::ICastInterface1_Ptr CreateLocalCastComponent()
            { return _ctx.MakeComponent<benchmarks::ICastInterface1, CastComponent>(); }

            benchmarks::IEnumInvokable_Ptr CreateLocalEnumInvokable()
            { return _ctx.MakeComponent<benchmarks::IEnumInvokable, Invokable>(&_ctx); }

            benchmarks::IObjectInvokable_Ptr CreateLocalObjectInvokable()
            { return _ctx.MakeComponent<benchmarks::IObjectInvokable, Invokable>(&_ctx); }

            benchmarks::IStructInvokable_Ptr CreateLocalStructInvokable()
            { return _ctx.MakeComponent<benchmarks::IStructInvokable, Invokable>(&_ctx); }

            benchmarks::IThrower_Ptr CreateLocalThrower()
            { return _ctx.MakeComponent<benchmarks::IThrower, Thrower>(); }

            ::joint::IObject_Ptr CreateObject()
            { return _ctx.MakeComponent<::joint::IObject, Object>(); }

            template < typename Dst_, typename Src_ >
            JOINT_CPP_RET_TYPE(Dst_) Cast(const Src_& src) const
            { return ::joint::Cast<typename Dst_::Interface>(src); }
        };

        static std::string GetName() { return "joint"; }
    };

}}

#endif
