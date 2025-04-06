#include "tui_tool_sets.hpp"

int main()
{
    using namespace ftxui;
    using namespace tui::component;
    auto block = ftxui::text("no");
    auto block1 = Renderer([&]() {
        return ftxui::text("block1")  | border ;
    });
    auto block2 = Renderer([&]() {
        return ftxui::text("block2") | border;
    });
    auto block3 = Renderer([&]() {
        return ftxui::text("block3") | border;
    });
    auto screen = ScreenInteractive::FitComponent();
    ResizableSplitBlockOptions options;
    options.split_type = ResizableSplitBlockOptions::SplitType::Block3Up;
    // options.base_y = 80;
    auto c = ResizableSplitBlock(options, block1, block3, block2);
    screen.Loop(c);
    return 0;
}
    