#include <Python.h>

#include <update_module.h>
#include <list.h>

PyObject*
updateAt(PyObject* op, PyObject* args){

    if (op == NULL){
        PyErr_SetString(PyExc_ValueError, "List is NULL");
        return NULL;
    }
    
    MyList* self = (MyList*) op;
    PyObject* value;
    int pos = 0;
    if(!PyArg_ParseTuple(args, "Oi", &value, &pos)) {
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if(pos < 0){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }

    int i = 0;
    MyList* current = self;
    while( i != pos){
        if(current == NULL){
            PyErr_SetString(PyExc_IndexError, "index is out of range");
            return NULL;
        }
        else{
            current = current->next;
        }
        i = i+1;
    }
    if (current == NULL){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }
    Py_XSETREF(current->value, Py_NewRef(value));
    Py_RETURN_NONE;

}