#ifndef JOINT_DEVKIT_ACCESSORS_MAKEACCESSOR_HPP
#define JOINT_DEVKIT_ACCESSORS_MAKEACCESSOR_HPP


#include <memory>


namespace joint {
namespace devkit {
namespace accessors
{

	template < typename Impl_, typename... Args_ >
	typename Impl_::AccessorType MakeAccessor(Args_&&... args)
	{
		using AccessorType = typename Impl_::AccessorType;

		std::unique_ptr<Impl_> impl(new Impl_(std::forward<Args_>(args)...));

		AccessorType result;
		result.VTable = Impl_::GetVTablePtr();
		result.Instance = impl.release();
		return result;
	}

}}}

#endif
