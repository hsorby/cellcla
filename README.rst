
Cell Commmand Line Application
==============================

A C++ application for performing tasks on CellML models.  The application will
process a CellML model file passed as an argument.  There are options to validate
the model and or generate C or Python code.

Usage
=====

::

  Usage: ./src/cellcla [-pvu][--generate-c,--generate-python] file
      Options:
        -u Print out this usage information and exit.
        -p Parse and print out any errors.
        -v Validate and print out any errors.
        --generate-c Generate C code [--gc].
        --generate-python Generate Python code [--gp].

Building
========

The application requires the libCellML library, libXml2 library, libXslt library, and the
libb64 library.

With these libraries available the application can be built with the following commands on
OS X or Linux systems using CMake::

  mkdir build-cellcla
  git clone https://github.com/hsorby/cellcla
  cd build-cellcla
  cmake ../cellcla
  make

For the Windows operating system changes to these commands will need to be made.
