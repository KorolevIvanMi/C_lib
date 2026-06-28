#include <Python.h>

#include <core_module.h>
#include <list.h>

static PyObject*
create_mylist_from_object(PyObject* item) {
    if (!PyList_Check(item)) {
        return Py_NewRef(item);
    }
    
    MyList* sub_list = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    PyObject* item_tuple = PyTuple_Pack(1, item);
    MyListType.tp_init((PyObject*)sub_list, item_tuple, NULL);
    Py_DECREF(item_tuple);
    
    return (PyObject*)sub_list;
}

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
    MyList* self = (MyList*) op;
    static char *kwlist[] = {"value", NULL};
    PyObject* value = NULL;
    
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &value)) 
        return -1;
    
    if(!value){
        self->value = NULL;
        self->next = NULL;
        return 0;
    }
    
    if(PyList_Check(value)){
        Py_ssize_t len = PyList_GET_SIZE(value);
        if (len == 0){
            self->value = NULL;
            self->next = NULL;
            return 0;
        }
        
        MyList* current = self;
        for(Py_ssize_t i = 0; i < len; i++){
            PyObject* item = PyList_GetItem(value, i);
            
            if (i == 0) {
                current->value = create_mylist_from_object(item);
                current->next = NULL;
            } else {
                MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
                new_node->value = create_mylist_from_object(item);
                new_node->next = NULL;
                current->next = new_node;
                current = new_node;
            }
        }
    }
    else if (PyObject_TypeCheck(value, &MyListType)){
        MyList* src = (MyList*) value;
        MyList* current = self;
        int first = 1;
        
        while(src != NULL){
            if (first) {
                self->value = Py_NewRef(src->value);
                self->next = NULL;
                first = 0;
            } else {
                MyList* new_node = (MyList*)MyListType.tp_alloc(&MyListType, 0);
                new_node->value = Py_NewRef(src->value);
                new_node->next = NULL;
                current->next = new_node;
                current = new_node;
            }
            src = src->next;
        }
    }
    else{
        self->value = Py_NewRef(value);
        self->next = NULL;
    }

    return 0;
}