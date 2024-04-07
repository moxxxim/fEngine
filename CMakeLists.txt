cmake_minimum_required(VERSION 3.27)

project(Feng)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeUtils")

option(VERBOSE_BUILD "Use verbose logging when building project" ON)

set(mySrcDir ${PROJECT_SOURCE_DIR}/Source)
set(thirdpartySrcDir ${PROJECT_SOURCE_DIR}/Thirdparty)
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum OS X deployment version")

# Do not allow build in source directory
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Specify different directory for binaries: not allowed to build into source directory.")
endif()

include(FengCMakeUtils)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
if(APPLE)
    set(CMAKE_COMPILE_WARNING_AS_ERROR NO)
elseif(WIN32)
    set(CMAKE_COMPILE_WARNING_AS_ERROR YES)
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_build/")

log_messages_optional(
    MESSAGES
        "Build project '${PROJECT_NAME}' from source: ${PROJECT_SOURCE_DIR}"
        "Build project to directory: ${PROJECT_BINARY_DIR}"
    DO_LOG ${VERBOSE_BUILD}
    FRAME_UP FRAME_DOWN)

# ---------------------------------------------------------------------
# ------------------------ Defind executables -------------------------
# ---------------------------------------------------------------------

add_executable(Feng_GlSandbox)
add_executable(Feng::GlSandbox ALIAS Feng_GlSandbox)

# -----------------------------------------------------------------------
# --------------------- My source files for project ---------------------
# -----------------------------------------------------------------------

file(GLOB_RECURSE mySrcFiles LIST_DIRECTORIES false
    "${mySrcDir}/*.cpp"
    "${mySrcDir}/*.hpp"
    "${mySrcDir}/*.c"
    "${mySrcDir}/*.h"
)

# ----------------------------------------------------------------------
# ---------------- Collect platform-specific libraries -----------------
# ----------------------------------------------------------------------

set(platformSpecificLibs)
if(APPLE)
    find_library(MAC_COCOA_LIB Cocoa REQUIRED)
    find_library(MAC_IO_KIT_LIB IOKit REQUIRED)
    find_library(MAC_CORE_VIDEO_LIB CoreVideo REQUIRED)
    find_library(MAC_OPEN_GL_LIB OpenGL REQUIRED)

    set(platformSpecificLibs 
                    ${MAC_COCOA_LIB}
                    ${MAC_IO_KIT_LIB}
                    ${MAC_CORE_VIDEO_LIB}
                    ${MAC_OPEN_GL_LIB})
endif()

# glad
if(WIN32)
    add_subdirectory(${thirdpartySrcDir}/Glad SYSTEM)
    list(APPEND platformSpecificLibs GladLibInterface)
    source_group_from_target("Thirdparty/Glad" GladLibInterface)
endif()

log_messages_optional(
    MESSAGES 
        "Platform-specific libraries:"
        ${platformSpecificLibs}
    DO_LOG ${VERBOSE_BUILD}
    FRAME_UP FRAME_DOWN)

# -----------------------------------------------------------------------
# ---------------- Thirdparty source files for project ------------------
# -----------------------------------------------------------------------

# Glfw
add_subdirectory(${thirdpartySrcDir}/Glfw SYSTEM)
source_group_from_dir("Thirdparty/Glfw" ${thirdpartySrcDir}/Glfw)

# Stb
add_subdirectory(${thirdpartySrcDir}/Stb SYSTEM)
source_group_from_target("Thirdparty/Stb" StbLibInterface)

# ---------------------------------------------------------------------
# ------------------------- Setup executable --------------------------
# ---------------------------------------------------------------------

set_target_properties(Feng_GlSandbox PROPERTIES
        OUTPUT_NAME GlSandbox
        EXPORT_NAME GlSandbox
        FOLDER "Apps"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        XCODE_GENERATE_SCHEME TRUE
        XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_sources(Feng_GlSandbox
    PRIVATE 
        ${mySrcFiles})

if(APPLE)
    # disable warnings aboug OpenGL deprecation of Mac
    target_compile_definitions(Feng_GlSandbox 
        PRIVATE 
            GL_SILENCE_DEPRECATION GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)
endif()
target_compile_definitions(Feng_GlSandbox PRIVATE NOMINMAX)

if(MSVC)
    target_compile_options(Feng_GlSandbox PRIVATE /W4 /MP)
else()
    target_compile_options(Feng_GlSandbox PRIVATE -Wall -Wextra -pedantic -Wno-missing-field-initializers)
endif()

target_include_directories(Feng_GlSandbox 
    PRIVATE
        "${mySrcDir}"
        "${thirdpartySrcDir}")

# Link libraries
target_link_libraries(Feng_GlSandbox
    PRIVATE
        ${platformSpecificLibs}
        StbLibInterface)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Feng_GlSandbox)
source_group(TREE ${mySrcDir} FILES ${mySrcFiles})

# TODO: m.alekseev make resources folder visible in project.
