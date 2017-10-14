#ifndef SRC_BENCHMARKS_BENCHMARKS_CAST_HPP
#define SRC_BENCHMARKS_BENCHMARKS_CAST_HPP


#include <benchmarks/BenchmarksBase.hpp>


namespace benchmarks
{

	template < typename Desc_ >
	class CastBenchmarks : public BenchmarksBase<Desc_, typename Desc_::CastBenchmarksPtr>
	{
		using Base = BenchmarksBase<Desc_, typename Desc_::CastBenchmarksPtr>;
		using Base::AddSimpleBenchmark;
		using BenchmarkCtx = typename Desc_::BenchmarkCtx;

	public:
		CastBenchmarks()
			: Base("cast", [](const BenchmarkCtx& ctx) { return ctx.CreateCastBenchmarks(); })
		{
			using i64 = int64_t;
			using C = typename Desc_::BenchmarkCtx;
			using B = typename Desc_::CastBenchmarksPtr;

			AddSimpleBenchmark("castNative", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeCast(n) ); });
			AddSimpleBenchmark("castComponentSide", [](C& ctx, i64 n, B b){
				auto c = CALL( b->GetCastComponent() );
				for (int64_t i = 0; i < n; ++i)
					CALL( ctx.template Cast<typename Desc_::ICastInterface2>(c) );
			});
			AddSimpleBenchmark("castProxySide", [](C& ctx, i64 n, B b){ CALL( b->MeasureProxySideCast(ctx.CreateLocalCastComponent(), n) ); });
		}
	};

}

#endif
