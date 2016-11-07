#ifndef UTILS_JPTR_HPP
#define UTILS_JPTR_HPP


#include <joint/devkit/Exceptions.hpp>
#include <joint/devkit/JointException.hpp>

#include <algorithm>
#include <memory>
#include <type_traits>

#include <jni.h>
#include <utils/JniError.hpp>


namespace joint {
namespace java
{

	inline JNIEnv* GetJavaEnv(JavaVM* jvm, int version = JNI_VERSION_1_6)
	{
		JOINT_DEVKIT_FUNCTION_LOCAL_LOGGER("Joint.Java.Utils");

		JNIEnv* env = NULL;
		int retcode = jvm->GetEnv((void **)&env, version);
		if (retcode == JNI_EDETACHED)
			JOINT_THROW("Java VM is not attached to the current thread!");
		if (retcode == JNI_EVERSION)
			JOINT_THROW(devkit::StringBuilder() % "Java VM does not support version " % (version >> 16) % "." % ((version >> 8) & 0xFF) % "." % (version & 0xFF));
		if (retcode != JNI_OK)
			JOINT_THROW(devkit::StringBuilder() % "Cannot get JNIEnv from Java VM, retcode: " % retcode);
		return env;
	}


	template < typename T_ >
	struct LocalRefPolicy
	{
		using PointedType = typename std::remove_pointer<T_>::type;
		using SharedT = std::shared_ptr<PointedType>;

		static SharedT InitRef(JavaVM* jvm, T_ obj)
		{ return SharedT(obj, [jvm] (T_ obj) { GetJavaEnv(jvm)->DeleteLocalRef(obj); }); }

		static SharedT NewRef(JavaVM* jvm, T_ obj)
		{ return SharedT((T_)GetJavaEnv(jvm)->NewLocalRef(obj), [jvm] (T_ obj) { GetJavaEnv(jvm)->DeleteLocalRef(obj); }); }
	};


	template < typename T_ >
	struct GlobalRefPolicy
	{
		using PointedType = typename std::remove_pointer<T_>::type;
		using SharedT = std::shared_ptr<PointedType>;

		static SharedT InitRef(JavaVM* jvm, T_ obj)
		{
			auto env = GetJavaEnv(jvm);
			T_ global = (T_)env->NewGlobalRef(obj);
			env->DeleteLocalRef(obj);
			return SharedT(global, [jvm] (T_ obj) { GetJavaEnv(jvm)->DeleteGlobalRef(obj); });
		}

		static SharedT NewRef(JavaVM* jvm, T_ obj)
		{ return SharedT((T_)GetJavaEnv(jvm)->NewGlobalRef(obj), [jvm] (T_ obj) { GetJavaEnv(jvm)->DeleteGlobalRef(obj); }); }
	};


	template < typename T, template < typename > class RefPolicy >
	class BasicJPtr
	{
		template < typename T2, template < typename > class RefPolicy2 >
		friend class BasicJPtr;

		using PointedType = typename std::remove_pointer<T>::type;
		using SharedT = std::shared_ptr<PointedType>;

		JOINT_DEVKIT_LOGGER("Joint.Java.JPtr")

	private:
		JavaVM*     _jvm;
		SharedT     _obj;

	public:
		BasicJPtr()
			: _jvm(NULL), _obj()
		{ }

		template < template < typename > class OtherRefPolicy >
		BasicJPtr(const BasicJPtr<T, OtherRefPolicy>& other)
			: _jvm(other._jvm), _obj(other._obj)
		{
			static_assert(!std::is_same<BasicJPtr<T, RefPolicy>, BasicJPtr<T, OtherRefPolicy> >::value, "wtf");
			if (other._obj)
				_obj = RefPolicy<T>::NewRef(_jvm, other._obj.get());
		}

		template < typename U >
		BasicJPtr(const BasicJPtr<U, RefPolicy>& other,
			typename std::enable_if<std::is_base_of<typename std::remove_pointer<T>::type, typename std::remove_pointer<U>::type>::value>::type* = 0)
			: _jvm(other._jvm), _obj(other._obj)
		{ }

		template < template < typename > class OtherRefPolicy, typename U >
		BasicJPtr(const BasicJPtr<U, OtherRefPolicy>& other,
			typename std::enable_if<std::is_base_of<typename std::remove_pointer<T>::type, typename std::remove_pointer<U>::type>::value>::type* = 0)
			: _jvm(other._jvm), _obj(NULL)
		{
			static_assert(!std::is_same<BasicJPtr<T, RefPolicy>, BasicJPtr<T, OtherRefPolicy> >::Value, "wtf");
			if (other._obj)
				_obj = RefPolicy<T>::NewRef(_jvm, other._obj);
		}

		BasicJPtr(JNIEnv* env, T obj)
			: _jvm(NULL), _obj(NULL)
		{
			JNI_CALL( env->GetJavaVM(&_jvm) );
			if (obj)
				_obj = RefPolicy<T>::InitRef(_jvm, obj);
		}

		BasicJPtr(JavaVM* jvm, T obj)
			: _jvm(JOINT_DEVKIT_REQUIRE_NOT_NULL(jvm)), _obj(NULL)
		{
			if (obj)
				_obj = RefPolicy<T>::InitRef(_jvm, obj);
		}

		BasicJPtr(const BasicJPtr<T, RefPolicy>& other)
			: _jvm(other._jvm), _obj(other._obj)
		{ }

		~BasicJPtr()
		{ }

		JavaVM* GetJvm() const
		{ return JOINT_DEVKIT_REQUIRE_NOT_NULL(_jvm); }

		operator T() const
		{ return _obj.get(); }

		T Get() const
		{ return _obj.get(); }

		T operator -> () const
		{ CheckPtr(); return _obj; }

		explicit operator bool() const
		{ return (bool)_obj; }

		void Reset()
		{ *this = BasicJPtr<T, RefPolicy>(); }

		JNIEnv* GetEnv() const
		{ return GetJavaEnv(GetJvm()); }

	private:
		void CheckPtr() const
		{ JOINT_DEVKIT_REQUIRE_NOT_NULL(_obj); }
	};


	template < typename T_ >
	class JRef
	{
	protected:
		T_            _obj;

	public:
		operator T_() const
		{ return _obj; }

		T_ Get() const
		{ return _obj; }

	protected:
		JRef(T_ obj)
			: _obj(obj)
		{ }

		~JRef()
		{ }
	};

	template < typename T_ >
	class JGlobalRef;

	template < typename T_ >
	class JLocalRef : public JRef<T_>
	{
		using Base = JRef<T_>;

	private:
		JNIEnv*       _env;

	public:
		JLocalRef(const JLocalRef&) = delete;
		JLocalRef& operator = (const JLocalRef&) = delete;

		JLocalRef()
			: Base(nullptr), _env(nullptr)
		{ }

		JLocalRef(JLocalRef&& other)
			: Base(other._obj), _env(other._env)
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

		JNIEnv* GetEnv() const
		{ return _env; }

		JGlobalRef<T_> Global() const;

		template < bool Enabled_ = !std::is_same<T_, jobject>::value >
		static typename std::enable_if<Enabled_, JLocalRef>::type StealLocal(JNIEnv* env, jobject obj)
		{ return JLocalRef(env, reinterpret_cast<T_>(obj)); }

		static JLocalRef StealLocal(JNIEnv* env, T_ obj)
		{ return JLocalRef(env, obj); }

	private:
		JLocalRef(JNIEnv* env, T_ obj)
			: Base(obj), _env(env)
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

		operator T_() const
		{ return _obj; }

		T_ Get() const
		{ return _obj; }

		JLocalRef<T_> Local() const
		{
			auto env = GetJavaEnv(_jvm);
			return _obj ? JLocalRef<T_>::StealLocal(env, env->NewLocalRef(_obj)) : JLocalRef<T_>();
		}

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


#define DETAIL_JOINT_JAVA_DECLARE_JPTR(JType_, Name_) \
	typedef BasicJPtr < JType_, GlobalRefPolicy >    JGlobal##Name_##Ptr; \
	typedef BasicJPtr < JType_, LocalRefPolicy >     JLocal##Name_##Ptr

	DETAIL_JOINT_JAVA_DECLARE_JPTR( jobject,       Obj );
	DETAIL_JOINT_JAVA_DECLARE_JPTR( jthrowable,    Throwable );
	DETAIL_JOINT_JAVA_DECLARE_JPTR( jobjectArray,  ObjArray );
	DETAIL_JOINT_JAVA_DECLARE_JPTR( jstring,       String );
	DETAIL_JOINT_JAVA_DECLARE_JPTR( jclass,        Class );

#define DETAIL_JOINT_JAVA_DECLARE_JREFS(JType_, Name_) \
	typedef JRef<JType_>         J##Name_##Ref; \
	typedef JLocalRef<JType_>    J##Name_##LocalRef; \
	typedef JGlobalRef<JType_>   J##Name_##GlobalRef

	DETAIL_JOINT_JAVA_DECLARE_JREFS( jobject,       Obj );
	DETAIL_JOINT_JAVA_DECLARE_JREFS( jthrowable,    Throwable );
	DETAIL_JOINT_JAVA_DECLARE_JREFS( jobjectArray,  ObjArray );
	DETAIL_JOINT_JAVA_DECLARE_JREFS( jstring,       String );
	DETAIL_JOINT_JAVA_DECLARE_JREFS( jclass,        Class );

#undef DETAIL_JOINT_JAVA_DECLARE_JPTR

}}


#endif
