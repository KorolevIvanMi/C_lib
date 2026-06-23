// this module contains read-like functions
#ifndef READ_MODULE_H
#define READ_MODULE_H


// method for outputting data from a list
static PyObject*
show(PyObject *op, PyObject *Py_UNUSED(dummy));

//for beautiful outpu
PyObject*
show_line(PyObject *op, PyObject *Py_UNUSED(dummy));


// method of taking an element by index
PyObject*
get(PyObject* op, PyObject* args);

// method of taking an element by index for []
PyObject*
get_for_seq(PyObject* op, Py_ssize_t req_pos);

//method to get length of the list
PyObject*
length(PyObject* op, PyObject *Py_UNUSED(dummy));
Py_ssize_t
length_for_seq(PyObject* op);

//method to get the maximum element of the list
PyObject*
max(PyObject* op, PyObject* Py_UNUSED(dummy));

//method to get the minimun element of the list
PyObject*
min(PyObject* op, PyObject* Py_UNUSED(dummy));

//methods to get string value of my list
PyObject*
repr(PyObject* op);

PyObject*
repr_line(PyObject* op);

//method to check if value in List
PyObject*
contains(PyObject* op, PyObject* args);

//method to check if List eq to other List
PyObject*
equal(PyObject* op, PyObject* args);

#endif