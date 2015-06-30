cmake_minimum_required(VERSION 2.6)
project(dawn)
add_definitions(-std=c++11)
add_definitions(-Os)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

pkg_search_module(GLEW REQUIRED glew)
include_directories(${GLEW_INCLUDE_DIRS})

pkg_search_module(GLU REQUIRED glu)
include_directories(${GLU_INCLUDE_DIRS})

pkg_search_module(GL REQUIRED gl)
include_directories(${GL_INCLUDE_DIRS})

pkg_search_module(EIGEN3 REQUIRED eigen3)
include_directories(${EIGEN3_INCLUDE_DIRS})

pkg_search_module(LIBPNG REQUIRED libpng)
include_directories(${LIBPNG_INCLUDE_DIRS})

pkg_search_module(LIBLOG4CXX REQUIRED liblog4cxx)
include_directories(${LIBLOG4CXX_INCLUDE_DIRS})

include_directories(dbtweener)

set(LIBRARIES ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${GLU_LIBRARIES} ${GL_LIBRARIES} ${EIGEN3_LIBRARIES} ${LIBPNG_LIBRARIES} ${LIBLOG4CXX_LIBRARIES})

set(SOURCES hashlib/md5.cpp dbtweener/dbtweener.cpp UUID.cpp ImageUtils.cpp OpenGLUtils.cpp OpenGLDebug.cpp OpenGLTexture.cpp OpenGLTexturePool.cpp OpenGLShaderProgram.cpp OpenGLShaderProgramPool.cpp OpenGLRenderTarget.cpp OpenGLRenderer.cpp duktape.c libdawn.cpp)

add_executable(sandbox ${SOURCES} sandbox.cpp)
target_link_libraries(sandbox ${LIBRARIES})

add_executable(sandboxjs ${SOURCES} sandboxjs.cpp)
target_link_libraries(sandboxjs ${LIBRARIES})
