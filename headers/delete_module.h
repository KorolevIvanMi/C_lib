//this module contains delete-like functions
#ifndef DELETE_MODULE_H
#define DELETE_MODULE_H

// method of deleting last element
PyObject*
pop(PyObject* op, PyObject* Py_UNUSED(dummy));

//method of deleting first item
PyObject*
pop_front(PyObject* op, PyObject* Py_UNUSED(dummy));
#endif