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

#include "app.h"

/* ----------------------------------------------------------------------------*/
static int recv_value_from_driver(void);

/* ----------------------------------------------------------------------------*/
 
static int recv_value_from_driver(void) {
	int fd;
    int power;
    int * retval;

	fd = open("/dev/pwrloss_device", O_RDWR);
    if(fd < 0) {
        perror("Failed to open device!\n");
        return errno;
    }
	
    if(retval = ioctl(fd, PWRLOSS_READ, (int32_t *) &power) < 0) {
        printf("ioctl failed and errno is: %s\n", strerror(retval));
        return -1;
    }
    
    return retval;
}


static PyObject * recv_value_wrapper(PyObject * self, PyObject * args) {
    int * result;
    PyObject * pyVal;
    PyObject * ret;

    result = recv_value_from_driver(void);
    pyVal = Py_BuildValue("i", result);

    if(PyObject_IsTrue(pyVal) < 0) {
        printf("Py_BuildValue returned an error. Could not determine result.\n")};
        return -1;
    }
    else {
        ret = pyVal;
        return ret;
    }
};

static PyMethodDef PwrLossMethods[] = {
    { "pwrloss", recv_value_wrapper, METH_VARARGS, "Get pin value from the driver"},
    {NULL, NULL, 0, NULL}
};

DL_EXPORT(void) initpwrloss(void) {
    Py_InitModule("pwrloss", PwrLossMethods);
}

