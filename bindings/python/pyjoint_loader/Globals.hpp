#ifndef PYJOINT_LOADER_GLOBALS_HPP
#define PYJOINT_LOADER_GLOBALS_HPP


#include <Python.h>


namespace joint_python {
namespace pyjoint_loader
{

	PyObject* LoadModule(PyObject* self, PyObject* args, PyObject* kwds);

}}

#endif
