#include <Python.h>
#include "utils.h"

static PyObject* pomme_module(PyObject* self, PyObject* args){
	
	double date, lat, lon, elv, est, ist, imf_by, f107, em;

	if (!PyArg_ParseTuple(args, "fffffffff", &date, &lat, &lon, &elv, &est, &ist, &imf_by, &f107, &em)) {
		return NULL;
	}

	pommecalc(date, lat, lon, elv, est, ist, imf_by, f107, em);
}



 
