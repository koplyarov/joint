#ifndef SRC_BENCHMARKS_BENCHMARKS_HPP
#define SRC_BENCHMARKS_BENCHMARKS_HPP

// Copyright (c) 2016, Dmitry Koplyarov <koplyarov.da@gmail.com>
//
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


#include <benchmarks/BenchmarkClass.hpp>
#include <benchmarks/utils/Storage.hpp>


namespace benchmarks
{

	template < typename Desc_, typename BenchmarkPtr_ >
	class BenchmarksBase : public BenchmarksClass
	{
	public:
		using BenchmarkCtx = typename Desc_::BenchmarkCtx;
		using CreateBenchmarksFunc = std::function<BenchmarkPtr_(const BenchmarkCtx&)>;

	private:
		CreateBenchmarksFunc     _createBenchmarks;

	public:
		BenchmarksBase(std::string name, const CreateBenchmarksFunc& createBenchmarks)
			: BenchmarksClass(std::move(name)), _createBenchmarks(createBenchmarks)
		{ }

	protected:
		template < typename Func_ >
		void AddSimpleBenchmark(const std::string& name, const Func_& func)
		{ AddBenchmark<std::string>(name, MakeSimple(func), {"module"}); }

		template < typename Func_ >
		std::function<void(BenchmarkContext& context, const std::string& moduleManifest)> MakeSimple(const Func_& func)
		{
			auto cb = _createBenchmarks;
			return [func, cb](BenchmarkContext& context, const std::string& moduleManifest) {
				const auto n = context.GetIterationsCount();
				BenchmarkCtx ctx(moduleManifest);
				auto b = cb(ctx);
				if (!b)
					throw std::runtime_error("Could not create Benchmarks object!");

				context.WarmUpAndProfile("main", n, [&]{ func(ctx, n, b); });
			};
		}
	};


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

			AddSimpleBenchmark("invokeNative_void_void", [](C& ctx, i64 n, B b){ b->MeasureNativeVoidToVoid(n); });
			AddSimpleBenchmark("invokeNative_void_i32", [](C& ctx, i64 n, B b){ b->MeasureNativeI32ToVoid(n); });
			AddSimpleBenchmark("invokeNative_i32_void", [](C& ctx, i64 n, B b){ b->MeasureNativeVoidToI32(n); });
			AddSimpleBenchmark("invokeNative_void_string3", [](C& ctx, i64 n, B b){ b->MeasureNativeString3ToVoid(n); });
			AddSimpleBenchmark("invokeNative_string3_void", [](C& ctx, i64 n, B b){ b->MeasureNativeVoidToString3(n); });
			AddSimpleBenchmark("invokeNative_void_string100", [](C& ctx, i64 n, B b){ b->MeasureNativeString100ToVoid(n); });
			AddSimpleBenchmark("invokeNative_string100_void", [](C& ctx, i64 n, B b){ b->MeasureNativeVoidToString100(n); });

			AddSimpleBenchmark("invoke_void_void", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->VoidToVoid(); });

			AddSimpleBenchmark("invoke_void_i32", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->I32ToVoid(0); });
			AddSimpleBenchmark("invoke_i32_void", [](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->VoidToI32(); });

			std::string string3("abc");
			AddSimpleBenchmark("invoke_void_string3", [=](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->StringToVoid(string3); });
			AddSimpleBenchmark("invoke_string3_void", [=](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->VoidToString3(); });

			std::string string100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
			AddSimpleBenchmark("invoke_void_string100", [=](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->StringToVoid(string100); });
			AddSimpleBenchmark("invoke_string100_void", [=](C& ctx, i64 n, B b){ for (int64_t i = 0; i < n; ++i) b->VoidToString100(); });

			AddSimpleBenchmark("invokeOutgoing_void_void", [](C& ctx, i64 n, B b){ b->MeasureOutgoingVoidToVoid(ctx.CreateLocalInvokable(), n); });

			AddSimpleBenchmark("invokeOutgoing_void_i32", [](C& ctx, i64 n, B b){ b->MeasureOutgoingI32ToVoid(ctx.CreateLocalInvokable(), n); });
			AddSimpleBenchmark("invokeOutgoing_i32_void", [](C& ctx, i64 n, B b){ b->MeasureOutgoingVoidToI32(ctx.CreateLocalInvokable(), n); });

			AddSimpleBenchmark("invokeOutgoing_void_string3", [](C& ctx, i64 n, B b){ b->MeasureOutgoingString3ToVoid(ctx.CreateLocalInvokable(), n); });
			AddSimpleBenchmark("invokeOutgoing_string3_void", [](C& ctx, i64 n, B b){ b->MeasureOutgoingVoidToString3(ctx.CreateLocalInvokable(), n); });

			AddSimpleBenchmark("invokeOutgoing_void_string100", [](C& ctx, i64 n, B b){ b->MeasureOutgoingString100ToVoid(ctx.CreateLocalInvokable(), n); });
			AddSimpleBenchmark("invokeOutgoing_string100_void", [](C& ctx, i64 n, B b){ b->MeasureOutgoingVoidToString100(ctx.CreateLocalInvokable(), n); });
		}
	};


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

			AddSimpleBenchmark("castNative", [](C& ctx, i64 n, B b){ b->MeasureNativeCast(n); });
			AddSimpleBenchmark("castComponentSide", [](C& ctx, i64 n, B b){
				auto c = b->GetCastComponent();
				for (int64_t i = 0; i < n; ++i)
					ctx.template Cast<typename Desc_::ICastInterface2>(c);
			});
			AddSimpleBenchmark("castProxySide", [](C& ctx, i64 n, B b){ b->MeasureProxySideCast(ctx.CreateLocalCastComponent(), n); });
		}
	};

}

#endif
