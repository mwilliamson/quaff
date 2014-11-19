#!/usr/bin/env python

import os
import subprocess
import tarfile
import shutil

from setuptools import setup
from distutils.extension import Extension
from distutils.command.build_ext import build_ext


def path_in_dir(relative_path):
    return os.path.join(os.path.dirname(__file__), relative_path)

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

quaff_lib_dir = path_in_dir(".")

quaff_extension = Extension(
    "quaff_py",
    sources=["quaff_py.c"],
    include_dirs=[quaff_lib_dir],
    extra_objects=[os.path.join(quaff_lib_dir, "libquaff.a")],
)

setup(
    name='quaff',
    version='0.0.1',
    description='TODO',
    author='Michael Williamson',
    url='',
    license='BSD 2-Clause',
    ext_modules = [quaff_extension],
    cmdclass={"build_ext": build_ext},
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.6',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.2',
        'Programming Language :: Python :: 3.3',
        'Programming Language :: Python :: 3.4',
    ],
)
