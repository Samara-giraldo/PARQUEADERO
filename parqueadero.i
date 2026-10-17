%module parqueadero

%{
#include "Parqueadero.h"
%};

%include "std_string.i"

// Incluimos la cabecera original para que SWIG la lea
%include "Parqueadero.h"
