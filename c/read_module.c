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
    if(self == NULL) {
        PyErr_SetString(PyExc_ValueError, "List is NULL");
        return NULL;
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