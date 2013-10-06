# Specify version being used
cmake_minimum_required(VERSION 2.6)

#Project name
PROJECT(SNOOKER)
FIND_PACKAGE(OpenCV REQUIRED)
set(SNSRC ${SNOOKER_SOURCE_DIR}/src)
set(SNINC ${SNOOKER_SOURCE_DIR}/include)
include_directories(${SNINC})

set(SRC ${SNSRC}/snBgsub.cpp )
#set(INC ${SNINC}/snCore.hpp )

add_executable(snooker ${SRC} ${INC})
target_link_libraries(snooker ${OpenCV_LIBS})
