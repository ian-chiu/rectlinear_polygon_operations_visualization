#include "Solution.h"
using namespace gtl::operators;

Solution::Solution(std::string infile, std::string outfile) 
    : input_file(infile), output_file(outfile) 
{
    if (!input_file)
    {
        throw std::runtime_error("Cannot open " + infile);
    }
}

Solution::~Solution()
{
    input_file.close();
    output_file.close();
}

void Solution::read_operations()
{
    input_file >> token;
    if (token != "OPERATION")
    {
        throw std::runtime_error("The first line of the input file must be operation list!");
    }
    while (input_file >> token && token != ";")
    {
        operations.push_back(token);
    }
    split_method = operations.back();
    operations.pop_back();
}

std::deque<std::string> Solution::copy_operations()
{
    return operations;
}

void Solution::execute_operation(std::string oper, App &app)
{
    input_file.clear();
    input_file.seekg(0, input_file.beg);
    std::string line;
    while (getline(input_file, line))
    {
        if (line[0] != 'D')
        {
            continue;
        }
        if (line.rfind(oper) != std::string::npos)
        {
            while (getline(input_file, line))
            {
                iss.clear();
                iss.str(line);
                iss >> token;
                if (token == "END")
                {
                    break;
                }
                if (token == "POLYGON")
                {
                    Polygon_Holes polygon;
                    std::vector<Point> pts{};
                    pts.reserve(10);

                    int x{}, y{};
                    while (iss >> x)
                    {
                        iss >> y;
                        pts.emplace_back(gtl::construct<Point>(x, y));
                    }
                    pts.pop_back();
                    gtl::set_points(polygon, pts.begin(), pts.end());

                    if (polygon_set.empty())
                    {
                        polygon_set.push_back(polygon);
                    }
                    else
                    {
                        if (oper[0] == 'M')
                            polygon_set += polygon;
                        if (oper[0] == 'C')
                            polygon_set -= polygon;
                    }
                    app.render(*this);
                }
            }
            break;
        }
    }
}

void Solution::execute_split()
{
    if (split_method == "SV")
        gtl::get_rectangles(output_rects, polygon_set);
    else if (split_method == "SH")
        gtl::get_rectangles(output_rects, polygon_set, gtl::HORIZONTAL);
    else
        gtl::get_max_rectangles(output_rects, polygon_set);

    for (auto rect : output_rects)
    {
        output_file << "RECT " << gtl::xl(rect) << " " << gtl::yl(rect)
                    << " " << gtl::xh(rect) << " " << gtl::yh(rect) << " ;\n";
    }
}

std::string Solution::get_split_method()
{
    return split_method;
}