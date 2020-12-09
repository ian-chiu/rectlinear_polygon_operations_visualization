#pragma once

#include "App.fwd.h"
#include "Solution.fwd.h"

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
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
    void execute_and_render_operations(App &app);
    void execute_split();
    std::vector<std::string> copy_operations() const;

    std::string get_split_method() const;

private:
    // class Polygon : public Polygon_Holes
    // {
    // public:
    //     bool input_polygon_has_hole = false;
    // };
    // struct Polygon
    // {
    //     Polygon_Holes polygon;
    //     bool has_hole = false;
    // };
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