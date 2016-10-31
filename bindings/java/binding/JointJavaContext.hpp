#ifndef BINDING_JOINTJAVACONTEXT_HPP
#define BINDING_JOINTJAVACONTEXT_HPP


#include <joint/Joint.h>
#include <joint/devkit/Logger.hpp>
#include <joint/devkit/Singleton.hpp>

#include <utils/JPtr.hpp>


namespace joint {
namespace java
{

	class JointJavaContext : private devkit::Singleton<JointJavaContext>
	{
		JOINT_DEVKIT_SINGLETON_INTERNALS(JointJavaContext);
		JOINT_DEVKIT_LOGGER("Joint.Java.JointJavaContext");

	public:
		class TypeDescriptor
		{
		private:
			const JLocalObjPtr&  _obj;

		public:
			TypeDescriptor(const JLocalObjPtr& obj) : _obj(obj) { }

			Joint_TypeId GetId() const;
			Joint_InterfaceChecksum GetInterfaceChecksum() const;
		};

	private:
		JGlobalClassPtr TypeDescriptor_cls;
		JGlobalClassPtr MethodDescriptor_cls;
		JGlobalClassPtr InterfaceDescriptor_cls;
		JGlobalClassPtr MemberInfo_cls;

		jfieldID TypeDescriptor_typeId;
		jfieldID TypeDescriptor_interfaceChecksum;

	private:
		JointJavaContext();
	};


}}

#endif
