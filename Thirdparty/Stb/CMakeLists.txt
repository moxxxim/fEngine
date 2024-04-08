add_library(StbLibInterface INTERFACE)

target_sources(StbLibInterface
    INTERFACE
        CMakeLists.txt
        stb_image.h)

target_include_directories(StbLibInterface
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR})