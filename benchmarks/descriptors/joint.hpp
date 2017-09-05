#ifndef BENCHMARKS_DESCRIPTORS_JOINT_HPP
#define BENCHMARKS_DESCRIPTORS_JOINT_HPP


#include <Benchmarks_adapters.hpp>


namespace descriptors {
namespace joint
{

	struct Desc
	{
		using Enum = benchmarks::Enum;

		class Invokable
		{
		public:
			using JointInterfaces = ::joint::TypeList<
				benchmarks::IInvokable,
				benchmarks::IEnumInvokable
			>;

			void VoidToVoid() { }

			void I32ToVoid(int32_t) { }
			int32_t VoidToI32() { return 0; }

			void StringToVoid(const std::string& s) { }
			std::string VoidToString3() { return "abc"; }
			std::string VoidToString100() { return "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"; }

			void EnumToVoid(Enum) { }
			Enum VoidToEnum() { return Enum(); }
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

		using BenchmarksPtr = benchmarks::IBenchmarks_Ptr;
		using CastBenchmarksPtr = benchmarks::ICastBenchmarks_Ptr;
		using ExceptionBenchmarksPtr = benchmarks::IExceptionBenchmarks_Ptr;
		using EnumBenchmarksPtr = benchmarks::IEnumBenchmarks_Ptr;

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

			benchmarks::IBenchmarks_Ptr CreateBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IBenchmarks>("GetBenchmarks"); }

			benchmarks::ICastBenchmarks_Ptr CreateCastBenchmarks() const
			{ return _module.GetRootObject<benchmarks::ICastBenchmarks>("GetBenchmarks"); }

			benchmarks::IExceptionBenchmarks_Ptr CreateExceptionBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IExceptionBenchmarks>("GetBenchmarks"); }

			benchmarks::IEnumBenchmarks_Ptr CreateEnumBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IEnumBenchmarks>("GetBenchmarks"); }

			benchmarks::IEnumInvokable_Ptr CreateLocalEnumInvokable()
			{ return _ctx.MakeComponent<benchmarks::IEnumInvokable, Invokable>(); }

			benchmarks::IInvokable_Ptr CreateLocalInvokable()
			{ return _ctx.MakeComponent<benchmarks::IInvokable, Invokable>(); }

			benchmarks::ICastInterface1_Ptr CreateLocalCastComponent()
			{ return _ctx.MakeComponent<benchmarks::ICastInterface1, CastComponent>(); }

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
