#include <SFML/Graphics.hpp>
#include <Thor/Shapes/ConcaveShape.hpp>

#include "App.h"
#include "Solution.h"
#include "cmake_variables.h"
#include "gtl_poly_types.h"
using namespace gtl::operators;

int main()
{
    try
    {
        bool isSplit{ false };
        Solution solution(CMAKE_SOURCE_DIR + "/data/input.txt", CMAKE_SOURCE_DIR + "/data/output.txt");
        solution.read_operations();
        std::deque<std::string> operations = solution.copy_operations();
        
        App app{};

        while (app.window.isOpen())
        {
            // ----------------START EACH OPERATION----------------
            bool can_start_oper{ false };
            while (!operations.empty() && app.window.isOpen())
            {
                std::string oper{operations.front()};

                // let user press enter to start each operation
                // events control when operations are not done
                sf::Event event;
                while (app.window.pollEvent(event))
                {
                    ImGui::SFML::ProcessEvent(event);
                    if (event.type == sf::Event::Closed)
                    {
                        app.window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed)
                    {
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Space:
                            app.hint_text = oper + " processing...";
                            can_start_oper = true;
                            app.is_step_by_step = false;
                            operations.pop_front();
                            break;

                        case sf::Keyboard::Enter:
                            app.hint_text = oper + " processing...";
                            can_start_oper = true;
                            app.is_step_by_step = true;
                            operations.pop_front();
                            break;
                        }
                    }
                }

                if (!can_start_oper)
                {
                    std::string message{ "Operation " + oper };
                    message += "\n\t(1) Press Spacebar to execute the whole operation\n";
                    message += "\t(2) Press Enter to execute step by step";
                    app.hint_text = message;
                  app.render(solution);
                }
                else
                {
                    solution.execute_and_render_operation(oper, app);
                    can_start_oper = false;
                }
            }

            if (!isSplit)
            {
                solution.execute_split();
                isSplit = true;
            }

            // ------------ALL OPERATIONS ARE DONE-----------------
            // events control when all operations are done 
            sf::Event event;
            while (app.window.pollEvent(event))
            {
                ImGui::SFML::ProcessEvent(event);
                if (event.type == sf::Event::Closed)
                {
                    app.window.close();
                }
                else if (event.type == sf::Event::KeyPressed) 
                {
                    switch (event.key.code) 
                    {
                    case sf::Keyboard::S:
                        app.split_mode = true;
                        break;
                    
                    case sf::Keyboard::Escape:
                        app.split_mode = false;
                        break;
                    }
                }
            }

            std::string message{"All operations are done.\nThe output result is in data/output.txt.\n"};
            if (app.split_mode) 
            {
                message += "Press \"esc\" to change to non-split version...";
                app.hint_text = message;
            }            else
            {
                message += "Press \"s\" to change to split mode("+ solution.get_split_method() +")...";
                app.hint_text = message;
            }            
            app.render(solution);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    ImGui::SFML::Shutdown();
}