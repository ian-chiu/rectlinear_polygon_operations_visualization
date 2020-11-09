#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "gtl_poly_types.h"
using namespace gtl::operators;

int main()
{
    /// ----------------SETTINGS-----------------
    // load fonts
    sf::Font font;
    if (!font.loadFromFile("../resource/arial.ttf"))
    {
        std::cerr << "Cannot load font!\n";
        return -1;
    }

    std::vector<Polygon_Holes> polygons{};

    // read polygons (read the data from polygons.txt)
    std::ifstream input_file{"../data/polygons3.txt"};
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
                pts.push_back(gtl::construct<Point>(x, y));
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

        // print mouse cursor position
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePlotPos = plotPos(localPosition.x, localPosition.y);
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        text.setString(std::to_string((int)mousePlotPos.x) + ", " + std::to_string((int)mousePlotPos.y));
        window.draw(text);

        // draw polygons
        sf::ConvexShape convex;
        for (const auto &poly : ps) {
            // draw outline polygon
            convex.setFillColor(sf::Color::Blue);
            convex.setOutlineColor(sf::Color::White);
            convex.setOutlineThickness(-3.f);
            convex.setPointCount(poly.size());
            int cnt = 0;
            for (const auto &vertex : poly) {
                convex.setPoint(cnt, plotPos(vertex.x(), vertex.y()));
                cnt++;
            }
            window.draw(convex);

            // draw holes inside polygon if there are holes
            if (!poly.holes_.empty()) {
                convex.setOutlineColor(sf::Color::Yellow);
                convex.setFillColor(sf::Color::Black);
                for (const auto &hole : poly.holes_) {
                    cnt = 0;
                    for (const auto &hole_vertex : hole) {
                        convex.setPoint(cnt, plotPos(hole_vertex.x(), hole_vertex.y()));
                        cnt++;
                    }
                }
                window.draw(convex);
            }
        }

        window.display();
    }

    return 0;
}