#include "tui_tool_sets.hpp"

namespace tui {
    namespace component {
        using namespace ftxui;
        ResizableSplitBlockbase::ResizableSplitBlockbase(Component block1, Component block2, Component block3, Component block4, const ResizableSplitBlockOptions options) 
        :   block1_(std::move(block1)),
            block2_(std::move(block2)),
            block3_(std::move(block3)),
            block4_(std::move(block4)),
            vseparator_up_box_(std::make_shared<Box>()),
            vseparator_down_box_(std::make_shared<Box>()),
            hseparator_box_(std::make_shared<Box>()),
            options_(options) {

            Add(Container::Vertical({
                Container::Horizontal({
                    block1_,
                    block2_,
                }, &hselector),
                Container::Horizontal({
                    block3_,
                    block4_,
                }, &hselector)
            }, &vselector));
        };

        ResizableSplitBlockbase::ResizableSplitBlockbase(Component block1, Component block2, Component block3, const ResizableSplitBlockOptions options)
        :   block1_(std::move(block1)),
            block2_(std::move(block2)),
            block3_(std::move(block3)),
            options_(options) {
            // block1
            // ------------
            // block2|block3
            if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block3Up) {
                vseparator_up_box_ = nullptr;
                vseparator_down_box_=std::make_shared<Box>();
                hseparator_box_=std::make_shared<Box>();
                Add(Container::Vertical({
                    block1_,
                    Container::Horizontal({
                        block2_,
                        block3_,
                    }, &hselector),
                }, &vselector));
            // block1|block2
            // -------------
            // block3
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block3Down) {
                vseparator_up_box_ = std::make_shared<Box>();
                vseparator_down_box_= nullptr;
                hseparator_box_=std::make_shared<Box>();
                Add(Container::Vertical({
                    Container::Horizontal({
                        block1_,
                        block2_,
                    }, &hselector),
                    block3_,
                }, &vselector));
            // block1|block2
            //       |------
            //       |block3
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block3Left) {
                vseparator_up_box_ = std::make_shared<Box>();
                vseparator_down_box_= nullptr;
                hseparator_box_=std::make_shared<Box>();
                Add(Container::Horizontal({
                    block1_,
                    Container::Vertical({
                        block2_,
                        block3_,
                    }, &vselector),
                }, &hselector));
            // block1|block2
            // ------|
            // block3|
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block3Right) {
                vseparator_up_box_ = std::make_shared<Box>();
                vseparator_down_box_= nullptr;
                hseparator_box_ = std::make_shared<Box>();
                Add(Container::Horizontal({
                    Container::Vertical({
                        block1_,
                        block3_,
                    }, &vselector),
                    block2_,
                }, &hselector));
            } else {
                throw std::runtime_error("Invalid Block3 split type:" + std::to_string(static_cast<int>(options_.split_type)));
            }
        };

        ResizableSplitBlockbase::ResizableSplitBlockbase(Component block1, Component block2, const ResizableSplitBlockOptions options)
        :   block1_(std::move(block1)),
            block2_(std::move(block2)),
            options_(options) {
            // block2|block3
            if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block2Horizontal) {
                vseparator_up_box_ = std::make_shared<Box>();
                vseparator_down_box_ = nullptr;
                hseparator_box_= nullptr;
                Add(Container::Horizontal({
                        block1_,
                        block2_,
                    }, &hselector));
            // block1
            // ------
            // block3
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block2Vertical) {
                vseparator_up_box_ = nullptr;
                vseparator_down_box_= nullptr;
                hseparator_box_=std::make_shared<Box>();
                Add(Container::Vertical({
                    block1_,
                    block2_,
                }, &vselector));
            } else {
                throw std::runtime_error("Invalid Block2 split type:" + std::to_string(static_cast<int>(options_.split_type)));
            }
        };
        
        Element ResizableSplitBlockbase::Render()  {
            int container_width = std::max(contianer.x_max - contianer.x_min, 30);
            int container_height = std::max(contianer.y_max - contianer.y_min, 10);
            int block_width = std::min((int)(options_.base_x_percent() * container_width + bias_x_), container_width);
            int block_height = std::min((int)(options_.base_y_percent() * container_height + bias_y_), container_height);
            auto set_block_height = [&]() {
                return size(HEIGHT, EQUAL, block_height);
            };
            auto set_left_height = [&]() {
                return size(HEIGHT, EQUAL, container_height - block_height);
            };
            auto set_full_height = [&]() {
                return size(HEIGHT, EQUAL, container_height);
            };
            auto set_block_width = [&]() {
                return size(WIDTH, EQUAL, block_width);
            };
            auto set_left_width = [&]() {
                return size(WIDTH, EQUAL, container_width - block_width);
            };
            auto set_full_width = [&]() {
                return size(WIDTH, EQUAL, container_width);
            };
            // block1|block2
            // -------------
            // block3|block4
            if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block4){
                return vbox({
                    hbox({
                        (options_.placeholder_block1 && isDragging() ? options_.placeholder_block1 : block1_ -> Render()) | set_block_width() | set_block_height(),
                        getVSeparator() | reflect(*vseparator_up_box_),
                        (options_.placeholder_block2 && isDragging() ? options_.placeholder_block2 : block2_ -> Render()) | set_left_width() | set_block_height()
                    }) | set_block_height(),
                    getHSeparator() | reflect(*hseparator_box_),
                    hbox({
                        (options_.placeholder_block3 && isDragging() ? options_.placeholder_block3 : block3_ -> Render()) | set_block_width() | set_left_height(),
                        getVSeparator() | reflect(*vseparator_down_box_),
                        (options_.placeholder_block4 && isDragging() ? options_.placeholder_block4 : block4_ -> Render()) | set_left_width() | set_left_height()
                    }) | set_left_height()
                }) | reflect(contianer);

            // block1
            // ------------
            // block2|block3
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block3Up) {
                return vbox({
                    (options_.placeholder_block1 && isDragging() ? options_.placeholder_block1 : block1_ -> Render()) | set_full_width() | set_block_height(),
                    getHSeparator() | reflect(*hseparator_box_),
                    hbox({
                        (options_.placeholder_block2 && isDragging() ? options_.placeholder_block2 : block2_ -> Render()) | set_block_width() | set_left_height(),
                        getVSeparator() | reflect(*vseparator_down_box_),
                        (options_.placeholder_block3 && isDragging() ? options_.placeholder_block3 : block3_ -> Render()) | set_left_width() | set_left_height()
                    }) | set_left_height()
                }) | reflect(contianer);
            // block1|block2
            // -------------
            // block3
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block3Down) {
                return vbox({
                    hbox({
                        (options_.placeholder_block1 && isDragging() ? options_.placeholder_block1 : block1_ -> Render()) | set_block_width() | set_block_height(),
                        getVSeparator() | reflect(*vseparator_up_box_),
                        (options_.placeholder_block2 && isDragging() ? options_.placeholder_block2 : block2_ -> Render()) | set_left_width() | set_block_height(),
                    }) | set_block_height(),
                    getHSeparator() | reflect(*hseparator_box_),
                    (options_.placeholder_block3 && isDragging() ? options_.placeholder_block3 : block3_ -> Render()) | set_full_width() | set_left_height()
                }) | reflect(contianer);
            // block1|block2
            //       |------
            //       |block3
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block3Left) {
                return hbox({
                    (options_.placeholder_block1 && isDragging() ? options_.placeholder_block1 : block1_ -> Render()) | set_full_height() | set_block_width(),
                    getVSeparator() | reflect(*vseparator_up_box_),
                    vbox({
                        (options_.placeholder_block2 && isDragging() ? options_.placeholder_block2 : block2_ -> Render()) | set_left_width() | set_block_height(),
                        getHSeparator() | reflect(*hseparator_box_),
                        (options_.placeholder_block3 && isDragging() ? options_.placeholder_block3 : block3_ -> Render()) | set_left_width() | set_left_height()
                    }) | set_full_height()
                }) | reflect(contianer);
            // block1|block2
            // ------|
            // block3|
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block3Right) {
                return hbox({
                    vbox({
                        (options_.placeholder_block1 && isDragging() ? options_.placeholder_block1 : block1_ -> Render()) | set_block_width() | set_block_height(),
                        getHSeparator() | reflect(*hseparator_box_),
                        (options_.placeholder_block3 && isDragging() ? options_.placeholder_block3 : block3_ -> Render()) | set_block_width() | set_left_height()
                    }) | set_full_height(),
                    getVSeparator() | reflect(*vseparator_up_box_),
                    (options_.placeholder_block2 && isDragging() ? options_.placeholder_block2 : block2_ -> Render()) | set_full_height() | set_left_width()
                }) | reflect(contianer);
            // block1|block2
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block2Horizontal) {
                return hbox({
                    (options_.placeholder_block1 && isDragging() ? options_.placeholder_block1 : block1_ -> Render()) |  set_full_height() | set_block_width(),
                    getVSeparator() | reflect(*vseparator_up_box_),
                    (options_.placeholder_block2 && isDragging() ? options_.placeholder_block2 : block2_ -> Render()) |  set_full_height() | set_left_width()
                }) | reflect(contianer);
            // block1
            // ------
            // block2
            } else if (options_.split_type == ResizableSplitBlockOptions::SplitType::Block2Vertical) {
                return vbox({
                    (options_.placeholder_block1 && isDragging() ? options_.placeholder_block1 : block1_ -> Render()) | set_full_width() | set_block_height(),
                    getHSeparator() | reflect(*hseparator_box_),
                    (options_.placeholder_block2 && isDragging() ? options_.placeholder_block2 : block2_ -> Render()) | set_full_width() | set_left_height()
                }) | reflect(contianer);
            } else {
                throw std::runtime_error("Invalid split type:" + std::to_string(static_cast<int>(options_.split_type)));
            }
        };


        bool ResizableSplitBlockbase::OnEvent(Event event) {
            if (ComponentBase::OnEvent(std::move(event))) {
                return true;
            }
            if (event.is_mouse())
            {
                return OnMouseEvent(std::move(event));
            }
            return false;
        };

        bool ResizableSplitBlockbase::OnMouseEvent(Event event) {
            // if (event.is_mouse() && (event.mouse().button == Mouse::WheelDown ||
            // event.mouse().button == Mouse::WheelUp) ) {
            //     if (event.mouse().button == Mouse::WheelUp && vselector > 0) {
            //         (vselector)--;
            //     }
            //     if (event.mouse().button == Mouse::WheelDown && vselector < this->ChildCount() - 1) {
            //         (vselector)++;
            //     }
            //     return true;
            // }

            is_hover_hseparator_ = (hseparator_box_ != nullptr && hseparator_box_->Contain(event.mouse().x, event.mouse().y));
            is_hover_vseparator_up_ = (vseparator_up_box_ != nullptr && vseparator_up_box_->Contain(event.mouse().x, event.mouse().y));
            is_hover_vseparator_down_ = (vseparator_down_box_ != nullptr && vseparator_down_box_->Contain(event.mouse().x, event.mouse().y));

            if (isDragging() && event.mouse().motion == Mouse::Released) {
                is_dragging_hseparator_ = false;
                is_dragging_vseparator_up_ = false;
                is_dragging_vseparator_down_ = false;
                return false;
            }
            if (event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed && !isDragging()) {
                if (is_hover_hseparator_) {
                    is_dragging_hseparator_ = true;
                    return true;
                } else if (is_hover_vseparator_up_) {
                    is_dragging_vseparator_up_ = true;
                    return true;
                } else if (is_hover_vseparator_down_) {
                    is_dragging_vseparator_down_ = true;
                    return true;
                }
            }
            if (!isDragging()) {
                return false;
            }
            if (is_dragging_hseparator_ ) {
            // y direction movement
                if (contianer.y_min < event.mouse().y && contianer.y_max > event.mouse().y) {
                    bias_y_ += event.mouse().y - hseparator_box_->y_min;
                }
            } else {
            // x direction movement
                if (contianer.x_min < event.mouse().x && contianer.x_max > event.mouse().x) {
                    if (vseparator_up_box_ != nullptr) {
                        bias_x_ += event.mouse().x - vseparator_up_box_->x_min;
                    } else if (vseparator_down_box_ != nullptr) {
                        bias_x_ += event.mouse().x - vseparator_down_box_->x_min;
                    } else {
                        throw std::runtime_error("One of vseparator_up_box_ or vseparator_down_box_ should not be nullptr");
                    }
                }
            }
            return true;
        }


        bool ResizableSplitBlockbase::isDragging() {
            return (is_dragging_hseparator_ || is_dragging_vseparator_up_ || is_dragging_vseparator_down_ );
        };
        
        Element ResizableSplitBlockbase::getVSeparator() {
            return (is_hover_vseparator_up_ || is_hover_vseparator_down_) ? options_.separator_hover_func() : options_.separator_func();
        };

        Element ResizableSplitBlockbase::getHSeparator() {
            return (is_hover_hseparator_) ? options_.separator_hover_func() : options_.separator_func();
        };

        
    }
}