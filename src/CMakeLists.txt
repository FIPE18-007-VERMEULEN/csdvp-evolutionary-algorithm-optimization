INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)
include_directories(${PARADISEO_INCLUDE_DIR})

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

SET (EXERCICE_SOURCES
  queenEval.cpp
)

ADD_LIBRARY(lQueen STATIC ${EXERCICE_SOURCES})


add_subdirectory(model)