#pragma once

#include "App.fwd.h"
#include "Solution.fwd.h"

#include <fstream>
#include <string>
#include <list>
#include <vector>
#include "nfd.h"
#include "cmake_variables.h"
#include "App.h"
#include "gtl_poly_types.h"

class Solution
{
    friend class App;

public:
    Solution();

    void read_operations();
    void execute_and_render_operations(App &app);
    void setInputFile(nfdchar_t *input_file_path);
    bool isInputFileOpen() { return input_file.is_open(); }
    std::string get_split_method() const;

private:
    int nRemains{};
    std::string curr_oper;
    int order_idx{ -1 };
    mutable std::ifstream input_file;
    mutable std::ifstream findRemainingsFile;
    std::ofstream output_file{ CMAKE_SOURCE_DIR + "/data/output.txt" };
    mutable nfdchar_t *input_file_path;
    std::vector<std::string> operations{};
    std::list<std::string> operations_queue;
    std::vector<Rect> output_rects{};
    PolygonSet polygon_set{};
    std::string split_method{ "SH" };
    int find_remain_polygons(int line_cnt);
    void execute_split();
};