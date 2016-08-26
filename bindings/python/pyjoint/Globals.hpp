#ifndef PYJOINT_GLOBALS_H
#define PYJOINT_GLOBALS_H


#include <pyjoint/Common.hpp>


namespace joint_python {
namespace pyjoint
{

	extern PyObject* InvalidInterfaceChecksumException;
	PyObject* Cast(PyObject* self, PyObject* args, PyObject* kwds);

}}


#endif
