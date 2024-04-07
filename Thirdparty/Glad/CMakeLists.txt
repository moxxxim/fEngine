add_library(GladLibInterface INTERFACE)

target_sources(GladLibInterface 
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/glad/glad.h
        ${CMAKE_CURRENT_LIST_DIR}/include/KHR/khrplatform.h
        ${CMAKE_CURRENT_LIST_DIR}/src/glad.c)

target_include_directories(GladLibInterface
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/>)