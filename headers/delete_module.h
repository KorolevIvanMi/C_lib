//this module contains delete-like functions
#ifndef DELETE_MODULE_H
#define DELETE_MODULE_H

// method of deleting last element
PyObject*
pop(PyObject* op, PyObject* Py_UNUSED(dummy));

// method of deleting first item
PyObject*
popFront(PyObject* op, PyObject* Py_UNUSED(dummy));

// delete element on the posion 
PyObject*
popAt(PyObject* op, PyObject* args);

// delete all elements after position
PyObject*
popAfter(PyObject* op, PyObject* args);

// delete all elements before position

#endif