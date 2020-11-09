#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "gtl_poly_types.h"
using namespace gtl::operators;

using rect_t = std::vector<float>;

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

    std::vector<rect_t> rects{};
    std::vector<Polygon_Holes> polygons{};

    // read the data from rects.txt
    std::ifstream rects_file{ "../data/rects.txt" };
    if (!rects_file) {
        std::cerr << "Cannot open rects.txt";
        return -1;
    }
    while (rects_file) {
        std::string strInput;
        rects_file >> strInput;
        if (strInput == "RECT") {
            rect_t rect{};
            while (rects_file >> strInput)  {
                if (strInput == ";") {
                    break;
                }
                rect.push_back(stof(strInput));
            }
            rects.push_back(rect);
        }
    }

    // read polygons (read the data from .txt)
    std::ifstream input_file{"../data/polygons2.txt"};
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
    // ps += poly1;
    // ps += poly2;

    /// -----------------DRAWING SECTION-----------------
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
            // "close requested" event: we close the window
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

        // lambda function used to transform the position to plotPosition
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

        // for (const auto &rect : rects) {
        //     sf::RectangleShape rectShape(sf::Vector2f(rect[2] - rect[0], rect[3] - rect[1]));
        //     rectShape.setPosition(plotPos(rect[0], rect[3]));
        //     rectShape.setFillColor(sf::Color::Magenta);
        //     rectShape.setOutlineColor(sf::Color::White);
        //     rectShape.setOutlineThickness(-1.f);
        //     window.draw(rectShape);
        // }

        for (const auto &poly : ps) {
            sf::ConvexShape convex;
            convex.setFillColor(sf::Color::Transparent);
            convex.setOutlineColor(sf::Color::White);
            convex.setOutlineThickness(-3.f);
            convex.setPointCount(poly.size());
            int cnt = 0;
            for (auto vertex : poly) {
                convex.setPoint(cnt, plotPos(vertex.x(), vertex.y()));
                cnt++;
            }
            window.draw(convex);
        }

        window.display();

    }

    return 0;
}