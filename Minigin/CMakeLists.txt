cmake_minimum_required(VERSION 3.26)

if(EMSCRIPTEN)
    message("Using Emscripten!")
    set(TARGET_NAME MiniginWeb)
else()
    set(TARGET_NAME Minigin)
endif()

# Automatically gather all .cpp and .h files in the Minigin folder
file(GLOB MINIGIN_SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB MINIGIN_HEADERS CONFIGURE_DEPENDS "*.h")

# Add library using the gathered files
add_library(${TARGET_NAME} STATIC ${MINIGIN_SOURCES})

# Group files under the "Components" section  
file(GLOB COMPONENTS_SOURCES CONFIGURE_DEPENDS "*Component.cpp")  
source_group("Components" FILES ${COMPONENTS_SOURCES})

# Group other files under the "Other" section
file(GLOB OTHER_SOURCES CONFIGURE_DEPENDS "*.cpp")
list(REMOVE_ITEM OTHER_SOURCES ${COMPONENTS_SOURCES})
source_group("Other" FILES ${OTHER_SOURCES})

# Enable C++20 features
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

include(FetchContent)

# Add glm
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_MakeAvailable(glm)

# Add ImGui
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8-docking
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ImGui)

# Add SDL2
if(WIN32)
    FetchContent_Declare(
        SDL2
        URL https://www.libsdl.org/release/SDL2-devel-2.30.10-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL2)
    set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
        set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
    else()
        set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
        set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
    endif()

    # Add SDL2_image
    FetchContent_Declare(
        SDL2_image
        URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.3-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL2_image)
    set(SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib")
        set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll")
    else()
        set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib")
        set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll")
    endif()

    # Add SDL2_ttf
    FetchContent_Declare(
        SDL2_ttf
        URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL2_ttf)
    set(SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib)
        set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll")
    else()
        set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib)
        set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll")
    endif()

    find_package(VLD CONFIG)

    set(IMGUI_SRC
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui.h

        # Backends
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    add_library(ImGui STATIC ${IMGUI_SRC})
    target_include_directories(ImGui PUBLIC ${imgui_SOURCE_DIR} ${SDL2_INCLUDE_DIR})
    target_link_libraries(ImGui PRIVATE ${SDL2_LIBRARIES})

    # Set all include directories
    target_include_directories(${TARGET_NAME} PRIVATE
        ${VLD_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ImGui
    )

    # Set libraries to link with
    target_link_libraries(${TARGET_NAME} PUBLIC
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${VLD_LIBRARY}
        ImGui
    )

elseif(NOT EMSCRIPTEN)

    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.10
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL2)

    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    set(SDL2IMAGE_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_image)

    FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    set(SDL2TTF_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_ttf)

    target_include_directories(${TARGET_NAME} PUBLIC
        ${sdl2_SOURCE_DIR}/include
        ${sdl2_image_SOURCE_DIR}/include
        ${sdl2_ttf_SOURCE_DIR}
    )
    target_link_libraries(${TARGET_NAME} PUBLIC
        SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
    )
endif()

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_include_directories(${TARGET_NAME} PUBLIC
    ${glm_SOURCE_DIR}
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

if(EMSCRIPTEN)
    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g")
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\" -sSDL2_IMAGE_FORMATS=['tga','png']")

    # Work around https://github.com/emscripten-core/emscripten/issues/11513
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -fno-stack-protector")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/EmscriptenToolchain.cmake)

    # Additional settings for Emscripten build
    set_target_properties(${TARGET_NAME} PROPERTIES
        COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
        LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

elseif(WIN32)
    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

    ## Add post build commands
    #add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    #    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    #)
    #add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    #    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    #)
    #add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    #    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    #)
endif()

#Set DLL var so others linking against this library can use it to copy the DLL's next to their exe
set(DLL_DEPENDENCIES
    ${SDL2_LIBRARY_DLL}
    ${SDL2_IMAGE_LIBRARY_DLL}
    ${SDL2_TTF_LIBRARY_DLL}
)

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_DLLS "${DLL_DEPENDENCIES}"
)

# Define target architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(${TARGET_NAME} PRIVATE _AMD64_)
else()
    target_compile_definitions(${TARGET_NAME} PRIVATE _X86_)
endif()