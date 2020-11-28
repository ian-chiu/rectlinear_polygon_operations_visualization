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
        Solution solution(CMAKE_SOURCE_DIR + "/data/input.txt", CMAKE_SOURCE_DIR + "/data/output.txt");
        solution.read_operations();
        
        App app{};
        app.set_operations(solution);

        while (app.window.isOpen())
        {
            // ----------------START EACH OPERATION----------------
            solution.execute_and_render_operations(app);

            // ------------ALL OPERATIONS ARE DONE-----------------
            std::string message{"All operations are done.\nThe output result is in data/output.txt.\n"};
            app.hint_text = message;
            app.render(solution);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    ImGui::SFML::Shutdown();
    return 0;
}