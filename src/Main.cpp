#include <SFML/Graphics.hpp>
#include <Thor/Shapes/ConcaveShape.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

#include "cmake_variables.h"
#include "gtl_poly_types.h"
using namespace gtl::operators;

inline sf::Vector2f plotPos(float x, float y, int scr_height)
{
    return {x, scr_height - y};
}

void draw_rectangles(sf::RenderWindow &window, const std::vector<Rect> &rects, int scr_height) 
{
    for (auto rect : rects) {
        float rect_width = rect.get(gtl::HORIZONTAL).high() - rect.get(gtl::HORIZONTAL).low();
        float rect_height = rect.get(gtl::VERTICAL).high() - rect.get(gtl::VERTICAL).low();
        sf::RectangleShape rectShape(sf::Vector2f(rect_width, rect_height));
        rectShape.setPosition(plotPos(gtl::xl(rect), gtl::yh(rect), scr_height));
        rectShape.setFillColor(sf::Color::Blue);
        rectShape.setOutlineColor(sf::Color::White);
        rectShape.setOutlineThickness(-1.f);
        window.draw(rectShape);
    }
}

void draw_polygon_set(sf::RenderWindow &window, const PolygonSet &ps, int scr_height)
{
    // draw polygons
    for (const auto &poly : ps)
    {
        // draw the outline polygon shape
        thor::ConcaveShape concave{};
        concave.setFillColor(sf::Color::Blue);
        concave.setOutlineColor(sf::Color::White);
        concave.setOutlineThickness(3.f);
        concave.setPointCount(poly.size());
        int cnt = 0;
        for (auto vertex = poly.begin(); vertex != poly.end(); vertex++)
        {
            concave.setPoint(cnt, plotPos(gtl::x(*vertex), gtl::y(*vertex), scr_height));
            cnt++;
        }
        window.draw(concave);

        // draw holes inside polygon if there are holes
        if (poly.begin_holes() != poly.end_holes())
        {
            concave.setOutlineColor(sf::Color::Yellow);
            concave.setFillColor(sf::Color::Black);
            for (auto hole = poly.begin_holes(); hole != poly.end_holes(); hole++)
            {
                concave.setPointCount(hole->size());
                cnt = 0;
                for (auto hole_vertex = hole->begin(); hole_vertex != hole->end(); hole_vertex++)
                {
                    concave.setPoint(cnt, plotPos(gtl::x(*hole_vertex), gtl::y(*hole_vertex), scr_height));
                    cnt++;
                }
            }
            window.draw(concave);
        }
    }
}

int main()
{
    /// ----------------SETTINGS-----------------
    // load fonts
    sf::Font font;
    if (!font.loadFromFile(CMAKE_SOURCE_DIR + "/resource/arial.ttf"))
    {
        std::cerr << "Cannot load font!\n";
        return -1;
    }

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

    PolygonSet ps{};
    std::vector<Rect> rects{};
    std::istringstream iss{};
    std::string line{};
    bool isSplit{false};
    bool split_mode{false};

    sf::Text mouse_text;
    mouse_text.setFont(font);
    mouse_text.setCharacterSize(20);
    mouse_text.setFillColor(sf::Color::White);

    sf::Text hint_text;
    hint_text.setFont(font);
    hint_text.setCharacterSize(20);
    hint_text.setFillColor(sf::Color::White);
    hint_text.setPosition(0, 30);

    /// -----------------START SFML WINDOW-----------------
    int win_width = 1200, win_height = 900;
    sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Visualization");

    sf::Clock clock;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        sf::Time duration = clock.restart();
        double dElapsedTime = duration.asMilliseconds();

        // ----------------OPERATION----------------
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
                        std::cout << "Pressed Enter!\n";
                        hint_text.setString(oper + " processing...");
                        start_oper = true;
                        operations.pop_front();
                    }
                }
            }

            if (start_oper == false)
            {
                window.clear(sf::Color::Black);

                hint_text.setString("Press enter to start the operation " + oper);
                draw_polygon_set(window, ps, win_height);
                window.draw(hint_text);

                // draw mouse position
                sf::Vector2f mousePlotPos = plotPos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, win_height);
                mouse_text.setString(std::to_string((int)mousePlotPos.x) + ", " + std::to_string((int)mousePlotPos.y));
                window.draw(mouse_text);

                window.display();
                continue;
            }

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

                            if (ps.empty())
                            {
                                ps.push_back(polygon);
                            }
                            else
                            {
                                if (oper[0] == 'M')
                                    ps += polygon;
                                if (oper[0] == 'C')
                                    ps -= polygon;
                            }

                            window.clear(sf::Color::Black);

                            draw_polygon_set(window, ps, win_height);
                            window.draw(hint_text);
                            // draw mouse position
                            sf::Vector2f mousePlotPos = plotPos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, win_height);
                            mouse_text.setString(std::to_string((int)mousePlotPos.x) + ", " + std::to_string((int)mousePlotPos.y));
                            window.draw(mouse_text);

                            window.display();
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
                gtl::get_rectangles(rects, ps);
            }
            else if (split_method == "SH")
            {
                gtl::get_rectangles(rects, ps, gtl::HORIZONTAL);
            }
            else
            {
                gtl::get_max_rectangles(rects, ps);
            }

            std::ofstream output_file{CMAKE_SOURCE_DIR + "/data/output.txt"};
            for (auto rect : rects)
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
                    split_mode = true;
                    break;
                
                case sf::Keyboard::Escape:
                    split_mode = false;
                    break;
                }
            }
        }

        window.clear(sf::Color::Black);

        std::string message{"All operations are done.\nThe output result is in data/output.txt.\n"};
        if (split_mode) 
        {
            message += "Press \"esc\" to change to non-split version...";
            hint_text.setString(message);
            draw_rectangles(window, rects, win_height);
            window.draw(hint_text);
            // draw mouse position
            sf::Vector2f mousePlotPos = plotPos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, win_height);
            mouse_text.setString(std::to_string((int)mousePlotPos.x) + ", " + std::to_string((int)mousePlotPos.y));
            window.draw(mouse_text);
        }
        else
        {
            message += "Press \"s\" to change to split mode("+ split_method +")...";
            hint_text.setString(message);
            draw_polygon_set(window, ps, win_height);
            window.draw(hint_text);
            // draw mouse position
            sf::Vector2f mousePlotPos = plotPos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, win_height);
            mouse_text.setString(std::to_string((int)mousePlotPos.x) + ", " + std::to_string((int)mousePlotPos.y));
            window.draw(mouse_text);
        }
        
        window.display();
    }

    return 0;
}