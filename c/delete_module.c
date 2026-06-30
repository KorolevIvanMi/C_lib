#include <Python.h>

#include <list.h>
#include <delete_module.h>

bool is_length_okey(MyList* head, int req_pos){
    MyList* current = head;
    int i = 0;
    while(i != req_pos){
        if (current == NULL){
            return false;
        }
        else{
            current=current->next;
            i++;
        }
    }
    return true;
}


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
    
    if (value == NULL){
        Py_RETURN_NONE;
    }
    else{
        return value;
    };
}

PyObject*
popFront(PyObject* op, PyObject* Py_UNUSED(dummy)){
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
    if (value == NULL){
        Py_RETURN_NONE;
    }
    else{
        return value;
    };
}

PyObject*
popAt(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;

    int req_pos = 0;
    if(!PyArg_ParseTuple(args, "i", &req_pos)){
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if (req_pos < 0){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }

    if(self->value == NULL && self->next == NULL){
        Py_RETURN_NONE;
    }

    int i = 0;
    PyObject* value = self->value;
    if (req_pos == 0){

        value = self->value;
        MyList* new_l = self->next;
        
        if (new_l == NULL) {
            self->value = NULL;
            return value;
        }
        
        self->value = new_l->value;
        self->next = new_l->next;

        Py_TYPE(new_l)->tp_free(new_l);
        if (value == NULL){
            Py_RETURN_NONE;
        }
        else{
            return value;
        };
    }else{
        MyList* current = self->next;
        MyList* curret_prev = self;
        while(i!=req_pos-1){
            if(current == NULL){
                PyErr_SetString(PyExc_IndexError, "index is out of range");
                return NULL;
            }
            else{
                current = current->next;
                curret_prev= curret_prev->next;
            }
            i = i+1;
        }
        if(current == NULL){
            PyErr_SetString(PyExc_IndexError, "index is out of range");
            return NULL;
        }
        value = current->value;
        current->value = NULL;
        curret_prev->next = current->next;
        Py_TYPE(current)->tp_free(current);
        if (value == NULL){
            Py_RETURN_NONE;
        }
        else{
            return value;
        };
    }
    Py_RETURN_NONE;
}

PyObject*
popAfter(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    int req_pos = 0;
    if(!PyArg_ParseTuple(args, "i", &req_pos)){
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if(req_pos < 0){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }

    MyList* current = self; 

    int i = 0;
    while(i != req_pos && current!=NULL){
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

    if(current->next == NULL){
        Py_RETURN_NONE;
    }
    MyList* src = current->next;
    current->next = NULL;
    while (src != NULL){
        MyList* temp = src;
        src=src->next;
        Py_XDECREF(temp->value);
        Py_TYPE(temp) -> tp_free(temp);
    }

    Py_RETURN_NONE;
}

PyObject*
popBefore(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    int req_pos = 0;
    if(!PyArg_ParseTuple(args, "i", &req_pos)){
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if(req_pos < 0){
        PyErr_SetString(PyExc_IndexError, "index is out of range in req");
        return NULL;
    }
    if(req_pos == 0){
        Py_RETURN_NONE;
    }

    if(is_length_okey(self, req_pos)){
        int i = 0;
        MyList* current = self->next;
        while (i < req_pos-1){
            MyList* temp = current;
            current = current->next;
            Py_XDECREF(temp->value);
            Py_TYPE(temp) -> tp_free(temp);
            i++;
        }
        self->value = current->value;
        self->next = current->next;
        current->value = NULL;
        Py_TYPE(current) -> tp_free(current);
        Py_RETURN_NONE;
    }
    else{
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }
}

PyObject*
clear(PyObject* op, PyObject* Py_UNUSED(dummy)){
    MyList* self = (MyList*)op;
    if(self->value == NULL && self->next == NULL){
        Py_RETURN_NONE;
    }
    if(self->next == NULL){
        self->value = NULL;
        Py_RETURN_NONE;
    }
    self->value = NULL;
    Py_XDECREF(self->value);
    MyList* current = self->next;
    self->next = NULL;
    while(current != NULL){
        MyList* temp = current;
        current=current->next;
        Py_XDECREF(temp->value);
        Py_TYPE(temp) -> tp_free(temp);
    }
    Py_RETURN_NONE;
}

PyObject*
popSection(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    int left_index = 0;
    int right_index = 0;
    if(!PyArg_ParseTuple(args, "ii", &left_index, &right_index)){
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if(left_index < 0 || right_index < 0){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }
    if(left_index > right_index){
        PyErr_SetString(PyExc_IndexError, "left index can not be bigger than right");
        return NULL;
    }

    MyList* left_part = self;
    int i = 0;
    while(i < left_index){
        if(left_part == NULL){
            PyErr_SetString(PyExc_IndexError, "index is out of range");
            return NULL;
        }
        i++;
        left_part = left_part->next;
    }
    if(left_part == NULL){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }
    MyList* right_part = left_part->next;
    while (i < right_index-1){
        if(right_part == NULL){
            PyErr_SetString(PyExc_IndexError, "index is out of range");
            return NULL;
        }
        MyList* temp = right_part;
        right_part = right_part->next;
        Py_XDECREF(temp->value);
        Py_TYPE(temp) -> tp_free(temp);
        i++;
    }
    left_part->next = NULL;
    left_part->next = right_part;
    Py_RETURN_NONE;
}