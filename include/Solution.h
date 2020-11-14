#pragma once

#include "App.fwd.h"
#include "Solution.fwd.h"

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <deque>
#include "App.h"
#include "gtl_poly_types.h"

class Solution
{
public:
    Solution() = default;
    Solution(std::string infile, std::string outfile);
    ~Solution();

    void read_operations();
    void execute_operation(std::string oper, App &app);
    void execute_split();
    std::string get_split_method();
    std::deque<std::string> operations{};
    std::ofstream output_file;
    std::vector<Rect> output_rects{};
    PolygonSet polygon_set{};

private:
    std::ifstream input_file;
    std::string token;
    std::istringstream iss;
    std::string split_method{ "SO" };
};