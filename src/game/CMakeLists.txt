file(GLOB GAME_SOURCES *.cpp include/*.h)
include_directories(include ../lib/internal/include
					"../../lib_ext/win32/SDL2-2.0.4/include/" 
					"../../lib_ext/win32/glew-1.13.0/include/GL/"
					"../../lib_ext/win32/glm_0.9.7.2/"
					"../../lib_ext/win32/fbx-sdk-2016.1/include"
					"../../lib_ext/win32/libnoise-1.0.0/include")

add_executable(game ${GAME_SOURCES})

# Copy resources from the resources 
# to the Debug exec directory, so 
# they are available at runtime. 
add_custom_command(TARGET game POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					"../../../resources/shaders/"
					#$<TARGET_FILE_DIR:game>
					"Debug")
					
add_custom_command(TARGET game POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					"../../../resources/models/"
					"Debug"
					)
					
add_custom_command(TARGET game POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					"../../../resources/images/"
					"Debug"
					)					

target_link_libraries(game fb_engine
		"../../../lib_ext/win32/SDL2-2.0.4/lib/x86/SDL2"
		"../../../lib_ext/win32/SDL2-2.0.4/lib/x86/SDL2main"
		"../../../lib_ext/win32/glew-1.13.0/lib/Release/Win32/glew32"
		"../../../lib_ext/win32/fbx-sdk-2016.1/lib/x86/debug/libfbxsdk"
		"../../../lib_ext/win32/libnoise-1.0.0/lib/x86/libnoise"
		OpenGL32)