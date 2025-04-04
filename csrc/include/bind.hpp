#pragma once
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

class RefData {
public:
    RefData() 
        : _data(std::make_shared<std::unordered_map<std::string, std::string>>()) {
        // Initialize functions using std::function
        init();
    }

    void init() {
        __str__ = [this]() -> std::string {
            auto data = get_data();
            std::ostringstream oss;
            oss << "<RefData {";
            bool first = true;
            for (const auto& pair : *data) {
                if (!first) {
                    oss << ", ";
                }
                first = false;
                oss << "'" << pair.first << "': '" << pair.second << "'";
            }
            oss << "}>";
            return oss.str();
        };

        __setitem__ = [this](std::string key, std::string value) {
            set_data(key, value);
        };

        __getitem__ = [this](std::string key) -> std::string {
            auto data = get_data();
            if ((*data).find(key) == (*data).end()) {
                throw std::runtime_error("key not found: " + key);
            }
            return (*data)[key];
        };

        __delitem__ = [this](std::string key) {
            del_key(key);
        };

        __iter__ = [this]() {
            auto data = get_data();
            iter = data->begin();
        };

        __next__ = [this]() -> std::pair<std::string, std::string> {
            auto data = get_data();
            if (iter != data->end()) {
                auto current = *iter;
                ++iter;
                return current;
            } else {
                throw std::runtime_error("StopIteration");
            }
        };

        __contains__ = [this](std::string key) -> bool {
            auto data = get_data();
            return data->find(key) != data->end();
        };

        keys = [this]() -> std::vector<std::string> {
            auto data = get_data();
            std::vector<std::string> keys_list;
            for (const auto& pair : *data) {
                keys_list.push_back(pair.first);
            }
            return keys_list;
        };

        values = [this]() -> std::vector<std::string> {
            auto data = get_data();
            std::vector<std::string> values_list;
            for (const auto& pair : *data) {
                values_list.push_back(pair.second);
            }
            return values_list;
        };

        get_data = [this]() -> std::shared_ptr<std::unordered_map<std::string, std::string>> {
            return _data;
        };
        set_data = [this](std::string key, std::string value) {
            (*_data)[key] = value;
        };
        del_key = [this](std::string key) {
            if ((*_data).find(key) != (*_data).end()) {
                (*_data).erase(key);
            }
        };
    }

    // Publicly accessible functions using std::function
    std::function<std::string()> __str__;
    std::function<void(std::string, std::string)> __setitem__;
    std::function<std::string(std::string)> __getitem__;
    std::function<void(std::string)> __delitem__;
    std::function<void()> __iter__;
    std::function<std::pair<std::string, std::string>()> __next__;
    std::function<bool(std::string)> __contains__;
    std::function<std::vector<std::string>()> keys;
    std::function<std::vector<std::string>()> values;
    std::function<std::shared_ptr<std::unordered_map<std::string, std::string>>()> get_data;
    std::function<void(std::string, std::string)> set_data;
    std::function<void(std::string)> del_key;

private:
    std::shared_ptr<std::unordered_map<std::string, std::string>> _data;
    std::unordered_map<std::string, std::string>::iterator iter;  // Iterator for iteration
};

void init_refdata(py::module& m) {
    py::class_<RefData, std::shared_ptr<RefData>>(m, "RefData")
        .def(py::init<>())
        .def("__str__", [](RefData& self) { return self.__str__(); })
        .def("__setitem__", [](RefData& self, std::string key, std::string value) { self.__setitem__(key, value); })
        .def("__getitem__", [](RefData& self, std::string key) { return self.__getitem__(key); })
        .def("__delitem__", [](RefData& self, std::string key) { self.__delitem__(key); })
        .def("__iter__", [](RefData& self) -> RefData& {
            self.__iter__();
            return self;
        })
        .def("__next__", [](RefData& self) { return self.__next__(); })
        .def("__repr__", [](RefData& self) { return self.__str__(); })
        .def("__contains__", [](RefData& self, std::string key) { return self.__contains__(key); })
        .def("keys", [](RefData& self) { return self.keys(); })
        .def("values", [](RefData& self) { return self.values(); });
}


class PyComponentBase : public ComponentBase {
public:
    using ComponentBase::ComponentBase;

    // Override virtual functions to allow Python-side overrides
    Element Render() override {
        PYBIND11_OVERRIDE_PURE(Element, ComponentBase, Render);
    }

    bool OnEvent(Event event) override {
        PYBIND11_OVERRIDE_PURE(bool, ComponentBase, OnEvent, event);
    }

    void OnAnimation(animation::Params& params) override {
        PYBIND11_OVERRIDE_PURE(void, ComponentBase, OnAnimation, params);
    }

    Component ActiveChild() override {
        PYBIND11_OVERRIDE_PURE(Component, ComponentBase, ActiveChild);
    }

    bool Focusable() const override {
        PYBIND11_OVERRIDE_PURE(bool, ComponentBase, Focusable);
    }

    void SetActiveChild(ComponentBase* child) override {
        PYBIND11_OVERRIDE_PURE(void, ComponentBase, SetActiveChild, child);
    }
};

void init_component(py::module& m) {
    py::class_<ComponentBase, PyComponentBase, std::shared_ptr<ComponentBase>>(m, "ComponentBase")
        .def(py::init<>())  // Default constructor
        .def(py::init<Components>(), py::arg("children"))  // Constructor with children

        // Bind non-virtual member functions
        .def("Parent", &ComponentBase::Parent)
        .def("ChildAt", &ComponentBase::ChildAt, py::arg("i"))
        .def("ChildCount", &ComponentBase::ChildCount)
        .def("Index", &ComponentBase::Index)
        .def("Add", &ComponentBase::Add, py::arg("children"))
        .def("Detach", &ComponentBase::Detach)
        .def("DetachAllChildren", &ComponentBase::DetachAllChildren)
        .def("Active", &ComponentBase::Active)
        .def("Focused", &ComponentBase::Focused)
        .def("TakeFocus", &ComponentBase::TakeFocus)

        // Bind virtual member functions
        .def("Render", &ComponentBase::Render)
        .def("OnEvent", &ComponentBase::OnEvent, py::arg("event"))
        .def("OnAnimation", &ComponentBase::OnAnimation, py::arg("params"))
        .def("ActiveChild", &ComponentBase::ActiveChild)
        .def("Focusable", &ComponentBase::Focusable)
        .def("SetActiveChild", (void (ComponentBase::*)(ComponentBase*)) &ComponentBase::SetActiveChild, py::arg("child"))
        .def("SetActiveChildShared", (void (ComponentBase::*)(Component)) &ComponentBase::SetActiveChild, py::arg("child"))
        ;
}