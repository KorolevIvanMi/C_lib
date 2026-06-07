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

static PyObject*
sub(PyObject* self, PyObject* args, PyObject* kwargs){
    double a;
    double b;
    double subb;
    static char* keys[] = {"a", "b", NULL};
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "dd", keys, &a, &b)) return NULL;
    subb = a-b;
    return PyFloat_FromDouble(subb);
}

static PyMethodDef calculator_methods [] = {
    {"sum", sum, METH_VARARGS, "Makes a+b and return result"},
    {"sub", (PyCFunction)(void(*)(void)) sub, METH_VARARGS | METH_KEYWORDS, "Makes a-b and return result"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef calculator_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "calculator",
    .m_doc = "Calculator module documentation",
    .m_size = 0,
    .m_methods = calculator_methods
};

PyMODINIT_FUNC
PyInit_calculator(void){
    return PyModuleDef_Init(&calculator_module);
}