#include <SFML/Graphics.hpp>
#include <Thor/Shapes/ConcaveShape.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "cmake_variables.h"
#include "gtl_poly_types.h"
using namespace gtl::operators;

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

    std::vector<Polygon_Holes> polygons{};

    // read polygons (read the data from polygons.txt)
    std::ifstream input_file{CMAKE_SOURCE_DIR + "/data/polygons2.txt"};
    if (!input_file) {
        std::cerr << "Cannot open file!\n";
        return -1;
    }

    std::string line{};
    while (std::getline(input_file, line)) {
        std::istringstream iss{line};
        std::string parser;
        iss >> parser;
        
        if (parser == "POLYGON") {
            std::vector<Point> pts{};
            float x{}, y{};
            while (iss >> x) {
                iss >> y;
                pts.push_back(gtl::construct<Point>((int)x, (int)y));
            }
            Polygon_Holes polygon;
            gtl::set_points(polygon, pts.begin(), pts.end());
            polygons.push_back(polygon);
        }
    }

    Polygon_Holes poly1 = polygons[0];
    Polygon_Holes poly2 = polygons[1];

    PolygonSet ps{};
    ps.push_back(poly1);
    ps.push_back(poly2);

    /// -----------------START SFML WINDOW-----------------
    int nScr_w = 800, nScr_h = 600;
    sf::RenderWindow window(sf::VideoMode(nScr_w, nScr_h), "Visualization");

    sf::Clock clock;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        sf::Time duration = clock.restart();
        double dElapsedTime = duration.asSeconds();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::M:
                        std::cout << "M pressed!\n";
                        ps.clear();
                        ps += poly1;
                        ps += poly2;
                        break;
                    
                    case sf::Keyboard::C:
                        std::cout << "C pressed!\n";
                        ps.clear();
                        ps.push_back(poly1);
                        ps -= poly2;
                        break;

                    case sf::Keyboard::R:
                        std::cout << "Reset\n";
                        ps.clear();
                        ps.push_back(poly1);
                        ps.push_back(poly2);
                        break;
                }
            }
        }

        window.clear(sf::Color::Black);

        // lambda function used to transform the position to plotting position
        auto plotPos = [nScr_h](float x, float y) {
            return sf::Vector2f(x, nScr_h - y);
        };

        // print the mouse cursor position
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePlotPos = plotPos((float)localPosition.x, (float)localPosition.y);
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        text.setString(std::to_string((int)mousePlotPos.x) + ", " + std::to_string((int)mousePlotPos.y));
        window.draw(text);

        // draw polygons
        for (const auto &poly : ps) {

            // draw the outline polygon shape
            thor::ConcaveShape concave{};
            concave.setFillColor(sf::Color::Blue);
            concave.setOutlineColor(sf::Color::White);
            concave.setOutlineThickness(3.f);
            concave.setPointCount(poly.size() - 1);
            int cnt = 0;
            for (auto vertex = poly.begin(); vertex < poly.end() - 1; vertex++) {
                concave.setPoint(cnt, plotPos((float)vertex->x(), (float)vertex->y()));
                cnt++;
            }
            window.draw(concave);

            // draw holes inside polygon if there are holes
            if (!poly.holes_.empty()) {
                concave.setOutlineColor(sf::Color::Yellow);
                concave.setFillColor(sf::Color::Black);
                for (const auto &hole : poly.holes_) {
                    cnt = 0;
                    for (auto hole_vertex = hole.begin(); hole_vertex < hole.end() - 1; hole_vertex++) {
                        concave.setPoint(cnt, plotPos((float)hole_vertex->x(), (float)hole_vertex->y()));
                        cnt++;
                    }
                }
                window.draw(concave);
            }
        }

        window.display();
    }

    return 0;
}