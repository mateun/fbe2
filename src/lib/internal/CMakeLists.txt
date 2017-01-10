file(GLOB LIB_ENGINE_SOURCES *.cpp include/*.h)
add_library(fb_engine SHARED ${LIB_ENGINE_SOURCES})

# Platform specific instructions
IF (WIN32)
message(STATUS "Microsoft Windows. Building engine for WIN32.")
include_directories(include 
					"../../../lib_ext/win32/SDL2-2.0.4/include/" 
					"../../../lib_ext/win32/glew-1.13.0/include/GL/"
					"../../../lib_ext/win32/glm_0.9.7.2/"
					"../../../lib_ext/win32/fbx-sdk-2016.1/include"
					"../../../lib_ext/win32/libnoise-1.0.0/include")
add_custom_command(TARGET fb_engine POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
					"../../../../lib_ext/win32/glew-1.13.0/bin/Release/Win32/glew32.dll"
					"../../game/Debug/glew32.dll"
					)

add_custom_command(TARGET fb_engine POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
					"../../../../lib_ext/win32/SDL2-2.0.4/lib/x86/SDL2.dll"
					"../../game/Debug/SDL2.dll"
					)
					
add_custom_command(TARGET fb_engine POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
					"../../../../lib_ext/win32/fbx-sdk-2016.1/lib/x86/debug/libfbxsdk.dll"
					"../../game/Debug/libfbxsdk.dll"
					)
					
add_custom_command(TARGET fb_engine POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
					"../../../../lib_ext/win32/libnoise-1.0.0/lib/x86/libnoise.dll"
					"../../game/Debug/libnoise.dll"
					)					
					
add_custom_command( TARGET fb_engine POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "Debug/fb_engine.dll"
                    "../../game/Debug/fb_engine.dll" )
# By declaring the linking of SDL2 here private, 
# we avoid that this link dependencies (towards SDL2, GLEW etc.)
# is inherited towards our main game app target. 
# This would lead to an additional linking of the game app 
# with SDL2, which we don't want.
target_link_libraries(fb_engine PRIVATE 
		"../../../../lib_ext/win32/SDL2-2.0.4/lib/x86/SDL2"
		"../../../../lib_ext/win32/glew-1.13.0/lib/Release/Win32/glew32"
		"../../../../lib_ext/win32/fbx-sdk-2016.1/lib/x86/debug/libfbxsdk"
		"../../../../lib_ext/win32/libnoise-1.0.0/lib/x86/libnoise"
		OpenGL32)
ENDIF()