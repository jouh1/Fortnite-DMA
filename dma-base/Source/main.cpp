#include <Pch.h>
#include <game.hpp>

int main()
{
    SetConsoleTitleA("ISLE FREE DMA");
    if (!mem.Init("FortniteClient-Win64-Shipping.exe", true, true)) {
        std::cout << "Failed to initialize DMA" << std::endl;
        return 1;
    }
    system("CLS");
    cache::base = mem.GetBaseAddress("FortniteClient-Win64-Shipping.exe");
    if (!init()) {
        printf("The gui was not initialized");
        Sleep(3000);
        exit(0);
    }

    system("CLS");
    create_overlay();
    std::thread base_thread(bases);
    std::thread actor_thread(actorloop);
    base_thread.detach();
    actor_thread.detach();
    directx_init();
    WPARAM result = render_loop();
    exit(0);
}