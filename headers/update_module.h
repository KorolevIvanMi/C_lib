//this module contains update-like functions
#ifndef UPDATE_MODULE_H
#define UPDATE_MODULE_H

// method of update value on index
PyObject*
updateAt(PyObject* op, PyObject* args);

// method of adding to the end of a list
PyObject*
append(PyObject* op, PyObject* args);

// method of adding to the start of a list
PyObject*
prepend(PyObject* op, PyObject* args); 
#endif