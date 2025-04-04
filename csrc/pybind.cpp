#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include "tui_tool_sets_runable.hpp"
#include "tui_tool_sets.hpp"
#include "bind.hpp"
using namespace tui::runable;
using namespace ftxui;
namespace py = pybind11;

template<typename T>
void PyPrint_matrix_(py::array_t<T> xs) {
    py::buffer_info info = xs.request();
    auto ptr = static_cast<T *>(info.ptr);
    if (info.ndim != 2) {
        throw std::runtime_error("only 2d arrays are supported");
    }
    int rows = info.shape[0];
    int cols = info.shape[1];

    print_matrix(ptr, rows, cols);
}

template<typename T>
void PyDiff_(py::array_t<T> a, py::array_t<T> b, double accuracy = 1e-3) {
    py::buffer_info info_a = a.request();
    py::buffer_info info_b = b.request();
    auto ptr_a = static_cast<T *>(info_a.ptr);
    auto ptr_b = static_cast<T *>(info_b.ptr);
    
    if (info_a.ndim != info_b.ndim || info_a.ndim != 2) {
        throw std::runtime_error("only 2d arrays are supported");
    }

    if (info_a.shape[0] != info_b.shape[0] || info_a.shape[1] != info_b.shape[1]) {
        throw std::runtime_error("matrix a and b must have the same shape");
    }

    int rows = info_a.shape[0];
    int cols = info_a.shape[1];
    diff(ptr_a, ptr_b, rows, cols, accuracy);
}

void PyStartMenuLoop(Component component) {
    tui::component::start_menu_loop(component);
}

Component PyCreateForm(std::string config, std::function<std::string(std::string, std::string)> on_change) {
    return tui::component::InputFormCreateFromJsonStr(config, on_change);
}

Component PyCreateFormWithData(std::shared_ptr<RefData> data, std::string config, std::function<std::string(std::string, std::string)> on_change) {
    std::shared_ptr<std::unordered_map<std::string, std::string>> input_text_map = std::make_shared<std::unordered_map<std::string, std::string>>();
    std::shared_ptr<std::unordered_map<std::string, int>> input_select_index_map = std::make_shared<std::unordered_map<std::string, int>>();
    data->get_data = [input_text_map, input_select_index_map]() -> std::shared_ptr<std::unordered_map<std::string, std::string>> {
        std::shared_ptr<std::unordered_map<std::string, std::string>> _data = std::make_shared<std::unordered_map<std::string, std::string>>();
        for (const auto& pair : (*input_text_map)) {
            (*_data)[pair.first] = pair.second;
        }
        for (const auto& pair : (*input_select_index_map)) {
            (*_data)[pair.first] = std::to_string(pair.second);
        }
        return _data;
    };
    data->set_data = [input_text_map, input_select_index_map](std::string key, std::string value) {
        if ((*input_text_map).find(key) != (*input_text_map).end()) {
            (*input_text_map)[key] = value;
            return;
        }
        if ((*input_select_index_map).find(key) != (*input_select_index_map).end()) {
            int index = std::stoi(value);
            (*input_select_index_map)[key] = index;
            return;
        }
        throw std::runtime_error("key not found"+ utils::map_repr(input_text_map.get()) + utils::map_repr(input_select_index_map.get()));
    };

    data->del_key = [input_text_map, input_select_index_map](std::string key) {
        if ((*input_text_map).find(key) != (*input_text_map).end()) {
            (*input_text_map).erase(key);
        }else if ((*input_select_index_map).find(key) != (*input_select_index_map).end()) {
            (*input_select_index_map).erase(key);
        }
    };
    return tui::component::InputFormCreateFromJsonStr(config, on_change, input_text_map, input_select_index_map);
}

Component PyCreateButton(std::string label, std::function<void()> on_click, std::string style = "default") {
    ButtonOption option;
    if (style == "default") {
       option = ButtonOption::Animated();
    } else if (style == "ascii") {
       option = ButtonOption::Ascii();
    } else {
       throw std::runtime_error("unknown style: " + style);
    }
    return Button(label, on_click, option);
}

PYBIND11_MODULE(_C, m) {
    m.doc() = "A set of useful TUI tools ";
    init_refdata(m);
    init_component(m);
    m.def("print_matrix_double", &PyPrint_matrix_<double>);
    m.def("print_matrix_float", &PyPrint_matrix_<float>);
    #ifdef __CUDA__
    m.def("print_matrix_half", &PyPrint_matrix_<half>);
    #endif
    m.def("diff_double", &PyDiff_<double>);
    m.def("diff_float", &PyDiff_<float>);
    #ifdef __CUDA__
    m.def("diff_half", &PyDiff_<half>);
    #endif
    m.def("start_menu_loop", &PyStartMenuLoop);
    m.def("create_form", &PyCreateForm);
    m.def("create_form_with_data", &PyCreateFormWithData);
    m.def("create_button", &PyCreateButton);
    
}