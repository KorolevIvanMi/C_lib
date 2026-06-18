#include <Python.h>

#include <update_module.h>
#include <list.h>

static MyList* from_PyList_to_MyList(PyObject* value){
    MyList* head = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    head->value = NULL;
    head->next = NULL;


    int length = PyList_GET_SIZE(value);
    if (length == 0){
        return head;
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
    // if argument - python list
    if(PyList_Check(value)){
        MyList* new_part = from_PyList_to_MyList(value);

        if (self->value == NULL && self->next == NULL) {
            self->value = new_part->value;
            Py_XINCREF(self->value);
            self->next = new_part->next;
            Py_TYPE(new_part)->tp_free((PyObject*)new_part); 
            Py_RETURN_NONE;
        }
        MyList* current = self;
        while(current->next != NULL){
            current = current->next;
        }
        if(new_part->value == NULL){
            Py_TYPE(new_part)->tp_free((PyObject*)new_part);
        }
        else{
            current->next = new_part;
        }
    }
    // if argument MyList
    else if (PyObject_TypeCheck(value, &MyListType)) {
        MyList* value_to_mylist = (MyList*) value;
        MyList* current = self;
        while(current->next != NULL) {
            current = current->next;
        }
        MyList* src = value_to_mylist;
        while(src != NULL) {
            if(current->value == NULL && current == self) {
                current->value = src->value;
                Py_XINCREF(current->value);
            } else {
                MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
                new_node->value = src->value;
                Py_XINCREF(new_node->value);
                new_node->next = NULL;
                current->next = new_node;
                current = new_node;
            }
            src = src->next;
        }
    }
    // if argument - single item
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

PyObject*
prepend(PyObject* op, PyObject* args){
    MyList* self = (MyList*)op;
    PyObject* value = NULL;
    if(!PyArg_ParseTuple(args, "O", &value)){
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }

    if(PyList_Check(value)){
        MyList* new_part = from_PyList_to_MyList(value);

        if(self->value == NULL && self->next == NULL){
            self->value = new_part->value;
            Py_XINCREF(self->value);
            self->next = new_part->next;
            Py_TYPE(new_part)->tp_free((PyObject*)new_part); 
        }
        else{
            if(new_part->value==NULL){
                Py_TYPE(new_part)->tp_free((PyObject*)new_part);
            }
            else{
                MyList* old_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
                old_node->value = self->value;      
                old_node->next = self->next;

                MyList* current_new_part = new_part;
                while(current_new_part->next != NULL){
                    current_new_part = current_new_part->next;
                }
                current_new_part->next = old_node;

                self->value = new_part->value;
                Py_XINCREF(self->value);
                self->next = new_part->next;

                new_part->value = NULL;
                new_part->next = NULL;
                Py_TYPE(new_part)->tp_free((PyObject*)new_part);
                
            }
        }
    }else if (PyObject_TypeCheck(value, &MyListType)){
        MyList* value_as_my_list = (MyList*) value;
        if(value_as_my_list->value != NULL){
            MyList* old_part = (MyList*)MyListType.tp_alloc(&MyListType, 0);
            old_part->value = self->value;
            old_part->next = self->next;

            
            self->value = value_as_my_list->value;
            Py_INCREF(self->value);
            self->next = NULL;

            MyList* src = value_as_my_list->next;
            MyList* current = self;
            while(src != NULL){
                MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
                new_node->value = src->value;
                Py_XINCREF(new_node->value);
                new_node->next = NULL;
                current->next = new_node;
                current = new_node;
                src = src->next;
            }
            current->next = old_part;

        }
        // new_part->value = NULL;
        // new_part->next = NULL;
        // Py_TYPE(new_part)->tp_free((PyObject*)new_part);
    }else{
        MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
        new_node->value = value;
        Py_XINCREF(new_node->value);
        new_node->next = NULL;

        MyList* old_node = (MyList*)MyListType.tp_alloc(&MyListType,0);
        old_node->value = self->value;
        old_node->next = self->next;

        new_node->next = old_node;

        self->value = new_node->value;
        self->next = new_node->next;

        Py_TYPE(new_node)->tp_free((PyObject*)new_node);

    }
    Py_RETURN_NONE;
}

PyObject*
insert(PyObject* op, PyObject* args){
    MyList* self = (MyList*)op;
    int req_pos = 0;
    PyObject* value = NULL;
    
    if(!PyArg_ParseTuple(args, "O|i", &value, &req_pos)){  
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if(req_pos < 0){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }
    if(!PyObject_TypeCheck(value, &MyListType)){
        if(PyErr_WarnEx(PyExc_UserWarning, 
            "insert() is designed for MyList objects. Use append() or prepend() for single values.", 1) < 0){
            return NULL;
        }
    }
    
    if(req_pos == 0){
        MyList* old_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
        old_node->value = self->value;
        old_node->next = self->next;
        
        self->value = value;
        Py_XINCREF(self->value);
        self->next = old_node;
        
        Py_RETURN_NONE;
    }

    
    int i = 0;
    MyList* current = self;
    while(i != req_pos - 1){  
        if(current == NULL){
            PyErr_SetString(PyExc_IndexError, "index is out of range");
            return NULL;
        }
        current = current->next;
        i++;
    }
    if(current == NULL){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }

    
    MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    new_node->value = value;
    Py_XINCREF(new_node->value);  
    new_node->next = current->next;
    current->next = new_node;

    Py_RETURN_NONE;
}