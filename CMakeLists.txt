cmake_minimum_required(VERSION 2.8.9)

# set(CMAKE_PREFIX_PATH "/opt/Qt5.2.1/5.2.1/gcc_64/bin/qmake")
# set(QT5WIDGETS_DIR "/opt/Qt5.2.1/5.2.1/")

PROJECT(SNOOKER)
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED) 
FIND_PACKAGE(Qt5Core REQUIRED) 
FIND_PACKAGE(Qt5Declarative REQUIRED) 

set(SNSRC ${SNOOKER_SOURCE_DIR}/src)
set(SNINC ${SNOOKER_SOURCE_DIR}/include)
find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
find_library(SQLITE3_LIBRARY sqlite3)

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
${SNSRC}/roi.cpp
${SNSRC}/gui.cpp
${SNSRC}/mainwindow.cpp
${SNSRC}/sclassifier.cpp)

set(INC
${SNINC}/buildprofile.hpp
${SNINC}/detectball.hpp
${SNINC}/detectshot.hpp
${SNINC}/managelogin.hpp
${SNINC}/shot.hpp
${SNINC}/collisionDetector.hpp
${SNINC}/shotArray.hpp
${SNINC}/database.hpp
${SNINC}/roi.hpp
${SNINC}/gui.hpp
${SNINC}/sclassifier.hpp)

QT5_WRAP_UI(UISrcs forms/mainwindow.ui)
QT5_WRAP_CPP(MOCSrcs include/mainwindow.h)

INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SINC})
ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

ADD_EXECUTABLE(spm ${SQLITE3_LIBRARY} ${SRC} ${INC} ${MOCSrcs} ${UISrcs})
TARGET_LINK_LIBRARIES(spm sqlite3 ${QT_QMAIN_LIBRARY} ${OpenCV_LIBS} ${QT_LIBRARIES} Qt5::Widgets)
