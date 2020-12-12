#include "Solution.h"

#include <sstream>
#include <future>
#include <array>
#include <limits>
#include <algorithm>
using namespace gtl::operators;

template<typename R>
bool is_ready(std::future<R> const& f)
{ 
    return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; 
}

struct PolygonSetHelper
{
    PolygonSetHelper() {
        ps.reserve(1000);
    }
    std::queue<std::future<void>> futures;
    PolygonSet ps{};

    void push_future(std::future<void> &&future)
    {
        while (!futures.empty() && is_ready(futures.front())) {
            futures.pop();
        }
        futures.push(std::move(future));
    }

    void wait_futures() 
    {
        while (!futures.empty()) {
            futures.front().wait();
            futures.pop();
        }
    }

    void mergePolygon(Polygon_Holes polygon)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        ps += polygon;
    }

private:
    std::mutex m_mtx;
};

Solution::Solution() 
{    
    output_rects.reserve(10000);
}

void Solution::setInputFile(nfdchar_t *input_file_path)
{
    input_file.close();
    input_file.clear();
    findRemainingsFile.close();
    findRemainingsFile.clear();

    input_file.open(input_file_path);
    findRemainingsFile.open(input_file_path);
    if (!input_file)
    {
        printf("1Cannot open %s", input_file_path);
        std::exit(-1);
    }
    if (!findRemainingsFile)
    {
        printf("2Cannot open %s", input_file_path);
        std::exit(-1);
    }
    output_rects.clear();
    polygon_set.clear();
    operations.clear();
}

// TODO: DELETE THIS
void Solution::read_operations()
{
    std::string token;
    input_file >> token;
    if (token != "OPERATION")
    {
        std::cerr << "The first line of the input file must be operation list!";
        std::exit(-1);
    }
    while (input_file >> token && token != ";")
    {
        operations.push_back(token);
        operations_queue.push_back(token);
    }
    operations_queue.pop_back();
    split_method = operations.back();
}

void Solution::execute_and_render_operations(App &app)
{
    const int psh_array_size = 50;
    std::array<PolygonSetHelper, psh_array_size> psh_array{};
    bool isPshArrReady = true;

    std::string token;
    std::istringstream iss;

    input_file >> token;
    if (token != "OPERATION")
    {
        std::cerr << "The first line of the input file must be operation list!";
        std::exit(-1);
    }

    while (input_file >> token && token != ";")
    {
        operations.push_back(token);
        operations_queue.push_back(token);
    }
    operations_queue.pop_back();
    split_method = operations.back();

    while (!operations_queue.empty() && app.isWindowOpen())
    {
        curr_oper = operations_queue.front();
        operations_queue.pop_front();

        input_file.clear();
        input_file.seekg(0, input_file.beg);
        std::string line;
        int line_cnt = 0;
        while (getline(input_file, line) && app.isWindowOpen())
        {
            line_cnt++;
            if (line[0] != 'D')
            {
                continue;
            }
            if (line.rfind(curr_oper) != std::string::npos)
            {
                order_idx++;
                nRemains = find_remain_polygons(line_cnt);
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

                        nRemains--;

                        std::string message;
                        if (app.step_cnt == 0)
                        {
                            message += curr_oper + " ";
                            message += "Current Task (at line " + std::to_string(line_cnt) + "): ";
                            message += line;
                            message += " (remaining " + std::to_string(nRemains) + " polygons...)";
                            app.hint_text = message;
                        }
                        else
                        {
                            app.hint_text = curr_oper + " processing... (Remaining polygons: " + std::to_string(nRemains) + ")";
                        }

                        Point boundary_center;
                        gtl::center(boundary_center, polygon);
                        app.focusPoint = app.plotPos(boundary_center.x(), boundary_center.y());
                        if (app.focusMode)
                        {
                            app.camera.setCenter(app.focusPoint);
                            app.window.setView(app.camera);
                        }

                        // ---------render and execute operation on polygon set----------
                        if (app.step_cnt == 0)
                        {
                            polygon_set.push_back(polygon);
                            while(!app.can_start_step && app.isWindowOpen()) 
                            {
                                app.render(*this);
                            }
                            isPshArrReady = false;
                            app.can_start_step = false;
                            polygon_set.pop_back();
                        }

                        PolygonSetHelper &curr_psh = psh_array[line_cnt % psh_array_size];
                        curr_psh.push_future(
                            std::async(std::launch::async, &PolygonSetHelper::mergePolygon, &curr_psh, polygon)
                        ); 
                        if (app.step_cnt > 0)
                            app.step_cnt--;

                        if ((!isPshArrReady && app.step_cnt == 0) || app.isPause == true)
                        {
                            for (auto &psh : psh_array) 
                            {
                                psh.wait_futures();
                                curr_oper[0] == 'M' ? polygon_set += psh.ps : polygon_set -= psh.ps;
                                psh.ps.clear();
                            }
                            app.step_cnt = 0;
                            app.isPause = false;
                            isPshArrReady = true;
                        }

                        app.render(*this, false);
                    }
                }
                app.render(*this);
                app.step_cnt = 0;
                break;
            }
        }
    }
    execute_split();
    order_idx++;
    curr_oper = operations.back();
    app.hint_text = "All operations are done. The output result is in data/output.txt.";
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
    output_file.close();
}

std::string Solution::get_split_method() const
{
    return split_method;
}

int Solution::find_remain_polygons(int line_cnt)
{
    findRemainingsFile.seekg(0);
    for (int i = 0; i < line_cnt; i++) 
        findRemainingsFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    int polygon_cnt = 0;
    while (getline(findRemainingsFile, line)) 
    {
        polygon_cnt++;
        if (line[0] == 'E')
            break;
    }

    findRemainingsFile.clear();
    return polygon_cnt;
}