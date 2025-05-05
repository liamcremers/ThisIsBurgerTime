cmake_minimum_required(VERSION 3.26)

if(EMSCRIPTEN)
    message("Using Emscripten!")
    set(TARGET_NAME BurgerTimeWeb)
    set(TARGET_NAME_MINIGIN MiniginWeb)
else()
    set(TARGET_NAME BurgerTime)
    set(TARGET_NAME_MINIGIN Minigin)
endif()

# Automatically gather all .cpp and .h files in the ${TARGET_NAME} folder
file(GLOB BURGERTIME_SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB BURGERTIME_HEADERS CONFIGURE_DEPENDS "*.h")

# Add executable using the gathered files
add_executable(${TARGET_NAME} ${BURGERTIME_SOURCES} "BurgerTimeLayers.h" )

# Link with the Minigin library
target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_NAME_MINIGIN})
get_target_property(BURGERTIME_DLLS ${TARGET_NAME_MINIGIN} RUNTIME_DLLS)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Data"
        "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
)

# Set include directories
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Minigin
    ${glm_SOURCE_DIR}
)

# Link with glm
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)
#target_link_libraries(${TARGET_NAME} PUBLIC ${TARGET_NAME_MINIGIN})

if(WIN32)
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different
			${BURGERTIME_DLLS}
			"$<TARGET_FILE_DIR:${TARGET_NAME}>"
	)
	# Link with OpenGL
	target_link_libraries(${TARGET_NAME} PUBLIC opengl32)

	# Add OpenGL
	find_package(OpenGL REQUIRED)
	target_link_libraries(${TARGET_NAME} PUBLIC OpenGL::GL)

	# Link with XInput
	target_link_libraries(${TARGET_NAME} PUBLIC XInput)
    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

    # Add post build commands
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
    )
endif()

if(WIN32)
# Set ${TARGET_NAME} as the default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
endif()
# Define target architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(${TARGET_NAME} PRIVATE _AMD64_)
else()
    target_compile_definitions(${TARGET_NAME} PRIVATE _X86_)
endif()

#if(EMSCRIPTEN)
#    add_custom_command(TARGET ${TARGET_NAME_BURGERTIME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E echo "Generating HTML file using emcc"
#        COMMAND ${CMAKE_CXX_COMPILER} 
#            "$<TARGET_FILE_DIR:${TARGET_NAME_BURGERTIME}>/${TARGET_NAME_BURGERTIME}.js"
#            -o "$<TARGET_FILE_DIR:${TARGET_NAME_BURGERTIME}>/${TARGET_NAME_BURGERTIME}.html"
#            --shell-file "${CMAKE_SOURCE_DIR}/path/to/your/shell_minimal.html"
#    )
#endif()