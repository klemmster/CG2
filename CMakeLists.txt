cmake_minimum_required (VERSION 2.6)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
project (PointMesh)

#add_subdirectory(shader)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

file (GLOB_RECURSE headers *.h)
file (GLOB_RECURSE headers *.hpp)

find_package( OpenGL )
find_package( GLUT )

IF(GLUT_FOUND)
    MESSAGE(STATUS "Found glut")
ELSE()
    MESSAGE(STATUS "ERROR, no glut")
ENDIF(GLUT_FOUND)

find_package( Qt4 REQUIRED QtCore QtGui QtOpenGL )
IF(QT4_FOUND)
    MESSAGE(STATUS "Found QT4")
    SET(QT_USE_QTOPENGL TRUE)
    SET(QT_WRAP_CPP TRUE)
    INCLUDE( ${QT_USE_FILE} )
    SET(MOCHEADER ./include/GLWidget.hpp ./include/offLoader.hpp )
    QT4_WRAP_CPP(MOCFILES ${MOCHEADER})
ELSE()
    MESSAGE(STATUS "ERROR, NO QT4")
ENDIF(QT4_FOUND)

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories ( ${Boost_LIBRARY_DIRS} )

IF(UNIX AND NOT WIN32)
    set(CMAKE_CXX_FLAGS "-g -Wall -O0 -march=corei7 -std=c++11")
ENDIF(UNIX AND NOT WIN32)

add_executable(PointMesh ./src/leaf.cpp ./src/tree.cpp ./src/TokenizedFile.cpp ./src/vertex.cpp ./src/GLWidget.cpp ./src/offLoader.cpp ./src/main.cpp ${MOCFILES} ${headers} )
target_link_libraries(PointMesh ${Boost_LIBRARIES} ${GLUT_LIBRARY} ${QT_LIBRARIES} ${OPENGL_LIBRARIES} )

