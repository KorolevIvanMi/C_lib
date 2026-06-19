#include <Python.h>

#include <list.h>
#include <delete_module.h>


PyObject*
pop(PyObject* op, PyObject* Py_UNUSED(dummy)){
    MyList* self = (MyList*) op;
    if(self->value == NULL){
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

PyObject*
pop_front(PyObject* op, PyObject* Py_UNUSED(dummy)){
    MyList* self = (MyList*) op;
    if(self->value == NULL){
        Py_RETURN_NONE;
    }
    PyObject* value = self->value;
    if(self->next == NULL){
        self->value = NULL;
        return value;
    }
    MyList* next_node = self->next;

    self->value = next_node->value;
    self->next = next_node->next;
    Py_TYPE(next_node)->tp_free(next_node);
    return value;

}