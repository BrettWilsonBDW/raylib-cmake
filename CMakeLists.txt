cmake_minimum_required(VERSION 3.22)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

##########################################################################################
# Project libs
##########################################################################################

# toggle libs to be used with ON and OFF

# raylib - wasm supported
set(useRaylib ON)
set(useMinimalRaylibModules OFF) # used mostly with apps with no need for game modules

# imgui files - wasm supported
set(useImGui OFF)
set(noAssets OFF) # disable asset macros used with mostly with gui programs without assets

# ldtk loader used with LDtk
set(useLDtkLoader OFF)

# fmt library - wasm supported
set(useFmt OFF)

# box2d physics 2d - wasm untested
set(useBox2d OFF)

# bullet physics 3d - wasm supported
set(useBullet3 OFF)

# note: wont play nice with gcc use msvc/clang instead
# reactphysics 3d - wasm unsupported
set(useReactPhysics3D OFF)

# Jolt Physics 3d - wasm supported
set(useJoltPhysics OFF)

##########################################################################################
# pre lib include settings
##########################################################################################

# this is here to ensure the runtime is linked staticly for msvc
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif()
endif()

if (NOT DEFINED PLATFORM)
    set(PLATFORM "default")
endif()

##########################################################################################
# Add dependencies with FetchContent
##########################################################################################

# libraries array to be linked
set(LIBRARIES ${PROJECT_NAME})

include(FetchContent)
function(add_git_dependency libName gitURL gitTag)

        FetchContent_Declare(${libName}
            GIT_REPOSITORY ${gitURL}
            GIT_TAG        ${gitTag}
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
        )

        if(NOT ("${libName}" STREQUAL "libname"))
            FetchContent_MakeAvailable(${libName})
        endif()

        # exclude lib to avoid errors
        if(NOT "${libName}" STREQUAL "libname" AND NOT "${libName}" STREQUAL "rlImGui")
            # target_compile_options(${libName} PRIVATE "-w")
        endif()  
endfunction()

#add new dependencies here
# raylib
if(useRaylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(CUSTOMIZE_BUILD OFF CACHE BOOL "" FORCE) 

    set(dep raylib)
    add_git_dependency(
        ${dep}
        https://github.com/raysan5/raylib.git
        head
    )
    
    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/external/raylib/config.h ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/raylib-src/src/config.h COPYONLY)
        FetchContent_Populate(${dep})
        
        add_subdirectory(${${dep}_SOURCE_DIR} ${${dep}_BINARY_DIR})
    endif()
        
    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

# imgui files
if(useImGui)
    add_git_dependency(
        RlImGui
        https://github.com/raylib-extras/rlImGui.git
        head
    )

    file(REMOVE_RECURSE build/_deps/rlimgui-src/examples)

    file(GLOB files_to_copy "${CMAKE_SOURCE_DIR}/build/_deps/rlimgui-src/*")
    file(COPY ${files_to_copy} DESTINATION ${CMAKE_SOURCE_DIR}/src/imgui)
endif()

# part of imgui files
if(useImGui)
    add_git_dependency(
        imGui
        https://github.com/ocornut/imgui.git
        head
    )

    file(REMOVE_RECURSE build/_deps/imgui-src/examples)
    file(REMOVE_RECURSE build/_deps/imgui-src/misc)
    file(REMOVE_RECURSE build/_deps/imgui-src/backends)
    file(REMOVE_RECURSE build/_deps/imgui-src/.editorconfig)
    file(REMOVE_RECURSE build/_deps/imgui-src/.github)
    
    file(GLOB files_to_copy "${CMAKE_SOURCE_DIR}/build/_deps/imgui-src/*")
    file(COPY ${files_to_copy} DESTINATION ${CMAKE_SOURCE_DIR}/src/imgui)
endif()

# ldtk loader
if(useLDtkLoader)
    set(dep LDtkLoader)
    add_git_dependency(
        ${dep}
        https://github.com/Madour/LDtkLoader.git
        head
    )

    FetchContent_GetProperties(${dep})

    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

# fmt
if(useFmt)
    set(dep fmt)
    add_git_dependency(
        ${dep}
        https://github.com/fmtlib/fmt.git
        head
    )

    FetchContent_GetProperties(${dep})
        
    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

# box2d
if(useBox2d)
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE) # don't build the supplied examples

    set(dep box2d)
    add_git_dependency(
        ${dep}
        https://github.com/erincatto/box2d.git
        head
    )

    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

# bullet physics 3d
if(useBullet3)
    set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE) # don't build the supplied examples

    set(dep BULLET_PHYSICS3)
    add_git_dependency(
        ${dep}
        https://github.com/bulletphysics/bullet3.git
        head
    )

    FetchContent_GetProperties(${dep})
    if(NOT ${dep}_POPULATED)
        
    endif()

    include_directories(RECURSE build/_deps/bullet_physics3-src/src)
        
    # add the dependency here to be linked BulletDynamics BulletCollision LinearMath BulletSoftBody
    list(APPEND LIBRARIES BulletDynamics)
    list(APPEND LIBRARIES BulletCollision)
    list(APPEND LIBRARIES LinearMath)
    list(APPEND LIBRARIES BulletSoftBody)
endif()

# react physics
if(useReactPhysics3D)
    set(dep reactphysics3d)
    add_git_dependency(
        ${dep}
        https://github.com/DanielChappuis/ReactPhysics3D.git
        head
    )
    FetchContent_GetProperties(${dep})

    include_directories(RECURSE build/_deps/reactphysics3d-src/include)
        
    # add the dependency here to be linked
    list(APPEND LIBRARIES ${dep})
endif()

# jolt physics
if(useJoltPhysics)
    set(DOUBLE_PRECISION OFF)
    set(GENERATE_DEBUG_SYMBOLS ON)
    set(CROSS_PLATFORM_DETERMINISTIC OFF)
    set(INTERPROCEDURAL_OPTIMIZATION ON)
    set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)
    set(OBJECT_LAYER_BITS 16)
    set(USE_SSE4_1 ON)
    set(USE_SSE4_2 ON)
    set(USE_AVX ON)
    set(USE_AVX2 ON)
    set(USE_AVX512 OFF)
    set(USE_LZCNT ON)
    set(USE_TZCNT ON)
    set(USE_F16C ON)
    set(USE_FMADD ON)

    if (${PLATFORM} STREQUAL "Web")
        option(ENABLE_MEMORY_PROFILER "Enable emscriptens memory profiler to help find leaks" OFF)
        option(DOUBLE_PRECISION "Compile the library in double precision mode" OFF)
        option(ENABLE_SIMD "Enable SIMD instructions (warning: last major browser to support this was Safari 16.4 in March 2023)" OFF)
        option(ENABLE_MULTI_THREADING "Compile the library in multi threading mode (warning: doesn't work in Safari nor with any JS callbacks currently)'" OFF) # SharedArrayBuffer is not defined when ON
        option(BUILD_WASM_COMPAT_ONLY "Compile online the WASM compat version of the library (speeds up compilation)" ON)

        set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution")
        set(FLOATING_POINT_EXCEPTIONS_ENABLED OFF)
        set(INTERPROCEDURAL_OPTIMIZATION OFF)
        set(GENERATE_DEBUG_SYMBOLS ON)
        set(DISABLE_CUSTOM_ALLOCATOR ON)
        set(OBJECT_LAYER_BITS 32)

        # Enable multi threading
        if (ENABLE_MULTI_THREADING)
            set(MULTI_THREADED_FLAG 
                -pthread 
                -s SHARED_MEMORY)
            set(ENVIRONMENT_FLAG
                -s ENVIRONMENT='web,node,worker' 
                -s PTHREAD_POOL_SIZE=16)
            set(CMAKE_CXX_FLAGS "-pthread -s SHARED_MEMORY")
        else()
            set(MULTI_THREADED_FLAG "")
            set(ENVIRONMENT_FLAG "-s ENVIRONMENT='web,node'")
        endif()
    endif()

    FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.0.0"
        SOURCE_SUBDIR "Build"
    )
    FetchContent_MakeAvailable(JoltPhysics)

    set(CMAKE_EXE_LINKER_FLAGS_DISTRIBUTION "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
    
    # add the dependency here to be linked
    list(APPEND LIBRARIES Jolt)
endif()


##########################################################################################
# Project executable setup
##########################################################################################

# Adding our source files
# Define PROJECT_SOURCES as a list of all source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

# Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

# Includes imgui source
if(useImGui)
    make_directory(../src/imgui)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/imgui)
endif()

##########################################################################################
# compiler settings
##########################################################################################

if(NOT ${PLATFORM} STREQUAL "Web")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -pthread")
        target_link_libraries(${LIBRARIES} -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(${LIBRARIES} -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        target_link_libraries(${LIBRARIES} -static-intel -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_libraries(${LIBRARIES})

        if(OFF)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")  
        endif()

        # dont forget to set  "cmake.configureSettings": { "CMAKE_BUILD_TYPE": "${buildType}" } in settings.json for vs code
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            message("-- console enabled")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
        else()
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")  
        endif()
    else()
        target_link_libraries(${LIBRARIES})
    endif()
else()
    target_link_libraries(${LIBRARIES})
endif()


##########################################################################################
# custom commands
##########################################################################################

# copy assets over into the build dir for the executable to have them for debug/run reasons
if(NOT noAssets)
    add_custom_target(copyAssetsOver
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMENT "Copying assets to build directory"
    )
    add_dependencies(${PROJECT_NAME} copyAssetsOver) 
endif()


add_definitions( -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} )

# for web build when using emscripten
if(ASYNCIFY)
  # Add Raylib's async support
  add_definitions(-DRAYLIB_ASYNC_ENABLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASYNCIFY")

  # Add the -O3 optimization flag
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# turn on minamal raylib config build to include minimal raylib modules
if(useMinimalRaylibModules AND useRaylib)
    # Function to toggle a define in a given config file
    function(toggle_define file_path define)
        file(READ "${file_path}" input_file_contents)
        string(REPLACE "// #define ${define}" "#define ${define}" modified_contents "${input_file_contents}")
        file(WRITE "${file_path}" "${modified_contents}")

        file(READ "${file_path}" input_file_contents)
        string(REPLACE "#define ${define}" "// #define ${define}" modified_contents "${input_file_contents}")
        file(WRITE "${file_path}" "${modified_contents}")
    endfunction()

    # choose raylib modules to be excluded by uncommenting the lines below
    set(module_strings
        # "SUPPORT_MODULE_RSHAPES"
        # "SUPPORT_MODULE_RTEXTURES" 
        # "SUPPORT_MODULE_RTEXT" # needs RTEXTURES to work
        "SUPPORT_MODULE_RMODELS"
        "SUPPORT_MODULE_RAUDIO"
    )

    # Toggle each define in the config file
    foreach(currentString ${module_strings})
        toggle_define("${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/raylib-src/src/config.h" ${currentString})
    endforeach()

    # enusre raylib is rebuilt with new config.h
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} .
    )
else()
    if(useRaylib)
        # Function to toggle a define in a given config file
        function(toggle_define file_path define)
            file(READ "${file_path}" input_file_contents)
            string(REPLACE "// #define ${define}" "#define ${define}" modified_contents "${input_file_contents}")
            file(WRITE "${file_path}" "${modified_contents}")
        endfunction()

        # used to make sure all modules are turned back on
        set(module_strings
            "SUPPORT_MODULE_RSHAPES"
            "SUPPORT_MODULE_RTEXTURES" 
            "SUPPORT_MODULE_RTEXT"
            "SUPPORT_MODULE_RMODELS"
            "SUPPORT_MODULE_RAUDIO"
        )

        # Toggle each define in the config file
        foreach(currentString ${module_strings})
            toggle_define("${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/raylib-src/src/config.h" ${currentString})
        endforeach()

        # enusre raylib is rebuilt with new config.h
        add_custom_command(
            TARGET ${PROJECT_NAME}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} .
        )
    endif()
endif()

##########################################################################################
# Project build settings
##########################################################################################

if (useReactPhysics3D)
    set(CMAKE_CXX_FLAGS "-m64")
endif()

# Setting ASSETS_PATH
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0 -DDEBUG")
    message("-- console enabled")
    if(NOT noAssets AND NOT ${PLATFORM} STREQUAL "Web")
        # Set the asset path macro to the absolute path on the dev machine
        target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")   
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    if(NOT noAssets AND NOT ${PLATFORM} STREQUAL "Web")
        # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
        target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/")
    endif()
    if(WIN32)
      if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows") 
      endif()    
      add_compile_definitions(NDEBUG)
    endif()
endif()

# Set common compiler flags
# SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wswitch")
# Set common compiler flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# if (NOT DEFINED PLATFORM)
#     set(PLATFORM "default")
# endif()

if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an .html file.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Os")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    if(NOT noAssets)
        # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -Os -Wall -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --preload-file assets/ --shell-file ../src/minshell.html")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -Os -Wall -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --preload-file assets/ --shell-file ../vendor/minshell.html")
        target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="/assets/") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -Os -Wall -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --shell-file ../vendor/minshell.html")
    endif()
endif()

# Ensure that hot-reload is enabled for VS
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
endif()
