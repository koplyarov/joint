#ifndef JOINT_PRIVATE_JOINTSTRUCTS_H
#define JOINT_PRIVATE_JOINTSTRUCTS_H


#include <joint/Joint.h>
#include <joint/private/Joint_Array.hpp>
#include <joint/private/JsonNode.hpp>

#include <atomic>
#include <string>
#include <vector>


extern "C"
{

	struct Joint_Context
	{
		int dummy;
	};

	struct Joint_Binding
	{
		void*                       userData;
		Joint_BindingDesc           desc;
		std::atomic<int>            refCount;

		Joint_Binding(void* userData, const Joint_BindingDesc& desc)
			: userData(userData), desc(desc), refCount(1)
		{ }
	};


	struct Joint_ManifestNode : public joint::JsonNode
	{
		Joint_ManifestNode(Joint_ManifestNode&& other)
			: joint::JsonNode(std::move(other))
		{ }

		Joint_ManifestNode(joint::JsonNode&& other)
			: joint::JsonNode(std::move(other))
		{ }
	};


	struct Joint_Manifest
	{
		const Joint_ManifestNode   rootNode;
		const std::string          location;
	};

	std::ostream& operator << (std::ostream& s, const Joint_ManifestHandle m)
	{
		if (!m)
			s << "null";
		else
			s << "{ location: " << m->location << ", root: " << m->rootNode << " }";
		return s;
	}


	struct Joint_Module
	{
		Joint_ModuleHandleInternal  internal;
		Joint_Binding*              binding;
		std::atomic<int>            refCount;

		Joint_Module(Joint_ModuleHandleInternal internal, Joint_Binding* binding)
			: internal(internal), binding(binding), refCount(1)
		{ }
	};


	struct Joint_Object
	{
		Joint_ObjectHandleInternal  internal;
		Joint_Module*               module;
		std::atomic<int>            refCount;

		Joint_Object(Joint_ObjectHandleInternal internal, Joint_Module* module)
			: internal(internal), module(module), refCount(1)
		{ }
	};


	struct Joint_StackFrameData
	{
		std::string      module;
		std::string      filename;
		Joint_SizeT      line;
		std::string      code;
		std::string      function;
	};

	struct Joint_Exception
	{
		std::string                        message;
		std::vector<Joint_StackFrameData>  backtrace;
	};

}


#endif
