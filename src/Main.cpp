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

        int win_width = 16 * 80, win_height = 9 * 80;
        sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Visualization");
        App app(window, win_width, win_height);

        while (window.isOpen())
        {
            // ----------------START EACH OPERATION----------------
            bool start_oper{ false };
            while (!solution.operations.empty() && window.isOpen())
            {
                std::string oper{solution.operations.front()};

                // let user press enter to start each operation
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        window.close();
                    }
                    else if (event.type == sf::Event::KeyPressed)
                    {
                        switch (event.key.code)
                        {
                        case sf::Keyboard::Enter:
                            app.hint_text.setString(oper + " processing...");
                            start_oper = true;
                            solution.operations.pop_front();
                        }
                    }
                }

                if (start_oper == false)
                {
                    app.hint_text.setString("Press enter to start the operation " + oper);
                    app.render(solution);
                    continue;
                }

                solution.execute_operation(oper, app);
                start_oper = false;
            }
            if (!isSplit)
            {
                solution.execute_split();
                isSplit = true;
            }

            // ------------ALL OPERATIONS ARE DONE-----------------
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
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
                app.hint_text.setString(message);
                app.render(solution);
            }
            else
            {
                message += "Press \"s\" to change to split mode("+ solution.get_split_method() +")...";
                app.hint_text.setString(message);
                app.render(solution);
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}