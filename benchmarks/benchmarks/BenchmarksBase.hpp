#ifndef BENCHMARKS_BENCHMARKSBASE_HPP
#define BENCHMARKS_BENCHMARKSBASE_HPP


#include <benchmarks/BenchmarkClass.hpp>


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
        { AddBenchmark<std::string>(name, MakeSimple(func), {"lang"}); }

        template < typename Func_ >
        std::function<void(BenchmarkContext& context, const std::string& language)> MakeSimple(const Func_& func)
        {
            auto cb = _createBenchmarks;
            return [func, cb](BenchmarkContext& context, const std::string& language) {
                const auto n = context.GetIterationsCount();
                BenchmarkCtx ctx(language);
                auto b = cb(ctx);
                if (!b)
                    throw std::runtime_error("Could not create Benchmarks object!");

                context.WarmUpAndProfile("main", n, [&]{ func(ctx, n, b); });
            };
        }
    };

}

#endif
