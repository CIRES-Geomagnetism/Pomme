#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "model.h"


static PyObject* compute(PyObject* self, PyObject* args){
	
	double date, lat, lon, elv, est, ist, imf_by, f107, em;

	if (!PyArg_ParseTuple(args, "ddddddddd", &date, &lat, &lon, &elv, &est, &ist, &imf_by, &f107, &em)) {
		return NULL;
	}

	pommecalc(date, lat, lon, elv, est, ist, imf_by, f107, em);

    Py_RETURN_NONE; 
}

static PyMethodDef pom_methods[] = {
    {"compute", compute, METH_VARARGS, "Pomme Model"},
    {NULL, NULL, 0, NULL}  // Sentinel
};

static struct PyModuleDef pomme = {
    PyModuleDef_HEAD_INIT,
    "pomme",  // Module name
    NULL,        // Module documentation
    -1,          // Size of per-interpreter state
    pom_methods
};

PyMODINIT_FUNC PyInit_pomme(void) {
    return PyModule_Create(&pomme);
}


 
