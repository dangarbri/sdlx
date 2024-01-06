# CMakeList.txt : CMake project for sdlx, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("sdlx")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS=['png']")
add_library(sdlx "${CMAKE_SOURCE_DIR}/../sdlx.c")
target_include_directories(sdlx PUBLIC ${CMAKE_SOURCE_DIR}/..)

set(CMAKE_EXECUTABLE_SUFFIX ".html")
add_executable(sdlx_main "main.c")
target_link_libraries(sdlx_main PRIVATE sdlx)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sdlx PROPERTY C_STANDARD 11)
endif()

# TODO: Add tests and install targets if needed.
