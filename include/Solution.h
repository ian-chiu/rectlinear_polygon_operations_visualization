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
#include "gtl_poly_types.h"

class Solution
{
    friend class App;

public:
    Solution() = delete;
    Solution(std::string infile, std::string outfile);
    ~Solution();

    void read_operations();
    void execute_operation(std::string oper, App &app);
    void execute_split();
    std::deque<std::string> copy_operations();
    std::string get_split_method();

private:
    std::ifstream input_file;
    std::ofstream output_file;
    std::deque<std::string> operations{};
    std::vector<Rect> output_rects{};
    PolygonSet polygon_set{};
    std::string token;
    std::istringstream iss;
    std::string split_method{ "SH" };

    std::vector<std::future<void>> futures;
    static void mergePolygon(PolygonSet &ps, Polygon_Holes polygon);
    void wait_futures();
};