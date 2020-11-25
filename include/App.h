#pragma once

#include "App.fwd.h"
#include "Solution.fwd.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include "Solution.h"
#include "cmake_variables.h"
#include "gtl_poly_types.h"

class App
{
public:
    App(int win_width = 1280, int win_height = 720);
    void render(const Solution &solution);

    sf::RenderWindow window;
    sf::Text hint_text;
    sf::Text mouse_text;
    bool split_mode{ false };

private:
    int win_width;
    int win_height;
    sf::Font font;
    std::deque<std::string> operations_queue;
    inline sf::Vector2f plotPos(float x, float y);
    void draw_rectangles(const std::vector<Rect> &rects);
    void draw_polygon_set(const PolygonSet &ps);
};