include_directories(${CMAKE_SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
link_libraries(sdlx)
file(COPY ${SDL2_RUNTIME} ${SDL2_TTF_RUNTIME} ${SDL2_IMAGE_RUNTIME}
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(TestFloatEq "test_float_equals.c")
add_test(NAME SDLX_FloatEq COMMAND TestFloatEq)
add_executable(TestDistance test_distance.c)
add_test(NAME SDLX_Distance COMMAND TestDistance)