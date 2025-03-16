#pragma once

#ifdef __CUDA__
#include <cuda_fp16.h>
#endif

namespace tui
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///// runable
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    namespace runable {
        /// @brief 
        /// @param ptr  a matrix pointer
        /// @param rows  your matrix row size
        /// @param cols  your matrix col size
        /// @param screen_size_x    print size in x axis 
        /// @param screen_size_y    print size in y axis 

        // dragable scroll bar
        //  ↓                       
        // ████████████████████████__________________________
        // .0│-2.│0.0│-4.│-3.│-5.│-4.│1.0│-1.│-5.│1.0│-4.│3 
        // ──┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───
        // .0│3.0│-5.│3.0│2.0│2.0│3.0│-2.│3.0│-2.│2.0│-4.│-9
        // ──┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───
        // 5.│4.0│4.0│1.0│3.0│-2.│-1.│3.0│-1.│4.0│4.0│-3.│0  █
        // ──┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───█
        // .0│-5.│3.0│3.0│-5.│1.0│3.0│-4.│4.0│3.0│4.0│2.0│-1 █
        // ──┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───█ <-dragable scroll bar

        // print matrix in tui that interacts with the mouse
        void print_matrix(float* ptr, int rows, int cols, int screen_size_x = 50, int screen_size_y = 15, int text_width = 5);
        void print_matrix(double* ptr, int rows, int cols, int screen_size_x = 50, int screen_size_y = 15, int text_width = 8);
        void print_matrix(int* ptr, int rows, int cols, int screen_size_x = 50, int screen_size_y = 15, int text_width = 5);
        #ifdef __CUDA__
        void print_matrix(half* ptr, int rows, int cols, int screen_size_x = 50, int screen_size_y = 15, int text_width = 5);
        #endif

         // static view but you can setting the offset of x and y by specify a center point 
        void print_matrix_glance(float *ptr, int rows, int cols, int row_id, int col_id, int screen_size_x = 50, int screen_size_y = 15);
        void print_matrix_glance(double *ptr, int rows, int cols, int row_id, int col_id, int screen_size_x = 50, int screen_size_y = 15);
        void print_matrix_glance(int *ptr, int rows, int cols, int row_id, int col_id, int screen_size_x = 50, int screen_size_y = 15);
        #ifdef __CUDA__
        void print_matrix_glance(half *ptr, int rows, int cols, int row_id, int col_id, int screen_size_x = 50, int screen_size_y = 15);
        #endif


// Error nums : 1792                                           │Accuracy : 0.001000
//  Max diff  : 9.000000                                       │  Rows   : 20
//  Avg diff  : 3.334500                                       │  Cols   : 100
//                                                             │
// ────────────────────────────────────────────────────────────┼───────────────────────────────────────────────────────────
// ╭─────────────────────────Matrix A─────────────────────────╮│╭────────────────────────Matrix B─────────────────────────╮
// │███████████████████████████                               │││██████████████████████████▌                              │
// │ │44 │45 │46 │47 │48 │49 │50 │51 │52 │53 │54 │55 │56 │    │││ │44 │45 │46 │47 │48 │49 │50 │51 │52 │53 │54 │55 │56     │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──  ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────  │
// │.│0.0│-5.│-4.│-1.│0.0│1.0│-3.│-5.│-2.│0.0│1.0│1.0│1.0│5   │││.│1.0│-4.│-5.│0.0│0.0│-3.│0.0│3.0│-2.│-3.│2.0│3.0│0.05   │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──  ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────  │
// │.│0.0│-3.│3.0│-5.│2.0│-3.│-3.│0.0│4.0│-4.│-5.│3.0│0.0│6   │││0│-1.│-1.│-1.│-2.│0.0│3.0│-5.│-3.│4.0│-1.│4.0│3.0│-2.6   │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──  ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────  │
// │.│1.0│3.0│4.0│-3.│4.0│0.0│4.0│-1.│0.0│-5.│0.0│3.0│3.0│7   │││.│4.0│-4.│3.0│-5.│-1.│2.0│2.0│-1.│-4.│-4.│4.0│3.0│-4.7   │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──  ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────  │
// │.│0.0│1.0│1.0│-4.│-1.│-1.│1.0│-3.│1.0│3.0│-3.│0.0│0.0│8   │││.│2.0│-2.│-5.│2.0│-2.│-5.│2.0│-2.│1.0│-1.│1.0│2.0│-1.8   │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──  ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────  │
// │0│0.0│0.0│-5.│4.0│-3.│3.0│-3.│-1.│0.0│-5.│-1.│-1.│-5.│9   │││.│-4.│2.0│0.0│-2.│-4.│-5.│-5.│-5.│0.0│-5.│-5.│2.0│3.09   │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──▄ ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────▄ │
// │0│-5.│-2.│1.0│-3.│-1.│0.0│3.0│3.0│-4.│-3.│-4.│-1.│4.0│10█ │││.│4.0│3.0│4.0│1.0│-1.│-5.│-3.│4.0│-5.│-2.│-3.│-3.│-1.10█ │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──█ ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────█ │
// │0│-5.│-2.│-5.│-2.│0.0│-5.│2.0│1.0│2.0│3.0│-2.│1.0│-1.│11█ │││.│-4.│0.0│-1.│-5.│1.0│1.0│3.0│-4.│-2.│-2.│-4.│-5.│0.011█ │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──█ ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────█ │
// │0│3.0│-3.│-3.│2.0│3.0│0.0│3.0│-2.│4.0│-1.│-1.│1.0│4.0│12█ │││.│3.0│2.0│4.0│-1.│-1.│-2.│0.0│-1.│-1.│-3.│2.0│2.0│0.012█ │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──█ ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────█ │
// │.│4.0│0.0│4.0│2.0│-3.│-5.│-3.│3.0│1.0│2.0│-5.│3.0│-4.│13█ │││0│-5.│-3.│-1.│-1.│-2.│-2.│1.0│2.0│1.0│1.0│-4.│-1.│3.013█ │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──█ ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────█ │
// │0│-2.│3.0│-3.│-2.│-3.│-4.│-5.│-1.│-5.│2.0│0.0│4.0│-5.│14█ │││.│-1.│-2.│4.0│3.0│0.0│-2.│3.0│-4.│-3.│-2.│3.0│4.0│3.014█ │
// ├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼──█ ││├─┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼─────█ 


        void diff(float *ptr_a, float *ptr_b, int rows, int cols, double accuracy = 1e-3);
        void diff(double *ptr_a, double *ptr_b, int rows, int cols, double accuracy = 1e-3);
        void diff(int *ptr_a, int *ptr_b, int rows, int cols, double accuracy = 1e-3);
        #ifdef __CUDA__
        void diff(half *ptr_a, half *ptr_b, int rows, int cols, double accuracy = 1e-3);
        #endif
        void start_menu_loop(std::string lowcode_command_builder_config);
    } // namespace runable
    
} // namespace tui