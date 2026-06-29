//this module contains update-like functions
#ifndef UPDATE_MODULE_H
#define UPDATE_MODULE_H

// method of update value on index
PyObject*
updateAt(PyObject* op, PyObject* args);
int
updateAt_for_seq(PyObject* op,  Py_ssize_t pos, PyObject* value);

// method of adding to the end of a list
PyObject*
append(PyObject* op, PyObject* args);

// method of adding to the start of a list
PyObject*
prepend(PyObject* op, PyObject* args); 

// method to build on list into another
PyObject*
insert(PyObject* op, PyObject* args);

// method to concat to Lists
PyObject*
concat(PyObject* o1, PyObject* o2);

// method which return the copy of the list
PyObject*
copy(PyObject* op, PyObject *Py_UNUSED(dummy));

// method to sort list
PyObject*
sort(PyObject* op, PyObject* args);

// method to replace with slices
int 
replace_for_map(PyObject* op, PyObject* key, PyObject* value);

// method to mult list
PyObject*
repeat_for_seq(PyObject* op, Py_ssize_t count);
#endif