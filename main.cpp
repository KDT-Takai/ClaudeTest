#include "Engine/Window/Window.h"

int main() {
    Window window("DirectX12 Game Window", 800, 600);
    window.Create();
    window.Run();
    return 0;
}