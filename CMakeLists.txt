cmake_minimum_required(VERSION 3.26)
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()
project(GameEngine)

#https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html#git-submodule-method
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive --remote failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
endif ()
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/Minigin/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif ()

if (EMSCRIPTEN)
    message("Using Emscripten!")
    set(TARGET_NAME_MINIGIN MiniginWeb)
    set(TARGET_NAME_BURGERTIME BurgerTimeWeb)
else ()
    set(TARGET_NAME_MINIGIN Minigin)
    set(TARGET_NAME_BURGERTIME BurgerTime)
endif ()

add_subdirectory(Minigin)
add_subdirectory(BurgerTime)

target_compile_features(${TARGET_NAME_MINIGIN} PUBLIC cxx_std_20)
target_compile_features(${TARGET_NAME_BURGERTIME} PUBLIC cxx_std_20)
target_compile_options(${TARGET_NAME_MINIGIN} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /EHsc>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>)
target_compile_options(${TARGET_NAME_BURGERTIME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /EHsc>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>)

# Define DEBUG_RENDER for Debug builds
# DEBUG_SOUND
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${TARGET_NAME_MINIGIN} PRIVATE DEBUG_RENDER DEBUG_STATES)
    target_compile_definitions(${TARGET_NAME_BURGERTIME} PRIVATE DEBUG_RENDER DEBUG_STATES)
endif ()

if(EMSCRIPTEN)
    set(EMSCRIPTEN_COMPILE_FLAGS
        -sUSE_SDL=2
        -sUSE_SDL_IMAGE=2
        -sUSE_SDL_TTF=2
        -sUSE_SDL_MIXER=2
        -pthread
        -g
    )
    set(EMSCRIPTEN_LINK_FLAGS
        -sUSE_SDL=2
        -sUSE_SDL_IMAGE=2
        -sUSE_SDL_TTF=2
        -sUSE_SDL_MIXER=2
        -pthread
        -sALLOW_MEMORY_GROWTH
        --preload-file "${CMAKE_SOURCE_DIR}/BurgerTime/Data@/"
        -sSDL2_IMAGE_FORMATS=['tga','png']
        -fno-stack-protector
        -g
    )
endif()

if(EMSCRIPTEN)
    target_compile_options(${TARGET_NAME_MINIGIN} PUBLIC ${EMSCRIPTEN_COMPILE_FLAGS})
    target_link_options(${TARGET_NAME_MINIGIN} PUBLIC ${EMSCRIPTEN_LINK_FLAGS})
    target_compile_options(${TARGET_NAME_BURGERTIME} PUBLIC ${EMSCRIPTEN_COMPILE_FLAGS})
    target_link_options(${TARGET_NAME_BURGERTIME} PUBLIC ${EMSCRIPTEN_LINK_FLAGS})
endif()
if(EMSCRIPTEN)
    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".html")
    set_target_properties(${TARGET_NAME_BURGERTIME} PROPERTIES SUFFIX ".html")
endif()

if(WIN32)
    # Clang-tidy integration for Windows (see /EHsc)
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        set(CLANG_TIDY_COMMAND
            "${CLANG_TIDY_EXE}"
            "--fix"              # This tells clang-tidy to apply fixes (might be slow)
            "--extra-arg=/EHsc"  # Allow throws in MSVC (see: https://stackoverflow.com/questions/75496284/clang-tidy-emits-errors-about-exceptions-being-disabled-when-they-are-enabled)
        )
        set_target_properties(Minigin PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
        set_target_properties(BurgerTime PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    endif()

    # Clang-format integration for source files
    find_program(CLANG_FORMAT_EXE NAMES "clang-format")
    if(CLANG_FORMAT_EXE)
        file(GLOB_RECURSE ALL_SOURCE_FILES CONFIGURE_DEPENDS 
            "${CMAKE_SOURCE_DIR}/Minigin/*.cpp"
            "${CMAKE_SOURCE_DIR}/Minigin/*.h"
            "${CMAKE_SOURCE_DIR}/BurgerTime/*.cpp"
            "${CMAKE_SOURCE_DIR}/BurgerTime/*.h"
        )
        add_custom_target(clang-format
            COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-format on source files"
        )
        add_dependencies(Minigin clang-format)
        add_dependencies(BurgerTime clang-format)
    endif()
endif()
