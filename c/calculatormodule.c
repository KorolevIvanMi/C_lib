#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject*
sum(PyObject* self, PyObject* args){
    double a= 0;
    double b = 0;
    double summa = 0;
    if(!PyArg_ParseTuple(args , "dd", &a, &b)) return NULL;
    summa = a+b;
    return PyFloat_FromDouble(summa);
}

static PyMethodDef calculator_methods [] = {
    {"sum", sum, METH_VARARGS, "Makes a+b and return result"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef calculator_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "calculator",
    .m_size = 0,
    .m_methods = calculator_methods
};

PyMODINIT_FUNC
PyInit_calculator(void){
    return PyModuleDef_Init(&calculator_module);
}