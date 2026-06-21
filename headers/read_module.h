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

Py_ssize_t
length_for_seq(PyObject* op);
#endif