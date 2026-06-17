#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//my modules 
#include <list.h>
#include <core_module.h>
#include <read_module.h>
#include <update_module.h>
#include <delete_module.h>


// description of class fields
static PyMemberDef MyList_members[] = {
    {"value", Py_T_OBJECT_EX, offsetof(MyList, value), 0, "value of the list"},
    {NULL, 0, 0, 0, NULL}
};


// method of adding to the end of a list
static PyObject*
append(PyObject* op, PyObject* args);

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



static PyObject*
append(PyObject* op, PyObject* args){
    MyList* self = (MyList*) op;
    PyObject* value;
    if(!PyArg_ParseTuple(args, "O", &value)){
    PyErr_SetString(PyExc_TypeError, "This arguments are not suppose to bu used with this function! Or maybe you didn't send any arguments");
    return NULL;
    }
    
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

