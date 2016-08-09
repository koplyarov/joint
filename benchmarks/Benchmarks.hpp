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
			AddBenchmark<std::string, std::string>("invokeNoParams", &Benchmarks::InvokeNoParams, {"binding", "module"});
		}

	private:
		static void InvokeNoParams(BenchmarkContext& context, const std::string& binding, const std::string& module)
		{
			const auto n = context.GetIterationsCount();
			BenchmarkCtx ctx(binding, module);
			auto b = ctx.CreateBenchmarks();

			context.Profile("invokeNoParams", n, [&]{ for (auto i = 0; i < n; ++i) b->NoParamsMethod(); });
			context.Profile("invokeNoParams_native", n, [&]{ b->MeasureNativeNoParams(n); });

			auto invokable = ctx.CreateLocalInvokable();
			context.Profile("invokeNoParams_outgoing", n, [&]{ b->MeasureOutgoingNoParams(invokable, n); });
		}
	};

}

#endif
