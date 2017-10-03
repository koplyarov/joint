#ifndef SRC_BENCHMARKS_BENCHMARKS_OBJECT_HPP
#define SRC_BENCHMARKS_BENCHMARKS_OBJECT_HPP


#include <benchmarks/BenchmarksBase.hpp>


namespace benchmarks
{

	template < typename Desc_ >
	class ObjectBenchmarks : public BenchmarksBase<Desc_, typename Desc_::ObjectBenchmarksPtr>
	{
		using Base = BenchmarksBase<Desc_, typename Desc_::ObjectBenchmarksPtr>;
		using Base::AddSimpleBenchmark;
		using BenchmarkCtx = typename Desc_::BenchmarkCtx;

	public:
		ObjectBenchmarks()
			: Base("object", [](const BenchmarkCtx& ctx) { return ctx.CreateObjectBenchmarks(); })
		{
			using i64 = int64_t;
			using C = typename Desc_::BenchmarkCtx;
			using B = typename Desc_::ObjectBenchmarksPtr;

			AddSimpleBenchmark("invokeNative_void_object", [](C& ctx, i64 n, B b){ b->MeasureNativeObjectToVoid(n); });
			AddSimpleBenchmark("invokeNative_object_void", [](C& ctx, i64 n, B b){ b->MeasureNativeVoidToObject(n); });

			AddSimpleBenchmark("invoke_void_object", [](C& ctx, i64 n, B b){ auto o = ctx.CreateObject(); for (int64_t i = 0; i < n; ++i) b->ObjectToVoid(o); });
			AddSimpleBenchmark("invoke_object_void", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->VoidToObject(); });

			AddSimpleBenchmark("invokeOutgoing_void_object", [](C& ctx, i64 n, B b){ b->MeasureOutgoingObjectToVoid(ctx.CreateLocalObjectInvokable(), n); });
			AddSimpleBenchmark("invokeOutgoing_object_void", [](C& ctx, i64 n, B b){ b->MeasureOutgoingVoidToObject(ctx.CreateLocalObjectInvokable(), n); });
		}
	};

}

#endif
