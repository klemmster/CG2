cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
project (PointMesh)

#add_subdirectory(shader)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

file (GLOB_RECURSE headers *.h)
file (GLOB_RECURSE headers *.hpp)

find_package( OpenGL )
find_package( GLUT )
find_package( Eigen3 )


IF(GLUT_FOUND)
    MESSAGE(STATUS "Found glut")
ELSE()
    MESSAGE(STATUS "ERROR, no glut")
ENDIF(GLUT_FOUND)

IF(EIGEN3_FOUND)
    include_directories(${EIGEN3_INCLUDE_DIR})
ELSE()
    MESSAGE(STATUS "ERROR, Eigen3 not found")
ENDIF(EIGEN3_FOUND)

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

find_package(Boost COMPONENTS system filesystem thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories ( ${Boost_LIBRARY_DIRS} )

IF(UNIX AND NOT WIN32)
    set(CMAKE_CXX_FLAGS "-w -O3 -march=native -std=c++0x")
    set(CMAKE_CXX_FLAGS_DEBUG "-w -O0 -g -march=native -std=c++0x")
    MESSAGE(STATUS "Use c++0x")
ELSEIF(APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -stdlib=libc++ -g -Wall")
ENDIF()

add_executable(PointMesh ./src/Grid.cpp ./src/domain.cpp ./src/limitedPriorityQueue.cpp ./src/hyperRegion.cpp ./src/hyperSphere.cpp ./src/node.cpp ./src/tree.cpp ./src/TokenizedFile.cpp ./src/vertex.cpp ./src/GLWidget.cpp ./src/offLoader.cpp ./src/main.cpp ${MOCFILES} ${headers} )
target_link_libraries(PointMesh ${Boost_LIBRARIES} ${GLUT_LIBRARY} ${QT_LIBRARIES} ${OPENGL_LIBRARIES} )

