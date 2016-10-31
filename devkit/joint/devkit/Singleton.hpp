#ifndef JOINT_DEVKIT_SINGLETON_HPP
#define JOINT_DEVKIT_SINGLETON_HPP


#include <algorithm>
#include <memory>
#include <mutex>


namespace joint {
namespace devkit
{

#define JOINT_DEVKIT_SINGLETON_INTERNALS(ClassName_) \
		friend class joint::devkit::Singleton<ClassName_>; \
		ClassName_(const ClassName_&) = delete; \
		ClassName_(ClassName_&&) = delete; \
		ClassName_& operator = (const ClassName_&) = delete; \
		ClassName_& operator = (ClassName_&&) = delete


	template < typename T_ >
	class Singleton
	{
	public:
		static T_& Instance()
		{
#if __cplusplus < 201100L
#	error "local static variables are not thread-safe in older standards"
#endif
			static T_ inst;
			return inst;
		}

		static const T_& ConstInstance()
		{ return Instance(); }
	};

}}

#endif
