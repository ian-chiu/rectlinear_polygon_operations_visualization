#include "Solution.h"
#include <limits>
using namespace gtl::operators;

Solution::Solution(std::string infile, std::string outfile) 
    : input_file(infile), output_file(outfile), input_file_path(infile)
{
    if (!input_file)
    {
        throw std::runtime_error("Cannot open " + infile);
    }
    output_rects.reserve(10000);
}

Solution::~Solution()
{
    input_file.close();
    output_file.close();
}

void Solution::read_operations()
{
    std::string token;
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

std::queue<std::string> Solution::copy_operations() const
{
    return std::queue<std::string>(operations);
}

void Solution::execute_and_render_operations(App &app)
{
    std::string token;
    std::istringstream iss;
    while (!app.operations_queue.empty())
    {
        app.pop_operations_queue();
        input_file.clear();
        input_file.seekg(0, input_file.beg);
        std::string line;
        int line_cnt = 0;
        while (getline(input_file, line))
        {
            line_cnt++;
            if (line[0] != 'D')
            {
                continue;
            }
            if (line.rfind(app.curr_oper) != std::string::npos)
            {
                int nRemains = find_remain_polygons(line_cnt);
                while (getline(input_file, line) && nRemains > 0)
                {
                    line_cnt++;
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
                            pts.push_back(gtl::construct<Point>(x, y));
                        }
                        pts.pop_back();
                        gtl::set_points(polygon, pts.begin(), pts.end());

                        nRemains = find_remain_polygons(line_cnt);
                        std::string message = app.curr_oper + " current Task:\n\t";
                        message += (app.curr_oper[0] == 'M') ? "MERGE " : "CLIP ";
                        message += line;
                        message += "\n\t(remain " + std::to_string(nRemains) + " polygons that need to operate...)";
                        app.hint_text = message;

                        // ---------render and execute operation on polygon set----------
                        if (app.is_step_by_step && app.step_cnt == 0)
                        {
                            polygon_set.push_back(polygon);
                            while(!app.can_start_step) 
                            {
                                app.render(*this);
                            }
                            app.can_start_step = false;
                            polygon_set.pop_back();
                        }
                        
                        if (app.curr_oper[0] == 'M') 
                            polygon_set += polygon;
                        if (app.curr_oper[0] == 'C')
                            polygon_set -= polygon;
                        if (app.step_cnt > 0)
                            app.step_cnt--;
                        app.render(*this, false);
                    }
                }
                app.hint_text = app.curr_oper + " is Done\n";
                app.render(*this);
                app.is_step_by_step = true;
                app.step_cnt = 0;
                break;
            }
        }
    }
    this->execute_split();
    app.isAllDone = true;
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
    input_file.close();
    output_file.close();
}

std::string Solution::get_split_method() const
{
    return split_method;
}

int Solution::find_remain_polygons(int line_cnt)
{
    std::ifstream ifile{ input_file_path };
    for (int i = 0; i < line_cnt; i++) 
        ifile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    int polygon_cnt = 0;
    while (getline(ifile, line)) 
    {
        polygon_cnt++;
        if (line[0] == 'E')
            break;
    }

    ifile.close();
    return polygon_cnt;
}