#include <pch.h>

void style() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 5.0f;

    auto adjustColor = [](ImVec4 color, float factor) -> ImVec4 {
        color.x = std::clamp(color.x * factor, 0.0f, 1.0f);
        color.y = std::clamp(color.y * factor, 0.0f, 1.0f);
        color.z = std::clamp(color.z * factor, 0.0f, 1.0f);
        return color;
        };

    style.Colors[ImGuiCol_Text] = ImVec4(settings::misc::textColor[0], settings::misc::textColor[1], settings::misc::textColor[2], settings::misc::textColor[3]);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(settings::misc::bgColor[0], settings::misc::bgColor[1], settings::misc::bgColor[2], settings::misc::bgColor[3]);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(settings::misc::bgColor[0], settings::misc::bgColor[1], settings::misc::bgColor[2], settings::misc::bgColor[3]);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.17f, 0.18f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = adjustColor(style.Colors[ImGuiCol_FrameBg], 1.2f);
    style.Colors[ImGuiCol_FrameBgActive] = adjustColor(style.Colors[ImGuiCol_FrameBg], 0.8f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = adjustColor(style.Colors[ImGuiCol_TitleBg], 1.2f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    style.Colors[ImGuiCol_CheckMark] = ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]);
    style.Colors[ImGuiCol_SliderGrabActive] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 0.8f);
    style.Colors[ImGuiCol_Button] = ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]);
    style.Colors[ImGuiCol_ButtonHovered] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_ButtonActive] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 0.8f);
    style.Colors[ImGuiCol_Header] = ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]);
    style.Colors[ImGuiCol_HeaderHovered] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_HeaderActive] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 0.8f);
    style.Colors[ImGuiCol_SeparatorHovered] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_SeparatorActive] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 0.8f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]);
    style.Colors[ImGuiCol_ResizeGripHovered] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_ResizeGripActive] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 0.8f);
    style.Colors[ImGuiCol_TabHovered] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_PlotLinesHovered] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_TextSelectedBg] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_DragDropTarget] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
    style.Colors[ImGuiCol_NavHighlight] = adjustColor(ImVec4(settings::misc::blueColor[0], settings::misc::blueColor[1], settings::misc::blueColor[2], settings::misc::blueColor[3]), 1.2f);
}


// Example of config saving & loading

//if (ImGui::Button("Save Config")) save_config();
//if (ImGui::Button("Load Config")) load_config();

void load_config() {
    std::ifstream txt("config.txt");
    if (!txt.is_open()) return;

    std::string line;
    while (std::getline(txt, line)) {
        if (line.find("Aimbot") != std::string::npos) {
            txt >> settings::aimbot::enable;
        }
        else if (line.find("Visuals") != std::string::npos) {
            txt >> settings::visuals::enable;
        }
        else if (line.find("Misc") != std::string::npos) {
            txt >> settings::visuals::fps;
        }
    }
    txt.close();
}

void save_config() {
    std::ofstream txt("config.txt");
    if (!txt.is_open()) return;

    txt << "[Aimbot]\n";
    txt << settings::aimbot::enable << "\n";
  
    txt << "[Visuals]\n";
    txt << settings::visuals::enable << "\n";

    txt << "[Misc]\n";
    txt << settings::visuals::fps << "\n";


    txt.close();
}

