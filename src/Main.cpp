#include <SFML\Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using rect_t = std::vector<float>;

int main()
{
    std::vector<rect_t> rects{};
    // read the data from output.txt
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

    int nScr_w = 800, nScr_h = 600;
    sf::RenderWindow window(sf::VideoMode(nScr_w, nScr_h), "My window");

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
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        auto plotPos = [nScr_h](float x, float y) {
            return sf::Vector2f(x, nScr_h - y);
        };

        for (auto rect : rects) {
            sf::RectangleShape rectShape(sf::Vector2f(rect[2] - rect[0], rect[3] - rect[1]));
            rectShape.setPosition(plotPos(rect[0], rect[3]));
            rectShape.setFillColor(sf::Color::Blue);
            rectShape.setOutlineColor(sf::Color::White);
            rectShape.setOutlineThickness(-1.f);
            window.draw(rectShape);
        }

        window.display();
    }

    return 0;
}