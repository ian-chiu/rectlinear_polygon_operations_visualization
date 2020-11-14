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
    App() = default;
    App(sf::RenderWindow &window, int win_width, int win_height);
    void render(const Solution &solution);

    int win_width{};
    int win_height{};
    sf::Text hint_text;
    sf::Text mouse_text;
    bool split_mode{ false };

private:
    sf::RenderWindow &window;
    sf::Font font;
    inline sf::Vector2f plotPos(float x, float y) const;
    void draw_rectangles(const std::vector<Rect> &rects) const;
    void draw_polygon_set(const PolygonSet &ps) const;
};