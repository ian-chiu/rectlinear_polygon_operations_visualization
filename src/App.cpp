#include "App.h"
#include "PolygonSetHelper.h"
#include <stdexcept>
#include <sstream>
#include <array>

// contructor
App::App(int w, int h)
   : win_width(w), win_height(h)
{
    if (!texture.create(500, 500))
    {
        std::cerr << "Cannot create texture!";
        std::exit(-1);
    }
    sf::Image image;
    image.create(500, 500, sf::Color::White);
    texture.update(image);
    texture.setRepeated(true);

    window.create(sf::VideoMode(win_width, win_height), "Visualization", sf::Style::Close | sf::Style::Resize);
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window, false);
    window.resetGLStates();

    std::string font_path = CMAKE_SOURCE_DIR + "/resource/arial.ttf";
    ImGui::GetIO().Fonts->Clear(); 
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), 20.f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), 25.f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), 17.5f);
    ImGui::GetIO().Fonts->AddFontDefault();
    ImGui::SFML::UpdateFontTexture(); 

    // ImGuiStyle& style = ImGui::GetStyle();
    // style.WindowRounding    =   0.0f;
    // style.WindowMinSize     =   ImVec2(1.0f, 1.0f);
    // style.Colors[ImGuiCol_WindowBg]              =   ImVec4(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 0.8f);
    // style.Colors[ImGuiCol_Border]                =   ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
    // style.Colors[ImGuiCol_Button]                =   ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
    // style.Colors[ImGuiCol_ButtonHovered]         =   ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
    // style.Colors[ImGuiCol_ButtonActive]          =   ImVec4(10.0f / 255.0f, 10.0f / 255.0f, 10.0f / 255.0f, 1.0f);
    // style.Colors[ImGuiCol_ResizeGrip]            =   ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f);
    // style.Colors[ImGuiCol_ResizeGripHovered]     =   ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 1.0f);
    // style.Colors[ImGuiCol_ResizeGripActive]      =   ImVec4(30.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f, 1.0f);
    // style.Colors[ImGuiCol_FrameBg]               =   ImVec4(25.0f / 255.0f, 25.0f / 255.0f, 25.0f / 255.0f, 0.8f);
    // style.Colors[ImGuiCol_TitleBgActive]         =   ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 0.8f);

    ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text]                   = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg]               = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border]                 = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg]                = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark]              = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button]                 = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered]          = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive]           = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header]                 = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive]        = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip]             = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab]                    = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive]              = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused]           = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	// colors[ImGuiCol_DockingPreview]         = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
	// colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget]         = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;
    style->WindowMinSize     =   ImVec2(1.0f, 1.0f);

    memset(InputBuf, 0, sizeof(InputBuf));

    camera.reset(sf::FloatRect(0.0f, 0.0f, win_width, win_height));
    window.setView(camera);
    lines.reserve(100000);
}

void App::execute_and_render_operations()
{
    isImportFile = false;
    const int psh_array_size = 50;
    std::array<PolygonSetHelper, psh_array_size> psh_array{};
    bool isPshArrReady = true;

    std::string token;
    std::istringstream iss;

    input_file >> token;

    while (input_file >> token && token != ";")
    {
        operations.push_back(token);
        operations_queue.push_back(token);
    }

    if(!operations_queue.empty())
        operations_queue.pop_back();

    split_method = operations.back();

    while (!operations_queue.empty() && window.isOpen() && !isImportFile)
    {
        curr_oper = operations_queue.front();

        if (!operations_queue.empty())
            operations_queue.pop_front();

        input_file.clear();
        input_file.seekg(0, input_file.beg);
        std::string line;
        int line_cnt = 0;
        while (getline(input_file, line) && window.isOpen() && !isImportFile)
        {
            line_cnt++;
            if (line[0] != 'D')
            {
                continue;
            }
            if (line.rfind(curr_oper) != std::string::npos)
            {
                order_idx++;
                nRemains = find_remain_polygons(line_cnt);
                while (getline(input_file, line) && nRemains > 0 && !isImportFile)
                {
                    line_cnt++;
                    iss.clear();
                    iss.str(line);
                    iss >> token;
                    if (token == "END")
                    {
                        break;
                    }
                    if (token == "POLYGON")
                    {
                        Polygon_NoHoles polygon;
                        std::vector<Point> pts{};
                        pts.reserve(10);

                        int x{}, y{};
                        while (iss >> x)
                        {
                            iss >> y;
                            pts.push_back(gtl::construct<Point>(x, y));
                        }

                        if (!pts.empty())
                            pts.pop_back();

                        gtl::set_points(polygon, pts.begin(), pts.end());

                        nRemains--;

                        std::string message;
                        if (step_cnt == 0)
                        {
                            message += curr_oper + " ";
                            message += "Current Task (at line " + std::to_string(line_cnt) + "): ";
                            message += line;
                            message += " (remaining " + std::to_string(nRemains) + " polygons...)";
                            hint_text = message;
                        }
                        else
                        {
                            hint_text = curr_oper + " processing... (Remaining polygons: " + std::to_string(nRemains) + ")";
                        }

                        Point boundary_center;
                        gtl::center(boundary_center, polygon);
                        focusPoint = plotPos(boundary_center.x(), boundary_center.y());
                        if (focusMode)
                        {
                            camera.setCenter(focusPoint);
                            window.setView(camera);
                        }

                        // ---------render and execute operation on polygon set----------
                        if (step_cnt == 0)
                        {
                            polygon_set.push_back(polygon);
                            while(!can_start_step && window.isOpen() && !isImportFile) 
                            {
                                render();
                            }
                            isPshArrReady = false;
                            can_start_step = false;

                            if (!polygon_set.empty())
                                polygon_set.pop_back();
                        }

                        if (isImportFile)
                            break;

                        PolygonSetHelper &curr_psh = psh_array[line_cnt % psh_array_size];
                        curr_psh.push_future(
                            std::async(std::launch::async, &PolygonSetHelper::mergePolygon, &curr_psh, polygon)
                        ); 

                        if (step_cnt > 0)
                            step_cnt--;

                        if ((!isPshArrReady && step_cnt == 0) || isPause == true)
                        {
                            for (auto &psh : psh_array) 
                            {
                                psh.wait_futures();
                                curr_oper[0] == 'M' ? polygon_set += psh.ps : polygon_set -= psh.ps;
                                psh.ps.clear();
                            }
                            step_cnt = 0;
                            isPause = false;
                            isPshArrReady = true;
                        }

                        render(false);
                    }
                }

                render();
                step_cnt = 0;
                break;
            }
        }
    }

    if (!isImportFile)
    {
        if (split_method == "SV")
            gtl::get_rectangles(output_rects, polygon_set);
        else if (split_method == "SH")
            gtl::get_rectangles(output_rects, polygon_set, gtl::HORIZONTAL);
        else
            gtl::get_max_rectangles(output_rects, polygon_set);   

        order_idx++;
        hint_text = "All operations are done. You can export the output result now.";
        split_mode = true;
        isAllDone = true;
    }
}

void App::render(bool can_draw_shapes)
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
            std::exit(0);
        }
        else if (event.type == sf::Event::Resized)
        {
            // update the view to the new size of the window
            win_height = event.size.height;
            win_width = event.size.width;
            camera.setSize(win_width * worldScale, win_height * worldScale);
            camera.setCenter(plotPos(focusPoint));
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
                if (!isAllDone && !can_show_inputWindow) 
                    can_start_step = true;
                break;

            case sf::Keyboard::O:
                focusMode = !focusMode;
                break;

            case sf::Keyboard::F:
                camera.setCenter(focusPoint);
                window.setView(camera);
                break;

            case sf::Keyboard::I:
                if (!isAllDone && step_cnt == 0) 
                    can_show_inputWindow = true;
                break;

            case sf::Keyboard::Space:
                isPause = true;
                break;

            case sf::Keyboard::Escape:
                can_show_inputWindow = false;
                break;
            }   
        }
        // else if (event.type == sf::Event::MouseButtonPressed)
        // {
        //     if (event.mouseButton.button == sf::Mouse::Left && !ImGui::IsAnyWindowFocused())
        //     {
        //         sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        //         sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        //         sf::Vector2f mousePlotPos = plotPos(worldPos);
        //         camera.setCenter(mousePlotPos);
        //         window.setView(camera);
        //     }
        // }
    }

    if (!can_show_inputWindow)
    {
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
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);

    showMemuBar();
    if (can_show_hintBar)           showHintBar();
    if (can_show_colorSelector)     showColorSelector();
    if (can_show_inputWindow)       showInputWindow();
    showBottomBar();

    ImGui::PopFont();

    window.clear(bgColor);
    if (can_draw_shapes)
    {
        draw_polygon_set(polygon_set);
        if (split_mode)
            draw_rects_edge(output_rects);
    }

    ImGui::SFML::Render(window);
    window.display();
}

bool App::isWindowOpen()
{
    return window.isOpen();
}

// ----------------private methods------------------
sf::Vector2f App::plotPos(float x, float y)
{
    return {x, win_height - y};
}

sf::Vector2f App::plotPos(const sf::Vector2f &pt)
{
    return {pt.x, win_height - pt.y};
}

sf::Vector2i App::plotPos(const sf::Vector2i &pt)
{
    return {pt.x, win_height - pt.y};
}

void App::draw_rectangles(const std::vector<Rect> &rects, const sf::Color &color)
{
    sf::RectangleShape rectShape;
    rectShape.setTexture(&texture);
    rectShape.setFillColor(color);

    for (auto rect : rects)
    {
        int rect_width = rect.get(gtl::HORIZONTAL).high() - rect.get(gtl::HORIZONTAL).low();
        int rect_height = rect.get(gtl::VERTICAL).high() - rect.get(gtl::VERTICAL).low();
        rectShape.setSize(sf::Vector2f(rect_width, rect_height));
        rectShape.setPosition(plotPos(gtl::xl(rect), gtl::yh(rect)));
        window.draw(rectShape);
    }
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

        if (split_mode)
        {
            if (gtl::contains(rect, Point(getMousePlotPos().x, getMousePlotPos().y), false))
            {
                std::string message;
                message += "left: " + std::to_string(gtl::xl(rect)) + "\n";
                message += "top: " + std::to_string(gtl::yh(rect)) + "\n";
                message += "width: " + std::to_string(gtl::delta(gtl::horizontal(rect))) + "\n";
                message += "height: " + std::to_string(gtl::delta(gtl::vertical(rect))) + "\n";
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::TextUnformatted(message.c_str());
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }
        }
    }
    window.draw(&lines[0], lines.size(), sf::Lines);
    lines.clear();
}

void App::draw_polygon_set(const PolygonSet_NoHoles &ps)
{
    static std::vector<Rect> rect_shapes;
    static sf::Color shape_color;
    int polygon_cnt = 0;
    for (const auto &poly : ps)
    {
        shape_color = boardColor;
        if (!isAllDone && polygon_cnt == ps.size() - 1 )
            shape_color = operColor;
        
        gtl::get_rectangles(rect_shapes, poly);
        draw_rectangles(rect_shapes, shape_color);
        rect_shapes.clear();

        polygon_cnt++;
    }
}

void App::showMemuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        memuBarHeight = ImGui::GetWindowHeight();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import"))
            {
                nfdresult_t result = NFD_OpenDialog( "txt", NULL, &input_file_path);
                if ( result == NFD_OKAY )
                {
                    isImportFile = true;

                    input_file.close();
                    input_file.clear();
                    findRemainingsFile.close();
                    findRemainingsFile.clear();

                    input_file.open(input_file_path);
                    findRemainingsFile.open(input_file_path);
                    if (!input_file)
                    {
                        printf("1Cannot open %s", input_file_path);
                        std::exit(-1);
                    }
                    if (!findRemainingsFile)
                    {
                        printf("2Cannot open %s", input_file_path);
                        std::exit(-1);
                    }
                    order_idx = -1;
                    output_rects.clear();
                    polygon_set.clear();
                    operations.clear();
                    operations_queue.clear();

                    step_cnt = 0;
                    can_start_step = false;
                    isAllDone = false;
                    split_mode = false;

                    puts("Success!");
                    puts(input_file_path);
                    free(input_file_path);
                }
                else if ( result == NFD_CANCEL )
                {
                    puts("User pressed cancel.");
                }
                else 
                {
                    printf("Error: %s\n", NFD_GetError() );
                    std::exit(-1);
                }
            }
            if (ImGui::MenuItem("Export", 0, false, isAllDone && input_file_path != NULL ))
            {
                nfdchar_t *savePath = NULL;
                nfdresult_t result = NFD_SaveDialog( "txt", NULL, &savePath );
                if ( result == NFD_OKAY )
                {
                    std::ofstream output_file{ savePath };
                    for (auto rect : output_rects)
                    {
                        output_file << "RECT " << gtl::xl(rect) << " " << gtl::yl(rect)
                                    << " " << gtl::xh(rect) << " " << gtl::yh(rect) << " ;\n";
                    }
                    output_file.close();

                    puts("Success!");
                    puts(savePath);
                    free(savePath);
                }
                else if ( result == NFD_CANCEL )
                {
                    puts("User pressed cancel.");
                }
                else 
                {
                    printf("Error: %s\n", NFD_GetError() );
                    std::exit(-1);
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Hint")) { can_show_hintBar = true; }
            if (ImGui::MenuItem("Color Selector")) { can_show_colorSelector =  true; } 
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void App::showBottomBar()
{
    static sf::Vector2f last_mouse_pos, last_camera_center;
    static float last_world_scale;

    ImGuiWindowFlags window_flags = 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetIO().DisplaySize.y), 0, ImVec2(0.0f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 5.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 1.0f));
    if (step_cnt > 0)  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(80.0f / 255.0f, 80.0f / 255.0f, 80.0f / 255.0f, 1.0f));

    if (ImGui::Begin("test", (bool *)0, window_flags))
    {
        ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetFontSize() + 2 * ImGui::GetStyle().WindowPadding.y));

        if (step_cnt <= 0)
        {
            sf::Vector2f mousePlotPos = getMousePlotPos();
            last_mouse_pos = mousePlotPos;
            ImGui::Text("Mouse Position: (%.1f,%.1f)\t", mousePlotPos.x, mousePlotPos.y);
            ImGui::SameLine();

            last_world_scale = worldScale * 100.0f;
            ImGui::Text("World Scale: %.1f %%\t", worldScale * 100.0f);
            ImGui::SameLine();

            last_camera_center = camera.getCenter();
            ImGui::Text("Camera Center: (%.1f,%.1f)\t", last_camera_center.x, win_height - last_camera_center.y);
            ImGui::SameLine();

            if (input_file_path != NULL)
            {
                ImGui::Text("Operation Order: ");
                int oper_cnt = 0;
                for (const auto &oper_str : operations) 
                {
                    if (order_idx == oper_cnt)
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
                    oper_cnt++;
                }
            }
            
        }
        else
        {
            ImGui::Text("Mouse Position: (%.1f,%.1f)\t", last_mouse_pos.x, last_mouse_pos.y);
            ImGui::SameLine();

            ImGui::Text("World Scale: %.1f %%\t", last_world_scale * 100.0f);
            ImGui::SameLine();

            ImGui::Text("Camera Center: (%.1f,%.1f)\t", last_camera_center.x, win_height - last_camera_center.y);
            ImGui::SameLine();

            ImGui::Text("Operation Order: ");
            for (const auto &oper_str : operations) 
            {
                ImGui::SameLine();
                ImGui::Text((oper_str + " ").c_str());
            }
        }
        ImGui::End();
    }   
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    if (step_cnt > 0)  
        ImGui::PopStyleColor();
}

void App::showHintBar()
{
    ImGuiWindowFlags window_flags = 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | 
        ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowPos(ImVec2(0.0f, memuBarHeight), 0, ImVec2(0.0f, 0.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 5.0f));  
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    if (ImGui::Begin("Hint", &can_show_hintBar, window_flags))
    {
        ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetFontSize() + 2 * ImGui::GetStyle().WindowPadding.y ));
        ImGui::Text(hint_text.c_str());

        ImGui::SameLine(ImGui::GetWindowWidth() - 80.0f);
        if (ImGui::Button("Close"))
            can_show_hintBar = false;

        ImGui::End();
    }  
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

void App::showColorSelector()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;
    if (ImGui::Begin("Color Selector", &can_show_colorSelector))
    {
        if (ImGui::ColorEdit3("Background color", bg_rbg)) 
        {
            bgColor.r = static_cast<sf::Uint8>(bg_rbg[0] * 255.0f);
            bgColor.g = static_cast<sf::Uint8>(bg_rbg[1] * 255.0f);
            bgColor.b = static_cast<sf::Uint8>(bg_rbg[2] * 255.0f);
        }
        if (ImGui::ColorEdit3("Board color", board_rbg)) 
        {
            boardColor.r = static_cast<sf::Uint8>(board_rbg[0] * 255.0f);
            boardColor.g = static_cast<sf::Uint8>(board_rbg[1] * 255.0f);
            boardColor.b = static_cast<sf::Uint8>(board_rbg[2] * 255.0f);
        }
        if (!isAllDone)
        {
            if (ImGui::ColorEdit3("Operation color", oper_rbg)) 
            {
                operColor.r = static_cast<sf::Uint8>(oper_rbg[0] * 255.0f);
                operColor.g = static_cast<sf::Uint8>(oper_rbg[1] * 255.0f);
                operColor.b = static_cast<sf::Uint8>(oper_rbg[2] * 255.0f);
            }
        }
        ImGui::End();
    }
    
}

void App::showInputWindow()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 0.8f));

    if (ImGui::Begin(" ", nullptr, window_flags))
    {
        ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - ImGui::GetWindowWidth() / 2.0f, 80.0f));

        std::string message = curr_oper + " remaining polygons: " + std::to_string(nRemains);
        message += ". Please input how many steps you want to operate.";
        ImGui::Text(message.c_str());
        ImGui::Separator();

        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;
        if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags))
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0]) 
            {
                ExecCommand(s);
                can_show_inputWindow = false;
            }
                
            strcpy(s, "");
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        if (!ImGui::IsWindowFocused())
            can_show_inputWindow = false;
        
        ImGui::End();
    }
    ImGui::PopStyleColor();
}

void App::ExecCommand(const char* command_line)
{
    if (Stricmp(command_line, "skip") == 0) 
    {
        can_start_step = true;
    }
    else
    {
        step_cnt = atoi(command_line);
        if (step_cnt > 0)
            can_start_step = true;
        else 
            step_cnt = 0;
    }
}

int App::find_remain_polygons(int line_cnt)
{
    findRemainingsFile.seekg(0);
    for (int i = 0; i < line_cnt; i++) 
        findRemainingsFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    int polygon_cnt = 0;
    while (getline(findRemainingsFile, line)) 
    {
        polygon_cnt++;
        if (line[0] == 'E')
            break;
    }

    findRemainingsFile.clear();
    return polygon_cnt;
}

sf::Vector2f App::getMousePlotPos()
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    return plotPos(worldPos);
}