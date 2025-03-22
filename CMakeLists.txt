cmake_minimum_required (VERSION 3.18)
project(GFTuiHelper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# use MT runtime for win
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMAKE_MSVC_RUNTIME_LIBRARY: ${CMAKE_MSVC_RUNTIME_LIBRARY}")
find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
# find_package(OpenMP)
find_package(Threads REQUIRED)

# ftxui
add_subdirectory(3rd/FTXUI EXCLUDE_FROM_ALL)
# add_subdirectory(3rd/FTXUI)
# pybind11
add_subdirectory(3rd/pybind11 EXCLUDE_FROM_ALL)
add_subdirectory(3rd/nlohmann EXCLUDE_FROM_ALL)
add_subdirectory(tests)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
# static lib tui_static
file(GLOB src_components CONFIGURE_DEPENDS "csrc/Component/*.cpp")
file(GLOB src_runables CONFIGURE_DEPENDS "csrc/runable/*.cpp")
add_library(tui_static STATIC  ${src_components} ${src_runables})
target_link_libraries(tui_static PRIVATE ftxui::screen
                                    PRIVATE ftxui::dom
                                    PRIVATE ftxui::component
                                    PUBLIC Threads::Threads
                                    PUBLIC nlohmann_json::nlohmann_json
                                    )
target_include_directories(tui_static PUBLIC "csrc/include")
include(tools/merge_static_libs.cmake)
# protable static lib (merged all in one) tui_toolsets 
merge_static_libs(tui_toolsets tui_static ftxui::screen ftxui::dom ftxui::component)
# python c extension tui_toolsets_runables
add_library(_C MODULE csrc/pybind.cpp)
target_include_directories(_C PRIVATE "csrc/include")
target_link_libraries(_C PRIVATE tui_static
                                    PRIVATE ftxui::screen
                                    PRIVATE ftxui::dom
                                    PRIVATE ftxui::component
                                    PUBLIC Threads::Threads
                                    PRIVATE pybind11::module)
set_target_properties(_C PROPERTIES SUFFIX ${PYTHON_MODULE_EXTENSION})
set_target_properties(_C PROPERTIES PREFIX "")
# install
install(TARGETS _C DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/src/tui_helper COMPONENT PyExtension)

install(TARGETS tui_toolsets DESTINATION lib COMPONENT CPPInterface)
install(DIRECTORY csrc/include/ DESTINATION include COMPONENT CPPInterface
        PATTERN "color_info_*" EXCLUDE)
