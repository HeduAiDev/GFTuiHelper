#include "tui_tool_sets_runable.hpp"

int main() {
    using namespace tui::runable;
    int* matrix = new int[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    // print_matrix(matrix, 3, 3);
    print_matrix_glance(matrix, 3, 3, 1, 1);

    return 0;
}