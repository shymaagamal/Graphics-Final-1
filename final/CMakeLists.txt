include(CheckIncludeFiles)
cmake_minimum_required(VERSION 3.10) # common to every CLion project
project(OpenGLLinuxTest) # project name


if (UNIX)
    set(OpenGlLinkers -lglut -lGLU -lGL) # setting all the Glut libraries as one variable.
    ################################################
    configure_file(1.bmp ${CMAKE_CURRENT_BINARY_DIR} COPYONLY UNIX)
    configure_file(1_DIFFUSE.bmp ${CMAKE_CURRENT_BINARY_DIR} COPYONLY UNIX)
    add_executable(OpenGLLinuxTest1 SBE306B_Spring20_Project_Group_.cpp startRendering.cpp texture.cpp imageLoader/imageloader.cpp) #common to all clion project
    target_link_libraries(OpenGLLinuxTest1 ${OpenGlLinkers}) # linking opengl libraries to the project
    #################################################
endif ()

if (WIN32)
    cmake_minimum_required(VERSION 3.16)
    project(Full-Scene-Animation)

    set(CMAKE_CXX_STANDARD 17)

    # GLUT
    find_package(GLUT REQUIRED)
    include_directories(${GLUT_INCLUDE_DIR})
    if(NOT GLUT_FOUND)
        message(ERROR "GLUT not found!")
    endif(NOT GLUT_FOUND)

    # OpenGL
    find_package(OpenGL REQUIRED)
    include_directories(${OpenGL_INCLUDE_DIRS})
    link_directories(${OpenGL_LIBRARY_DIRS})
    add_definitions(${OpenGL_DEFINITIONS})
    if(NOT OPENGL_FOUND)
        message(ERROR "OPENGL not found!")
    endif(NOT OPENGL_FOUND)

    link_libraries(${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} --verbose)
    configure_file(data/flowers.obj ${CMAKE_CURRENT_BINARY_DIR}/data COPYONLY WIN32)
    configure_file(data/flowers.mtl ${CMAKE_CURRENT_BINARY_DIR}/data COPYONLY WIN32)
    configure_file(data/rose+vase.mtl ${CMAKE_CURRENT_BINARY_DIR}/data COPYONLY WIN32)
    configure_file(data/rose+vase.obj ${CMAKE_CURRENT_BINARY_DIR}/data COPYONLY WIN32)
    configure_file(data/al.obj ${CMAKE_CURRENT_BINARY_DIR}/data COPYONLY WIN32)
    configure_file(data/al.mtl ${CMAKE_CURRENT_BINARY_DIR}/data COPYONLY WIN32)
    configure_file(floor.bmp ${CMAKE_CURRENT_BINARY_DIR} COPYONLY WIN32)
    add_executable(main.cpp imageloader.cpp glm.cpp)
    
endif ()