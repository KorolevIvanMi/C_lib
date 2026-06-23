#include <Python.h>

#include <list.h>
#include <read_module.h>


static PyObject*
show(PyObject *op, PyObject *Py_UNUSED(dummy)){
    if(op == NULL){
        PyErr_SetString(PyExc_ValueError, "List is NULL");
        return NULL;
    }
    MyList* self = (MyList* ) op;
    MyList* current = self;
    printf("[");
    while(current != NULL){
        if (current->value != NULL){
            if(PyObject_TypeCheck(current->value, &MyListType)){
                MyList* val = (MyList*)current->value;
                show(current->value,NULL );
                printf(", ");
            }else{
                PyObject* repr = PyObject_Repr(current->value);
                if (repr != NULL){
                    const char* str = PyUnicode_AsUTF8(repr);
                    if(str != NULL){
                        printf("%s", str);
                        if(current->next != NULL){
                            printf(", ");
                        }
                    }
                    Py_DECREF(repr);
                }else{
                    printf("<?>");
                    PyErr_Clear();
                }
            }
        }else{
            printf("NULL");
        }
        current = current->next;
        
    }
    printf("]");
    fflush(stdout);
    Py_RETURN_NONE;
}

PyObject*
show_line(PyObject *op, PyObject *Py_UNUSED(dummy)){
    PyObject* result = show(op, NULL);
    printf("\n");
    fflush(stdout);
    return result;
}

PyObject*
get(PyObject* op, PyObject* args){
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
    int i = 0;
    if(self->value == NULL) {
        Py_RETURN_NONE;
    }
    MyList* current = self;
    while( i != req_pos){
        if(current == NULL){
            PyErr_SetString(PyExc_IndexError, "index is out of range");
            return NULL;
        }
        else{
            current = current->next;
        }
        i = i+1;
    }
    if(current == NULL){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }
    Py_INCREF(current->value); 
    return current->value;
}

PyObject*
get_for_seq(PyObject* op, Py_ssize_t req_pos){
    MyList* self = (MyList*) op;
    // if (req_pos < 0){
    //     PyErr_SetString(PyExc_IndexError, "index is out of range");
    //     return NULL;
    // }
    int i = 0;
    if(self->value == NULL) {
        Py_RETURN_NONE;
    }
    MyList* current = self;
    while( i != req_pos){
        if(current == NULL){
            PyErr_SetString(PyExc_IndexError, "index is out of range");
            return NULL;
        }
        else{
            current = current->next;
        }
        i = i+1;
    }
    if(current == NULL){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }
    Py_INCREF(current->value); 
    return current->value;
}

PyObject*
length(PyObject* op, PyObject *Py_UNUSED(dummy)){
    Py_ssize_t len = 0;
    MyList* self = (MyList*) op;
    if(self->value==NULL && self->next == NULL){
        return PyLong_FromLong(len);
    }
    MyList* current = self;
    while(current!=NULL){
        len++;
        current= current->next;
    }

    return PyLong_FromLong(len);
}

Py_ssize_t
length_for_seq(PyObject* op){
    Py_ssize_t len = 0;
    MyList* self = (MyList*) op;
    if(self->value==NULL && self->next == NULL){
        return len;
    }
    MyList* current = self;
    while(current!=NULL){
        len++;
        current= current->next;
    }

    return len;
}

PyObject*
max(PyObject* op, PyObject* Py_UNUSED(dummy)){
    MyList* self = (MyList*) op;
    if(self->value == NULL && self->next == NULL){
        PyErr_SetString(PyExc_ValueError, "max() on empty list");
        return NULL;
    }

    PyObject* max_value = self->value;
    Py_XINCREF(max_value);
    MyList* current = self->next;
    while(current != NULL){
        if (PyObject_RichCompareBool(current->value, max_value, Py_GT) == true){
            max_value = current->value;
            Py_XINCREF(max_value);
        }
        current= current->next;
    }

    return max_value;
}

PyObject*
min(PyObject* op, PyObject* Py_UNUSED(dummy)){
    MyList* self = (MyList*) op;
    if(self->value == NULL && self->next == NULL){
        PyErr_SetString(PyExc_ValueError, "max() on empty list");
        return NULL;
    }

    PyObject* max_value = self->value;
    Py_XINCREF(max_value);
    MyList* current = self->next;
    while(current != NULL){
        if (PyObject_RichCompareBool(current->value, max_value, Py_LT) == true){
            max_value = current->value;
            Py_XINCREF(max_value);
        }
        current= current->next;
    }

    return max_value;
}

PyObject*
repr(PyObject* op){
        if (op == NULL) {
        PyErr_SetString(PyExc_ValueError, "List is NULL");
        return NULL;
    }
    
    MyList* self = (MyList*) op;
    MyList* current = self;
    PyObject* items = PyList_New(0);
    
    if (items == NULL) {
        return NULL;
    }
    
    while (current != NULL) {
        PyObject* item_str;
        
        if (current->value != NULL) {
            if (PyObject_TypeCheck(current->value, &MyListType)) {
                item_str = repr(current->value);
            } else {
                item_str = PyObject_Repr(current->value);
            }
        } else {
            item_str = PyUnicode_FromString("NULL");
        }
        
        if (item_str == NULL) {
            Py_DECREF(items);
            return NULL;
        }
        
        if (PyList_Append(items, item_str) < 0) {
            Py_DECREF(item_str);
            Py_DECREF(items);
            return NULL;
        }
        Py_DECREF(item_str);
        
        current = current->next;
    }
    
    // Соединяем все элементы через ", "
    PyObject* separator = PyUnicode_FromString(", ");
    if (separator == NULL) {
        Py_DECREF(items);
        return NULL;
    }
    
    PyObject* joined = PyUnicode_Join(separator, items);
    Py_DECREF(separator);
    Py_DECREF(items);
    
    if (joined == NULL) {
        return NULL;
    }
    
    // Оборачиваем в квадратные скобки
    PyObject* result = PyUnicode_FromFormat("[%U]", joined);
    Py_DECREF(joined);
    
    return result;
}

PyObject*
repr_line(PyObject *op) {
    PyObject* result = repr(op);
    
    // Добавляем новую строку
    PyObject* with_newline = PyUnicode_FromFormat("%U\n", result);
    Py_DECREF(result);
    
    return with_newline;
}

PyObject*
contains(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    MyList* current = self;

    PyObject* req_value = NULL;
    if(!PyArg_ParseTuple(args, "O", &req_value)){
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if(current->value == NULL && current->next == NULL ){
        Py_RETURN_FALSE;
    }
    if(PyObject_TypeCheck(req_value, &MyListType)){
        MyList* req_value_list = (MyList*) req_value;

        while (current != NULL){
            if(PyObject_RichCompareBool(current->value, req_value_list->value, Py_EQ) == true){
                MyList* req_value_list_current = req_value_list;
                MyList* current_current = current;
                bool flag = false;
                while(req_value_list_current != NULL && current_current != NULL){

                    if(PyObject_RichCompareBool(current_current->value, req_value_list_current->value, Py_EQ) == false){
                        flag = true;
                        break;
                    }
                    current_current = current_current->next;
                    req_value_list_current = req_value_list_current->next;
                }
                if (flag == false){
                    Py_RETURN_TRUE;
                }
            }
            current = current->next;
        }
        Py_RETURN_FALSE;
    }
    else{
        while(current != NULL){
            if(PyObject_RichCompareBool(current->value, req_value, Py_EQ) == true){
                Py_RETURN_TRUE;
            }
            current = current->next;
        }
        Py_RETURN_FALSE;
    }
}

PyObject*
equal(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    PyObject* value = NULL;
    if(!PyArg_ParseTuple(args, "O", &value)){
        PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
        return NULL;
    }
    if(PyObject_TypeCheck(value, &MyListType)){
        MyList* other = (MyList*)value;

        MyList* current = self;
        MyList* other_current = other;

        if(current->value == NULL && current->next == NULL && other_current->next == NULL && other_current->value == NULL){
            Py_RETURN_TRUE;
        }
        while(current!= NULL && other_current != NULL){
            if(current->value == NULL || other_current == NULL){
                Py_RETURN_FALSE;
            }
            int res = PyObject_RichCompareBool(current->value, other_current->value, Py_EQ);
            if(res == 0){
                Py_RETURN_FALSE;
            }
            current = current->next;
            other_current = other_current->next;
        }
        if ((current == NULL && other_current != NULL) || (current!=NULL && other_current == NULL)){
            Py_RETURN_FALSE;
        }
        Py_RETURN_TRUE;
    }
    else{
        PyErr_SetString(PyExc_TypeError, "This argument is not List");
        return NULL;
    }

}