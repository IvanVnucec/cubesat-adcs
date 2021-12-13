set(clang_format clang-format)

function(ADD_CLANG_FORMAT_TARGET)

    add_custom_target(clang_format
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ./scripts/clang-format-run.sh .
)

endfunction()
