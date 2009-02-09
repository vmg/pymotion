 /*
 *  PyMotion - Python bindings for the UniMotion library
 *
 *  Copyright (c) 2009 Vicent Marti. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation Inc. 59 Temple Place, Suite 330, Boston MA 02111-1307 USA
 */

#include <Python.h>
#include <string.h>
#include <stdint.h>

#include "unimotion.h"

static int g_accelerometerType = unknown;

static PyObject*
pymotion_read_scaled(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	int x, y, z;
    
	if (!read_sms_scaled(g_accelerometerType, &x, &y, &z))
	{
		PyErr_SetString(PyExc_RuntimeError, "Failed to read accelerometer data.");
		return NULL;
	}

	return Py_BuildValue("(iii)", x, y, z); 
}

static PyObject*
pymotion_read_raw(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	int x, y, z;
    
	if (!read_sms_raw(g_accelerometerType, &x, &y, &z))
	{
		PyErr_SetString(PyExc_RuntimeError, "Failed to read accelerometer data.");
		return NULL;
	}

	return Py_BuildValue("(iii)", x, y, z); 
}

static PyObject*
pymotion_read_real(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	double x, y, z;
    
	if (!read_sms_real(g_accelerometerType, &x, &y, &z))
	{
		PyErr_SetString(PyExc_RuntimeError, "Failed to read accelerometer data.");
		return NULL;
	}

	return Py_BuildValue("(ddd)", x, y, z); 
}

static PyObject*
pymotion_read_bytes(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
   
	unsigned char *raw_bytes = 0;
	int bytes_len;

	if ((raw_bytes = read_sms_raw_bytes(g_accelerometerType, &bytes_len)) == 0)
	{
		PyErr_SetString(PyExc_RuntimeError, "Failed to read accelerometer data.");
		return NULL;
	}

	PyObject *array = PyByteArray_FromStringAndSize((char *)raw_bytes, bytes_len);
	free(raw_bytes);
	return array;
}

static PyObject*
pymotion_device_id(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	PyObject *device_id = NULL;
	switch (g_accelerometerType)
	{
		case powerbook:
			device_id = PyString_FromString("PowerBook");
			break;

		case ibook:
			device_id = PyString_FromString("iBook");
			break;

		case highrespb:
			device_id = PyString_FromString("HighResPB");
			break;

		case macbookpro:
			device_id = PyString_FromString("MacBookPro");
			break;
	}

	return device_id;
}

static PyMethodDef pymotionMethods[] = 
{
	{"scaled_motion", pymotion_read_scaled, METH_VARARGS, 
		"Reads scaled values from the accelerometer."},

	{"real_motion", pymotion_read_real, METH_VARARGS, 
		"Reads real (calibrated) values from the accelerometer."},

	{"raw_bytes", pymotion_read_bytes, METH_VARARGS, 
		"Reads raw data from the accelerometer as a byte string."},

	{"raw_motion", pymotion_read_raw, METH_VARARGS, 
		"Reads raw motion data from the accelerometer (no calibration/scaling)."},

	{"device_id", pymotion_device_id, METH_VARARGS, 
		"Returns a string containing the identified Apple laptop model."},

	{NULL, NULL}
};

PyMODINIT_FUNC
initpymotion(void)
{
	g_accelerometerType = detect_sms();

	if (g_accelerometerType == unknown)
	{
		PyErr_SetString(PyExc_ImportError, 
			"Failed to initialize the UniMotion library: No known motion sensor devices detected");
		return;
	}

	Py_InitModule("pymotion", pymotionMethods);
}

