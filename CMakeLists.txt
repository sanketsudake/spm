#Snooker Project CMake File
###################################################

cmake_minimum_required(VERSION 2.8.9)

# set(CMAKE_PREFIX_PATH "/opt/Qt5.2.1/5.2.1/gcc_64/bin/qmake")
# set(QT5WIDGETS_DIR "/opt/Qt5.2.1/5.2.1/")

PROJECT(SNOOKER)

#Packages Required
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED) 
FIND_PACKAGE(Qt5Core REQUIRED) 
FIND_PACKAGE(Qt5Declarative REQUIRED) 

# SRC and INCLUDE variables
set(SNSRC ${SNOOKER_SOURCE_DIR}/src)
set(SNINC ${SNOOKER_SOURCE_DIR}/include)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#include directories

include_directories(${SNINC})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
find_library(SQLITE3_LIBRARY sqlite3)

# Load all cpp files in src folder
file( GLOB SRC ${SNSRC}/*.cpp )

# Qt specific
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
QT5_WRAP_UI(UISrcs forms/mainwindow.ui)
QT5_WRAP_CPP(MOCSrcs include/mainwindow.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

ADD_EXECUTABLE(spm ${SRC} ${MOCSrcs} ${UISrcs} ${SQLITE3_LIBRARY})
TARGET_LINK_LIBRARIES(spm sqlite3 ${QT_QMAIN_LIBRARY} ${OpenCV_LIBS} ${QT_LIBRARIES} Qt5::Widgets)

