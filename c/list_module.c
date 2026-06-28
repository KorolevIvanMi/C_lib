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
    {"value", Py_T_OBJECT_EX, offsetof(MyList, value), 0, "value of the list, can be any type"},
    {NULL, 0, 0, 0, NULL}
};

// description of added methods
static PyMethodDef MyList_methods[] = {
    {"show", show_line, METH_NOARGS, "List.show(): \nshow all list elements"},
    {"append", append,  METH_VARARGS, "List.append(x): \n x - AnyType value \n Add element to the end of the List"},
    {"prepend", prepend, METH_VARARGS, "List.prepend(x): \n x - AnyType value \n Add element to the begginig of the List"},
    {"get", get, METH_VARARGS, "List.get(position):\n position - int value. Position of the element\n Return value on the position"},
    {"getSec", getSection, METH_VARARGS, "List.getSec(x,y):\n x, y - left and right indexes\n Return part of the List, where index are in [x,y)"},
    {"pop", pop, METH_NOARGS, "List.pop(): \n Delete last element of the list"},
    {"popFront", popFront, METH_NOARGS, "List.popFront(): \n Delete first element of the list"},
    {"popAt", popAt, METH_VARARGS, "List.popAt(position)\n position - int value. Position of the element \n Delete element on the position"},
    {"popAfter", popAfter, METH_VARARGS, "List.popAfter(position)\n position - int value. Position of the element \n Delete element after the position"},
    {"popBefore", popBefore, METH_VARARGS, "List.popBefore(position)\n position - int value. Position of the element \n Delete element before the position"},
    {"popSec", popSection, METH_VARARGS, "List.popSec(x,y):\n x, y - left and right indexes\n Delete part of the List, where index are in [x,y]"},
    {"clear", clear, METH_NOARGS, "List.clear() \n Delete all elements in the list"},
    {"updateAt", updateAt, METH_VARARGS, "List.updateAt(value, position)\n value - AnyType value \n position - int value. Position of the element \n Update value of the element on the position \n WARNING: Intended for replacing single values only"},
    {"insert", insert, METH_VARARGS, "List.insert(x, position, flag)\n x - AnyType value \n position - int value \n flag - bool flag. If True - List is expanded into elements, if False - creates a nested list \n Insert value on the position"},
    {"length", length, METH_NOARGS, "List.length(): \n return amout of elements in the list"},
    {"max", max,  METH_NOARGS, "List.max(): \n return the max element of the list"},
    {"min", min,  METH_NOARGS, "List.min(): \n return the min element of the list"},
    {"contains", contains,  METH_VARARGS, "List.contains(x):\n x - AnyType value \n return True if List contains value, False otherwise"},
    {"equal", equal,  METH_VARARGS, "List.equal(x):\n x - other List \n return True if all elements of one List == all elements of other List, False otherwise"},
    {"find", find,  METH_VARARGS, "List.index(x):\n x - AnyType value \n return position of x if List contains x, -1 otherwise"},
    {"countIfEq", countIfEq, METH_VARARGS, "List.countIfEq(x): \n x - AnyType value \n Returns the number of occurrences of an element in List"},
    {"findBetw", findBetw, METH_VARARGS, "List.findBetw(x,y): \n x - min value \n y - max value \n Returns all elements which are in [x,y]. Works correctly only when x,y are the same type as value in List"},
    {"copy", copy, METH_NOARGS, "List.copy(): \n Return copy of the list"},
    {NULL}
};

static PySequenceMethods MyList_as_sequence = {
    .sq_length = length_for_seq,
    .sq_item = get_for_seq,
    .sq_ass_item = updateAt_for_seq,
    .sq_concat = concat,
};

// data type descriptions for MyList with the purpose of various functions
PyTypeObject MyListType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "list.List",
    .tp_doc = PyDoc_STR("My list\n\n"
                    "List() -> new empty list\n"
                    "List(iterable) -> new list initialized from iterable's items\n"
                    "List(value) -> new list with single element"),
    .tp_basicsize = sizeof(MyList),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = Custom_new,
    .tp_init = Custom_init,
    .tp_dealloc = Custom_dealloc,
    .tp_members = MyList_members,
    .tp_methods = MyList_methods,
    .tp_as_sequence = &MyList_as_sequence,
    .tp_repr = repr_line,

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

