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
	PyObject* dict = PyDict_New();
	pomme_results results;

	if (!PyArg_ParseTuple(args, "ddddddddd", &date, &lat, &lon, &elv, &est, &ist, &imf_by, &f107, &em)) {
		return NULL;
	}

	results = pommecalc(date, lat, lon, elv, est, ist, imf_by, f107, em);

	PyDict_SetItemString(dict, "Bx", PyFloat_FromDouble(results.x));
	PyDict_SetItemString(dict, "By", PyFloat_FromDouble(results.y));
	PyDict_SetItemString(dict, "Bz", PyFloat_FromDouble(results.z));
	PyDict_SetItemString(dict, "Bh", PyFloat_FromDouble(results.h));
	PyDict_SetItemString(dict, "Bf", PyFloat_FromDouble(results.f));
	PyDict_SetItemString(dict, "Bdec", PyFloat_FromDouble(results.dec));
	PyDict_SetItemString(dict, "Binc", PyFloat_FromDouble(results.inc));


    return dict;
}

static PyMethodDef pom_methods[] = {
    {"compute", compute, METH_VARARGS, "Pomme Model"},
    {NULL, NULL, 0, NULL}  // Sentinel
};

static struct PyModuleDef cpomme = {
    PyModuleDef_HEAD_INIT,
    "pomme",  // Module name
    NULL,        // Module documentation
    -1,          // Size of per-interpreter state
    pom_methods
};

PyMODINIT_FUNC PyInit_cpomme(void) {
    return PyModule_Create(&cpomme);
}


 
