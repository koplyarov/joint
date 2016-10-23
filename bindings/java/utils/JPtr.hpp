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
				_obj = RefPolicy<T>::NewRef(_jvm, other._obj);
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


#define DETAIL_JOINT_JAVA_DECLARE_JPTR(JType_, Name_) \
	typedef BasicJPtr < JType_, GlobalRefPolicy >    JGlobal##Name_##Ptr; \
	typedef BasicJPtr < JType_, LocalRefPolicy >     JLocal##Name_##Ptr

	DETAIL_JOINT_JAVA_DECLARE_JPTR( jobject,       Obj );
	DETAIL_JOINT_JAVA_DECLARE_JPTR( jthrowable,    Throwable );
	DETAIL_JOINT_JAVA_DECLARE_JPTR( jobjectArray,  ObjArray );
	DETAIL_JOINT_JAVA_DECLARE_JPTR( jstring,       String );
	DETAIL_JOINT_JAVA_DECLARE_JPTR( jclass,        Class );

#undef DETAIL_JOINT_JAVA_DECLARE_JPTR

}}


#endif
