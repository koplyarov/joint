#ifndef BINDINGS_PYTHON_PYJOINT_JOINTEXCEPTION_H
#define BINDINGS_PYTHON_PYJOINT_JOINTEXCEPTION_H


#include <joint/devkit/StackFrameData.hpp>

#include <string>
#include <vector>

#include <pyjoint/Common.hpp>


namespace joint_python {
namespace pyjoint
{

	struct JointException
	{
		PyException_HEAD
		std::string* message;
		std::vector<joint::devkit::StackFrameData>* backtrace;
	};


	extern PyTypeObject JointException_type;

}}


#endif
