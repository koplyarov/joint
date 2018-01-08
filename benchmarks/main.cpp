// Copyright (c) 2016, Dmitry Koplyarov <koplyarov.da@gmail.com>
//
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


#include <benchmarks/BenchmarkApp.hpp>
#include <benchmarks/BenchmarkSuite.hpp>

#include <iostream>

std::string g_benchmarksDir;

#include <benchmarks/Array.hpp>
#include <benchmarks/Basic.hpp>
#include <benchmarks/Cast.hpp>
#include <benchmarks/Enum.hpp>
#include <benchmarks/Exception.hpp>
#include <benchmarks/Object.hpp>
#include <benchmarks/Struct.hpp>

#include <descriptors/joint.hpp>
#include <descriptors/swig.hpp>


int main(int argc, const char* argv[])
{
    std::string executablePath(argv[0]);
    g_benchmarksDir = executablePath.substr(0, executablePath.find_last_of("/\\"));

    try
    {
        using namespace benchmarks;

        BenchmarkSuite s;

        s.RegisterBenchmarks<ArrayBenchmarks, descriptors::joint::Desc>();
        s.RegisterBenchmarks<BasicBenchmarks, descriptors::joint::Desc, descriptors::swig::Desc>();
        s.RegisterBenchmarks<EnumBenchmarks, descriptors::joint::Desc>();
        s.RegisterBenchmarks<ObjectBenchmarks, descriptors::joint::Desc>();
        s.RegisterBenchmarks<StructBenchmarks, descriptors::joint::Desc>();
        s.RegisterBenchmarks<CastBenchmarks, descriptors::joint::Desc>();
        s.RegisterBenchmarks<ExceptionBenchmarks, descriptors::joint::Desc>();

        return RunBenchmarkApp(s, argc, argv);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Uncaught exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
