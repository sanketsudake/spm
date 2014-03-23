# Specify version being used
cmake_minimum_required(VERSION 2.6)

#Project name
PROJECT(SNOOKER)
FIND_PACKAGE(OpenCV REQUIRED)
set(SNSRC ${SNOOKER_SOURCE_DIR}/src)
set(SNINC ${SNOOKER_SOURCE_DIR}/include)
include_directories(${SNINC})

set(INC
${SNINC}/buildprofile.hpp
${SNINC}/detectball.hpp
${SNINC}/detectshot.hpp
${SNINC}/managelogin.hpp
${SNINC}/shot.hpp
${SNINC}/collisionDetector.hpp
${SNINC}/shotArray.hpp
${SNINC}/database.hpp
${SNINC}/sclassifier.hpp)
set(SRC
${SNSRC}/buildprofile.cpp
${SNSRC}/detectball.cpp
${SNSRC}/detectshot.cpp
${SNSRC}/managelogin.cpp
${SNSRC}/main.cpp
${SNSRC}/shot.cpp
${SNSRC}/collisionDetector.cpp
${SNSRC}/shotArray.cpp
${SNSRC}/database.cpp
${SNSRC}/sclassifier.cpp)

add_executable(spm ${SRC} ${INC})
target_link_libraries(spm sqlite3 ${OpenCV_LIBS})
