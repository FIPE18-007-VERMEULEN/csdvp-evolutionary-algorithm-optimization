INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/constraints)
include_directories(${PARADISEO_INCLUDE_DIR})

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

SET (EXERCICE_SOURCES
  ectsConstraints.cpp
  repetitionConstraints.cpp
  professionConstraints.cpp
  prerequisitesConstraints.cpp
)

ADD_LIBRARY(lCstr STATIC ${EXERCICE_SOURCES})