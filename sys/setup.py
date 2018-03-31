#!/usr/bin/env python

# Setup script for building the PowerLoss wrapper module
#


from distutils.core import setup, Extension

pwrloss_mod = Extension("pwrloss", ['app.c'])

setup(name = "pwrloss", ext_modules=[pwrloss_mod])
