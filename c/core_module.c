#include <Python.h>

#include <core_module.h>
#include <list.h>

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
    if(value){
        if(PyList_Check(value)){
            int len = PyList_GET_SIZE(value);
            if (len == 0){
                self->value = NULL;
                self->next = NULL;
                return 0;
            }
            MyList* current = self;
            for(int i = 0; i < len; i++){
                if(i == 0){
                    current->value = PyList_GetItem(value, i);
                    Py_INCREF(current->value);
                    current->next = NULL;
                }
                else{
                    MyList* new_element = (MyList*)MyListType.tp_alloc(&MyListType, 0);
                    new_element->value = Py_NewRef(PyList_GetItem(value, i));
                    new_element->next = NULL;

                    current->next = new_element;
                    current = new_element;
                }
            }
        }
        else{
            if (value){
            Py_XSETREF(self->value, Py_NewRef(value));
            self->next = NULL;
            }
            else{
                self->value = NULL;
                self->next = NULL;
            }
        }
    }
    else{
        self->value = NULL;
        self->next = NULL;
    }


    return 0;
}