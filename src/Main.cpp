#include <SFML/Graphics.hpp>

#include "App.h"
#include "cmake_variables.h"
#include "gtl_poly_types.h"
using namespace gtl::operators;

int main()
{
    App app{1280, 720};
    app.hint_text = "Welcome. Please import a file to start visualization.";

    while (app.isWindowOpen())
    {
        if (!app.isAllDone)
        {
            app.execute_and_render_operations();
        }

        app.render();
    }

    ImGui::SFML::Shutdown();
    return 0;
}