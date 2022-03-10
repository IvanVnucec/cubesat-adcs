function(ADD_CHECK_CLANG_FORMAT_TARGET)

    add_custom_target(check_clang_format
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND bash ./scripts/check-clang-format.sh
)

endfunction()
