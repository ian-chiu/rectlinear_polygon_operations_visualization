#pragma once

#include "App.fwd.h"
#include "Solution.fwd.h"

#include <vector>
#include <deque>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "Solution.h"
#include "cmake_variables.h"
#include "gtl_poly_types.h"

class App
{
    friend Solution;

public:
    App(int win_width = 1280, int win_height = 720);
    void render(const Solution &solution, bool can_draw_shapes = true);

    sf::RenderWindow window;
    std::string hint_text;
    sf::Clock deltaClock;
    bool split_mode{ false };
    bool is_step_by_step{ true };

private:
    int win_width;
    int win_height;
    sf::Font font;
    sf::Color boardColor{ 25, 68, 43 };
    sf::Color bgColor{ sf::Color::Black };
    float bg_rbg[3] = { 0.f, 0.f, 0.f };
    float board_rbg[3] = { 25.0f / 255.f, 68.0f / 255.f, 43.0f / 255.f };
    inline sf::Vector2f plotPos(float x, float y);
    void draw_rectangles(const std::vector<Rect> &rects);
    void draw_polygon_set(const PolygonSet &ps);
    void show_hint_window();
};