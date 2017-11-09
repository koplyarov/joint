#ifndef TEST_TESTS_TESTCTX_HPP
#define TEST_TESTS_TESTCTX_HPP


#include <string>

#include <joint.cpp/Module.hpp>


std::string g_moduleManifest = "";

struct TestCtx
{
    joint::Context  Ctx;
    joint::Module   Module;

    TestCtx()
        : Module(joint::Manifest(g_moduleManifest))
    { }
};

namespace test {
namespace detail
{
    template < typename T_ >
    struct ToString<T_, typename std::enable_if<std::is_same<joint::String, T_>::value>::type>
    {
        static std::string Do(const T_& val)
        { return val.Storage(); }
    };
}}

#endif
