#include "tui_tool_sets.hpp"
#include <unordered_map>
#include <map>
#include <nlohmann/json.hpp>
namespace tui {
    namespace component {
        using namespace ftxui;
        using json = nlohmann::json;

        InputFormBase::InputFormBase(InputFormOptions& options): InputFormOptions(options) {
            Component vertical = Container::Vertical({});
            for (int i = 0; i < options.elements_config.size(); i++) {
                components_.push_back(renderFormRow(options.elements_config[i]));
                vertical->Add(Container::Horizontal(components_[i]));
            }
            Add(vertical);
        };

        Element InputFormBase::Render() {
            std::vector<Elements> lines_;
            for (auto &row : components_) {
                Elements line;
                for (auto component : row) {
                    line.push_back(component -> Render());
                }
                lines_.push_back(line);
            }
            return gridbox(lines_);
        }

       std::vector<Component> InputFormBase::renderFormRow(ElementRowConfig row) {
            std::vector<Component> row_components;
            for (std::variant<TextInputElementConfig, SelectInputElementConfig> var_config : row) {
                Component input;
                BaseElementConfig base_config;
                // auto base_config = std::get<BaseElementConfig>(config);
                std::visit([&input, &base_config](auto&& config) {
                    using T = std::decay_t<decltype(config)>;
                    if constexpr (std::is_base_of_v<BaseElementConfig, T>) {
                        base_config = static_cast<BaseElementConfig>(config);
                        if constexpr (std::is_same_v<T, TextInputElementConfig>) {
                            if (config.input_type != InputType::Text && config.input_type != InputType::Password && config.input_type != InputType::Number) {
                                throw std::runtime_error("TextInputElementConfig's input_type should in one of [InputType::Text, InputType::Password, InputType::Number] but now is:" + std::to_string(static_cast<int>(config.input_type)));
                            }
                            if (config.input_type == InputType::Password) {
                                config.password = true;
                            }
                            input = Input(config);
                        } else if constexpr (std::is_same_v<T, SelectInputElementConfig>) {
                            if (config.input_type != InputType::Select) {
                                throw std::runtime_error("SelectInputElement's input_type should use InputType::Select but now is:" + std::to_string(static_cast<int>(config.input_type)));
                            }
                            input = Dropdown(config);
                        }
                    } else {
                        throw std::runtime_error(std::string("unsupported config type:") + typeid(config).name());
                    }
                }, var_config);
               
                input = setWidth(input, default_max_input_width, default_min_input_width);
                if (base_config.max_input_width + base_config.min_input_width >= 0) {
                    input = setWidth(input, base_config.max_input_width, base_config.min_input_width);
                }
                if (base_config.input_type == InputType::Number) {
                    input |= CatchEvent([](Event e) {
                        return e.is_character() && !isdigit(e.character()[0]);
                    });
                }
                Component input_r = Renderer(input, [base_config, &default_input_style = this->default_input_style, input] {
                    return base_config.input_style ? base_config.input_style(input->Render()) : default_input_style(input->Render());
                });
                Element label = text(base_config.label());
                label = setWidth(label, default_max_label_width, default_min_label_width);
                if (base_config.max_label_width + base_config.min_label_width >= 0) {
                    label = setWidth(label, base_config.max_label_width, base_config.min_label_width);
                }
                Component label_r = Renderer([base_config, &default_label_style = this->default_label_style, label] {
                    return base_config.label_style ? base_config.label_style(label) : default_label_style(label);
                });
                row_components.push_back(label_r);
                row_components.push_back(input_r);
            }
            return row_components;
        }

        Component InputFormBase::setWidth(Component component, int max_width, int min_width) {
            return component | size(WIDTH, LESS_THAN, max_width) | size(WIDTH, GREATER_THAN, min_width);
        }
        Element InputFormBase::setWidth(Element element, int max_width, int min_width) {
            return element | size(WIDTH, LESS_THAN, max_width) | size(WIDTH, GREATER_THAN, min_width);
        }

        Component InputForm(std::vector<InputFormOptions::ElementRowConfig> elements_config, InputFormOptions options) {
            options.elements_config = std::move(elements_config);
            return std::make_shared<InputFormBase>(options);
        }

        Component InputFormCreateFromJsonStr(std::string json_str, \
            std::unordered_map<std::string, StringRef> &input_text_map, std::unordered_map<std::string, Ref<int>>& input_select_index_map, \
            std::unordered_map<std::string, std::vector<std::string>>& input_select_entries_map, std::function<Element(InputState)> text_input_transform, \
            std::function<Element(bool open, Element checkbox, Element radiobox)> select_input_transform) {
            std::function<Element(InputState)> default_input_transform = [](InputState state)
            {
                state.element |= borderRounded;
                state.element |= color(Color::White);

                if (state.is_placeholder)
                {
                    state.element |= dim;
                }

                if (state.focused)
                {
                    state.element |= bgcolor(Color::Black);
                }

                if (state.hovered)
                {
                    state.element |= bgcolor(Color::GrayDark);
                }

                return state.element;
            };
            if (text_input_transform == nullptr) {
                text_input_transform = default_input_transform;
            }

            int default_max_input_width = 30;
            int default_min_input_width = 25;
            int default_max_input_height = 5;
            int default_max_select_input_height = 15;
            std::string default_align_h_label = "right";
            std::string default_align_v_label = "center";

            auto default_select_input_style = [](Element ele)
            { return ele | size(WIDTH, LESS_THAN, 30) | size(WIDTH, GREATER_THAN, 25) | size(HEIGHT, LESS_THAN, 15); };
            auto default_input_style = [](Element ele)
            { return ele | size(WIDTH, LESS_THAN, 30) | size(WIDTH, GREATER_THAN, 25) | size(HEIGHT, LESS_THAN, 5); };
            auto default_label_style = [](Element ele)
            { return ele | align_right | vcenter; };
            
            auto text_input_cell = [&](std::string label, StringRef& constent, std::string placeholder, InputType inputType, \
                std::function<Element(Element)> label_style, std::function<Element(Element)> input_style, std::function<Element(InputState)> transform)
            {
                TextInputElementConfig input_config;
                input_config.label = label;
                input_config.input_type = inputType;
                input_config.placeholder = placeholder;
                input_config.content = constent;
                input_config.transform = transform;
                input_config.input_style = input_style;
                input_config.label_style = label_style;
                input_config.multiline = false;
                return input_config;
            };

            auto select_cell = [&](std::string label, Ref<int>& selected, std::vector<std::string> entries, InputType inputType, \
                std::function<Element(Element)> label_style, std::function<Element(Element)> input_style, std::function<Element(bool open, Element checkbox, Element radiobox)> transform = nullptr)
            {
                SelectInputElementConfig input_config;
                input_config.label = label;
                input_config.input_type = inputType;
                input_config.radiobox.entries = std::move(entries);
                input_config.radiobox.selected = selected;
                input_config.transform = transform;
                input_config.input_style = input_style;
                input_config.label_style = label_style;
                return input_config;
            };

            auto parsed_json = json::parse(json_str);
            
            std::unordered_map<std::string, int> input_type_map = {
                {"text", 0},
                {"password", 1},
                {"number", 2},
                {"select", 3}
            };
            std::vector<InputFormOptions::ElementRowConfig> form_rows;

            auto get_style = [=] (json element, std::function<Element(Element)> default_style = nullptr) {
                std::function<Element(Element)> style = [](Element ele) { return ele; };
                if (default_style != nullptr) { style = default_style; }
                std::function<Element(Element)> helper = style;
                if (element.contains("min_width")) {
                    style = [=](Element ele) { return helper(ele) | size(WIDTH, GREATER_THAN, element["min_width"]); };
                }
                helper = style;
                if (element.contains("max_width")) {
                    style = [=](Element ele) { return helper(ele) | size(WIDTH, LESS_THAN, element["max_width"]); };
                }
                helper = style;
                if (element.contains("min_height")) {
                    style = [=](Element ele) { return helper(ele) | size(HEIGHT, GREATER_THAN, element["min_height"]); };
                }
                helper = style;
                if (element.contains("max_height")) {
                    style = [=](Element ele) { return helper(ele) | size(HEIGHT, LESS_THAN, element["max_height"]); };
                }
                helper = style;
                if (element.contains("align_h") && element["align_h"] == "center") {
                    style = [=](Element ele) { return helper(ele) | hcenter; };
                } else if (element.contains("align_h") && element["align_h"] == "right") {
                    style = [=](Element ele) { return helper(ele) | align_right; };
                } else if (element.contains("align_h") && element["align_h"] == "left") {
                    style = [=](Element ele) { return helper(ele) | [](Element child){ return hbox(std::move(child), filler()); }; };
                }
                helper = style;
                if (element.contains("align_v") && element["align_v"] == "center") {
                    style = [=](Element ele) { return helper(ele) | vcenter; };
                } else if (element.contains("align_v") && element["align_v"] == "up") {
                    style = [=](Element ele) { return helper(ele) | [](Element child){ return vbox(std::move(child), filler()); }; };
                } else if (element.contains("align_v") && element["align_v"] == "bottom") {
                    style = [=](Element ele) { return helper(ele) | [](Element child){ return vbox(filler(), std::move(child)); }; };
                }
                return style;
            };
            
            for (auto& item : parsed_json) {
                InputFormOptions::ElementRowConfig cols;
                auto elements = item.is_array() ? item : json::array({item});
                for (auto& cols_item : elements) {
                    if (cols_item["input_type"] == "text" || cols_item["input_type"] == "password" || cols_item["input_type"] == "number") {
                        input_text_map[cols_item["label"]] = StringRef();
                        std::function<Element(Element)> label_style = nullptr;
                        std::function<Element(Element)> input_style = nullptr;
                        if (cols_item.contains("label_style")) {
                            label_style = get_style(cols_item["label_style"], default_label_style);
                        }
                        if (cols_item.contains("input_style")) {
                            input_style = get_style(cols_item["input_style"], default_input_style);
                        }
                        cols.push_back(
                            text_input_cell(cols_item["label"], input_text_map[cols_item["label"]], cols_item["placeholder"], (InputType)input_type_map[cols_item["input_type"]], label_style, input_style, text_input_transform)
                        );
                    } else if (cols_item["input_type"] == "select") {
                        int default_index = 0;
                        if (cols_item.contains("default_index")) {
                            default_index = cols_item["default_index"];
                        }
                        input_select_index_map[cols_item["label"]] = Ref<int>(default_index);
                        input_select_entries_map[cols_item["label"]] = cols_item["entries"];
                        std::function<Element(Element)> label_style = nullptr;
                        std::function<Element(Element)> input_style = nullptr;
                        if (cols_item.contains("label_style")) {
                            label_style = get_style(cols_item["label_style"], default_label_style);
                        }
                        if (cols_item.contains("input_style")) {
                            input_style = get_style(cols_item["input_style"], default_select_input_style);
                        }
                        cols.push_back(
                            select_cell(cols_item["label"], input_select_index_map[cols_item["label"]], input_select_entries_map[cols_item["label"]], (InputType)input_type_map[cols_item["input_type"]], label_style, input_style, select_input_transform)
                        );
                    }
                }
                form_rows.push_back(cols);
            }
            Component form = InputForm(form_rows);
            return form;
        }
    }
}