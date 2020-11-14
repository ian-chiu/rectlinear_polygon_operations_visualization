#include <SFML/Graphics.hpp>
#include <Thor/Shapes/ConcaveShape.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

#include "App.h"
#include "cmake_variables.h"
#include "gtl_poly_types.h"
using namespace gtl::operators;

int main()
{
    /// ----------------SETTINGS-----------------
    // read the input file
    std::ifstream input_file{CMAKE_SOURCE_DIR + "/data/input.txt"};
    if (!input_file)
    {
        std::cerr << "Cannot open input file!";
        return -1;
    }
    std::deque<std::string> operations{};
    std::string token{};
    input_file >> token;
    if (token != "OPERATION")
    {
        std::cerr << "The first line of the input file must be operation list!";
        return -1;
    }
    while (input_file >> token && token != ";")
    {
        operations.push_back(token);
    }
    std::string split_method{operations.back()};
    operations.pop_back();

    std::istringstream iss{};
    std::string line{};
    bool isSplit{false};

    /// -----------------START SFML WINDOW-----------------
    int win_width = 16 * 80, win_height = 9 * 80;
    sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Visualization");

    App app(window, win_width, win_height);

    sf::Clock clock;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        sf::Time duration = clock.restart();
        double dElapsedTime = duration.asMilliseconds();

        // ----------------START OPERATION----------------
        bool start_oper{false};
        while (!operations.empty() && window.isOpen())
        {
            std::string oper{operations.front()};

            // let user press enter to start each operation
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Enter:
                        app.hint_text.setString(oper + " processing...");
                        start_oper = true;
                        operations.pop_front();
                    }
                }
            }

            if (start_oper == false)
            {
                app.hint_text.setString("Press enter to start the operation " + oper);
                app.render();
                continue;
            }

            // start each operation
            input_file.clear();
            input_file.seekg(0, input_file.beg);
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

                            if (app.polygon_set.empty())
                            {
                                app.polygon_set.push_back(polygon);
                            }
                            else
                            {
                                if (oper[0] == 'M')
                                    app.polygon_set += polygon;
                                if (oper[0] == 'C')
                                    app.polygon_set -= polygon;
                            }
                            app.render();
                        }
                    }
                    start_oper = false;
                    break;
                }
            }
        }
        if (!isSplit)
        {
            if (split_method == "SV")
            {
                gtl::get_rectangles(app.output_rects, app.polygon_set);
            }
            else if (split_method == "SH")
            {
                gtl::get_rectangles(app.output_rects, app.polygon_set, gtl::HORIZONTAL);
            }
            else
            {
                gtl::get_max_rectangles(app.output_rects, app.polygon_set);
            }

            std::ofstream output_file{CMAKE_SOURCE_DIR + "/data/output.txt"};
            for (auto rect : app.output_rects)
            {
                output_file << "RECT " << gtl::xl(rect) << " " << gtl::yl(rect)
                            << " " << gtl::xh(rect) << " " << gtl::yh(rect) << " ;\n";
            }
            input_file.close();
            output_file.close();
            isSplit = true;
        }

        //---------------------DONE------------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) 
            {
                switch (event.key.code) 
                {
                case sf::Keyboard::S:
                    app.split_mode = true;
                    break;
                
                case sf::Keyboard::Escape:
                    app.split_mode = false;
                    break;
                }
            }
        }

        std::string message{"All operations are done.\nThe output result is in data/output.txt.\n"};
        if (app.split_mode) 
        {
            message += "Press \"esc\" to change to non-split version...";
            app.hint_text.setString(message);
            app.render();
        }
        else
        {
            message += "Press \"s\" to change to split mode("+ split_method +")...";
            app.hint_text.setString(message);
            app.render();
        }
    }

    return 0;
}