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

	template < typename Desc_ >
	class Benchmarks : public BenchmarksClass
	{
		using BenchmarkCtx = typename Desc_::BenchmarkCtx;

	public:
		Benchmarks()
			: BenchmarksClass("basic")
		{
			AddBenchmark<std::string, std::string>("invokeNative", &Benchmarks::InvokeVoidNative, {"binding", "module"});
			AddBenchmark<std::string, std::string>("invoke", &Benchmarks::InvokeVoid, {"binding", "module"});
			AddBenchmark<std::string, std::string>("invokeOutgoing", &Benchmarks::InvokeVoidOutgoing, {"binding", "module"});
		}

	private:
		static void InvokeVoidNative(BenchmarkContext& context, const std::string& binding, const std::string& module)
		{
			const auto n = context.GetIterationsCount();
			BenchmarkCtx ctx(binding, module);
			auto b = ctx.CreateBenchmarks();

			context.WarmUpAndProfile("void_void", n, [&]{ b->MeasureNativeVoidToVoid(n); });

			context.WarmUpAndProfile("void_i32", n, [&]{ b->MeasureNativeI32ToVoid(n); });
			context.WarmUpAndProfile("i32_void", n, [&]{ b->MeasureNativeVoidToI32(n); });

			context.WarmUpAndProfile("void_string3", n, [&]{ b->MeasureNativeString3ToVoid(n); });
			context.WarmUpAndProfile("string3_void", n, [&]{ b->MeasureNativeVoidToString3(n); });

			context.WarmUpAndProfile("void_string100", n, [&]{ b->MeasureNativeString100ToVoid(n); });
			context.WarmUpAndProfile("string100_void", n, [&]{ b->MeasureNativeVoidToString100(n); });
		}

		static void InvokeVoid(BenchmarkContext& context, const std::string& binding, const std::string& module)
		{
			const auto n = context.GetIterationsCount();
			BenchmarkCtx ctx(binding, module);
			auto b = ctx.CreateBenchmarks();

			context.WarmUpAndProfile("void_void", n, [&]{ for (auto i = 0; i < n; ++i) b->VoidToVoid(); });

			context.WarmUpAndProfile("void_i32", n, [&]{ for (auto i = 0; i < n; ++i) b->I32ToVoid(0); });
			context.WarmUpAndProfile("i32_void", n, [&]{ for (auto i = 0; i < n; ++i) b->VoidToI32(); });

			std::string string3("abc");
			context.WarmUpAndProfile("void_string3", n, [&]{ for (auto i = 0; i < n; ++i) b->StringToVoid(string3); });
			context.WarmUpAndProfile("string3_void", n, [&]{ for (auto i = 0; i < n; ++i) b->VoidToString3(); });

			std::string string100("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
			context.WarmUpAndProfile("void_string100", n, [&]{ for (auto i = 0; i < n; ++i) b->StringToVoid(string100); });
			context.WarmUpAndProfile("string100_void", n, [&]{ for (auto i = 0; i < n; ++i) b->VoidToString100(); });
		}

		static void InvokeVoidOutgoing(BenchmarkContext& context, const std::string& binding, const std::string& module)
		{
			const auto n = context.GetIterationsCount();
			BenchmarkCtx ctx(binding, module);
			auto b = ctx.CreateBenchmarks();
			auto invokable = ctx.CreateLocalInvokable();

			context.WarmUpAndProfile("void_void", n, [&]{ b->MeasureOutgoingVoidToVoid(invokable, n); });

			context.WarmUpAndProfile("void_i32", n, [&]{ b->MeasureOutgoingI32ToVoid(invokable, n); });
			context.WarmUpAndProfile("i32_void", n, [&]{ b->MeasureOutgoingVoidToI32(invokable, n); });

			context.WarmUpAndProfile("void_string3", n, [&]{ b->MeasureOutgoingString3ToVoid(invokable, n); });
			context.WarmUpAndProfile("string3_void", n, [&]{ b->MeasureOutgoingVoidToString3(invokable, n); });

			context.WarmUpAndProfile("void_string100", n, [&]{ b->MeasureOutgoingString100ToVoid(invokable, n); });
			context.WarmUpAndProfile("string100_void", n, [&]{ b->MeasureOutgoingVoidToString100(invokable, n); });
		}
	};

}

#endif
