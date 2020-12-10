#pragma once

#include "App.fwd.h"
#include "Solution.fwd.h"

#include <fstream>
#include <string>
#include <vector>
#include "App.h"
#include "gtl_poly_types.h"

class Solution
{
    friend class App;

public:
    Solution() = default;
    Solution(std::string infile, std::string outfile);
    ~Solution();

    void read_operations();
    void execute_and_render_operations(App &app);
    void execute_split();
    std::vector<std::string> copy_operations() const;

    std::string get_split_method() const;

private:
    int nRemains{};
    std::ifstream input_file;
    std::ofstream output_file;
    std::string input_file_path;
    std::vector<std::string> operations{};
    std::vector<Rect> output_rects{};
    PolygonSet polygon_set{};
    std::string split_method{ "SH" };
    int find_remain_polygons(int line_cnt);
};