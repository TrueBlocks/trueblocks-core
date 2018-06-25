#include <Python.h>
#include "etherlib.h"

static PyObject* init(PyObject* self, PyObject *args) {
    const char *baseURL;
    PyArg_ParseTuple(args, "s", &baseURL);
    getCurlContext()->baseURL = baseURL;
    etherlib_init();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* cleanup(PyObject* self, PyObject *args) {
    etherlib_cleanup();
    Py_INCREF(Py_None);
    return Py_None;
}

// my use case is not live so assuming all finalized
static PyObject* get_block(PyObject* self, PyObject *args) {
    int i;
    int cache;
    if (!PyArg_ParseTuple(args, "ip", &i, &cache)) {
        return NULL;
    }
    CBlock block;
    getBlock(block, i);
    block.finalized = true;
    if (cache) {
	writeBlockToBinary(block, getBinaryFilename(i));
    }
    return Py_BuildValue("s", block.Format().c_str());
}

static PyMethodDef _quickblocks_methods[] = {
    {"get_block", get_block, METH_VARARGS, "get_block"},
    {"init", init, METH_VARARGS, "init"},
    {"cleanup", cleanup, METH_VARARGS, "cleanup"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cModPyDem =
{
    PyModuleDef_HEAD_INIT,
    "_quickblocks",
    "",
    -1,
    _quickblocks_methods
};

PyMODINIT_FUNC PyInit__quickblocks(void) {
    return PyModule_Create(&cModPyDem);
}
