#include <Python.h>

#include <update_module.h>
#include <list.h>

static Py_ssize_t
get_length(MyList* head) {
    Py_ssize_t len = 0;
    MyList* current = head;
    while (current != NULL && current->value != NULL) {
        len++;
        current = current->next;
    }
    return len;
}

static MyList* 
from_PyList_to_MyList(PyObject* value){
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

static MyList*
copy_my_list(MyList* src) {
    if (!src) return NULL;
    
    MyList* head = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    if (!head) return NULL;
    
    head->value = src->value ? Py_NewRef(src->value) : NULL;
    head->next = NULL;
    
    MyList* current = head;
    MyList* src_current = src->next;
    
    while (src_current) {
        MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
        if (!new_node) {
            return NULL;
        }
        new_node->value = src_current->value ? Py_NewRef(src_current->value) : NULL;
        new_node->next = NULL;
        current->next = new_node;
        current = new_node;
        src_current = src_current->next;
    }
    
    return head;
}

static MyList*
get_last(MyList* head) {
    if (!head) return NULL;
    MyList* current = head;
    while (current->next) {
        current = current->next;
    }
    return current;
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

int 
updateAt_for_seq(PyObject* op,  Py_ssize_t pos, PyObject* value){
    if (op == NULL){
        PyErr_SetString(PyExc_ValueError, "List is NULL");
        return -1;
    }
    
    MyList* self = (MyList*) op;

    int i = 0;
    MyList* current = self;
    while( i != pos){
        if(current == NULL){
            PyErr_SetString(PyExc_IndexError, "index is out of range");
            return -1;
        }
        else{
            current = current->next;
        }
        i = i+1;
    }
    if (current == NULL){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return -1;
    }
    Py_XSETREF(current->value, Py_NewRef(value));
    return 0;
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
    int unpack = 0;
    
    if(!PyArg_ParseTuple(args, "O|ii", &value, &req_pos, &unpack)){  
        PyErr_SetString(PyExc_TypeError, "Invalid arguments for insert()");
        return NULL;
    }
    
    if(req_pos < 0){
        PyErr_SetString(PyExc_IndexError, "index is out of range");
        return NULL;
    }
    if (self->value == NULL && self->next == NULL) {
        if (PyObject_TypeCheck(value, &MyListType) && unpack == 1) {
            MyList* src = (MyList*)value;
            MyList* copy = copy_my_list(src);
            if (copy) {
                self->value = copy->value;
                self->next = copy->next;
                Py_TYPE(copy)->tp_free((PyObject*)copy);
            }
        } else if (PyList_Check(value)) {
            MyList* new_list = from_PyList_to_MyList(value);
            if (new_list) {
                self->value = new_list->value;
                self->next = new_list->next;
                Py_TYPE(new_list)->tp_free((PyObject*)new_list);
            }
        } else {
            self->value = Py_NewRef(value);
            self->next = NULL;
        }
        Py_RETURN_NONE;
    }
    if(PyObject_TypeCheck(value, &MyListType)){
        if(unpack == 0){
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
        }
        else {
            MyList* value_as_my_list = (MyList*) value;
            MyList* copy = copy_my_list(value_as_my_list);
            if (!copy) {
                PyErr_NoMemory();
                return NULL;
            }
            
            MyList* last = get_last(copy);
            
            if(req_pos == 0){
                last->next = self;
                PyObject* old_value = self->value;
                self->value = copy->value;
                self->next = copy->next;
                Py_XDECREF(old_value);
                
                copy->value = NULL;
                copy->next = NULL;
                Py_TYPE(copy)->tp_free((PyObject*)copy);
            } else {
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
                last->next = current->next;
                current->next = copy;
            }
        }
    }
    else if(PyList_Check(value)){
        if (unpack == 0) {
            MyList* new_list = from_PyList_to_MyList(value);
            if (!new_list) {
                PyErr_NoMemory();
                return NULL;
            }
            if(req_pos == 0){
                MyList* old_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
                old_node->value = self->value;
                old_node->next = self->next;
                
                self->value = (PyObject*)new_list;
                Py_INCREF(self->value);
                self->next = old_node;
            } else {
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
                new_node->value = (PyObject*)new_list;
                Py_INCREF(new_node->value);
                new_node->next = current->next;
                current->next = new_node;
            }
        } else {
            // Распаковка Python list
            MyList* new_part = from_PyList_to_MyList(value);
            if (!new_part) {
                PyErr_NoMemory();
                return NULL;
            }
            
            MyList* last = get_last(new_part);
            
            if(req_pos == 0){
                // Вставка в начало
                last->next = self;
                
                PyObject* old_value = self->value;
                self->value = new_part->value;
                self->next = new_part->next;
                Py_XDECREF(old_value);
                
                new_part->value = NULL;
                new_part->next = NULL;
                Py_TYPE(new_part)->tp_free((PyObject*)new_part);
            } else {
                // Вставка в середину/конец
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
                
                last->next = current->next;
                current->next = new_part;
            }
        }
    }
    else {
        // Обычное значение
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
    }
    Py_RETURN_NONE;
}

PyObject*
concat(PyObject* o1, PyObject* o2){
    if (!PyObject_TypeCheck(o1, &MyListType) || !PyObject_TypeCheck(o2, &MyListType)) {
        PyErr_BadArgument();
        return NULL;
    }
    MyList *self1 = (MyList*)o1;
    MyList *self2 = (MyList*)o2;
    MyList *result, *current = NULL;

    result = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    result->value = NULL;
    result->next = NULL;

    
    //копируем первый элемент
    current = result;
    MyList* src = self1;
    
    while(src != NULL&&src->value != NULL){
        if(current->value == NULL && current->next == NULL){
            current->value = src->value;
            Py_XINCREF(current->value);
            src = src->next;
        }
        else{
            MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
            new_node->value = src->value;
            Py_XINCREF(new_node->value);
            new_node->next = NULL;

            current->next = new_node;
            current = current->next;

            src = src->next;
        }
    }

    //копируем второй элемент
    src = self2;
    while(src !=NULL&&src->value != NULL){
        if(current->value == NULL && current->next == NULL){
            current->value = src->value;
            Py_XINCREF(current->value);
            src = src->next;
        }
        else{
            MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
            new_node->value = src->value;
            Py_XINCREF(new_node->value);
            new_node->next = NULL;

            current->next = new_node;
            current = current->next;

            src = src->next;
        }
        
    }

    return (PyObject*)result;
}

PyObject*
copy(PyObject* op, PyObject *Py_UNUSED(dummy)){
    MyList* self = (MyList*)op;
    if(self->value == NULL && self->next == NULL){
        MyList* node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
        node->next = NULL;
        node->value = NULL;
        return (PyObject*) node;
    }
    MyList* result = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    result->value = NULL;
    result->next = NULL;

    MyList* current_self = self;
    MyList* current_result = result;
    while(current_self != NULL){
        if (PyObject_TypeCheck(current_self->value, &MyListType)){
            current_result->value = (PyObject*)copy(current_self->value, NULL);
        }
        else{
            current_result->value = Py_NewRef(current_self->value);
        }

        if(current_self->next != NULL){
            MyList* node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
            node->next = NULL;
            node->value = NULL;
            current_result->next= node;
            current_result = current_result->next;
        }
        current_self = current_self->next;
    }
    return (PyObject*) result;
}

int
replace_for_map(PyObject* op, PyObject* key, PyObject* value) {
    MyList* self = (MyList*)op;
    
    // Обработка индекса (замена одного элемента)
    if (PyLong_Check(key)) {
        Py_ssize_t idx = PyLong_AsSsize_t(key);
        if (PyErr_Occurred()) {
            return -1;
        }
        
        MyList* current = self;
        Py_ssize_t i = 0;
        while (i < idx && current != NULL) {
            current = current->next;
            i++;
        }
        
        if (current == NULL) {
            PyErr_SetString(PyExc_IndexError, "index out of range");
            return -1;
        }
        
        
        Py_XSETREF(current->value, Py_NewRef(value));
        return 0;
    }
    
    
    if (PySlice_Check(key)) {
        if (!PyObject_TypeCheck(value, &MyListType)) {
            PyErr_SetString(PyExc_TypeError, "can only assign MyList to slice");
            return -1;
        }
        
        Py_ssize_t length = get_length(self);
        Py_ssize_t start, stop, step, slicelength;
        
        if (PySlice_GetIndicesEx(key, length, &start, &stop, &step, &slicelength) < 0) {
            return -1;
        }
        
        if (step != 1) {
            PyErr_SetString(PyExc_ValueError, "extended slice assignment not supported");
            return -1;
        }
        
        MyList* src = (MyList*)value;
        Py_ssize_t src_len = get_length(src);
        
        if (slicelength != src_len) {
            PyErr_SetString(PyExc_ValueError, "Icorrect size of operands");
            return -1;
        }
        
        MyList* dest = self;
        Py_ssize_t i = 0;
        while (i < start && dest != NULL) {
            dest = dest->next;
            i++;
        }
        
        MyList* src_current = src;
        while (dest != NULL && src_current != NULL && src_current->value != NULL) {
            Py_XSETREF(dest->value, Py_NewRef(src_current->value));
            dest = dest->next;
            src_current = src_current->next;
        }
        
        return 0;
    }
    
    PyErr_SetString(PyExc_TypeError, "indices must be integers or slices");
    return -1;
}

PyObject*
repeat_for_seq(PyObject* op, Py_ssize_t count){
    MyList* self = (MyList*) op;
    if(count <= 0){
        PyErr_SetString(PyExc_ValueError, "mult coef can not be cmaller than 1");
        return NULL;
    }

    MyList* result = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    result->value = NULL;
    result->next = NULL;

    MyList* result_current = result;
    
    for(int i = 0; i < count; i++){
        if(i == 0){
            result_current->value = self->value;
            Py_INCREF(result_current->value);
        }
        else{
            MyList* node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
            node->value = self->value;
            Py_INCREF(node->value);
            result_current->next = node;
            result_current= result_current->next;
        }
    }
    return (PyObject*) result;
}

PyObject* 
reverse(PyObject* op, PyObject *Py_UNUSED(dummy)){
    MyList* self = (MyList*) op;

    
    if(self== NULL || (self->next == NULL && self->value == NULL)){
        PyErr_SetString(PyExc_ValueError, "List is NULL");
        return NULL;
    }
    MyList* current = self;
    MyList* result = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    result->value = NULL;
    result->next = NULL;
    while(current != NULL){
        MyList* node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
        node->value = current->value;
        Py_INCREF(node->value);
        node->next = result->next;
        result->next = node;
        current = current->next;
    }
    MyList* tmp = result;
    result = result->next;
    Py_TYPE(tmp) -> tp_free(tmp);

    return (PyObject*)result;
} 


PyObject*
sort(PyObject* op, PyObject* args){
    MyList* self = (MyList*)op;
    int reverse = 0;
    if(!PyArg_ParseTuple(args, "|i", &reverse)){
        PyErr_SetString(PyExc_TypeError, "Invalid arguments for sort()");
        return NULL;
    }
    if (self == NULL || self->value == NULL) {
        MyList* empty = (MyList*)MyListType.tp_alloc(&MyListType, 0);
        if (!empty) return NULL;
        empty->value = NULL;
        empty->next = NULL;
        return (PyObject*)empty;
    }
    
    MyList* result = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    if (!result) return NULL;
    result->value = Py_NewRef(self->value);
    result->next = NULL;

    MyList* current = self->next;
    
    while(current != NULL && current->value != NULL){
        MyList* node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
        node->value = Py_NewRef(current->value);
        node->next = NULL;
        
        MyList* current_result = result;
        MyList* prev = NULL;
        int inserted = 0;
        while(current_result != NULL){
            int cmp;
            if (reverse == 0) {
                cmp = PyObject_RichCompareBool(current_result->value, node->value, Py_GT);
            } else {
                cmp = PyObject_RichCompareBool(current_result->value, node->value, Py_LT);
            }
            
            if (cmp == -1) {
                Py_DECREF(node);
                Py_DECREF(result);
                return NULL;
            }
            
            if (cmp) {
                if (prev == NULL) {
                    node->next = result;
                    result = node;
                } else {
                    node->next = current_result;
                    prev->next = node;
                }
                inserted = 1;
                break;
            }
            
            prev = current_result;
            current_result = current_result->next;
        }
        if (!inserted) {
            if (prev != NULL) {
                prev->next = node;
            } else {
                result = node;
            }
        }
        current = current->next;
    }
    
    return (PyObject*)result;
}