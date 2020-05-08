# 1) Include the sources

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

# 2) Define targets: executable and dependencies

ADD_EXECUTABLE(ceao main.cpp)
ADD_EXECUTABLE(ceao_competency ceao_competency.cpp)
ADD_EXECUTABLE(ceao_course ceao_course.cpp)
ADD_EXECUTABLE(ceao_profession ceao_profession.cpp)
ADD_EXECUTABLE(ceao_csdvp ceao_csdvp.cpp)

ADD_EXECUTABLE(tryInit tryInit.cpp)
ADD_EXECUTABLE(tryMutation tryMutation.cpp)
ADD_EXECUTABLE(tryCrossover tryCrossover.cpp)
ADD_EXECUTABLE(tryEval tryEval.cpp)

ADD_DEPENDENCIES(ceao lQueen lModel)
ADD_DEPENDENCIES(ceao_competency lQueen lModel)
ADD_DEPENDENCIES(ceao_course lQueen lModel)
ADD_DEPENDENCIES(ceao_profession lQueen lModel)
ADD_DEPENDENCIES(ceao_csdvp lQueen lModel)

ADD_DEPENDENCIES(tryInit lQueen)
ADD_DEPENDENCIES(tryMutation lQueen)
ADD_DEPENDENCIES(tryCrossover lQueen)
ADD_DEPENDENCIES(tryEval lQueen)

# 3) Link the librairies for your executable

TARGET_LINK_LIBRARIES(ceao ${PARADISEO_LIBRARIES} lQueen lModel)
TARGET_LINK_LIBRARIES(ceao_competency ${PARADISEO_LIBRARIES} lQueen lModel)
TARGET_LINK_LIBRARIES(ceao_course ${PARADISEO_LIBRARIES} lQueen lModel)
TARGET_LINK_LIBRARIES(ceao_profession ${PARADISEO_LIBRARIES} lQueen lModel)
TARGET_LINK_LIBRARIES(ceao_csdvp ${PARADISEO_LIBRARIES} lQueen lModel)

TARGET_LINK_LIBRARIES(tryInit ${PARADISEO_LIBRARIES} lQueen)
TARGET_LINK_LIBRARIES(tryMutation ${PARADISEO_LIBRARIES} lQueen)
TARGET_LINK_LIBRARIES(tryCrossover ${PARADISEO_LIBRARIES} lQueen)
TARGET_LINK_LIBRARIES(tryEval ${PARADISEO_LIBRARIES} lQueen)
