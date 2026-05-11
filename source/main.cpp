#include "../headers/Application.h"

int main()
{
    Application App;

    App.Initialize(800, 600, "Graphics");

    App.Run();

    App.Shutdown();

    return 0;
}

