#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "cmake_variables.h"
#include "gtl_poly_types.h"

class App
{
public:
    App() = delete;
    App(sf::RenderWindow &window, int win_width, int win_height);

    void render();

    int win_width{};
    int win_height{};
    sf::Text hint_text;
    sf::Text mouse_text;
    std::vector<Rect> output_rects{};
    PolygonSet polygon_set{};
    bool split_mode{ false };

private:
    sf::RenderWindow &window;
    sf::Font font;
    inline sf::Vector2f plotPos(float x, float y);
    void draw_rectangles();
    void draw_polygon_set();
};