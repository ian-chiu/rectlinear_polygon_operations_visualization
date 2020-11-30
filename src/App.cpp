#include "App.h"
#include <Thor/Shapes/ConcaveShape.hpp>
#include <stdexcept>
#include <future>

// contructor
App::App(int w, int h)
   : win_width(w), win_height(h)
{
    if (!font.loadFromFile(CMAKE_SOURCE_DIR + "/resource/arial.ttf"))
    {
        throw std::runtime_error("Cannot load font!");
    }

    
    if (!texture.create(500, 500))
    {
        throw std::runtime_error("Cannot create texture!");
    }
    sf::Image image;
    image.create(500, 500, sf::Color::White);
    texture.update(image);
    texture.setRepeated(true);

    window.create(sf::VideoMode(win_width, win_height), "Visualization");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window, false);
    window.resetGLStates();

    std::string font_path = CMAKE_SOURCE_DIR + "/resource/segoeui.ttf";
    ImGui::GetIO().Fonts->Clear(); 
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), 20.f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), 25.f);
    ImGui::SFML::UpdateFontTexture(); 

    lines.reserve(100000);
}

// ------------------App Console--------------------
struct App::AppConsole
{
    char                  InputBuf[256];
    App                   *connect_app = nullptr;

    AppConsole(App *app = nullptr) : connect_app(app)
    {
        memset(InputBuf, 0, sizeof(InputBuf));
    }

    // Portable helpers
    static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void Draw(const char* title, bool* p_open)
    {
        // ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::End();
            return;
        }
        ImGui::PopFont();

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Console"))
                *p_open = false;
            ImGui::EndPopup();
        }


        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

        sf::Vector2f mousePlotPos = connect_app->plotPos(sf::Mouse::getPosition(connect_app->window).x, sf::Mouse::getPosition(connect_app->window).y);
        ImGui::Text("Mouse Position: (%.1f,%.1f)", mousePlotPos.x, mousePlotPos.y);
        ImGui::Separator();

        ImGui::Text("Operation Order: ");
        for (const auto &oper_str : connect_app->all_operations) 
        {
            if (oper_str == connect_app->curr_oper)
            {
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                ImGui::Text((oper_str + " ").c_str());
                ImGui::PopStyleColor();
                ImGui::SameLine();
            }
            else
            {
                ImGui::SameLine();
                ImGui::Text((oper_str + " ").c_str());
            }
        }
            
        ImGui::Separator();

        if (!connect_app->isAllDone)
        {
            ImGui::Text( connect_app->hint_text.c_str() );
            ImGui::Separator();
            ImGui::Text(
                "commands:\n"
                "\tskip - skip to the end of the operation\n"
                "\t[:digit:] - execute specific numbers of steps\n"
            );
        }
        else
        {
            ImGui::Text( "All operations are done.\nThe output result is in data/output.txt.\n" );
            ImGui::Separator();
            ImGui::Text(
                "Press S to change to split mode\n"
                "Press esc to change to non-split mode"
            );
        }

        if (!connect_app->isAllDone) 
        {
            ImGui::Separator();

            // Command-line
            bool reclaim_focus = false;
            ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags))
            {
                char* s = InputBuf;
                Strtrim(s);
                if (s[0])
                    ExecCommand(s);
                strcpy(s, "");
                reclaim_focus = true;
            }

            // Auto-focus on window apparition
            ImGui::SetItemDefaultFocus();
            if (reclaim_focus)
                ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }
        ImGui::PopFont();

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        if (ImGui::CollapsingHeader("Color Selector"))
        {
            if (ImGui::ColorEdit3("Background color", connect_app->bg_rbg)) 
            {
                connect_app->bgColor.r = static_cast<sf::Uint8>(connect_app->bg_rbg[0] * 255.0f);
                connect_app->bgColor.g = static_cast<sf::Uint8>(connect_app->bg_rbg[1] * 255.0f);
                connect_app->bgColor.b = static_cast<sf::Uint8>(connect_app->bg_rbg[2] * 255.0f);
            }
            if (ImGui::ColorEdit3("Board color", connect_app->board_rbg)) 
            {
                connect_app->boardColor.r = static_cast<sf::Uint8>(connect_app->board_rbg[0] * 255.0f);
                connect_app->boardColor.g = static_cast<sf::Uint8>(connect_app->board_rbg[1] * 255.0f);
                connect_app->boardColor.b = static_cast<sf::Uint8>(connect_app->board_rbg[2] * 255.0f);
            }
            if (!connect_app->isAllDone)
            {
                if (ImGui::ColorEdit3("Operation color", connect_app->oper_rbg)) 
                {
                    connect_app->operColor.r = static_cast<sf::Uint8>(connect_app->oper_rbg[0] * 255.0f);
                    connect_app->operColor.g = static_cast<sf::Uint8>(connect_app->oper_rbg[1] * 255.0f);
                    connect_app->operColor.b = static_cast<sf::Uint8>(connect_app->oper_rbg[2] * 255.0f);
                    // connect_app->boardColor.a = 150;
                }
            }
        }
        ImGui::PopFont();
        ImGui::End();
    }

    void ExecCommand(const char* command_line)
    {
        if (!connect_app->isAllDone)
        {
            if (Stricmp(command_line, "skip") == 0) 
            {
                connect_app->is_step_by_step = false;
                connect_app->can_start_step = true;
            }
            else
            {
                connect_app->step_cnt = atoi(command_line);
                if (connect_app->step_cnt > 0)
                    connect_app->can_start_step = true;
                else 
                    connect_app->step_cnt = 0;
            }
        }
        else
        {
            if (Stricmp(command_line, "split") == 0) 
                connect_app->split_mode = true;
            else if (Stricmp(command_line, "non-split") == 0)
                connect_app->split_mode = false;
        }
    }
};

void App::render(const Solution &sol, bool can_draw_shapes)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        if (isAllDone)
        {
            if (event.type == sf::Event::KeyPressed) 
            {
                switch (event.key.code) 
                {
                case sf::Keyboard::S:
                    split_mode = true;
                    break;
                
                case sf::Keyboard::Escape:
                    split_mode = false;
                    break;
                }
            }
        }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    static AppConsole console(this);

    console.Draw("Control Panel", (bool *)0);

    window.clear(bgColor);
    if (can_draw_shapes)
    {
        draw_polygon_set(sol.polygon_set);
        if (split_mode)
            draw_rects_edge(sol.output_rects);
    }

    ImGui::SFML::Render(window);
    window.display();
}

void App::set_operations(const Solution &sol)
{
    for (int i = 0; i < sol.operations.size() - 1; i++)
        operations_queue.push(sol.operations[i]);
    
    all_operations = sol.copy_operations();
}

void App::pop_operations_queue()
{
    if (!operations_queue.empty())
    {
        curr_oper = operations_queue.front();
        operations_queue.pop();
    }
}

// ----------------private methods------------------
sf::Vector2f App::plotPos(float x, float y)
{
    return {x, win_height - y};
}

// void App::draw_rectangles(const std::vector<Rect> &rects)
// {
//     sf::RectangleShape rectShape;
//     rectShape.setTexture(&texture);
//     rectShape.setFillColor(boardColor);
//     rectShape.setOutlineColor(sf::Color::White);
//     rectShape.setOutlineThickness(-1.f);

//     std::vector<sf::Vertex> lines;
//     for (auto rect : rects)
//     {
//         float rect_width = rect.get(gtl::HORIZONTAL).high() - rect.get(gtl::HORIZONTAL).low();
//         float rect_height = rect.get(gtl::VERTICAL).high() - rect.get(gtl::VERTICAL).low();
//         rectShape.setSize(sf::Vector2f(rect_width, rect_height));
//         rectShape.setPosition(plotPos(gtl::xl(rect), gtl::yh(rect)));
//         window.draw(rectShape);
//     }
// }

void App::draw_rects_edge(const std::vector<Rect> &rects)
{
    std::vector<sf::Vertex> lines;
    for (auto rect : rects)
    {
        sf::Vertex lb(plotPos(gtl::xl(rect), gtl::yl(rect)), sf::Color::White);
        sf::Vertex rt(plotPos(gtl::xh(rect), gtl::yh(rect)), sf::Color::White);
        sf::Vertex lt(plotPos(gtl::xl(rect), gtl::yh(rect)), sf::Color::White);
        sf::Vertex rb(plotPos(gtl::xh(rect), gtl::yl(rect)), sf::Color::White);

        lines.emplace_back(lb);
        lines.emplace_back(rb);

        lines.emplace_back(rb);
        lines.emplace_back(rt);

        lines.emplace_back(rt);
        lines.emplace_back(lt);

        lines.emplace_back(lt);
        lines.emplace_back(lb);
    }
    window.draw(&lines[0], lines.size(), sf::Lines);
    lines.clear();
}

void App::draw_polygon_set(const PolygonSet &ps)
{
    int polygon_cnt = 0;
    // draw polygons
    for (const auto &poly : ps)
    {
        // draw the outline polygon shape
        thor::ConcaveShape concave{};
        concave.setFillColor(boardColor);
        if (!isAllDone && (polygon_cnt == ps.size() - 1 || !is_start_first_oper) )
            concave.setFillColor(operColor);
        // concave.setOutlineColor(sf::Color::White);
        // concave.setOutlineThickness(2.f);
        concave.setPointCount(poly.size());
        int cnt = 0;
        for (auto vertex = poly.begin(); vertex != poly.end(); vertex++)
        {
            concave.setPoint(cnt, plotPos(gtl::x(*vertex), gtl::y(*vertex)));
            cnt++;
        }
        window.draw(concave);
        polygon_cnt++;

        // draw holes inside polygon if there are holes
        if (poly.begin_holes() != poly.end_holes())
        {
            // concave.setOutlineColor(sf::Color::Yellow);
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