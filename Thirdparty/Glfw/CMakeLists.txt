target_sources(Feng_GlSandbox
    PRIVATE
        glfw3.h
        glfw3native.h)

set(glfwLibsDebug "")
set(glfwLibsRelease "")
if(APPLE)
    set(glfwLibsDebug ${CMAKE_CURRENT_LIST_DIR}/Libs/Mac/Debug/libglfw3.a)
    set(glfwLibsRelease ${CMAKE_CURRENT_LIST_DIR}/Libs/Mac/Release/libglfw3.a)
elseif(WIN32)
    set(glfwLibsDebug ${CMAKE_CURRENT_LIST_DIR}/Libs/Win/Debug/glfw3.lib)
    set(glfwLibsRelease ${CMAKE_CURRENT_LIST_DIR}/Libs/Win/Release/glfw3.lib)
endif()

log_messages_optional(
    MESSAGES 
        "Glfw libs (DEBUG):"
        ${glfwLibsDebug}
    DO_LOG ${VERBOSE_BUILD}
    FRAME_UP)
    
log_messages_optional(
    MESSAGES 
        "Glf libs (RELEASE):"
        ${glfwLibsRelease}
    DO_LOG ${VERBOSE_BUILD}
    FRAME_DOWN)

target_include_directories(Feng_GlSandbox
      PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

target_link_libraries(Feng_GlSandbox
    PRIVATE
        $<$<CONFIG:Debug>:${glfwLibsDebug}>
        $<$<CONFIG:Release>:${glfwLibsRelease}>)