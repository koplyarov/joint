#ifndef SRC_BENCHMARKS_BENCHMARKS_ENUM_HPP
#define SRC_BENCHMARKS_BENCHMARKS_ENUM_HPP


#include <benchmarks/BenchmarksBase.hpp>


namespace benchmarks
{

	template < typename Desc_ >
	class EnumBenchmarks : public BenchmarksBase<Desc_, typename Desc_::EnumBenchmarksPtr>
	{
		using Base = BenchmarksBase<Desc_, typename Desc_::EnumBenchmarksPtr>;
		using Base::AddSimpleBenchmark;
		using BenchmarkCtx = typename Desc_::BenchmarkCtx;

	public:
		EnumBenchmarks()
			: Base("enum", [](const BenchmarkCtx& ctx) { return ctx.CreateEnumBenchmarks(); })
		{
			using i64 = int64_t;
			using C = typename Desc_::BenchmarkCtx;
			using B = typename Desc_::EnumBenchmarksPtr;
			using Enum = typename Desc_::Enum;

			AddSimpleBenchmark("native_void_enum", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeEnumToVoid(n) ); });
			AddSimpleBenchmark("native_enum_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeVoidToEnum(n) ); });

			AddSimpleBenchmark("component_void_enum", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->EnumToVoid(Enum()) ); });
			AddSimpleBenchmark("component_enum_void", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->VoidToEnum() ); });

			AddSimpleBenchmark("proxy_void_enum", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingEnumToVoid(ctx.CreateLocalEnumInvokable(), n) ); });
			AddSimpleBenchmark("proxy_enum_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingVoidToEnum(ctx.CreateLocalEnumInvokable(), n) ); });
		}
	};

}

#endif
