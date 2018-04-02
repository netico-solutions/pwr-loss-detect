#!/usr/bin/env python

# Setup script for building the PowerLoss wrapper module
#

import os
from distutils.core import setup, Extension


os.environ["CC"] = "arm-linux-gnueabihf-gcc"                        

pwrloss_mod = Extension("pwrloss", 
                        sources=['app.c'], 
                        include_dirs=["/usr/local/include/"],
                        library_dirs=["/home/netico/yocto_varsomam33/tisdk/build/arago-tmp-external-linaro-toolchain/sysroots/varsomam33/lib"])

setup(name = "pwrloss", ext_modules=[pwrloss_mod])
