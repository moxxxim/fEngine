cmake_minimum_required(VERSION 3.27)

project(Feng)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeUtils")

option(USE_GLM_LIB "Add GLM lib to project" OFF)
option(VERBOSE_BUILD "Use verbose logging when building project" ON)

set(THIRDPARTY_SRC_DIR_NAME "Thirdparty")
set(GLM_DIR_NAME "Glm")
set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum OS X deployment version")

# Do not allow build in source directory
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Specify different directory for binaries: not allowed to build into source directory.")
endif()

include(FengCMakeUtils)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# ---------------- Define build process -----------------

# Define required C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify where to put targets when built
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_build/")

# define variables
set(MY_SOURCES_DIR ${PROJECT_SOURCE_DIR}/Source)
set(THIRDPARTY_SOURCES_DIR ${PROJECT_SOURCE_DIR}/Source/Thirdparty)
set(SOURCE_FILES_EXTENSIONS "*.cpp" "*.h" "*.hpp")

# ---------------- Log build configuration -----------------
if(VERBOSE_BUILD)
    log_delimeters()
    message("Build project '${PROJECT_NAME}' from source: ${PROJECT_SOURCE_DIR}")
    message("Build project to directory: ${PROJECT_BINARY_DIR}")
    message("Project source code directory: ${MY_SOURCES_DIR}")
    message("Project source code files' extensions: ${SOURCE_FILES_EXTENSIONS}")
    log_delimeters()
endif()

# ---------------- Collect thirdparty libraries -----------------
set(THIRDPARTY_STATIC_LIBS)

# serach for Mac-specific libraries
if(APPLE)
    # Static.
    file(GLOB_RECURSE THIRDPARTY_STATIC_LIBS REGEX "*.a")
    list(FILTER THIRDPARTY_STATIC_LIBS INCLUDE REGEX ".*${THIRDPARTY_SRC_DIR_NAME}.*Libs.*")
endif(APPLE)

if(VERBOSE_BUILD)
    log_delimeters()
    list(JOIN THIRDPARTY_STATIC_LIBS "\n" THIRDPARTY_STATIC_LIBS_SEPARATED)
    message("Thirdparty static libraries:\n${THIRDPARTY_STATIC_LIBS_SEPARATED}")
    log_delimeters()
endif()

# ---------------- Collect platform-specific libraries -----------------
# collect Mac-specific libraries
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
endif(APPLE)

if(VERBOSE_BUILD)
    list(JOIN PLATFORM_SPECIFIC_LIBS "\n" PLATFORM_SPECIFIC_LIBS_SEPARATED)
    log_message_framed("Platform-specific libraries:\n${PLATFORM_SPECIFIC_LIBS_SEPARATED}")
endif()

# ---------------- Collect all source files for project -----------------
file(GLOB_RECURSE MY_PROJECT_SOURCES_FILES ${SOURCE_FILES_EXTENSIONS})
# remove cmake build directory from source files.
list(FILTER MY_PROJECT_SOURCES_FILES EXCLUDE REGEX ".*${PROJECT_BINARY_DIR}.*")

if(NOT USE_GLM_LIB)
    list(FILTER MY_PROJECT_SOURCES_FILES EXCLUDE REGEX ".*${THIRDPARTY_SRC_DIR_NAME}.*${GLM_DIR_NAME}.*")
endif()

foreach(src_path IN ITEMS ${MY_PROJECT_SOURCES_FILES})
    get_filename_component(src_dir ${src_path} DIRECTORY)
    string(APPEND src_dir "/")
    list(APPEND MY_PROJECT_SOURCES_FILES_DIRS ${src_dir})
endforeach()
list(REMOVE_DUPLICATES MY_PROJECT_SOURCES_FILES_DIRS)

if(VERBOSE_BUILD)
    list(JOIN MY_PROJECT_SOURCES_FILES_DIRS "\n" MY_PROJECT_SOURCES_FILES_DIRS_SEPARATED)
    log_message_framed("Source code directories for project '${PROJECT_NAME}':\n\n${MY_PROJECT_SOURCES_FILES_DIRS_SEPARATED}")
endif()

# ---------------- Setup executable -----------------

set(FENG_APP_NAME GlSandbox CACHE STRING "Name of the main target")
set(FENG_TARGET_NAME ${PROJECT_NAME}_${FENG_APP_NAME})

add_executable(${FENG_TARGET_NAME})
add_executable(${PROJECT_NAME}::${FENG_APP_NAME} ALIAS ${FENG_TARGET_NAME})
set_target_properties(${FENG_TARGET_NAME} PROPERTIES
      OUTPUT_NAME ${FENG_APP_NAME}
      EXPORT_NAME ${FENG_APP_NAME}
  )

target_sources(${FENG_TARGET_NAME}
    PUBLIC 
        ${MY_PROJECT_SOURCES_FILES})

if(APPLE)
    # disable warnings aboug OpenGL deprecation of Mac
    target_compile_definitions(${FENG_TARGET_NAME} 
        PRIVATE 
            "GL_SILENCE_DEPRECATION")
endif(APPLE)

# add the binary tree to the search path for include files
# can include files only using brackets(<>), but not with with quotes(""). Why?
target_include_directories(${FENG_TARGET_NAME} 
    PUBLIC 
        "${PROJECT_SOURCE_DIR}"
        "${MY_SOURCES_DIR}"
        "${THIRDPARTY_SOURCES_DIR}")       

# Link libraries
target_link_libraries(${FENG_TARGET_NAME}
    PRIVATE
        ${THIRDPARTY_STATIC_LIBS}
        ${PLATFORM_SPECIFIC_LIBS})

set_target_properties(${FENG_TARGET_NAME} PROPERTIES FOLDER "Apps")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MY_PROJECT_SOURCES_FILES})

# TODO: m.alekseev make resources folder visible in project.
