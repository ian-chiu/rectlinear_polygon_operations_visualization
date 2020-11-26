#pragma once

#include "App.fwd.h"
#include "Solution.fwd.h"

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <deque>
#include <future>
#include <vector>
#include "App.h"
#include "PolygonSetHelper.h"
#include "gtl_poly_types.h"

class Solution
{
    friend class App;

public:
    Solution() = default;
    Solution(std::string infile, std::string outfile);
    ~Solution();

    void read_operations();
    void execute_and_render_operation(std::string oper, App &app);
    void execute_split();
    std::deque<std::string> copy_operations();
    std::string get_split_method();

private:
    std::ifstream input_file;
    std::ofstream output_file;
    std::string input_file_path;
    std::deque<std::string> operations{};
    std::vector<Rect> output_rects{};
    PolygonSet polygon_set{};
    std::string split_method{ "SH" };
    int find_remain_polygons(int line_cnt);
};