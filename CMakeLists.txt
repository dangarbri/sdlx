# CMakeList.txt : CMake project for sdlx, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# make cache variables for install destinations
include(GNUInstallDirs)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("sdlx")
enable_testing()
add_library(sdlx "sdlx.c" "sdlx.h")

find_package(SDL2 REQUIRED)
get_target_property(SDL2_RUNTIME SDL2::SDL2 IMPORTED_LOCATION)
file(COPY ${SDL2_RUNTIME}
	 DESTINATION ${CMAKE_BINARY_DIR})

find_package(SDL2_ttf REQUIRED)
get_target_property(SDL2_TTF_INCLUDE_DIRS SDL2_ttf::SDL2_ttf INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SDL2_TTF_RUNTIME SDL2_ttf::SDL2_ttf IMPORTED_LOCATION)
get_target_property(SDL2_TTF_LIB SDL2_ttf::SDL2_ttf IMPORTED_IMPLIB)
file(COPY ${SDL2_TTF_RUNTIME}
	 DESTINATION ${CMAKE_BINARY_DIR})

find_package(SDL2_image REQUIRED)
get_target_property(SDL2_IMAGE_INCLUDE_DIRS SDL2_image::SDL2_image INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(SDL2_IMAGE_RUNTIME SDL2_image::SDL2_image IMPORTED_LOCATION)
get_target_property(SDL2_IMAGE_LINK_LIB SDL2_image::SDL2_image IMPORTED_IMPLIB)
file(COPY ${SDL2_IMAGE_RUNTIME}
	 DESTINATION ${CMAKE_BINARY_DIR})

install(FILES ${SDL2_RUNTIME} ${SDL2_TTF_RUNTIME} ${SDL2_IMAGE_RUNTIME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(sdlx PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
	${SDL2_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(sdlx PUBLIC
	${SDL2_LIBRARIES}
	${SDL2_TTF_LIB}
	${SDL2_IMAGE_LINK_LIB}
)

install(TARGETS sdlx
        EXPORT SDLXTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(FILES "sdlx.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

export(EXPORT SDLXTargets
       FILE "${CMAKE_BINARY_DIR}/SDLXTargets.cmake"
       NAMESPACE SDLX::
)
install(FILES "${CMAKE_BINARY_DIR}/SDLXTargets.cmake" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(test)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sdlx PROPERTY C_STANDARD 11)
endif()

# TODO: Add tests and install targets if needed.
