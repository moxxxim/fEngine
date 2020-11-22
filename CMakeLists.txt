cmake_minimum_required(VERSION 3.0)

# set the project name
set(PROJECT_NAME "OpenGL_Tutorial")
project(${PROJECT_NAME})

message(${PROJECT_SOURCE_DIR})
message(${PROJECT_BINARY_DIR})
# do not allow build in source directory
IF(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "Specify different directory for binaries: not allowed to build into source directory.")
ENDIF()

# ---------------- Define utility functions for build process -----------------

# define function to generate gropus/filters in IDE corresponding to the folders structure in file system
# source: https://stackoverflow.com/questions/31422680/how-to-set-visual-studio-filters-for-nested-sub-directory-using-cmake
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

function(log_delimeters)
    message("\n-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-\n")
endfunction(log_delimeters)

# helper-function to make build logging more eye-catching.
function(log_message_framed)
    log_delimeters()
    message(${ARGN})
    log_delimeters()
endfunction(log_message_framed)

# ---------------- Define build process -----------------

# Define required C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify where to put targets when built
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/_build/")

# define variables
set(MY_SOURCES_DIR ${PROJECT_SOURCE_DIR}/Source)
set(THIRDPARTY_SOURCES_DIR ${PROJECT_SOURCE_DIR}/Source/Thirdparty)
set(SOURCE_FILES_EXTENSIONS "*.cpp" "*.h" "*.hpp")

# ---------------- Log build configuration -----------------
log_delimeters()
message("Build project '${PROJECT_NAME}' from source: ${PROJECT_SOURCE_DIR}")
message("Build project to directory: ${PROJECT_BINARY_DIR}")
message("Project source code directory: ${MY_SOURCES_DIR}")
message("Project source code files' extensions: ${SOURCE_FILES_EXTENSIONS}")
log_delimeters()

# ---------------- Collect thirdparty libraries -----------------
set(THIRDPARTY_LIBS)
set(THIRDPARTY_LIBS_DIRS)

# serach for Mac-specific libraries
IF(APPLE)
    file(GLOB_RECURSE THIRDPARTY_LIBS REGEX "*.a")
    list(FILTER THIRDPARTY_LIBS INCLUDE REGEX ".*Thirdparty.*Libs.*")
ENDIF(APPLE)

# evaluate thirdparty libs' directories
foreach(lib_path IN ITEMS ${THIRDPARTY_LIBS})
    get_filename_component(lib_dir ${lib_path} DIRECTORY)
    string(APPEND lib_dir "/")
    list(APPEND THIRDPARTY_LIBS_DIRS ${lib_dir})
endforeach()

# log thirdparty libs and directories
log_delimeters()
list(JOIN THIRDPARTY_LIBS "\n" THIRDPARTY_LIBS_SEPARATED)
message("Thirdparty libraries:\n${THIRDPARTY_LIBS_SEPARATED}")

list(JOIN THIRDPARTY_LIBS_DIRS "\n" THIRDPARTY_LIBS_DIRS_SEPARATED)
message("\nThirdparty lib directories:\n${THIRDPARTY_LIBS_DIRS_SEPARATED}")
log_delimeters()

# ---------------- Collect platform-specifil libraries -----------------
# collect Mac-specific libraries
set(PLATFORM_SPECIFIC_LIBS)
IF(APPLE)
    find_library(MAC_COCOA_LIB Cocoa REQUIRED)
    find_library(MAC_IO_KIT_LIB IOKit REQUIRED)
    find_library(MAC_CORE_VIDEO_LIB CoreVideo REQUIRED)
    find_library(MAC_OPEN_GL_LIB OpenGL REQUIRED)

    set(PLATFORM_SPECIFIC_LIBS 
                    ${MAC_COCOA_LIB}
                    ${MAC_IO_KIT_LIB}
                    ${MAC_CORE_VIDEO_LIB}
                    ${MAC_OPEN_GL_LIB})
ENDIF(APPLE)

list(JOIN PLATFORM_SPECIFIC_LIBS "\n" PLATFORM_SPECIFIC_LIBS_SEPARATED)
log_message_framed("Platform-specific libraries:\n${PLATFORM_SPECIFIC_LIBS_SEPARATED}")


# ---------------- Collect all source files for project -----------------
file(GLOB_RECURSE MY_PROJECT_SOURCES_FILES ${SOURCE_FILES_EXTENSIONS})
# remove cmake build directory from source files.
list(FILTER MY_PROJECT_SOURCES_FILES EXCLUDE REGEX ".*${PROJECT_BINARY_DIR}.*")
list(JOIN MY_PROJECT_SOURCES_FILES "\n" MY_SOURCES_FILES_SEPARATED)
log_message_framed("Source code files for project '${PROJECT_NAME}':\n\n${MY_SOURCES_FILES_SEPARATED}")

# ---------------- Setup executable -----------------
add_executable(OpenGLWindow)
target_sources(OpenGLWindow PUBLIC ${MY_PROJECT_SOURCES_FILES})

IF(APPLE)
    # disable warnings aboug OpenGL deprecation of Mac
    target_compile_definitions(OpenGLWindow PRIVATE "GL_SILENCE_DEPRECATION")
ENDIF(APPLE)

# add the binary tree to the search path for include files
# can include files only using brackets(<>), but not with with quotes(""). Why?
target_include_directories(OpenGLWindow PUBLIC 
                            "${PROJECT_SOURCE_DIR}"
                            "${MY_SOURCES_DIR}"
                            "${THIRDPARTY_SOURCES_DIR}"
                            )       

# Link libraries
target_link_libraries(OpenGLWindow
                        ${THIRDPARTY_LIBS}
                        ${PLATFORM_SPECIFIC_LIBS}
                        )

# ---------------- Make project groups -----------------
assign_source_group(${MY_PROJECT_SOURCES_FILES})

# TODO: m.alekseev make resources folder visible in project.