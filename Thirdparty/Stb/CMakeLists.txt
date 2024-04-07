set(CMAKE_COMPILE_WARNING_AS_ERROR NO)

add_library(StbLibInterface INTERFACE)

target_sources(StbLibInterface 
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/stb_image.h)

target_include_directories(StbLibInterface
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR})