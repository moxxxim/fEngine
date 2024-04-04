cmake_minimum_required(VERSION 3.27)

project(Feng)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeUtils")

option(VERBOSE_BUILD "Use verbose logging when building project" ON)

set(THIRDPARTY_SRC_DIR_NAME "Thirdparty")
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum OS X deployment version")

# Do not allow build in source directory
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Specify different directory for binaries: not allowed to build into source directory.")
endif()

include(FengCMakeUtils)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_build/")

log_messages_optional(
    MESSAGES
        "Build project '${PROJECT_NAME}' from source: ${PROJECT_SOURCE_DIR}"
        "Build project to directory: ${PROJECT_BINARY_DIR}"
    DO_LOG ${VERBOSE_BUILD}
    FRAMED)

# ---------------------------------------------------------------------
# ------------------- Collect thirdparty libraries --------------------
# ---------------------------------------------------------------------

set(THIRDPARTY_STATIC_LIBS)
set(THIRDPARTY_STATIC_LIBS_DEBUG)
set(THIRDPARTY_STATIC_LIBS_RELEASE)

if(APPLE)
    file(GLOB_RECURSE THIRDPARTY_STATIC_LIBS REGEX "*.a")
elseif(WIN32)
    message(FATAL_ERROR "Not implemented libs collections for Win")
endif()

list(FILTER THIRDPARTY_STATIC_LIBS_DEBUG INCLUDE REGEX ".*${THIRDPARTY_SRC_DIR_NAME}.*Libs/Debug/*")
list(FILTER THIRDPARTY_STATIC_LIBS_RELEASE INCLUDE REGEX ".*${THIRDPARTY_SRC_DIR_NAME}.*Libs/Release/*")

log_messages_optional(
    MESSAGES 
        "Thirdparty static libraries:"
        ${THIRDPARTY_STATIC_LIBS}
    DO_LOG
    FRAMED)

# ----------------------------------------------------------------------
# ---------------- Collect platform-specific libraries -----------------
# ----------------------------------------------------------------------

set(PLATFORM_SPECIFIC_LIBS)
if(APPLE)
    find_library(MAC_COCOA_LIB Cocoa REQUIRED)
    find_library(MAC_IO_KIT_LIB IOKit REQUIRED)
    find_library(MAC_CORE_VIDEO_LIB CoreVideo REQUIRED)
    find_library(MAC_OPEN_GL_LIB OpenGL REQUIRED)

    set(PLATFORM_SPECIFIC_LIBS 
                    ${MAC_COCOA_LIB}
                    ${MAC_IO_KIT_LIB}
                    ${MAC_CORE_VIDEO_LIB}
                    ${MAC_OPEN_GL_LIB})
endif()

log_messages_optional(
    MESSAGES 
        "Platform-specific libraries:"
        ${PLATFORM_SPECIFIC_LIBS}
    DO_LOG ${VERBOSE_BUILD}
    FRAMED)

# -----------------------------------------------------------------------
# ---------------- Collect all source files for project -----------------
# -----------------------------------------------------------------------

set(SOURCE_FILES_EXTENSIONS "*.cpp" "*.h" "*.hpp")
file(GLOB_RECURSE MY_PROJECT_SOURCES_FILES ${SOURCE_FILES_EXTENSIONS})
# remove cmake build directory from source files.
list(FILTER MY_PROJECT_SOURCES_FILES EXCLUDE REGEX ".*${PROJECT_BINARY_DIR}.*")

foreach(src_path IN ITEMS ${MY_PROJECT_SOURCES_FILES})
    get_filename_component(src_dir ${src_path} DIRECTORY)
    string(APPEND src_dir "/")
    list(APPEND MY_PROJECT_SOURCES_FILES_DIRS ${src_dir})
endforeach()
list(REMOVE_DUPLICATES MY_PROJECT_SOURCES_FILES_DIRS)

log_messages_optional(
    MESSAGES 
        "Source code directories for project '${PROJECT_NAME}':"
        ${MY_PROJECT_SOURCES_FILES_DIRS}
    DO_LOG ${VERBOSE_BUILD}
    FRAMED)

# ---------------------------------------------------------------------
# ------------------------- Setup executable --------------------------
# ---------------------------------------------------------------------

set(FENG_APP_NAME GlSandbox CACHE STRING "Name of the main target")
set(FENG_TARGET_NAME ${PROJECT_NAME}_${FENG_APP_NAME})

add_executable(${FENG_TARGET_NAME})
add_executable(${PROJECT_NAME}::${FENG_APP_NAME} ALIAS ${FENG_TARGET_NAME})
set_target_properties(${FENG_TARGET_NAME} PROPERTIES
      OUTPUT_NAME ${FENG_APP_NAME}
      EXPORT_NAME ${FENG_APP_NAME})

target_sources(${FENG_TARGET_NAME}
    PUBLIC 
        ${MY_PROJECT_SOURCES_FILES})

if(APPLE)
    # disable warnings aboug OpenGL deprecation of Mac
    target_compile_definitions(${FENG_TARGET_NAME} 
        PRIVATE 
            "GL_SILENCE_DEPRECATION")
endif()

target_include_directories(${FENG_TARGET_NAME} 
    PUBLIC 
        "${PROJECT_SOURCE_DIR}/Source"
        "${PROJECT_SOURCE_DIR}/Source/Thirdparty")

# Link libraries
target_link_libraries(${FENG_TARGET_NAME}
    PRIVATE
        ${THIRDPARTY_STATIC_LIBS}
        ${PLATFORM_SPECIFIC_LIBS})

set_target_properties(${FENG_TARGET_NAME} PROPERTIES FOLDER "Apps")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MY_PROJECT_SOURCES_FILES})

# TODO: m.alekseev make resources folder visible in project.
