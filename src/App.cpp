#include "App.h"
#include <Thor/Shapes/ConcaveShape.hpp>
#include <stdexcept>

// contructor
App::App(int w, int h)
   : win_width(w), win_height(h)
{
    if (!font.loadFromFile(CMAKE_SOURCE_DIR + "/resource/arial.ttf"))
    {
        throw std::runtime_error("Cannot load font!");
    }

    window.create(sf::VideoMode(win_width, win_height), "Visualization");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);
    window.resetGLStates();

    mouse_text.setFont(font);
    mouse_text.setCharacterSize(20);
    mouse_text.setFillColor(sf::Color::White);

    hint_text.setFont(font);
    hint_text.setCharacterSize(20);
    hint_text.setFillColor(sf::Color::White);
    hint_text.setPosition(0, 30);
}

void App::render(const Solution &sol, bool can_draw_shapes)
{
    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::Begin("Color window"); // begin window
    if (ImGui::ColorEdit3("Background color", bg_rbg)) {
        bgColor.r = static_cast<sf::Uint8>(bg_rbg[0] * 255.0f);
        bgColor.g = static_cast<sf::Uint8>(bg_rbg[1] * 255.0f);
        bgColor.b = static_cast<sf::Uint8>(bg_rbg[2] * 255.0f);
    }
    if (ImGui::ColorEdit3("Board color", board_rbg)) {
        boardColor.r = static_cast<sf::Uint8>(board_rbg[0] * 255.0f);
        boardColor.g = static_cast<sf::Uint8>(board_rbg[1] * 255.0f);
        boardColor.b = static_cast<sf::Uint8>(board_rbg[2] * 255.0f);
    }
    ImGui::End(); // end window

    window.clear(bgColor);
    if (can_draw_shapes)
        split_mode ? draw_rectangles(sol.output_rects) : draw_polygon_set(sol.polygon_set);

    window.draw(hint_text);

    sf::Vector2f mousePlotPos = plotPos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
    mouse_text.setString(std::to_string((int)mousePlotPos.x) + ", " + std::to_string((int)mousePlotPos.y));
    window.draw(mouse_text);

    ImGui::SFML::Render(window);

    window.display();
}

// ----------------private methods------------------
sf::Vector2f App::plotPos(float x, float y)
{
    return {x, win_height - y};
}

void App::draw_rectangles(const std::vector<Rect> &rects)
{
    for (auto rect : rects) {
        float rect_width = rect.get(gtl::HORIZONTAL).high() - rect.get(gtl::HORIZONTAL).low();
        float rect_height = rect.get(gtl::VERTICAL).high() - rect.get(gtl::VERTICAL).low();
        sf::RectangleShape rectShape(sf::Vector2f(rect_width, rect_height));
        rectShape.setPosition(plotPos(gtl::xl(rect), gtl::yh(rect)));
        rectShape.setFillColor(boardColor);
        rectShape.setOutlineColor(sf::Color::White);
        rectShape.setOutlineThickness(-1.f);
        window.draw(rectShape);
    }
}

void App::draw_polygon_set(const PolygonSet &ps)
{
    // draw polygons
    for (const auto &poly : ps)
    {
        // draw the outline polygon shape
        thor::ConcaveShape concave{};
        concave.setFillColor(boardColor);
        concave.setOutlineColor(sf::Color::White);
        concave.setOutlineThickness(2.f);
        concave.setPointCount(poly.size());
        int cnt = 0;
        for (auto vertex = poly.begin(); vertex != poly.end(); vertex++)
        {
            concave.setPoint(cnt, plotPos(gtl::x(*vertex), gtl::y(*vertex)));
            cnt++;
        }
        window.draw(concave);

        // draw holes inside polygon if there are holes
        if (poly.begin_holes() != poly.end_holes())
        {
            concave.setOutlineColor(sf::Color::Yellow);
            concave.setFillColor(bgColor);
            for (auto hole = poly.begin_holes(); hole != poly.end_holes(); hole++)
            {
                concave.setPointCount(hole->size());
                cnt = 0;
                for (auto hole_vertex = hole->begin(); hole_vertex != hole->end(); hole_vertex++)
                {
                    concave.setPoint(cnt, plotPos(gtl::x(*hole_vertex), gtl::y(*hole_vertex)));
                    cnt++;
                }
            }
            window.draw(concave);
        }
    }
}