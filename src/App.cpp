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

    window.create(sf::VideoMode(win_width, win_height), "Visualization", sf::Style::Close | sf::Style::Resize);
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window, false);
    window.resetGLStates();

    std::string font_path = CMAKE_SOURCE_DIR + "/resource/segoeui.ttf";
    ImGui::GetIO().Fonts->Clear(); 
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), 20.f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), 25.f);
    ImGui::GetIO().Fonts->AddFontDefault();
    ImGui::SFML::UpdateFontTexture(); 

    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding      =   ImVec2( 5.0f, 5.0f );
    style.WindowRounding    =   0.0f;
    style.Colors[ImGuiCol_WindowBg]     =   ImVec4(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_Border]       =   ImVec4(1.0f, 1.0f, 1.0f, 0.0f);


    camera.reset(sf::FloatRect(0.0f, 0.0f, win_width, win_height));
    window.setView(camera);
    lines.reserve(100000);
}

// ------------------App Console--------------------
// struct App::AppConsole
// {
//     char                  InputBuf[256];
//     App                   *connect_app = nullptr;

//     AppConsole(App *app = nullptr) : connect_app(app)
//     {
//         memset(InputBuf, 0, sizeof(InputBuf));
//     }

//     // Portable helpers
//     static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
//     static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

//     void Draw(const char* title, bool* p_open)
//     {
//         // ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
//         ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
//         if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_AlwaysAutoResize))
//         {
//             ImGui::End();
//             return;
//         }
//         ImGui::PopFont();

//         // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
//         // So e.g. IsItemHovered() will return true when hovering the title bar.
//         // Here we create a context menu only available from the title bar.
//         if (ImGui::BeginPopupContextItem())
//         {
//             if (ImGui::MenuItem("Close Console"))
//                 *p_open = false;
//             ImGui::EndPopup();
//         }


//         ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

//         sf::Vector2i pixelPos = sf::Mouse::getPosition(connect_app->window);
//         sf::Vector2f worldPos = connect_app->window.mapPixelToCoords(pixelPos);

//         sf::Vector2f mousePlotPos = connect_app->plotPos(worldPos.x, worldPos.y);
//         ImGui::Text("Mouse Position: (%.1f,%.1f)", mousePlotPos.x, mousePlotPos.y);
//         ImGui::Separator();

//         ImGui::Text("World Scale: %.1f %%", connect_app->worldScale * 100.0f);
//         ImGui::Separator();

//         ImGui::Text("Camera Center: (%.1f,%.1f)", connect_app->camera.getCenter().x, connect_app->win_height - connect_app->camera.getCenter().y);
//         ImGui::Separator();

//         ImGui::Text("Operation Order: ");
//         for (const auto &oper_str : connect_app->all_operations) 
//         {
//             if (oper_str == connect_app->curr_oper)
//             {
//                 ImGui::SameLine();
//                 ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
//                 ImGui::Text((oper_str + " ").c_str());
//                 ImGui::PopStyleColor();
//                 ImGui::SameLine();
//             }
//             else
//             {
//                 ImGui::SameLine();
//                 ImGui::Text((oper_str + " ").c_str());
//             }
//         }
            
//         ImGui::Separator();

//         if (!connect_app->isAllDone)
//         {
//             ImGui::TextWrapped( connect_app->hint_text.c_str() );
//             // ImGui::Separator();
//             // ImGui::Text(
//             //     "commands:\n"
//             //     "\tskip - skip to the end of the operation\n"
//             //     "\t[:digit:] - execute specific numbers of steps\n"
//             // );
//         }
//         else
//         {
//             ImGui::Text( "All operations are done.\nThe output result is in data/output.txt.\n" );
//             // ImGui::Separator();
//             // ImGui::Text(
//             //     "Press S to change to split mode\n"
//             //     "Press esc to change to non-split mode"
//             // );
//         }

//         if (!connect_app->isAllDone) 
//         {
//             // ImGui::Separator();

//             // Command-line
//             bool reclaim_focus = false;
//             ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
//             if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags))
//             {
//                 char* s = InputBuf;
//                 Strtrim(s);
//                 if (s[0])
//                     ExecCommand(s);
//                 strcpy(s, "");
//                 reclaim_focus = true;
//             }

//             // Auto-focus on window apparition
//             ImGui::SetItemDefaultFocus();
//             if (reclaim_focus)
//                 ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
//         }
//         ImGui::PopFont();

//         ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
//         if (ImGui::CollapsingHeader("Color Selector"))
//         {
//             if (ImGui::ColorEdit3("Background color", connect_app->bg_rbg)) 
//             {
//                 connect_app->bgColor.r = static_cast<sf::Uint8>(connect_app->bg_rbg[0] * 255.0f);
//                 connect_app->bgColor.g = static_cast<sf::Uint8>(connect_app->bg_rbg[1] * 255.0f);
//                 connect_app->bgColor.b = static_cast<sf::Uint8>(connect_app->bg_rbg[2] * 255.0f);
//             }
//             if (ImGui::ColorEdit3("Board color", connect_app->board_rbg)) 
//             {
//                 connect_app->boardColor.r = static_cast<sf::Uint8>(connect_app->board_rbg[0] * 255.0f);
//                 connect_app->boardColor.g = static_cast<sf::Uint8>(connect_app->board_rbg[1] * 255.0f);
//                 connect_app->boardColor.b = static_cast<sf::Uint8>(connect_app->board_rbg[2] * 255.0f);
//             }
//             if (!connect_app->isAllDone)
//             {
//                 if (ImGui::ColorEdit3("Operation color", connect_app->oper_rbg)) 
//                 {
//                     connect_app->operColor.r = static_cast<sf::Uint8>(connect_app->oper_rbg[0] * 255.0f);
//                     connect_app->operColor.g = static_cast<sf::Uint8>(connect_app->oper_rbg[1] * 255.0f);
//                     connect_app->operColor.b = static_cast<sf::Uint8>(connect_app->oper_rbg[2] * 255.0f);
//                     // connect_app->boardColor.a = 150;
//                 }
//             }
//         }
//         ImGui::PopFont();
//         ImGui::End();
//     }

//     void ExecCommand(const char* command_line)
//     {
//         if (!connect_app->isAllDone)
//         {
//             if (Stricmp(command_line, "skip") == 0) 
//             {
//                 connect_app->is_step_by_step = false;
//                 connect_app->can_start_step = true;
//             }
//             else
//             {
//                 connect_app->step_cnt = atoi(command_line);
//                 if (connect_app->step_cnt > 0)
//                     connect_app->can_start_step = true;
//                 else 
//                     connect_app->step_cnt = 0;
//             }
//         }
//         else
//         {
//             if (Stricmp(command_line, "split") == 0) 
//                 connect_app->split_mode = true;
//             else if (Stricmp(command_line, "non-split") == 0)
//                 connect_app->split_mode = false;
//         }
//     }
// };

void App::render(const Solution &sol, bool can_draw_shapes)
{
    sf::Event event;
    sf::Time elapsed_time = deltaClock.getElapsedTime();
    int ms = elapsed_time.asMilliseconds();

    while (window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::Resized)
        {
            // update the view to the new size of the window
            win_height = event.size.height;
            win_width = event.size.width;
            camera.setSize(win_width * worldScale, win_height * worldScale);
            camera.setCenter(focusPoint);
            window.setView(camera);
        }
        else if (event.type == sf::Event::MouseWheelMoved)
        {
            if (event.mouseWheel.delta > 0)
            {
                worldScale /= 1.05f;
                camera.zoom(1.0f / 1.05f);
            }
            else
            {
                worldScale *= 1.05f;
                camera.zoom(1.05f);
            }
            window.setView(camera);
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code) 
            {
            case sf::Keyboard::Num1:
                if (isAllDone)  split_mode = true;
                break;
            
            case sf::Keyboard::Num2:
                if (isAllDone)  split_mode = false;
                break;
            
            case sf::Keyboard::Enter:
                can_start_step = true;
                break;

            case sf::Keyboard::O:
                focusMode = !focusMode;
                break;

            case sf::Keyboard::F:
                camera.setCenter(focusPoint);
                window.setView(camera);
                break;
            }   
        }
        // TODO: if we click in the console window the camera will also move
        // else if (event.type == sf::Event::MouseButtonPressed)
        // {
        //     sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        //     sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        //     sf::Vector2f mousePlotPos = plotPos(worldPos);
        //     camera.setCenter(mousePlotPos);
        //     window.setView(camera);
        // }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        camera.move( 0.0f, -camera_speed * ms * worldScale );
        window.setView(camera);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        camera.move( 0.0f, camera_speed * ms * worldScale );
        window.setView(camera);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        camera.move( -camera_speed * ms * worldScale, 0.0f );
        window.setView(camera);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        camera.move( camera_speed * ms * worldScale, 0.0f );
        window.setView(camera);
    }
    if (worldScale >= 0.01f && sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
    {
        worldScale /= 1.05f;
        camera.zoom(1.0f / 1.05f);
        window.setView(camera);
    }
    if (worldScale <= 10000000.0f &&sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
    {
        worldScale *= 1.05f;
        camera.zoom(1.05f);
        window.setView(camera);
    }

    // if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    // {
    //     while (sf::Mouse::)
    //     sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    //     sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    //     sf::Vector2f mousePlotPos = plotPos(worldPos);
    //     camera.setCenter(mousePlotPos);
    //     window.setView(camera);
    // }

    

    ImGui::SFML::Update(window, deltaClock.restart());
    // static AppConsole console(this);

    // console.Draw("Control Panel", (bool *)0);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    showMemuBar();
    showHintBar();
    showBottomBar();
    ImGui::PopFont();

    window.clear(bgColor);
    if (can_draw_shapes)
    {
        // split_mode ? draw_rectangles(sol.output_rects) : draw_polygon_set(sol.polygon_set);
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

sf::Vector2f App::plotPos(const sf::Vector2f &pt)
{
    return pt;
}

void App::draw_rectangles(const std::vector<Rect> &rects, const sf::Color &color)
{
    sf::RectangleShape rectShape;
    rectShape.setTexture(&texture);
    rectShape.setFillColor(color);
    // rectShape.setOutlineColor(sf::Color::White);
    // rectShape.setOutlineThickness(-1.f);
    for (auto rect : rects)
    {
        // sf::Vertex lb(plotPos(gtl::xl(rect), gtl::yl(rect)), boardColor);
        // sf::Vertex rt(plotPos(gtl::xh(rect), gtl::yh(rect)), boardColor);
        // sf::Vertex lt(plotPos(gtl::xl(rect), gtl::yh(rect)), boardColor);
        // sf::Vertex rb(plotPos(gtl::xh(rect), gtl::yl(rect)), boardColor);

        // quads.append(lb);
        // quads.append(rt);
        // quads.append(lt);
        // quads.append(rb);
        float rect_width = rect.get(gtl::HORIZONTAL).high() - rect.get(gtl::HORIZONTAL).low();
        float rect_height = rect.get(gtl::VERTICAL).high() - rect.get(gtl::VERTICAL).low();
        rectShape.setSize(sf::Vector2f(rect_width, rect_height));
        rectShape.setPosition(plotPos(gtl::xl(rect), gtl::yh(rect)));
        window.draw(rectShape);
    }
    // window.draw(quads);
    // quads.clear();
}

void App::draw_rects_edge(const std::vector<Rect> &rects)
{
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
        bool operate_shape_has_hole = false;

        // draw the outline polygon shape
        sf::Color shape_color = boardColor;
        if (!isAllDone && (polygon_cnt == ps.size() - 1 || !is_start_first_oper) )
            shape_color = operColor;

        // 1. if poly's size < 8 we know this polygon does not have holes
        // 2. we do not have to consider the main shape (polygon_cnt == 0)
        if (poly.size() >= 8 && (polygon_cnt != 0 || is_start_first_oper))
        {
            operate_shape_has_hole = polygon_noHoles_has_hole(poly);
        }

        if (!operate_shape_has_hole)
        {
            thor::ConcaveShape concave{};
            concave.setFillColor(shape_color);
            concave.setPointCount(poly.size());
            int cnt = 0;
            for (auto vertex = poly.begin(); vertex != poly.end(); vertex++)
            {
                concave.setPoint(cnt, plotPos(gtl::x(*vertex), gtl::y(*vertex)));
                cnt++;
            }
            window.draw(concave);

            // draw holes inside polygon if there are holes
            if (poly.size_holes() != 0)
            {
                // concave.setOutlineColor(sf::Color::Yellow);
                concave.setFillColor(bgColor);
                for (auto hole = poly.begin_holes(); hole != poly.end_holes(); hole++)
                {
                    bool hole_shape_has_intersection = polygon_noHoles_has_hole(*hole);
                    if (hole->size() >= 8)
                    {
                        operate_shape_has_hole = polygon_noHoles_has_hole(poly);
                    }

                    if (!hole_shape_has_intersection)
                    {
                        concave.setPointCount(hole->size());
                        int cnt = 0;
                        for (auto hole_vertex = hole->begin(); hole_vertex != hole->end(); hole_vertex++)
                        {
                            concave.setPoint(cnt, plotPos(gtl::x(*hole_vertex), gtl::y(*hole_vertex)));
                            cnt++;
                        }
                        window.draw(concave);
                    }
                    else
                    {
                        std::vector<Rect> rect_shapes;
                        gtl::get_rectangles(rect_shapes, *hole);
                        draw_rectangles(rect_shapes, bgColor);
                    }
                }
                // concave.scale(sf::Vector2f(worldScale, worldScale));
            }
        }
        else
        {   
            std::vector<Rect> rect_shapes;
            gtl::get_rectangles(rect_shapes, poly);
            draw_rectangles(rect_shapes, shape_color);
        }
        
        polygon_cnt++;
    }
}

bool App::polygon_noHoles_has_hole(const Polygon_Holes &poly)
{
    for (auto pt1 = poly.begin(); pt1 != poly.end(); pt1++)
    {
        for (auto pt2 = poly.begin(); pt2 != poly.end(); pt2++)
        {
            if (pt1 != pt2 && *pt1 == *pt2)
            {
                return true;
            }
        }
    }
    return false;
}

bool App::polygon_noHoles_has_hole(const Polygon_NoHoles &poly)
{
    for (auto pt1 = poly.begin(); pt1 != poly.end(); pt1++)
    {
        for (auto pt2 = poly.begin(); pt2 != poly.end(); pt2++)
        {
            if (pt1 != pt2 && *pt1 == *pt2)
            {
                return true;
            }
        }
    }
    return false;
}

void App::showMemuBar()
{
    // ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    
    if (ImGui::BeginMainMenuBar())
    {
        memuBarHeight = ImGui::GetWindowHeight();
        if (ImGui::BeginMenu("File"))
        {
            // ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    // ImGui::PopFont();
}

void App::showBottomBar()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetIO().DisplaySize.y), 0, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f));

    // ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(10.0f, 10.0f));
    // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, 2.0f);
    // ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 1.0f));
    if (ImGui::Begin("test", (bool *)0, window_flags))
    {
        // ImGui::Columns(4, "mycolumns"); // 4-ways, with border
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        sf::Vector2f mousePlotPos = plotPos(worldPos.x, worldPos.y);
        ImGui::Text("Mouse Position: (%.1f,%.1f)\t", mousePlotPos.x, mousePlotPos.y);
        // ImGui::NextColumn();
        ImGui::SameLine();

        ImGui::Text("World Scale: %.1f %%\t", worldScale * 100.0f);
        // ImGui::NextColumn();
        ImGui::SameLine();

        ImGui::Text("Camera Center: (%.1f,%.1f)\t", camera.getCenter().x, win_height - camera.getCenter().y);
        // ImGui::NextColumn();
        ImGui::SameLine();

        ImGui::Text("Operation Order: ");
        for (const auto &oper_str : all_operations) 
        {
            if (oper_str == curr_oper)
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
        // ImGui::NextColumn();
        ImGui::End();
    }   
    ImGui::PopStyleColor();
    // ImGui::PopStyleVar();
    // ImGui::PopFont();
}

void App::showHintBar()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground;
    ImGui::SetNextWindowPos(ImVec2(0.0f, memuBarHeight), 0, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f));

    // ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2( 5.0f, 5.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(25.0f / 255.0f, 25.0f / 255.0f, 25.0f / 255.0f, 1.0f));
    // ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
    // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(255.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 1.0f));
    if (ImGui::Begin(hint_text.c_str(), (bool *)0, window_flags))
    {
        ImGui::Text(" ");
        ImGui::End();
    }   
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    // ImGui::PopStyleVar();
    // ImGui::PopFont();
}

void App::showInputStepWindow()
{
    
}