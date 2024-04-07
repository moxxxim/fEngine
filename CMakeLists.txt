cmake_minimum_required(VERSION 3.27)

project(Feng)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeUtils")

option(VERBOSE_BUILD "Use verbose logging when building project" ON)

set(thirdpartySrcDirName "Thirdparty")
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum OS X deployment version")

# Do not allow build in source directory
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Specify different directory for binaries: not allowed to build into source directory.")
endif()

include(FengCMakeUtils)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
set(CMAKE_COMPILE_WARNING_AS_ERROR YES)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_build/")

message("CMAKE_VS_DEBUGGER_WORKING_DIRECTORY = ${CMAKE_VS_DEBUGGER_WORKING_DIRECTORY}")
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

# ---------------------------------------------------------------------
# ------------------- Collect thirdparty libraries --------------------
# ---------------------------------------------------------------------

set(thirdpartyStaticLibs "")

if(APPLE)
    file(GLOB_RECURSE thirdpartyStaticLibs REGEX "*.a")
elseif(WIN32)
    file(GLOB_RECURSE thirdpartyStaticLibs REGEX "*.lib")
endif()

set(thirdpartyStaticLibsDebug ${thirdpartyStaticLibs})
list(FILTER thirdpartyStaticLibsDebug INCLUDE REGEX ".*${thirdpartySrcDirName}.*Libs.*Debug.*")

set(thirdpartyStaticLibsRelease ${thirdpartyStaticLibs})
list(FILTER thirdpartyStaticLibsRelease INCLUDE REGEX ".*${thirdpartySrcDirName}.*Libs.*Release.*")

log_messages_optional(
    MESSAGES 
        "Thirdparty static libraries (DEBUG):"
        ${thirdpartyStaticLibsDebug}
    DO_LOG ${VERBOSE_BUILD}
    FRAME_UP)

log_messages_optional(
    MESSAGES 
        "Thirdparty static libraries (RELEASE):"
        ${thirdpartyStaticLibsRelease}
    DO_LOG ${VERBOSE_BUILD}
    FRAME_DOWN)

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

log_messages_optional(
    MESSAGES 
        "Platform-specific libraries:"
        ${platformSpecificLibs}
    DO_LOG ${VERBOSE_BUILD}
    FRAME_UP FRAME_DOWN)

# -----------------------------------------------------------------------
# --------------------- My source files for project ---------------------
# -----------------------------------------------------------------------

set(mySrcDir ${PROJECT_SOURCE_DIR}/Source)
file(GLOB_RECURSE mySrcFiles LIST_DIRECTORIES false
    "${mySrcDir}/*.cpp"
    "${mySrcDir}/*.hpp"
    "${mySrcDir}/*.c"
    "${mySrcDir}/*.h"
)

# -----------------------------------------------------------------------
# ---------------- Thirdparty source files for project ------------------
# -----------------------------------------------------------------------

set(thirdpartySrcDir ${PROJECT_SOURCE_DIR}/${thirdpartySrcDirName})

# glad
if(WIN32)
    add_subdirectory(${thirdpartySrcDir}/Glad SYSTEM)
    list(APPEND platformSpecificLibs GladLibInterface)
    get_target_property(gladSrc GladLibInterface INTERFACE_SOURCES)
    source_group("Thirdparty/Glad" FILES ${gladSrc})
endif()

# Glfw
file(GLOB_RECURSE glfwSrc LIST_DIRECTORIES false
    "${thirdpartySrcDir}/Glfw/*.cpp"
    "${thirdpartySrcDir}/Glfw/*.hpp"
    "${thirdpartySrcDir}/Glfw/*.c"
    "${thirdpartySrcDir}/Glfw/*.h"
)
source_group("Thirdparty/Glfw" FILES ${glfwSrc})

# Stb
add_subdirectory(${thirdpartySrcDir}/Stb SYSTEM)
get_target_property(stbSrc StbLibInterface INTERFACE_SOURCES)
source_group("Thirdparty/Stb" FILES ${stbSrc})

# ---------------------------------------------------------------------
# ------------------------- Setup executable --------------------------
# ---------------------------------------------------------------------

set_target_properties(Feng_GlSandbox PROPERTIES
      OUTPUT_NAME GlSandbox
      EXPORT_NAME GlSandbox
      FOLDER "Apps"
      VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(Feng_GlSandbox
    PUBLIC 
        ${mySrcFiles} 
        ${glfwSrc})

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
        $<$<CONFIG:Debug>:${thirdpartyStaticLibsDebug}>
        $<$<CONFIG:Release>:${thirdpartyStaticLibsRelease}>
        ${platformSpecificLibs}
        StbLibInterface)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Feng_GlSandbox)
source_group(TREE ${mySrcDir} FILES ${mySrcFiles})

# TODO: m.alekseev make resources folder visible in project.
