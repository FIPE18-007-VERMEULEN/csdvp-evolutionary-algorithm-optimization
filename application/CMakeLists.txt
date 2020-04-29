# 1) Include the sources

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

# 2) Define targets: executable and dependencies

ADD_EXECUTABLE(solveQueen main.cpp)
ADD_EXECUTABLE(tryInit tryInit.cpp)
ADD_EXECUTABLE(tryMutation tryMutation.cpp)
ADD_EXECUTABLE(tryCrossover tryCrossover.cpp)
ADD_EXECUTABLE(tryEval tryEval.cpp)

ADD_DEPENDENCIES(solveQueen lQueen)
ADD_DEPENDENCIES(tryInit lQueen)
ADD_DEPENDENCIES(tryMutation lQueen)
ADD_DEPENDENCIES(tryCrossover lQueen)
ADD_DEPENDENCIES(tryEval lQueen)

# 3) Link the librairies for your executable

TARGET_LINK_LIBRARIES(solveQueen ${PARADISEO_LIBRARIES} lQueen)
TARGET_LINK_LIBRARIES(tryInit ${PARADISEO_LIBRARIES} lQueen)
TARGET_LINK_LIBRARIES(tryMutation ${PARADISEO_LIBRARIES} lQueen)
TARGET_LINK_LIBRARIES(tryCrossover ${PARADISEO_LIBRARIES} lQueen)
TARGET_LINK_LIBRARIES(tryEval ${PARADISEO_LIBRARIES} lQueen)
