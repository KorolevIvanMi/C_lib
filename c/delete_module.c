#include <Python.h>

#include <list.h>
#include <delete_module.h>


PyObject*
pop(PyObject* op, PyObject* Py_UNUSED(dummy)){
    MyList* self = (MyList*) op;
    if(self == NULL){
        Py_RETURN_NONE;
    }
    if(self->next==NULL){
        PyObject* value = self->value;
        self->value = NULL;
        return value;
    }
    MyList* current = self;
    while(current->next->next != NULL){
        current = current->next;
    }
    PyObject* value = current->next->value;
    current->next->value = NULL;
    Py_TYPE(current->next)->tp_free(current->next);
    current->next = NULL;
    
    return value;
}
