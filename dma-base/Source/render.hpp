#include <pch.h>
#include <dwmapi.h>
#include <settings.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <chrono>
#include <thread>
#include "config.hpp"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

IDirect3D9Ex* p_object = nullptr;
IDirect3DDevice9Ex* p_device = nullptr;
D3DPRESENT_PARAMETERS p_params = { NULL };
MSG messager = { nullptr };
HWND my_wnd = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (p_device != NULL && wParam != SIZE_MINIMIZED)
        {
            p_params.BackBufferWidth = LOWORD(lParam);
            p_params.BackBufferHeight = HIWORD(lParam);
            p_device->Reset(&p_params);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


HRESULT directx_init()
{
    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_object)))
        return E_FAIL;

    ZeroMemory(&p_params, sizeof(p_params));
    p_params.Windowed = TRUE;
    p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    p_params.hDeviceWindow = my_wnd;
    p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
    p_params.BackBufferWidth = settings::width;
    p_params.BackBufferHeight = settings::height;
    p_params.EnableAutoDepthStencil = TRUE;
    p_params.AutoDepthStencilFormat = D3DFMT_D16;
    p_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    if (FAILED(p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, my_wnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_params, nullptr, &p_device)))
    {
        p_object->Release();
        return E_FAIL;
    }

    ImGui::CreateContext();
    ImGui_ImplWin32_Init(my_wnd);
    ImGui_ImplDX9_Init(p_device);
    //ImGui::GetIO().Fonts->AddFontFromFileTTF("include/font.otf", 18.0f);
    style();

    p_object->Release();
    return S_OK;
}

void create_overlay()
{
    WNDCLASSEXA wcex = {
        sizeof(WNDCLASSEXA),
        CS_CLASSDC,
        WindowProc,
        0,
        0,
        GetModuleHandle(NULL),
        LoadIcon(0, IDI_APPLICATION),
        LoadCursor(0, IDC_ARROW),
        0,
        0,
        "OverlayWindowClass",
        LoadIcon(0, IDI_APPLICATION)
    };
    RegisterClassExA(&wcex);

    my_wnd = CreateWindowExA(
        0,
        "OverlayWindowClass",
        "OverlayWindowTitle",
        WS_POPUP | WS_VISIBLE,
        0, 0,
        settings::width, settings::height,
        nullptr, nullptr,
        wcex.hInstance, nullptr
    );

    SetWindowLong(my_wnd, GWL_EXSTYLE, GetWindowLong(my_wnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(my_wnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(my_wnd, &margin);
    ShowWindow(my_wnd, SW_SHOW);
    UpdateWindow(my_wnd);
}


void case0()
{
    ImGui::Checkbox("Enable Aimbot", &settings::aimbot::enable);
    if (settings::aimbot::enable)
    {
        if (settings::kmbox::kmboxb || settings::kmbox::kmboxnet) {
            ImGui::Checkbox("Hold Right Click", &settings::aimbot::key);
            ImGui::Checkbox("Show FOV Circle", &settings::aimbot::show_fov);
            if (settings::aimbot::show_fov) {
                ImGui::SameLine();
                ImGui::ColorEdit4("Fov Color", settings::aimbot::fovColor, ImGuiColorEditFlags_NoInputs);
            }
            ImGui::Checkbox("Triggerbot", &settings::aimbot::triggerbot);
            ImGui::SliderFloat("FOV Radius", &settings::aimbot::fov, 50.0f, 300.0f, "%.2f");
            ImGui::SliderFloat("Smoothness", &settings::aimbot::smoothness, 1.0f, 25.0f, "%.2f");
        }
        else {
            ImGui::Text("Please select kmbox type in misc first");
        }
    }
}

void case1()
{
    ImGui::Checkbox("Enable Visuals", &settings::visuals::enable);
    if (settings::visuals::enable)
    {
        ImGui::Checkbox("Box", &settings::visuals::box);
        ImGui::Checkbox("Fill Box", &settings::visuals::fill_box);
        ImGui::Checkbox("Line", &settings::visuals::line);
        ImGui::Checkbox("Distance", &settings::visuals::distance);
        ImGui::ColorEdit4("Visible", settings::visuals::boxColor, ImGuiColorEditFlags_NoInputs);
        ImGui::SameLine();
        ImGui::ColorEdit4("Non-Visible", settings::visuals::boxColor2, ImGuiColorEditFlags_NoInputs);
    }
}

void case2()
{
    ImGui::Checkbox("Transparent", &settings::misc::zero_alpha);
    bool colorUpdated = false;
    ImGui::Checkbox("Menu Colors", &settings::misc::color);
        if (settings::misc::color) {
        colorUpdated |= ImGui::ColorEdit4("Text Color", settings::misc::textColor, ImGuiColorEditFlags_NoInputs);
        colorUpdated |= ImGui::ColorEdit4("Background Color", settings::misc::bgColor, ImGuiColorEditFlags_NoInputs);
        colorUpdated |= ImGui::ColorEdit4("Accent Color", settings::misc::blueColor, ImGuiColorEditFlags_NoInputs);
        if (colorUpdated) {
            style();
        }
    }
    if (settings::misc::zero_alpha)
    {
        SetLayeredWindowAttributes(my_wnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    }
    else
    {
        SetLayeredWindowAttributes(my_wnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    }

    ImGui::Text("ISLE DMA Free Version");
    ImGui::Text("discord.gg/6RDv4Ha4yX");
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemSpacing.y);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Close", { 120, 20 })) exit(0);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemSpacing.y);
    ImGui::SetCursorPosX(10);
    if (ImGui::Button("KmBox Debug", { 120, 20 })) {
        settings::debug::showDebugOptions = true;
    }
    if (settings::debug::showDebugOptions) {
        ImGui::OpenPopup("Debug Options");
    }

    if (settings::debug::showDebugOptions)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2((io.DisplaySize.x - 400) / 2, (io.DisplaySize.y - 300) / 2));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        if (ImGui::BeginPopupModal("Debug Options", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Select kmbox type\nin aimbot section.");
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            if (ImGui::Button("Test KmboxB move", { 150, 20 })) {
                kmBox::sendMove(50, 50);
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            if (ImGui::Button("Test KmboxB click", { 150, 20 })) {
                kmBox::kmclick();
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f)); 
            if (ImGui::Button("Test Kmbox.NET move", { 150, 20 })) {
                kmNet_mouse_move(50, 50);
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f)); 
            if (ImGui::Button("Test Kmbox.NET click", { 150, 20 })) {
                int down = 0;
                kmNet_mouse_left(down);
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            if (ImGui::Button("Close Debug Options", { 150, 20 })) {
                settings::debug::showDebugOptions = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);
            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }       
    ImGui::SameLine();
    if (ImGui::Button("Select kmbox type", { 140, 20 })) settings::kmbox::select = true;
    ImGui::SameLine();
    if (settings::kmbox::select) {
        ImGui::OpenPopup("Kmbox Settings");
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2((io.DisplaySize.x - 400) / 2, (io.DisplaySize.y - 300) / 2));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
        if (ImGui::BeginPopupModal("Kmbox Settings", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Dummy(ImVec2(0.0f, 7.5f));
            if (ImGui::Button("Kmbox B+", { 200, 20 })) {
                if (!kmBox::init()) {
                    std::cout << "Failed to connect to KmBox B+" << std::endl;
                }
                else {
                    settings::kmbox::kmboxb = true;
                }
            }
            ImGui::Dummy(ImVec2(0.0f, 7.5f));
            if (ImGui::Button("Kmbox .NET", { 200, 20 })) settings::kmbox::shownet = true;
            if (settings::kmbox::shownet) {
                ImGui::Dummy(ImVec2(0.0f, 7.5f));
                ImGui::SetNextItemWidth(170);
                ImGui::InputText("Ip", settings::kmbox::ip, IM_ARRAYSIZE(settings::kmbox::ip));
                ImGui::Dummy(ImVec2(0.0f, 7.5f));
                ImGui::SetNextItemWidth(170);
                ImGui::InputText("Port", settings::kmbox::port, IM_ARRAYSIZE(settings::kmbox::port));
                ImGui::Dummy(ImVec2(0.0f, 7.5f));
                ImGui::SetNextItemWidth(170);
                ImGui::InputText("UUID", settings::kmbox::uuid, IM_ARRAYSIZE(settings::kmbox::uuid));
                ImGui::Dummy(ImVec2(0.0f, 7.5f));
                if (ImGui::Button("Connect to .NET", { 200, 20 })) {
                    if (~kmNet_init(settings::kmbox::ip, settings::kmbox::port, settings::kmbox::uuid)) {
                        std::cout << "Failed to connect to KmBox .NET" << std::endl;
                    }
                    else {
                        settings::kmbox::kmboxnet = true;
                    }
                }
            }
            ImGui::Dummy(ImVec2(0.0f, 7.5f));
            if (ImGui::Button("Close", { 200, 20 })) {
                settings::kmbox::select = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleVar();
            ImGui::EndPopup();
        }
    }
}
void render_menu()
{
    ImGuiIO& io = ImGui::GetIO();

    if (GetAsyncKeyState(VK_INSERT) & 1)
    {
        settings::show_menu = !settings::show_menu;
    }

    if (settings::aimbot::show_fov) {
        ImGui::GetForegroundDrawList()->AddCircle(
            ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2),
            settings::aimbot::fov,
            ImColor(settings::aimbot::fovColor[0], settings::aimbot::fovColor[1], settings::aimbot::fovColor[2], settings::aimbot::fovColor[3]),
            100,
            1.0f
        );
    }

    if (settings::show_menu)
    {
        ImGui::SetNextWindowSize({ 620, 350 });
        ImGui::Begin("FREE", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("IS\nLE");
        ImGui::SameLine();
        if (ImGui::Button("Aimbot", { 190, 20 })) settings::tab = 0;
        ImGui::SameLine();
        if (ImGui::Button("Visuals", { 188, 20 })) settings::tab = 1;
        ImGui::SameLine();
        if (ImGui::Button("Misc", { 190, 20 })) settings::tab = 2;

        switch (settings::tab)
        {
        case 0:
            case0();
            break;

        case 1:
            case1();
            break;

        case 2:
            case2();
            break;
        }
        ImGui::End();
    }
    ImGui::EndFrame();
}

void draw_cornered_box(int x, int y, int w, int h, ImColor color, int thickness)
{

    ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 3.0f, 0, thickness);
}

void draw_filled_rect(int x, int y, int w, int h, const ImColor color)
{
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color);
}

void draw_line(const Vector2& target, const ImColor color)
{
    ImGui::GetForegroundDrawList()->AddLine(ImVec2(settings::screen_center_x, settings::height), ImVec2(target.x, target.y), color, 0.1f);
}

void draw_distance(const Vector2& location, float distance, const ImColor color)
{
    char dist[64];
    sprintf_s(dist, "%.fm", distance);
    ImVec2 text_size = ImGui::CalcTextSize(dist);
    ImGui::GetForegroundDrawList()->AddText(ImVec2(location.x - text_size.x / 2, location.y - text_size.y / 2), color, dist);
}