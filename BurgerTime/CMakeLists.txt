cmake_minimum_required(VERSION 3.26)

if(EMSCRIPTEN)
    message("Using Emscripten!")
    set(TARGET_NAME BurgerTimeWeb)
else()
    set(TARGET_NAME BurgerTime)
endif()

# Automatically gather all .cpp and .h files in the ${TARGET_NAME} folder
file(GLOB BURGERTIME_SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB BURGERTIME_HEADERS CONFIGURE_DEPENDS "*.h")

# Add executable using the gathered files
add_executable(${TARGET_NAME} ${BURGERTIME_SOURCES})

# Set ${TARGET_NAME} as the default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})

# Link with the Minigin library
if(EMSCRIPTEN)
target_link_directories(${TARGET_NAME} PRIVATE MiniginWeb)
else()
target_link_libraries(${TARGET_NAME} PRIVATE Minigin)
endif()

get_target_property(BURGERTIME_DLLS Minigin RUNTIME_DLLS)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        ${BURGERTIME_DLLS}
        "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)

# Set include directories
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/${TARGET_NAME}
    ${CMAKE_SOURCE_DIR}/Minigin
)

# Compile at max warning level + treat warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /EHsc>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# Link with OpenGL
target_link_libraries(${TARGET_NAME} PUBLIC opengl32)

# Add OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET_NAME} PUBLIC OpenGL::GL)

# Link with XInput
target_link_libraries(${TARGET_NAME} PUBLIC XInput)

# Link with glm
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)

if(WIN32)
    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

    # Add post build commands
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
    )
endif()

# Define target architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(${TARGET_NAME} PRIVATE _AMD64_)
else()
    target_compile_definitions(${TARGET_NAME} PRIVATE _X86_)
endif()