cmake_minimum_required(VERSION 3.26)
project(GameEngine)

#https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html#git-submodule-method
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/Minigin/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(Minigin)
add_subdirectory(BurgerTime)

target_compile_features(Minigin PUBLIC cxx_std_20)
target_compile_features(BurgerTime PUBLIC cxx_std_20)

target_compile_options(Minigin PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /EHsc>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_compile_options(BurgerTime PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /EHsc>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# Define DEBUG_RENDER for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Minigin PRIVATE DEBUG_RENDER DEBUG_SOUND)
    target_compile_definitions(BurgerTime PRIVATE DEBUG_RENDER DEBUG_SOUND)
endif()

#if(WIN32)
#    # Clang-tidy integration for Windows (see /EHsc)
#    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
#    if(CLANG_TIDY_EXE)
#        set(CLANG_TIDY_COMMAND
#            "${CLANG_TIDY_EXE}"
#            "--fix"              # This tells clang-tidy to apply fixes (might be slow)
#            "--extra-arg=/EHsc"  # Allow throws in MSVC (see: https://stackoverflow.com/questions/75496284/clang-tidy-emits-errors-about-exceptions-being-disabled-when-they-are-enabled)
#        )
#        set_target_properties(Minigin PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
#        set_target_properties(BurgerTime PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
#    endif()
#
#    # Clang-format integration for source files
#    find_program(CLANG_FORMAT_EXE NAMES "clang-format")
#    if(CLANG_FORMAT_EXE)
#        file(GLOB_RECURSE ALL_SOURCE_FILES 
#            "${CMAKE_SOURCE_DIR}/Minigin/*.cpp"
#            "${CMAKE_SOURCE_DIR}/Minigin/*.h"
#            "${CMAKE_SOURCE_DIR}/BurgerTime/*.cpp"
#            "${CMAKE_SOURCE_DIR}/BurgerTime/*.h"
#        )
#        add_custom_target(clang-format
#            COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_SOURCE_FILES}
#            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#            COMMENT "Running clang-format on source files"
#        )
#        add_dependencies(Minigin clang-format)
#        add_dependencies(BurgerTime clang-format)
#    endif()
#endif()
