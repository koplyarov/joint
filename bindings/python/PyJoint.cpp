#include <JointPythonCore.h>

PyMODINIT_FUNC PyInit_pyjoint(void)
{
    return JointPythonCore_InitModule_py3();
}
