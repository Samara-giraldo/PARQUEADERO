from setuptools import setup, Extension
import os

os.environ["CC"] = "g++"
os.environ["CXX"] = "g++"

parqueadero_extension = Extension(
    '_parqueadero',
    sources=['parqueadero_wrap.cxx'],  # <-- Solo dejamos el wrapper generado por SWIG
    include_dirs=['.'],
    extra_compile_args=['-std=c++11'],
)

setup(
    name='parqueadero',
    version='1.0',
    ext_modules=[parqueadero_extension],
    py_modules=['parqueadero'],
)