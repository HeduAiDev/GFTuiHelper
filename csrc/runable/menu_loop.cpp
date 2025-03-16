#include "tui_tool_sets.hpp"


namespace tui {
    namespace runable {
        using namespace ftxui;
        using namespace tui::component;
        void start_menu_loop()
        {
            auto screen = ScreenInteractive::Fullscreen();
            /////////////////////////////////////////////////////////////////////
            // lowcode command builder
            /////////////////////////////////////////////////////////////////////

            auto input_transform = [](InputState state)
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

            auto select_input_style = [](Element ele)
            { return ele | size(WIDTH, LESS_THAN, 30) | size(WIDTH, GREATER_THAN, 25) | size(HEIGHT, LESS_THAN, 15); };
            auto label_style = [](Element ele)
            { return ele | align_right | vcenter; };
            
            auto text_input_cell = [&](std::string label, StringRef& constent, std::string placeholder, InputType inputType, std::function<Element(InputState)> transform)
            {
                TextInputElementConfig input_config;
                input_config.label = label;
                input_config.input_type = inputType;
                input_config.placeholder = placeholder;
                input_config.content = constent;
                input_config.transform = transform;
                input_config.multiline = false;
                return input_config;
            };

            auto select_cell = [&](std::string label, Ref<int>& selected, std::vector<std::string> entries, InputType inputType, std::function<Element(bool open, Element checkbox, Element radiobox)> transform = nullptr)
            {
                SelectInputElementConfig input_config;
                input_config.label = label;
                input_config.input_type = inputType;
                input_config.radiobox.entries = std::move(entries);
                input_config.radiobox.selected = selected;
                input_config.transform = transform;
                input_config.input_style = select_input_style;
                input_config.label_style = label_style;
                return input_config;
            };



            StringRef config_gf_env;
            StringRef config_gf_model;
            Ref<int> config_gf_task_type = 1;
            std::vector<std::string> config_gf_task_type_entries = {"train", "mindie_infer", "vllm_infer"};

            Component command_form = InputForm({
                {
                   text_input_cell("GF_ENV: ", config_gf_env, "right click to paste", tui::component::InputType::Text, input_transform),

                },
                {
                    text_input_cell("GF_MODEL: ", config_gf_model, "right click to paste", tui::component::InputType::Text, input_transform),
                },
                {
                    select_cell("GF_TASK_TYPE: ", config_gf_task_type, config_gf_task_type_entries, tui::component::InputType::Select),
                }
            });
            
            
            auto block = ftxui::text("no");
            auto block_r = Renderer(command_form, [&]() {
                return block;
            });
            Resizable4BlockOptions options;
            Component menu1_renderer = Resizable4Block(command_form, block_r, block_r, block_r, screen, options);
            
            

            int tab_index = 0;
            std::vector<std::string> tab_entries = {
                "lowcode command builder", "cmc explorer", "s3 explorer", "mindie toolkit"};
            auto tab_section = Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
            auto tab_content = Container::Tab({
                                                  menu1_renderer,
                                                  // block2,
                                                  // block3
                                              },
                                              &tab_index);
            Component main_container = Container::Vertical({tab_section,
                                                            tab_content});
            Component main_renderer = Renderer(main_container, [&]
                                               { return vbox({text("Tui Helper") | bold | hcenter,
                                                              tab_section->Render(),
                                                              tab_content->Render()}); });
            screen.Loop(main_renderer);
        }
    }
}