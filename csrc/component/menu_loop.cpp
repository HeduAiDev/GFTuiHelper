#include "tui_tool_sets.hpp"
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace tui {
    namespace component {
        using namespace ftxui;
        using json = nlohmann::json;
        void start_menu_loop(Component component, std::string type)
        {
            
            /////////////////////////////////////////////////////////////////////
            // lowcode command builder
            /////////////////////////////////////////////////////////////////////

            StringRef title("Tui Helper");
            
            
            // auto block = ftxui::text("no");
            // auto block_r = Renderer(component, [&]() {
            //     return block;
            // });
            // Resizable4BlockOptions options;
            // Component menu1_renderer = Resizable4Block(component, block_r, block_r, block_r, screen, options);
            
            

            // int tab_index = 0;
            // std::vector<std::string> tab_entries = {
            //     "lowcode command builder", "cmc explorer", "s3 explorer", "mindie toolkit"};
            // auto tab_section = Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
            // auto tab_content = Container::Tab({
            //                                       menu1_renderer,
            //                                       // block2,
            //                                       // block3
            //                                   },
            //                                   &tab_index);
            // Component main_container = Container::Vertical({tab_section,
            //                                                 tab_content});
            // Component main_renderer = Renderer(main_container, [&]
            //                                    { return vbox({text(title()) | bold | hcenter,
            //                                                   tab_section->Render(),
            //                                                   tab_content->Render()}); });
            if (type == "full_screen") {
                auto screen = ScreenInteractive::Fullscreen();
                screen.Loop(component);
            } else if (type == "fit_component") {
                auto screen = ScreenInteractive::FitComponent();
                screen.Loop(component);
            } else {
                throw std::runtime_error("unknown screen type: " + type);
            }
        }
    }
}