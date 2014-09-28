/* -*- c++ -*- */

#define FTXXXREAD_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "FTxxxRead_swig_doc.i"

%{
#include "FTxxxRead/ftdi_read_source.h"
%}


%include "FTxxxRead/ftdi_read_source.h"
GR_SWIG_BLOCK_MAGIC2(FTxxxRead, ftdi_read_source);
