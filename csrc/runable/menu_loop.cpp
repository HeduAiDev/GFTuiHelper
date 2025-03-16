#include "tui_tool_sets.hpp"
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace tui {
    namespace runable {
        using namespace ftxui;
        using namespace tui::component;
        using json = nlohmann::json;
        void start_menu_loop(std::string lowcode_command_builder_config)
        {
            auto screen = ScreenInteractive::Fullscreen();
            /////////////////////////////////////////////////////////////////////
            // lowcode command builder
            /////////////////////////////////////////////////////////////////////
            std::unordered_map<std::string, StringRef> input_text_map;
            std::unordered_map<std::string, Ref<int>> input_select_index_map;
            std::unordered_map<std::string, std::vector<std::string>> input_select_entries_map;

            Component command_form = InputFormCreateFromJsonStr(lowcode_command_builder_config, input_text_map, input_select_index_map, input_select_entries_map);
            
            
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