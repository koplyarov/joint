#ifndef BENCHMARKS_STRUCT_HPP
#define BENCHMARKS_STRUCT_HPP


#include <benchmarks/BenchmarksBase.hpp>


namespace benchmarks
{

	template < typename Desc_ >
	class StructBenchmarks : public BenchmarksBase<Desc_, typename Desc_::StructBenchmarksPtr>
	{
		using Base = BenchmarksBase<Desc_, typename Desc_::StructBenchmarksPtr>;
		using Base::AddSimpleBenchmark;
		using BenchmarkCtx = typename Desc_::BenchmarkCtx;

	public:
		StructBenchmarks()
			: Base("struct", [](const BenchmarkCtx& ctx) { return ctx.CreateStructBenchmarks(); })
		{
			using i64 = int64_t;
			using C = typename Desc_::BenchmarkCtx;
			using B = typename Desc_::StructBenchmarksPtr;
			using FlatStruct = typename Desc_::FlatStruct;
			using NestedStruct = typename Desc_::NestedStruct;
			using NestedStruct2 = typename Desc_::NestedStruct2;
			using NestedStruct3 = typename Desc_::NestedStruct3;
			using NestedStruct4 = typename Desc_::NestedStruct4;

			AddSimpleBenchmark("invokeNative_void_noStruct", [](C& ctx, i64 n, B b){ b->MeasureNativeNoStructToVoid(n); });
			AddSimpleBenchmark("invokeNative_void_flatStruct", [](C& ctx, i64 n, B b){ b->MeasureNativeFlatStructToVoid(n); });
			AddSimpleBenchmark("invokeNative_flatStruct_void", [](C& ctx, i64 n, B b){ b->MeasureNativeVoidToFlatStruct(n); });
			AddSimpleBenchmark("invokeNative_void_nestedStruct", [](C& ctx, i64 n, B b){ b->MeasureNativeNestedStructToVoid(n); });
			AddSimpleBenchmark("invokeNative_nestedStruct_void", [](C& ctx, i64 n, B b){ b->MeasureNativeVoidToNestedStruct(n); });

			AddSimpleBenchmark("invoke_void_noStruct", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->NoStructToVoid(1, 2, 3, 4); });
			AddSimpleBenchmark("invoke_void_flatStruct", [](C& ctx, i64 n, B b){ FlatStruct s{1, 2, 3, 4}; for (int64_t i = 0; i < n; ++i) b->FlatStructToVoid(s); });
			AddSimpleBenchmark("invoke_flatStruct_void", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->VoidToFlatStruct(); });
			AddSimpleBenchmark("invoke_void_nestedStruct", [](C& ctx, i64 n, B b){ NestedStruct s{1, NestedStruct2{2, NestedStruct3{3, NestedStruct4{4}}}}; for (int64_t i = 0; i < n; ++i) b->NestedStructToVoid(s); });
			AddSimpleBenchmark("invoke_nestedStruct_void", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->VoidToNestedStruct(); });

			AddSimpleBenchmark("invokeOutgoing_void_noStruct", [](C& ctx, i64 n, B b){ b->MeasureOutgoingNoStructToVoid(ctx.CreateLocalStructInvokable(), n); });
			AddSimpleBenchmark("invokeOutgoing_void_flatStruct", [](C& ctx, i64 n, B b){ b->MeasureOutgoingFlatStructToVoid(ctx.CreateLocalStructInvokable(), n); });
			AddSimpleBenchmark("invokeOutgoing_flatStruct_void", [](C& ctx, i64 n, B b){ b->MeasureOutgoingVoidToFlatStruct(ctx.CreateLocalStructInvokable(), n); });
			AddSimpleBenchmark("invokeOutgoing_void_nestedStruct", [](C& ctx, i64 n, B b){ b->MeasureOutgoingNestedStructToVoid(ctx.CreateLocalStructInvokable(), n); });
			AddSimpleBenchmark("invokeOutgoing_nestedStruct_void", [](C& ctx, i64 n, B b){ b->MeasureOutgoingVoidToNestedStruct(ctx.CreateLocalStructInvokable(), n); });
		}
	};

}

#endif
