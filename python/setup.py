#!/usr/bin/python3

from setuptools import Extension, setup
from Cython.Build import cythonize

# read the contents of your README file
from pathlib import Path
this_directory = Path(__file__).parent
long_description = (this_directory / "README.rst").read_text()

setup(name='lrcalc',
    version='2.1',
    description='Littlewood-Richardson Calculator bindings',
    long_description=long_description,
    long_description_type='text/x-rst',
    url='https://math.rutgers.edu/~asbuch/lrcalc',
    author='Anders Skovsted Buch',
    license='GPL3',
    python_requires='>=3',
    ext_modules = cythonize([
        Extension("lrcalc", ["lrcalc.pyx"],
                  libraries=["lrcalc"]),
    ], language_level="3"),
)
