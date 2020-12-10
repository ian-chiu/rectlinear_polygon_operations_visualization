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
    float oper_rbg[4] = { 246.0f / 255.f, 164.0f / 255.f, 58.0f / 255.f, 0.7f };
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
    sf::Vector2i plotPos(const sf::Vector2i &pt);
    
    void draw_rectangles(const std::vector<Rect> &rects, const sf::Color &color);
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

    void showInputWindow(int nRemains);
    char InputBuf[256];
    static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }
    void ExecCommand(const char* command_line);

    void showColorSelector();
};