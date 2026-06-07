#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>

static PyObject*
print_system(PyObject* self, PyObject* args){
    const char* words;
    if(!PyArg_ParseTuple(args, "s", &words)) return NULL;
    printf(words);
    return PyLong_FromLong(1);
}

static PyMethodDef print_methods[] = {
    {"myprint", print_system, METH_VARARGS, "Execute printf"}, 
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef print_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "print",
    .m_size = 0,
    .m_methods = print_methods
};

PyMODINIT_FUNC
PyInit_print(void){
    return PyModuleDef_Init(&print_module);
}