
cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME CellCLA)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

# Produces a target 'cellml'.
find_package(libCellML REQUIRED)

# Want the target 'b64' from here.
find_package(Base64 REQUIRED)

# Find libxml2
set(HAVE_LIBXML2_CONFIG FALSE)
if(MSVC)
  # If we want to use config packages on Windows with Visual Studio,
  # we need to have two find_package calls and explicitly state that
  # we wish to use Config mode in the first call.  Finding LibXml2 in config mode
  # is the preferred method so we will try this first quietly.
  #
  # This does change how we get information about include paths and such so we
  # need to track how we found LibXml2.
  find_package(LibXml2 CONFIG QUIET)
  if(LibXml2_FOUND)
    set(HAVE_LIBXML2_CONFIG TRUE)
    foreach(_XML2_VAR LIBXML2_LIBRARY LIBXML2_INCLUDE_DIR LIBXML2_XMLLINT_EXECUTABLE)
      if(DEFINED ${_XML2_VAR} AND NOT ${${_XML2_VAR}})
        unset(${_XML2_VAR} CACHE)
      endif()
    endforeach()
  else()
    find_package(LibXml2 REQUIRED)
    if(LibXml2_FOUND)
      unset(LibXml2_DIR CACHE)
    endif()
  endif()
else()
  find_package(LibXml2 REQUIRED)
endif()

find_package(LibXslt REQUIRED)

add_subdirectory(src)

