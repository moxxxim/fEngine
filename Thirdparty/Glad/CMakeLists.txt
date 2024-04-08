add_library(GladLibInterface INTERFACE)

target_sources(GladLibInterface 
    INTERFACE
        CMakeLists.txt
        include/glad/glad.h
        include/KHR/khrplatform.h
        src/glad.c)

target_include_directories(GladLibInterface
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>)