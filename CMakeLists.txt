# Specify version being used
cmake_minimum_required(VERSION 2.6)

#Project name
PROJECT(SNOOKER)
FIND_PACKAGE(OpenCV REQUIRED)

set(SNSRC ${SNOOKER_SOURCE_DIR}/src)
set(SNINC ${SNOOKER_SOURCE_DIR}/include)

include_directories(${SNINC})
file( GLOB SRC ${SNSRC}/*.cpp )

add_executable(spm ${SRC})
target_link_libraries(spm sqlite3 ${OpenCV_LIBS})
