function (add_test name)
    if(NOT DEFINED ENABLE_TEST)
        return()
    endif()
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE tui_static
                                    PRIVATE ftxui::screen
                                    PRIVATE ftxui::dom
                                    PRIVATE ftxui::component
                                    PUBLIC Threads::Threads
                                    PRIVATE pybind11::module)
    target_include_directories(${name} PRIVATE "csrc/include")
endfunction()

add_test(test_dropdown)
add_test(test_print_matrix)
add_test(test_3block)
