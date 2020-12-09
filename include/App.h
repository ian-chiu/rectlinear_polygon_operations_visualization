#pragma once

#include "App.fwd.h"
#include "Solution.fwd.h"

#include <vector>
#include <queue>
#include <future>

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
    bool isAllDone = false;
    bool can_start_step = false;
    std::string hint_text;
    bool split_mode{ false };
    bool is_step_by_step{ true };
    void set_operations(const Solution &sol);
    void pop_operations_queue();

private:
    struct AppConsole;
    sf::Clock deltaClock;
    std::queue<std::string> operations_queue;
    std::vector<std::string> all_operations;
    int win_width;
    int win_height;
    sf::Font font;
    sf::Texture texture;
    sf::Color boardColor{ 25, 68, 43 };
    sf::Color bgColor{ sf::Color::Black };
    sf::Color operColor{ 246, 164, 58, 100 };
    float bg_rbg[3] = { 0.f, 0.f, 0.f };
    float board_rbg[3] = { 25.0f / 255.f, 68.0f / 255.f, 43.0f / 255.f };
    float oper_rbg[3] = { 246.0f / 255.f, 164.0f / 255.f, 58.0f / 255.f };
    std::string curr_oper;

    sf::View camera;
    sf::Vector2f focusPoint;
    bool focusMode = true;
    float camera_speed = 0.5f;
    float worldScale = 1.0f;

    int step_cnt{};
    bool is_start_first_oper{ true };
    sf::Vector2f plotPos(float x, float y);
    sf::Vector2f plotPos(const sf::Vector2f &pt);
    void draw_rectangles(const std::vector<Rect> &rects, const sf::Color &color);
    sf::VertexArray quads;
    void draw_rects_edge(const std::vector<Rect> &rects);
    std::vector<sf::Vertex> lines;
    void draw_polygon_set(const PolygonSet &ps);
    bool polygon_noHoles_has_hole(const Polygon_Holes &poly);
    bool polygon_noHoles_has_hole(const Polygon_NoHoles &poly);

    bool can_show_hintBar = true;
    bool can_show_inputWindow = false;
    bool can_show_colorSelector = false;

    void showMemuBar();
    float memuBarHeight;

    void showHintBar();
    void showBottomBar();
    void showInputWindow();
    void showColorSelector();
};