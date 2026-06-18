// this module contains system commands - init, new, deakkoc and etc.
#ifndef CORE_MODULE_H
#define CORE_MODULE_H



// function which clear memory
void 
Custom_dealloc(PyObject* op);

// pre-initialization function
// basic field initialization occurs
PyObject*
Custom_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

// initialization function
// currently requires mandatory transfer of a value
// when initializing a class object
int
Custom_init(PyObject* op, PyObject* args, PyObject* kwds);

#endif