function (add_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE tui_static
                                    PRIVATE ftxui::screen
                                    PRIVATE ftxui::dom
                                    PRIVATE ftxui::component
                                    PUBLIC Threads::Threads
                                    PRIVATE pybind11::module)
    target_include_directories(${name} PRIVATE "csrc/include")
endfunction()

add_test(test_ref)
add_test(test_dropdown)
add_test(test_print_matrix)
