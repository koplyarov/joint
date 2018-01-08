#ifndef SRC_BENCHMARKS_BENCHMARKS_ARRAY_HPP
#define SRC_BENCHMARKS_BENCHMARKS_ARRAY_HPP


#include <CallAdapter.hpp>
#include <benchmarks/BenchmarksBase.hpp>


namespace benchmarks
{

    template < typename Desc_ >
    class ArrayBenchmarks : public BenchmarksBase<Desc_, typename Desc_::ArrayBenchmarksPtr>
    {
        using Base = BenchmarksBase<Desc_, typename Desc_::ArrayBenchmarksPtr>;
        using Base::AddSimpleBenchmark;
        using BenchmarkCtx = typename Desc_::BenchmarkCtx;

    public:
        ArrayBenchmarks()
            : Base("array", [](const BenchmarkCtx& ctx) { return ctx.CreateArrayBenchmarks(); })
        {
            using i64 = int64_t;
            using C = typename Desc_::BenchmarkCtx;
            using B = typename Desc_::ArrayBenchmarksPtr;

            AddSimpleBenchmark("native_array_get", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeGetI64Element(n) ); });
            AddSimpleBenchmark("native_array_set", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeSetI64Element(n) ); });

            AddSimpleBenchmark("proxy_array_get", [](C& ctx, i64 n, B b){ CALL( b->MeasureGetI64Element(n) ); });
            AddSimpleBenchmark("proxy_array_set", [](C& ctx, i64 n, B b){ CALL( b->MeasureSetI64Element(n) ); });
        }
    };

}

#endif
