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
    //ImGui::GetIO().Fonts->AddFontFromFileTTF(include\font.otf, 18.0f);
    load_settings_from_ini();
    style();
    
    p_object->Release();
    return S_OK;
}

void create_overlay()
{
    WNDCLASSEXA wcex = {
        sizeof(WNDCLASSEXA),
        0,
        DefWindowProcA,
        0,
        0,
        0,
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
        if (ImGui::Checkbox("Kmbox B+", &settings::kmbox::kmboxb))
        {
            if (!kmBox::init())
            {
                settings::kmbox::kmboxb = false;
            }
        }

        if (ImGui::Checkbox("Kmbox Net", &settings::kmbox::kmboxnet))
        {
            std::string ip = read_file("kmnet.txt", 1);
            std::string port = read_file("kmnet.txt", 2);
            std::string uuid = read_file("kmnet.txt", 3);

            strncpy(settings::kmbox::kmbox_ip, ip.c_str(), sizeof(settings::kmbox::kmbox_ip) - 1);
            settings::kmbox::kmbox_ip[sizeof(settings::kmbox::kmbox_ip) - 1] = '\0';

            strncpy(settings::kmbox::kmbox_port, port.c_str(), sizeof(settings::kmbox::kmbox_port) - 1);
            settings::kmbox::kmbox_port[sizeof(settings::kmbox::kmbox_port) - 1] = '\0';

            strncpy(settings::kmbox::kmbox_uuid, uuid.c_str(), sizeof(settings::kmbox::kmbox_uuid) - 1);
            settings::kmbox::kmbox_uuid[sizeof(settings::kmbox::kmbox_uuid) - 1] = '\0';

            kmNet_init(settings::kmbox::kmbox_ip, settings::kmbox::kmbox_port, settings::kmbox::kmbox_uuid);
        }
        if ((settings::kmbox::kmboxb) || (settings::kmbox::kmboxnet))
        {
            settings::aimbot::scaledProjectileSpeed = settings::aimbot::projectileSpeed * 1000.0f;
            ImGui::Checkbox("Show FOV Circle", &settings::aimbot::show_fov);
            if (settings::aimbot::show_fov) {
            ImGui::SameLine();
            ImGui::ColorEdit4("Fov Color", settings::aimbot::fovColor, ImGuiColorEditFlags_NoInputs);
            }
            ImGui::Checkbox("Triggerbot", &settings::aimbot::triggerbot);
            ImGui::SliderFloat("FOV Radius", &settings::aimbot::fov, 50.0f, 300.0f, "%.2f");
            ImGui::SliderFloat("Smoothness", &settings::aimbot::smoothness, 1.0f, 25.0f, "%.2f");
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
        ImGui::Checkbox("Skeleton", &settings::visuals::skeleton);
        ImGui::Checkbox("Line", &settings::visuals::line);
        ImGui::Checkbox("Distance", &settings::visuals::distance);
        ImGui::ColorEdit4("Visible", settings::visuals::boxColor, ImGuiColorEditFlags_NoInputs);
        ImGui::SameLine();
        ImGui::ColorEdit4("Non-Visible", settings::visuals::boxColor2, ImGuiColorEditFlags_NoInputs);
    }
}

void case2()
{
    ImGui::Checkbox("FPS", &settings::visuals::fps);
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

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemSpacing.y);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Close", { 120, 20 })) exit(0);
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().ItemSpacing.y);
    ImGui::SetCursorPosX(10);
    if (ImGui::Button("Save Config", { 120, 20 })) save_settings_to_ini();
    ImGui::SameLine();
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
                ImGui::Text("Mouse should've moved");
                kmBox::sendMove(50, 50);
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            if (ImGui::Button("Test KmboxB click", { 150, 20 })) {
                ImGui::Text("Mouse should've moved");
                kmBox::kmclick();
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f)); 
            if (ImGui::Button("Test Kmbox.NET move", { 150, 20 })) {
                ImGui::Text("Mouse should've moved");
                kmNet_mouse_move(50, 50);
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f)); 
            if (ImGui::Button("Test Kmbox.NET click", { 150, 20 })) {
                ImGui::Text("Mouse should've moved");
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
}
void render_menu()
{
    ImGuiIO& io = ImGui::GetIO();
    if (settings::visuals::fps)
    {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("FPS: %.2f", settings::framerate);
        ImGui::End();
    }

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
        ImGui::Begin("Jouh", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("JO\nUH");
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

void skeleton(uintptr_t skeleton_mesh, const ImColor color)
{
        if (!skeleton_mesh || !settings::visuals::skeleton) return;

        auto neck = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_HEAD));
        auto chest = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_NECK));
        auto left_shoulder = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_LSHOULDER));
        auto left_elbow = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_LELBOW));
        auto left_hand = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_LHAND));
        auto right_shoulder = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_RSHOULDER));
        auto right_elbow = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_RELBOW));
        auto right_hand = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_RHAND));
        auto pelvis = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_PELVIS));
        auto left_hip = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_LTHIGH));
        auto left_knee = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_LKNEE));
        auto left_foot = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_LFOOT));
        auto right_hip = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_RTHIGH));
        auto right_knee = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_RKNEE));
        auto right_foot = project_world_to_screen(get_entity_bone(skeleton_mesh, bone::BONE_RFOOT));

        auto draw_list = ImGui::GetBackgroundDrawList();
        auto thickness = settings::visuals::skeleton_thick + 2.5f;

        draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(chest.x, chest.y), color, thickness);
        draw_list->AddLine(ImVec2(chest.x, chest.y), ImVec2(right_shoulder.x, right_shoulder.y), color, thickness);
        draw_list->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(chest.x, chest.y), color, thickness);
        draw_list->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), color, thickness);
        draw_list->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), color, thickness);
        draw_list->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), color, thickness);
        draw_list->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), color, thickness);
        draw_list->AddLine(ImVec2(chest.x, chest.y), ImVec2(pelvis.x, pelvis.y), color, thickness);
        draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), color, thickness);
        draw_list->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), color, thickness);
        draw_list->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), color, thickness);
        draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), color, thickness);
        draw_list->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), color, thickness);
        draw_list->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), color, thickness);
   
}