#include "Solution.h"
#include <limits>
using namespace gtl::operators;

struct RangeBox
{
    int left{};
    int right{};
    int bottom{};
    int top{};
    bool is_initialize = false;
};

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
}

std::vector<std::string> Solution::copy_operations() const
{
    return std::vector<std::string>(operations);
}

void Solution::execute_and_render_operations(App &app)
{
    std::string token;
    std::istringstream iss;
    RangeBox rangeBox;
    while (!app.operations_queue.empty() && app.window.isOpen())
    {
        app.pop_operations_queue();
        input_file.clear();
        input_file.seekg(0, input_file.beg);
        std::string line;
        int line_cnt = 0;
        while (getline(input_file, line) && app.window.isOpen())
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
                        // TODO: fix the hole error
                        Polygon_Holes polygon;
                        std::vector<Point> pts{};
                        pts.reserve(10);

                        int x{}, y{};
                        int min_x{-1}, max_x{-1}, min_y{-1}, max_y{-1};
                        while (iss >> x)
                        {
                            iss >> y;
                            if (max_x == -1)    max_x = x;
                            if (min_x == -1)    min_x = x;
                            if (min_y == -1)    min_y = y;
                            if (max_y == -1)    max_y = y;
                            if (x < min_x)      min_x = x;
                            if (x > max_x)      max_x = x;
                            if (y < min_y)      min_y = y;
                            if (y > max_y)      max_y = y;

                            pts.push_back(gtl::construct<Point>(x, y));
                        }
                        pts.pop_back();
                        gtl::set_points(polygon, pts.begin(), pts.end());

                        nRemains--;

                        std::string message;
                        message += "Current Operation: ";
                        message += (app.curr_oper[0] == 'M') ? "MERGE " : "CLIP ";
                        message += app.curr_oper + "\n";
                        message += "Current Task (at line " + std::to_string(line_cnt) + "):\n\t";
                        message += line;
                        message += "\n\t(remain " + std::to_string(nRemains) + " polygons that need to operate...)";
                        app.hint_text = message;

                        // sf::Vector2f worldPos = app.plotPos(pts[0].x(), pts[0].y());
                        // sf::Vector2i pixelPos = app.window.mapCoordsToPixel(worldPos);
                        // while (pixelPos.x >= 500.0f)
                        // {
                        //     app.camera.zoom(1.05);
                        //     app.window.setView(app.camera);
                        //     pixelPos = app.window.mapCoordsToPixel(worldPos);
                        // }
                        // app.camera.zoom(std::abs(pts[1].x() - pts[0].x()) / 100);

                        app.focusPoint = app.plotPos((max_x + min_x)/2.0f, (max_y + min_y)/2.0f);
                        if (app.focusMode)
                        {
                            app.camera.setCenter(app.focusPoint);
                            app.window.setView(app.camera);
                        }

                        if (!rangeBox.is_initialize)
                        {
                            rangeBox = {min_x, max_x, min_y, max_y, true};
                        }
                        if (min_x < rangeBox.left)      rangeBox.left = min_x;
                        if (max_y > rangeBox.top)       rangeBox.top = max_y;
                        if (max_x > rangeBox.right)     rangeBox.right = max_x;
                        if (min_y < rangeBox.bottom)   rangeBox.bottom = min_y;   

                        // ---------render and execute operation on polygon set----------
                        if (app.is_step_by_step && app.step_cnt == 0)
                        {
                            polygon_set.push_back(polygon);
                            while(!app.can_start_step && app.window.isOpen()) 
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
                        app.is_start_first_oper = true;
                    }
                }
                // app.hint_text = app.curr_oper + " is Done\n";
                app.render(*this);
                app.is_step_by_step = true;
                app.step_cnt = 0;
                break;
            }
        }
    }
    this->execute_split();
    app.curr_oper = app.all_operations.back();
    app.focusPoint = app.plotPos((rangeBox.right + rangeBox.left)/2.0f, (rangeBox.top + rangeBox.bottom)/2.0f);
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