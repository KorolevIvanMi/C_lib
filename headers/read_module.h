// this module contains read-like functions
#ifndef READ_MODULE_H
#define READ_MODULE_H


// method for outputting data from a list
PyObject*
show(PyObject *op, PyObject *Py_UNUSED(dummy));

// method of taking an element by index
PyObject*
get(PyObject* op, PyObject* args);

#endif