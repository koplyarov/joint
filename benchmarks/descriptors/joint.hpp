#ifndef BENCHMARKS_DESCRIPTORS_JOINT_HPP
#define BENCHMARKS_DESCRIPTORS_JOINT_HPP


#include <Benchmarks_adapters.hpp>


namespace descriptors {
namespace joint
{

	struct Desc
	{
		using Enum = benchmarks::Enum;
		using FlatStruct = benchmarks::FlatStruct;
		using NestedStruct = benchmarks::NestedStruct;
		using NestedStruct2 = benchmarks::NestedStruct2;
		using NestedStruct3 = benchmarks::NestedStruct3;
		using NestedStruct4 = benchmarks::NestedStruct4;

		class Invokable
		{
		public:
			using JointInterfaces = ::joint::TypeList<
				benchmarks::IBasicInvokable,
				benchmarks::IEnumInvokable,
				benchmarks::IStructInvokable
			>;

			void VoidToVoid() { }

			void I32ToVoid(int32_t) { }
			int32_t VoidToI32() { return 0; }

			void StringToVoid(const std::string& s) { }
			std::string VoidToString3() { return "abc"; }
			std::string VoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }

			void EnumToVoid(Enum) { }
			Enum VoidToEnum() { return Enum(); }

			void NoStructToVoid(int32_t a, int32_t b, int32_t c, int32_t d) { }
			void FlatStructToVoid(FlatStruct p) { }
			FlatStruct VoidToFlatStruct() { return GetFlatStruct(); }
			void NestedStructToVoid(NestedStruct p) { }
			NestedStruct VoidToNestedStruct() { return GetNestedStruct(); }

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

			void Throw() { throw std::runtime_error("Requested exception"); }
		};

		using BenchmarksPtr = benchmarks::IBasicBenchmarks_Ptr;
		using CastBenchmarksPtr = benchmarks::ICastBenchmarks_Ptr;
		using ExceptionBenchmarksPtr = benchmarks::IExceptionBenchmarks_Ptr;
		using EnumBenchmarksPtr = benchmarks::IEnumBenchmarks_Ptr;
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
				: _module(_ctx.LoadModule(::joint::Manifest("build/bin/" + language + "/Benchmarks/Benchmarks.jm")))
			{ }

			BenchmarkCtx(const BenchmarkCtx&) = delete;
			BenchmarkCtx& operator = (const BenchmarkCtx&) = delete;

			benchmarks::IBasicBenchmarks_Ptr CreateBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IBasicBenchmarks>("GetBenchmarks"); }

			benchmarks::ICastBenchmarks_Ptr CreateCastBenchmarks() const
			{ return _module.GetRootObject<benchmarks::ICastBenchmarks>("GetBenchmarks"); }

			benchmarks::IExceptionBenchmarks_Ptr CreateExceptionBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IExceptionBenchmarks>("GetBenchmarks"); }

			benchmarks::IEnumBenchmarks_Ptr CreateEnumBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IEnumBenchmarks>("GetBenchmarks"); }

			benchmarks::IStructBenchmarks_Ptr CreateStructBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IStructBenchmarks>("GetBenchmarks"); }

			benchmarks::IBasicInvokable_Ptr CreateLocalInvokable()
			{ return _ctx.MakeComponent<benchmarks::IBasicInvokable, Invokable>(); }

			benchmarks::ICastInterface1_Ptr CreateLocalCastComponent()
			{ return _ctx.MakeComponent<benchmarks::ICastInterface1, CastComponent>(); }

			benchmarks::IEnumInvokable_Ptr CreateLocalEnumInvokable()
			{ return _ctx.MakeComponent<benchmarks::IEnumInvokable, Invokable>(); }

			benchmarks::IStructInvokable_Ptr CreateLocalStructInvokable()
			{ return _ctx.MakeComponent<benchmarks::IStructInvokable, Invokable>(); }

			benchmarks::IThrower_Ptr CreateLocalThrower()
			{ return _ctx.MakeComponent<benchmarks::IThrower, Thrower>(); }

			template < typename Dst_, typename Src_ >
			Dst_ Cast(const Src_& src) const
			{ return ::joint::Cast<typename Dst_::Interface>(src); }
		};

		static std::string GetName() { return "joint"; }
	};

}}

#endif
