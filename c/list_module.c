#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// the structure on which the class is based
typedef struct MyList MyList;

struct MyList {
    PyObject_HEAD
    PyObject* value;  // value
    MyList* next;  // pointer on next element
};


// description of class fields
static PyMemberDef MyList_members[] = {
    {"value", Py_T_OBJECT_EX, offsetof(MyList, value), 0, "value of the list"},
    {NULL, 0, 0, 0, NULL}
};


// function which clear memory
static void 
Custom_dealloc(PyObject* op);

// pre-initialization function
// basic field initialization occurs
static PyObject*
Custom_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

// initialization function
// currently requires mandatory transfer of a value
// when initializing a class object
static int
Custom_init(PyObject* op, PyObject* args, PyObject* kwds);

// method for outputting data from a list
static PyObject*
show(PyObject *op, PyObject *Py_UNUSED(dummy));

// method of adding to the end of a list
static PyObject*
append(PyObject* op, PyObject* args);

// method of taking an element by index
static PyObject*
get(PyObject* op, PyObject* args);

// method of deleting last element
static PyObject*
pop(PyObject* op, PyObject* Py_UNUSED(dummy));

// method of update value on index
static PyObject*
updateAt(PyObject* op, PyObject* args);

// description of added methods
static PyMethodDef MyList_methods[] = {
    {"show", show, METH_NOARGS, "show list"},
    {"append", append,  METH_VARARGS, "add element to the end of list"},
    {"get", get, METH_VARARGS, "return element on position x"},
    {"pop", pop, METH_NOARGS, "delete last element"},
    {"updateAt", updateAt, METH_VARARGS, "update value on index"},
    {NULL}
};

// data type descriptions for MyList with the purpose of various functions
static PyTypeObject MyListType = {
.ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "list.List",
    .tp_doc = PyDoc_STR("My list"),
    .tp_basicsize = sizeof(MyList),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Custom_new,
    .tp_init = Custom_init,
    .tp_dealloc = Custom_dealloc,
    .tp_members = MyList_members,
    .tp_methods = MyList_methods,
};

// module init
static int
mylist_exec(PyObject* m){
    if (PyType_Ready(&MyListType) < 0) {
        return -1;
    }

    if (PyModule_AddObjectRef(m, "List", (PyObject *) &MyListType) < 0) {
        return -1;
    }

    return 0;
}

// slot declaration
// the first one starts module initialization
// the second one indicates that the module cannot work with multiple
// interpreters simultaneously
static PyModuleDef_Slot mylist_slots[] = {
    {Py_mod_exec, mylist_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL}
};

// module description
static PyModuleDef mylist_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "mylist",
    .m_doc = "Example module that creates an list",
    .m_size = 0,
    .m_slots = mylist_slots,
};

// entry point
PyMODINIT_FUNC
PyInit_mylist(void){
    return PyModuleDef_Init(&mylist_module);
}

// implementation of functions
static void 
Custom_dealloc(PyObject* op){
    MyList* self = (MyList*) op;
    while (self != NULL){
        MyList* temp = self;
        self = self->next;
        Py_XDECREF(temp->value);
        Py_TYPE(temp) -> tp_free(temp);
    }
}


static PyObject*
Custom_new(PyTypeObject* type, PyObject* args, PyObject* kwds){
    MyList* self;
    self = (MyList* ) type->tp_alloc(type, 0);
    self->next = NULL;
    if(self != NULL){
        self->value = Py_GetConstant(Py_CONSTANT_ZERO);
        if(self->value == NULL){
            Py_DECREF(self);
            return NULL;
        }
    }
    return (PyObject*) self;
}  


static int
Custom_init(PyObject* op, PyObject* args, PyObject* kwds){
    MyList* self  = (MyList*) op;
    static char *kwlist[] = {"value", NULL};
    PyObject* value;
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &value)) return -1;
    if (value){
        Py_XSETREF(self->value, Py_NewRef(value));
    }

    return 0;
}


static PyObject*
append(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    PyObject* value;
    if(!PyArg_ParseTuple(args, "O", &value)) return NULL;

    
    if (self->value == NULL && self->next == NULL) {
        Py_XSETREF(self->value, Py_NewRef(value));
        Py_RETURN_NONE;
    }

    MyList* new_element = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    new_element->next = NULL;
    if (value){
        Py_XSETREF(new_element->value, Py_NewRef(value));
    }
    MyList* current = self;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = new_element;
    Py_RETURN_NONE;
}


static PyObject*
show(PyObject *op, PyObject *Py_UNUSED(dummy)){
    if(op == NULL){
        return NULL;
    }
    MyList* self = (MyList* ) op;
    MyList* current = self;
    while(current != NULL){
        if (current->value != NULL){
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
        }else{
            printf("NULL");
        }
        current = current->next;
        
    }
    printf("\n");
    fflush(stdout);
    Py_RETURN_NONE;
}


static PyObject*
get(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    int req_pos = 0;
    if(!PyArg_ParseTuple(args, "i", &req_pos)) return NULL;

    int i = 0;
    if(self == NULL) return NULL;
    MyList* current = self;
    while( i != req_pos){
        if(current == NULL){
            return NULL;
        }
        else{
            current = current->next;
        }
        i = i+1;
    }
    if(current == NULL){
        return NULL;
    }
    return current->value;
}

static PyObject*
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

static PyObject*
updateAt(PyObject* op, PyObject* args){

    if (op == NULL){
        return NULL;
    }
    
    MyList* self = (MyList*) op;
    PyObject* value;
    int pos = 0;
    if(!PyArg_ParseTuple(args, "Oi", &value, &pos)) return NULL;
    if(pos < 0){
        return NULL;
    }

    int i = 0;
    MyList* current = self;
    while( i != pos){
        if(current == NULL){
            return NULL;
        }
        else{
            current = current->next;
        }
        i = i+1;
    }
    if (current == NULL){
        return NULL;
    }
    Py_XSETREF(current->value, Py_NewRef(value));
    Py_RETURN_NONE;

}