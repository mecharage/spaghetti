cmake_minimum_required(VERSION 3.3)
project(spaghetti)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wfatal-errors -pedantic -fno-rtti -DGLM_FORCE_RADIANS")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3")

include_directories(AFTER include)

file(GLOB_RECURSE H_FILES include/*.h)
file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE TPP_FILES src/*.tpp)
set(SOURCE_FILES ${H_FILES} ${CPP_FILES} ${TPP_FILES} include/Map.h)

add_executable(spaghetti ${SOURCE_FILES} )

#add_subdirectory(glk)

FIND_PACKAGE(OpenGL)
IF(OPENGL_FOUND)
    MESSAGE(STATUS "OpenGL render API found.")
    MESSAGE(STATUS "Detected OpenGL path is : ${OPENGL_LIBRARIES}")
ENDIF(OPENGL_FOUND)

FIND_PACKAGE(GLEW)
IF(GLEW_FOUND)
    MESSAGE(STATUS "GLEW found.")
    MESSAGE(STATUS "Detected GLEW path is : ${GLEW_LIBRARIES}")
ENDIF(GLEW_FOUND)

target_link_libraries(spaghetti

	SDL2
	SDL2_image
)

set_target_properties(spaghetti PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)


#===============================