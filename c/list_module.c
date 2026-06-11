#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// структура на которой основан класс
typedef struct MyList MyList;

struct MyList {
    PyObject_HEAD
    PyObject* value;  // значение
    MyList* next;  // указатель на следующий элемент
};


// описание полей класса
static PyMemberDef MyList_members[] = {
    {"value", Py_T_OBJECT_EX, offsetof(MyList, value), 0, "value of the list"},
    {NULL, 0, 0, 0, NULL}
};


// функция очистки памяти
static void 
Custom_dealloc(PyObject* op);

// функция предынициализации. 
// Тут происходит базовая инициализациия полей
static PyObject*
Custom_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

// полноценная функция инициализации
// пока требует обязательной передачи значения при инициализации объекта класса
static int
Custom_init(PyObject* op, PyObject* args, PyObject* kwds);

// метод вывода данных из списка
static PyObject*
show(PyObject *op, PyObject *Py_UNUSED(dummy));

// метод добавления в конец списка
static PyObject*
append(PyObject* op, PyObject* args);

// метод взятия элемента по индексу
static PyObject*
get(PyObject* op, PyObject* args);

// описание добавленных методов
static PyMethodDef MyList_methods[] = {
    {"show", show, METH_NOARGS, "show list"},
    {"append", append,  METH_VARARGS, "add element to the end of list"},
    {NULL}
};

// описания типа данные для MyList с назначением различных функций
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

// инициализатор модуля
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

// объявление слотов
// первый запускает инициализацию модуля 
// второй говорит о том, что модуль не может работать одовременно с несколькими
// интерпритаторами
static PyModuleDef_Slot mylist_slots[] = {
    {Py_mod_exec, mylist_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL}
};

// описание модуля
static PyModuleDef mylist_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "mylist",
    .m_doc = "Example module that creates an list",
    .m_size = 0,
    .m_slots = mylist_slots,
};

// точка входа
PyMODINIT_FUNC
PyInit_mylist(void){
    return PyModuleDef_Init(&mylist_module);
}

// реализация функций
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

    MyList* new_element = (MyList*)MyListType.tp_alloc(&MyListType, 0);
    new_element->next = NULL;
    if (value){
        Py_XSETREF(new_element->value, Py_NewRef(value));
    }

    MyList* current = self;

    if(current == NULL){
        current = new_element;
    }
    else{
        
        while(current-> next != NULL){
            current = current->next;
        }
        current->next = new_element;
    }

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
        printf(", ");
    }
    fflush(stdout);
    Py_RETURN_NONE;
}


static PyObject*
get(PyObject* op, PyObject* args);