#include <JointPythonCore.h>

PyMODINIT_FUNC PyInit_pyjoint(void)
{
#if PY_VERSION_HEX >= 0x03000000
	return JointPythonCore_InitModule_py3();
#endif
}


PyMODINIT_FUNC initpyjoint(void)
{
#if PY_VERSION_HEX < 0x03000000
	return JointPythonCore_InitModule_py2();
#else
	return NULL;
#endif
}
