#include <Python.h>

#include <update_module.h>
#include <list.h>

static MyList* from_PyList_to_MyList(PyObject* value){
    MyList* head = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    head->value = NULL;
    head->next = NULL;


    int length = PyList_GET_SIZE(value);
    if (length == 0){
        head->value = NULL;
        head->next = NULL;
    }
    else{
        MyList* current = head;
        current->value = PyList_GetItem(value, 0);
        Py_INCREF(current->value);
        for(int i = 1; i < length; i++){
            MyList* new_element = (MyList*)MyListType.tp_alloc(&MyListType, 0);
            new_element->next = NULL;
            new_element->value = PyList_GetItem(value, i);
            Py_INCREF(new_element->value);
            current->next = new_element;
            current = current->next;
        }
    }
    return head;
    
}

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


PyObject*
append(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    PyObject* value = NULL;
    if(!PyArg_ParseTuple(args, "O", &value)){
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if(PyList_Check(value)){
        MyList* new_part = from_PyList_to_MyList(value);
        if (self->value == NULL && self->next == NULL) {
            self->value = new_part->value;
            self->next = new_part->next;
            Py_RETURN_NONE;
        }
        MyList* current = self;
        while(current->next != NULL){
            current = current->next;
        }
        if(new_part->value != NULL){
            current->next = new_part;
        }
        

    }
    else if (PyObject_TypeCheck(value, &MyListType)) {
        MyList* value_to_mylist = (MyList*) value;
        MyList* current = self;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = value_to_mylist;
    }
    else{
        if (self->value == NULL && self->next == NULL) {
            self->value = Py_NewRef(value);
            Py_RETURN_NONE;
        }

        MyList* new_element = (MyList*)MyListType.tp_alloc(&MyListType, 0);
        new_element->next = NULL;
        new_element->value = NULL;
        if (value){
            new_element->value = Py_NewRef(value);
        }
        MyList* current = self;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_element;
    }
    Py_RETURN_NONE;
}
