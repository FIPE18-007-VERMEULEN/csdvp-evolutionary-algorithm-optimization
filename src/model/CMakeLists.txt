INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/model)
include_directories(${PARADISEO_INCLUDE_DIR})

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

SET (EXERCICE_SOURCES
  magnitude.cpp
  competency.cpp
  course.cpp
  profession.cpp
)

ADD_LIBRARY(lModel STATIC ${EXERCICE_SOURCES})

add_subdirectory(exception)
add_subdirectory(scale)