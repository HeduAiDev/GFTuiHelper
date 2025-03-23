#include "tui_tool_sets.hpp"
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace tui {
    namespace runable {
        using namespace ftxui;
        using namespace tui::component;
        using json = nlohmann::json;
        void start_menu_loop(std::string menu1_config, std::function<std::string(std::string, std::string)> menu1_on_change)
        {
            auto screen = ScreenInteractive::Fullscreen();
            /////////////////////////////////////////////////////////////////////
            // lowcode command builder
            /////////////////////////////////////////////////////////////////////

            StringRef title("Tui Helper");
            std::function<void()> on_submit = []() {};
            Component command_form = InputFormCreateFromJsonStr(menu1_config, menu1_on_change);
            
            
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
                                               { return vbox({text(title()) | bold | hcenter,
                                                              tab_section->Render(),
                                                              tab_content->Render()}); });
            screen.Loop(main_renderer);
        }
    }
}