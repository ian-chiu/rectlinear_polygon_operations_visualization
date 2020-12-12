#include <SFML/Graphics.hpp>
#include <Thor/Shapes/ConcaveShape.hpp>

#include "App.h"
#include "Solution.h"
#include "cmake_variables.h"
#include "gtl_poly_types.h"
using namespace gtl::operators;

int main()
{
    Solution solution;
    App app{1280, 720};

    while (app.isWindowOpen())
    {
        if (!app.isAllDone)
            solution.execute_and_render_operations(app);
        
        app.render(solution);
    }

    ImGui::SFML::Shutdown();
    return 0;
}