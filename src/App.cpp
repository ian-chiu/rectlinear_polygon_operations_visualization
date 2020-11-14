#include "App.h"
#include <Thor/Shapes/ConcaveShape.hpp>
#include <stdexcept>

// contructor
App::App(sf::RenderWindow &window, int w, int h) :
    window(window), win_width(w), win_height(h)
{
    if (!font.loadFromFile(CMAKE_SOURCE_DIR + "/resource/arial.ttf"))
    {
        throw std::runtime_error("Cannot load font!");
    }

    mouse_text.setFont(font);
    mouse_text.setCharacterSize(20);
    mouse_text.setFillColor(sf::Color::White);

    hint_text.setFont(font);
    hint_text.setCharacterSize(20);
    hint_text.setFillColor(sf::Color::White);
    hint_text.setPosition(0, 30);
}


void App::render()
{
    window.clear(sf::Color::Black);
    split_mode ? draw_rectangles() : draw_polygon_set();
    window.draw(hint_text);
    sf::Vector2f mousePlotPos = plotPos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
    mouse_text.setString(std::to_string((int)mousePlotPos.x) + ", " + std::to_string((int)mousePlotPos.y));
    window.draw(mouse_text);
    window.display();
}

// ----------------private methods------------------
sf::Vector2f App::plotPos(float x, float y)
{
    return {x, win_height - y};
}

void App::draw_rectangles()
{
    for (auto rect : output_rects) {
        float rect_width = rect.get(gtl::HORIZONTAL).high() - rect.get(gtl::HORIZONTAL).low();
        float rect_height = rect.get(gtl::VERTICAL).high() - rect.get(gtl::VERTICAL).low();
        sf::RectangleShape rectShape(sf::Vector2f(rect_width, rect_height));
        rectShape.setPosition(plotPos(gtl::xl(rect), gtl::yh(rect)));
        rectShape.setFillColor(sf::Color::Blue);
        rectShape.setOutlineColor(sf::Color::White);
        rectShape.setOutlineThickness(-1.f);
        window.draw(rectShape);
    }
}

void App::draw_polygon_set()
{
    // draw polygons
    for (const auto &poly : polygon_set)
    {
        // draw the outline polygon shape
        thor::ConcaveShape concave{};
        concave.setFillColor(sf::Color::Blue);
        concave.setOutlineColor(sf::Color::White);
        concave.setOutlineThickness(3.f);
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
            concave.setFillColor(sf::Color::Black);
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