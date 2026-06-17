#include <Python.h>

#include <core_module.h>
#include <list.h>

// implementation of functions
void 
Custom_dealloc(PyObject* op){
    MyList* self = (MyList*) op;
    while (self != NULL){
        MyList* temp = self;
        self = self->next;
        Py_XDECREF(temp->value);
        Py_TYPE(temp) -> tp_free(temp);
    }
}


PyObject*
Custom_new(PyTypeObject* type, PyObject* args, PyObject* kwds){
    MyList* self;
    self = (MyList* ) type->tp_alloc(type, 0);
    self->next = NULL;
    if(self != NULL){
        self->value = NULL;

    }
    return (PyObject*) self;
}  


int
Custom_init(PyObject* op, PyObject* args, PyObject* kwds){
    MyList* self  = (MyList*) op;
    static char *kwlist[] = {"value", NULL};
    PyObject* value = NULL;
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &value)) return -1;
    if (value){
        Py_XSETREF(self->value, Py_NewRef(value));
    }
    else{
        self->value = NULL;
    }

    return 0;
}