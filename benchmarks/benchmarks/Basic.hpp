#ifndef SRC_BENCHMARKS_BENCHMARKS_BASIC_HPP
#define SRC_BENCHMARKS_BENCHMARKS_BASIC_HPP


#include <CallAdapter.hpp>
#include <benchmarks/BenchmarksBase.hpp>


namespace benchmarks
{

	template < typename Desc_ >
	class BasicBenchmarks : public BenchmarksBase<Desc_, typename Desc_::BenchmarksPtr>
	{
		using Base = BenchmarksBase<Desc_, typename Desc_::BenchmarksPtr>;
		using Base::AddSimpleBenchmark;
		using BenchmarkCtx = typename Desc_::BenchmarkCtx;

	public:
		BasicBenchmarks()
			: Base("basic", [](const BenchmarkCtx& ctx) { return ctx.CreateBenchmarks(); })
		{
			using i64 = int64_t;
			using C = typename Desc_::BenchmarkCtx;
			using B = typename Desc_::BenchmarksPtr;
			using String = typename Desc_::String;

			AddSimpleBenchmark("native_void_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeVoidToVoid(n) ); });
			AddSimpleBenchmark("native_void_i32", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeI32ToVoid(n) ); });
			AddSimpleBenchmark("native_i32_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeVoidToI32(n) ); });
			AddSimpleBenchmark("native_void_string3", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeString3ToVoid(n) ); });
			AddSimpleBenchmark("native_string3_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeVoidToString3(n) ); });
			AddSimpleBenchmark("native_void_string100", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeString100ToVoid(n) ); });
			AddSimpleBenchmark("native_string100_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureNativeVoidToString100(n) ); });

			AddSimpleBenchmark("component_void_void", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->VoidToVoid() ); });

			AddSimpleBenchmark("component_void_i32", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->I32ToVoid(0) ); });
			AddSimpleBenchmark("component_i32_void", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->VoidToI32() ); });

			String string3("abc");
			AddSimpleBenchmark("component_void_string3", [=](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->StringToVoid(string3) ); });
			AddSimpleBenchmark("component_string3_void", [=](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->VoidToString3() ); });

			String string100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
			AddSimpleBenchmark("component_void_string100", [=](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->StringToVoid(string100) ); });
			AddSimpleBenchmark("component_string100_void", [=](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) CALL( b->VoidToString100() ); });

			AddSimpleBenchmark("proxy_void_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingVoidToVoid(ctx.CreateLocalInvokable(), n) ); });

			AddSimpleBenchmark("proxy_void_i32", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingI32ToVoid(ctx.CreateLocalInvokable(), n) ); });
			AddSimpleBenchmark("proxy_i32_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingVoidToI32(ctx.CreateLocalInvokable(), n) ); });

			AddSimpleBenchmark("proxy_void_string3", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingString3ToVoid(ctx.CreateLocalInvokable(), n) ); });
			AddSimpleBenchmark("proxy_string3_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingVoidToString3(ctx.CreateLocalInvokable(), n) ); });

			AddSimpleBenchmark("proxy_void_string100", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingString100ToVoid(ctx.CreateLocalInvokable(), n) ); });
			AddSimpleBenchmark("proxy_string100_void", [](C& ctx, i64 n, B b){ CALL( b->MeasureOutgoingVoidToString100(ctx.CreateLocalInvokable(), n) ); });
		}
	};

}

#endif
