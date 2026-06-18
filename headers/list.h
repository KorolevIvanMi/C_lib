// the structure on which the class is based
typedef struct MyList MyList;

struct MyList {
    PyObject_HEAD
    PyObject* value;  // value
    MyList* next;  // pointer on next element
};

extern PyTypeObject MyListType;