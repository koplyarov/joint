#ifndef UTILS_JPTR_HPP
#define UTILS_JPTR_HPP


#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/util/Exceptions.hpp>

#include <algorithm>
#include <memory>
#include <type_traits>

#include <jni.h>
#include <utils/Env.hpp>
#include <utils/JniError.hpp>


namespace joint {
namespace java
{

    // States that someone else owns the object, so the one who uses it should die before the owner.
    template < typename T_ > class JWeakRef;

    // States that the object user is not going to save the reference. A typical usage is a function parameter.
    template < typename T_ > class JTempRef;

    // Owns the local object reference
    template < typename T_ > class JLocalRef;

    // Owns the global object reference
    template < typename T_ > class JGlobalRef;


    template < typename T_ >
    class JTempRef
    {
    protected:
        JNIEnv*       _env;
        T_            _obj;

    public:
        JTempRef(JNIEnv* env, T_ obj) : _env(env), _obj(obj) { }

        explicit operator bool() const { return _obj; }
        T_ Get() const { return _obj; }
        JNIEnv* GetEnv() const { return _env; }

        JWeakRef<T_> Weak() && = delete;
        JWeakRef<T_> Weak() const &;
    };


    template < typename T_ >
    class JWeakRef
    {
    protected:
        JNIEnv*       _env;
        T_            _obj;

    public:
        JWeakRef(JNIEnv* env, T_ obj) : _env(env), _obj(obj) { }

        explicit operator bool() const { return _obj; }
        T_ Get() const { return _obj; }
        JNIEnv* GetEnv() const { return _env; }

        JTempRef<T_> Temp() const { return JTempRef<T_>(_env, _obj); }
        operator JTempRef<T_> () const { return JTempRef<T_>(_env, _obj); }
    };

    template < typename T_ >
    JWeakRef<T_> JTempRef<T_>::Weak() const & { return JWeakRef<T_>(_env, _obj); }


    template < typename T_ >
    class JLocalRef
    {
    private:
        JNIEnv*       _env;
        T_            _obj;

    public:
        JLocalRef(const JLocalRef&) = delete;
        JLocalRef& operator = (const JLocalRef&) = delete;

        JLocalRef()
            : _env(nullptr), _obj(nullptr)
        { }

        JLocalRef(JLocalRef&& other)
            : _env(other._env), _obj(other._obj)
        {
            other._env = nullptr;
            other._obj = nullptr;
        }

        JLocalRef& operator = (JLocalRef&& other)
        {
            JLocalRef tmp(std::move(other));
            Swap(tmp);
            return *this;
        }

        ~JLocalRef()
        {
            if (this->_obj)
                _env->DeleteLocalRef(this->_obj);
        }

        void Swap(JLocalRef& other)
        {
            std::swap(_env, other._env);
            std::swap(this->_obj, other._obj);
        }

        T_ Release()
        {
            T_ result = this->_obj;
            this->_obj = nullptr;
            return result;
        }

        explicit operator bool() const { return _obj != nullptr; }
        T_ Get() const { return _obj; }
        JNIEnv* GetEnv() const { return _env; }

        JGlobalRef<T_> Global() const;

        JWeakRef<T_> Weak() && = delete;
        JWeakRef<T_> Weak() const & { return JWeakRef<T_>(_env, _obj); }

        JTempRef<T_> Temp() const { return JTempRef<T_>(_env, _obj); }

        operator JWeakRef<T_> () && = delete;
        operator JWeakRef<T_> () const & { return JWeakRef<T_>(_env, _obj); }

        operator JTempRef<T_> () const { return JTempRef<T_>(_env, _obj); }

        template < bool Enabled_ = !std::is_same<T_, jobject>::value >
        static typename std::enable_if<Enabled_, JLocalRef>::type StealLocal(JNIEnv* env, jobject obj)
        { return JLocalRef(env, reinterpret_cast<T_>(obj)); }

        static JLocalRef StealLocal(JNIEnv* env, T_ obj)
        { return JLocalRef(env, obj); }

    private:
        JLocalRef(JNIEnv* env, T_ obj)
            : _env(env), _obj(obj)
        { }
    };


    template < typename T_ >
    class JGlobalRef
    {
        template < typename U_ >
        friend class JLocalRef;

    private:
        JavaVM*       _jvm;
        T_            _obj;

    public:
        JGlobalRef(const JGlobalRef&) = delete;
        JGlobalRef& operator = (const JGlobalRef&) = delete;

        JGlobalRef()
            : _jvm(nullptr), _obj(nullptr)
        { }

        JGlobalRef(JGlobalRef&& other)
            : _jvm(other._jvm), _obj(other._obj)
        {
            other._jvm = nullptr;
            other._obj = nullptr;
        }

        JGlobalRef& operator = (JGlobalRef&& other)
        {
            JGlobalRef tmp(std::move(other));
            Swap(tmp);
            return *this;
        }

        ~JGlobalRef()
        {
            if (!_jvm || !_obj)
                return;

            auto env = GetJavaEnv(_jvm);
            env->DeleteGlobalRef(_obj);
        }

        void Swap(JGlobalRef& other)
        {
            std::swap(_jvm, other._jvm);
            std::swap(_obj, other._obj);
        }

        explicit operator bool() const { return _obj != nullptr; }
        T_ Get() const { return _obj; }

        JLocalRef<T_> Local() const
        {
            auto env = GetJavaEnv(_jvm);
            return _obj ? JLocalRef<T_>::StealLocal(env, env->NewLocalRef(_obj)) : JLocalRef<T_>();
        }

        JWeakRef<T_> Weak(JNIEnv* env) && = delete;
        JWeakRef<T_> Weak(JNIEnv* env) const & { return JWeakRef<T_>(env, _obj); }

        JTempRef<T_> Temp(JNIEnv* env) const { return JTempRef<T_>(env, _obj); }

        template < bool Enabled_ = !std::is_same<T_, jobject>::value >
        static typename std::enable_if<Enabled_, JGlobalRef>::type StealLocal(JNIEnv* env, jobject obj)
        { return JLocalRef<T_>::StealLocal(env, reinterpret_cast<T_>(obj)).Global(); }

        static JGlobalRef StealLocal(JNIEnv* env, T_ obj)
        { return JLocalRef<T_>::StealLocal(env, obj).Global(); }

        JavaVM* GetJvm() const
        { return _jvm; }

    private:
        JGlobalRef(JavaVM* jvm, T_ obj)
            : _jvm(jvm), _obj(obj)
        { }
    };


    template < typename T_ >
    JGlobalRef<T_> JLocalRef<T_>::Global() const
    {
        JOINT_DEVKIT_FUNCTION_LOCAL_LOGGER("Joint.Java.JLocalRef");
        JavaVM* jvm = nullptr;
        JNI_CALL( _env->GetJavaVM(&jvm) );
        return this->_obj ? JGlobalRef<T_>(jvm, reinterpret_cast<T_>(_env->NewGlobalRef(this->_obj))) : JGlobalRef<T_>();
    }


#define DETAIL_JOINT_JAVA_DECLARE_JREFS(JType_, Name_) \
    typedef JWeakRef<JType_>     J##Name_##WeakRef; \
    typedef JTempRef<JType_>     J##Name_##TempRef; \
    typedef JLocalRef<JType_>    J##Name_##LocalRef; \
    typedef JGlobalRef<JType_>   J##Name_##GlobalRef

    DETAIL_JOINT_JAVA_DECLARE_JREFS( jobject,       Obj );
    DETAIL_JOINT_JAVA_DECLARE_JREFS( jthrowable,    Throwable );
    DETAIL_JOINT_JAVA_DECLARE_JREFS( jobjectArray,  ObjArray );
    DETAIL_JOINT_JAVA_DECLARE_JREFS( jstring,       String );
    DETAIL_JOINT_JAVA_DECLARE_JREFS( jclass,        Class );

#undef DETAIL_JOINT_JAVA_DECLARE_JREFS

}}


#endif
