#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP


#include <joint/devkit/marshalling/ExceptionInfo.hpp>
#include <joint/devkit/util/JointException.hpp>
#include <joint/devkit/util/LifoAllocator.hpp>
#include <joint/devkit/util/ScopeExit.hpp>
#include <joint/devkit/util/StringBuilder.hpp>
#include <joint/devkit/util/Unicode.hpp>

#include <binding/JointJavaCoreContext.hpp>
#include <utils/JPtr.hpp>
#include <utils/TryOrTerminate.hpp>


#define JNI_WRAP_CPP_BEGIN \
    try {

#define JNI_WRAP_CPP_END(OkRetVal_, FailRetVal_) \
        return (OkRetVal_); \
    } catch (const std::exception& ex) { \
        GetLogger().Error() << JOINT_DEVKIT_SOURCE_LOCATION ": " << ex; \
        ::joint::java::ThrowJavaException(env, ex.what()); \
        return (FailRetVal_); \
    }

#define JNI_WRAP_CPP_END_VOID() \
    } catch (const std::exception& ex) { \
        GetLogger().Error() << JOINT_DEVKIT_SOURCE_LOCATION ": " << ex; \
        ::joint::java::ThrowJavaException(env, ex.what()); \
    }


#define JAVA_CALL(...) JAVA_CALL_EX(env, __VA_ARGS__)
#define JAVA_CALL_EX(Env_, ...) ::joint::java::JavaCallImpl(Env_, (__VA_ARGS__), JOINT_DEVKIT_SOURCE_LOCATION)

#define JAVA_CALL_VOID(...) JAVA_CALL_VOID_EX(env, __VA_ARGS__)
#define JAVA_CALL_VOID_EX(Env_, ...) do { __VA_ARGS__; ::joint::java::JavaCallImpl(Env_, 0, JOINT_DEVKIT_SOURCE_LOCATION); } while (false)


namespace joint {
namespace java
{

    template < typename Alloc_ >
    class JStringData
    {
    private:
        Alloc_&   _alloc;
        char*     _data;
        size_t    _memSize;

    public:
        JStringData(Alloc_& alloc, JStringWeakRef str)
            : _alloc(alloc)
        { Init(str.GetEnv(), str.Get()); }

        ~JStringData()
        { _alloc.Deallocate(_data, _memSize); }

        JStringData(JStringData&& other)
            : _alloc(other._alloc), _data(other._data), _memSize(other._memSize)
        { other._data = nullptr; }

        JStringData& operator=(JStringData&&) = delete;

        JStringData(const JStringData&) = delete;
        JStringData& operator=(const JStringData&) = delete;

        const char* data() const { return _data; }
        size_t size() const { return _memSize - 1; }

        const char* begin() const { return _data; }
        const char* end() const { return _data + _memSize; }

        const char* GetData() const { return _data; }

    private:
        void Init(JNIEnv* env, jstring str)
        {
            JOINT_DEVKIT_REQUIRE_NOT_NULL(env);
            JOINT_DEVKIT_REQUIRE_NOT_NULL(str);

            jsize num2ByteChars = env->GetStringLength(str);
            _memSize = num2ByteChars * 3 + 1; // A BMP unicode character takes at most 3 bytes as UTF-8

            _data = (char*)_alloc.Allocate(_memSize, 1);

            jboolean is_copy = false;
            const jchar* jdata = env->GetStringCritical(str, &is_copy);
            auto sg = devkit::ScopeExit([&]{ env->ReleaseStringCritical(str, jdata); });

            char* endPtr = devkit::CopyUtf16RangeToUtf8Iterator(jdata, jdata + num2ByteChars, _data);
            *(endPtr++) = 0;

            size_t newSize = (endPtr - _data);
            _alloc.Shrink(_data, _memSize, newSize, 1);
            _memSize = newSize;
        }
    };


    template < typename Alloc_ >
    JStringData<Alloc_> GetJStringData(Alloc_& alloc, JStringWeakRef str)
    { return JStringData<Alloc_>(alloc, str); }


    inline std::string ToStdString(JStringTempRef str)
    {
        devkit::DefaultLifoAllocator alloc;
        return GetJStringData(alloc, str.Weak()).GetData();
    }


    inline void ThrowJavaException(JNIEnv* env, const std::string& msg)
    {
        const char* LoggerName = "Joint.Java.Utils";
        const auto& c = JointJavaCoreContext::Instance();

        auto jmsg = JStringLocalRef::StealLocal(env, env->NewStringUTF(msg.c_str()));

        auto ex = JThrowableLocalRef::StealLocal(env, reinterpret_cast<jthrowable>(env->NewObject(c.RuntimeException_cls.Get(), c.RuntimeException_ctor_id, jmsg.Get())));
        if (!ex)
            JOINT_DEVKIT_TERMINATE("Could not create java.lang.RuntimeException object");

        env->Throw(ex.Get());
    }


    void ThrowExceptionFromJava(JNIEnv* env, const char* location);

    template < typename T_ >
    T_ JavaCallImpl(JNIEnv *env, T_ result, const char* location)
    {
        if (env->ExceptionCheck())
            ThrowExceptionFromJava(env, location);

        return result;
    }

}}

#endif
