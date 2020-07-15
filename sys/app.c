/*
 *  
 * Power loss detection python wrapper module 
 *
 * Author: Nemanja Stefanovic <nemanja.stef92@gmail.com>
 *
 * Licensed under GPL-v2 or later
 * ----------------------------------------------------------------------------
 *
 * Description:
 *
 * This program acquires powerloss status from the lkm and makes it usable by 
 * python libraries. Should be imported as python module into application.
 *
 * ----------------------------------------------------------------------------
 */
#include <Python.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include "app.h"

/* ----------------------------------------------------------------------------*/
static int recv_value_from_driver(void);

/* ----------------------------------------------------------------------------*/
 
static int recv_value_from_driver(void) {
	int fd;
    int power;
    int retval;

    // fd is file descriptor
	fd = open("/dev/pwrloss_device", O_RDWR);
    if(fd < 0) {
        perror("Failed to open device!\n");
        return errno;
    }
	
    if((retval = ioctl(fd, PWRLOSS_READ, (int32_t *) &power) < 0)) {
        printf("ioctl failed and errno is: %s\n", strerror(retval));
    }
    
    // TODO Maybe return power here because retval contains 0 or ERRORCODE?
    
    return retval;
}

static int dummy_number(void){

    srand(time(0));
    int tmp = (int) ((rand() % 
           (100 - 0 + 1)) + 0);
    return tmp;
};

static PyObject * show_dummy_number(PyObject * self, PyObject * args){
    int result = dummy_number();
    PyObject * PyVal;
    PyVal = Py_BuildValue("i", result);
    return PyVal;
};

static PyObject * recv_value_wrapper(PyObject * self, PyObject * args) {
    int result;
    PyObject * pyVal;
    PyObject * ret;

    result = recv_value_from_driver();
    pyVal = Py_BuildValue("i", result);

    if(PyObject_IsTrue(pyVal) < 0) {
        printf("Py_BuildValue returned an error. Could not determine result.\n");
    }
    ret = pyVal;
    
    return ret;
};

static PyMethodDef PwrLossMethods[] = {
    // "pwrloss" here is function name
    { "pwrloss", recv_value_wrapper, METH_VARARGS, "Get pin value from the driver"},
    { "dummy_number", show_dummy_number, METH_VARARGS, "Just show some number"},
    {NULL, NULL, 0, NULL}
};

DL_EXPORT(void) initpwrloss(void) {
    // "pwrloss" here is python package name
    Py_InitModule("pwrloss", PwrLossMethods);
}

