include_guard()

function(log_delimeters)
    message("\n-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-\n")
endfunction()

# helper-function to make build logging more eye-catching.
function(log_message_framed)
    log_delimeters()
    message(${ARGN})
    log_delimeters()
endfunction()

function(demoArgs)
      set(noValues     "")
      set(singleValues SPECIAL)
      set(multiValues  EXTRAS)
      cmake_parse_arguments(
          PARSE_ARGV 0
          ARG
          "${noValues}" "${singleValues}" "${multiValues}")

      message("Left-over args: ${ARG_UNPARSED_ARGUMENTS}")
      foreach(arg IN LISTS ARG_UNPARSED_ARGUMENTS)
          message("${arg}")
      endforeach()
endfunction()

function(log_messages_optional)
    set(prefix ARG)
    set(options      FRAMED)
    set(singleValues DO_LOG)
    set(multiValues  MESSAGES)

    cmake_parse_arguments(
        PARSE_ARGV 0
        ${prefix}
        "${options}" "${singleValues}" "${multiValues}")

    # Check if log should be showed
    set(showLog TRUE)
    if((DEFINED ARG_DO_LOG) AND (NOT ARG_DO_LOG))
        set(showLog FALSE)
    elseif("DO_LOG" IN_LIST ARG_KEYWORDS_MISSING_VALUES)
        set(showLog TRUE)
    endif()

    if(showLog)
        set(showFrame FALSE)
        if(DEFINED ARG_FRAMED)
            set(showFrame TRUE)
        endif()

        if(DEFINED ARG_MESSAGES)
            foreach(msg IN LISTS ARG_MESSAGES)
                list(APPEND messages ${msg})
            endforeach()
        endif()

        if(showFrame AND messages)
            log_delimeters()
        endif()

        foreach(msg IN LISTS messages)
            message("${msg}")
        endforeach()

        if(showFrame AND messages)
            log_delimeters()
        endif()
    endif()
endfunction()