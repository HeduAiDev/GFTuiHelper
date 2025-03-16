#include "tui_tool_sets.hpp"

namespace tui {
    namespace component {
        using namespace ftxui;

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
                        throw std::runtime_error("unsupported config type:" + typeid(config).name());
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
    }
}