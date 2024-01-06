# CMakeList.txt : CMake project for sdlx, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("sdlx")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=['png']")
add_library(sdlx "sdlx.c")

find_package(SDL2)
find_package(SDL2_ttf)
find_package(SDL2_image)
target_include_directories(sdlx PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if (SDL2_FOUND)
target_include_directories(sdlx PUBLIC ${SDL2_INCLUDE_DIRS})
endif()

if (SDL2_ttf_FOUND)
get_target_property(SDL2_TTF_INCLUDES SDL2_ttf::SDL2_ttf INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(sdlx PUBLIC ${SDL2_TTF_INCLUDES})
endif()

if (SDL2_image_FOUND)
get_target_property(SDL_IMAGE_INCLUDES SDL2_image::SDL2_image INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(sdlx PUBLIC ${SDL_IMAGE_INCLUDES})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sdlx PROPERTY C_STANDARD 11)
endif()

# TODO: Add tests and install targets if needed.
