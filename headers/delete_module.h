//this module contains delete-like functions
#ifndef DELETE_MODULE_H
#define DELETE_MODULE_H

// method of deleting last element
PyObject*
pop(PyObject* op, PyObject* Py_UNUSED(dummy));

//method of deleting first item
PyObject*
popFront(PyObject* op, PyObject* Py_UNUSED(dummy));

PyObject*
popAt(PyObject* op, PyObject* args);

PyObject*
popFrom(PyObject* op, PyObject* args);
#endif