#include "GUI.h"

GUI::GUI()
	: m_style(nullptr) {}

GUI::GUI(sf::RenderWindow& window)
	: m_style(nullptr) {

	Init(window);
    InitStyle();
}

void GUI::Init(sf::RenderWindow& window) {

	LOG("-- Initializing ImGUI --");
	ImGui::SFML::Init(window);
}

void GUI::Update(sf::RenderWindow& window, sf::Time dt) {

	ImGui::SFML::Update(window, dt);
}

void GUI::ProcessEvent(sf::RenderWindow& window, const sf::Event& event) {

	ImGui::SFML::ProcessEvent(window, event);
}

void GUI::Render(sf::RenderWindow& window) {

	ImGui::SFML::Render(window);
}

void GUI::Close() {

	LOG("-- Closing ImGUI --");
	ImGui::SFML::Shutdown();
}

void GUI::InitStyle() {

	m_style = &ImGui::GetStyle();

    m_style->Alpha = 1.0f;
    m_style->DisabledAlpha = 0.6000000238418579f;
    m_style->WindowPadding = ImVec2(8.0f, 8.0f);
    m_style->WindowRounding = 0.0f;
    m_style->WindowBorderSize = 1.0f;
    m_style->WindowMinSize = ImVec2(32.0f, 32.0f);
    m_style->WindowTitleAlign = ImVec2(0.0f, 0.5f);
    m_style->WindowMenuButtonPosition = ImGuiDir_Left;
    m_style->ChildRounding = 0.0f;
    m_style->ChildBorderSize = 1.0f;
    m_style->PopupRounding = 0.0f;
    m_style->PopupBorderSize = 1.0f;
    m_style->FramePadding = ImVec2(4.0f, 3.0f);
    m_style->FrameRounding = 0.0f;
    m_style->FrameBorderSize = 1.0f;
    m_style->ItemSpacing = ImVec2(8.0f, 4.0f);
    m_style->ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    m_style->CellPadding = ImVec2(4.0f, 2.0f);
    m_style->IndentSpacing = 21.0f;
    m_style->ColumnsMinSpacing = 6.0f;
    m_style->ScrollbarSize = 14.0f;
    m_style->ScrollbarRounding = 0.0f;
    m_style->GrabMinSize = 10.0f;
    m_style->GrabRounding = 0.0f;
    m_style->TabRounding = 0.0f;
    m_style->TabBorderSize = 0.0f;
    m_style->TabMinWidthForCloseButton = 0.0f;
    m_style->ColorButtonPosition = ImGuiDir_Right;
    m_style->ButtonTextAlign = ImVec2(0.5f, 0.5f);
    m_style->SelectableTextAlign = ImVec2(0.0f, 0.0f);
    m_style->Colors[ImGuiCol_Text] = ImVec4(1.000000f, 1.000000f, 1.000000f, 1.000000f);
    m_style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.498039f, 0.498039f, 0.498039f, 1.000000f);
    m_style->Colors[ImGuiCol_WindowBg] = ImVec4(0.286275f, 0.337255f, 0.258824f, 1.000000f);
    m_style->Colors[ImGuiCol_ChildBg] = ImVec4(0.286275f, 0.337255f, 0.258824f, 1.000000f);
    m_style->Colors[ImGuiCol_PopupBg] = ImVec4(0.239216f, 0.266667f, 0.200000f, 1.000000f);
    m_style->Colors[ImGuiCol_Border] = ImVec4(0.537255f, 0.568627f, 0.509804f, 0.500000f);
    m_style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.137255f, 0.156863f, 0.109804f, 0.520000f);
    m_style->Colors[ImGuiCol_FrameBg] = ImVec4(0.239216f, 0.266667f, 0.200000f, 1.000000f);
    m_style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.266667f, 0.298039f, 0.227451f, 1.000000f);
    m_style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.298039f, 0.337255f, 0.258824f, 1.000000f);
    m_style->Colors[ImGuiCol_TitleBg] = ImVec4(0.239216f, 0.266667f, 0.200000f, 1.000000f);
    m_style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.286275f, 0.337255f, 0.258824f, 1.000000f);
    m_style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.000000f, 0.000000f, 0.000000f, 0.510000f);
    m_style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.239216f, 0.266667f, 0.200000f, 1.000000f);
    m_style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.349020f, 0.419608f, 0.309804f, 1.000000f);
    m_style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.278431f, 0.317647f, 0.239216f, 1.000000f);
    m_style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.247059f, 0.298039f, 0.219608f, 1.000000f);
    m_style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.227451f, 0.266667f, 0.207843f, 1.000000f);
    m_style->Colors[ImGuiCol_CheckMark] = ImVec4(0.588235f, 0.537255f, 0.176471f, 1.000000f);
    m_style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.349020f, 0.419608f, 0.309804f, 1.000000f);
    m_style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.537255f, 0.568627f, 0.509804f, 0.500000f);
    m_style->Colors[ImGuiCol_Button] = ImVec4(0.286275f, 0.337255f, 0.258824f, 0.400000f);
    m_style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.349020f, 0.419608f, 0.309804f, 1.000000f);
    m_style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.537255f, 0.568627f, 0.509804f, 0.500000f);
    m_style->Colors[ImGuiCol_Header] = ImVec4(0.349020f, 0.419608f, 0.309804f, 1.000000f);
    m_style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.349020f, 0.419608f, 0.309804f, 0.600000f);
    m_style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.537255f, 0.568627f, 0.509804f, 0.500000f);
    m_style->Colors[ImGuiCol_Separator] = ImVec4(0.137255f, 0.156863f, 0.109804f, 1.000000f);
    m_style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.537255f, 0.568627f, 0.509804f, 1.000000f);
    m_style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.588235f, 0.537255f, 0.176471f, 1.000000f);
    m_style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.188235f, 0.227451f, 0.176471f, 0.000000f);
    m_style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.537255f, 0.568627f, 0.509804f, 1.000000f);
    m_style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.588235f, 0.537255f, 0.176471f, 1.000000f);
    m_style->Colors[ImGuiCol_Tab] = ImVec4(0.349020f, 0.419608f, 0.309804f, 1.000000f);
    m_style->Colors[ImGuiCol_TabHovered] = ImVec4(0.537255f, 0.568627f, 0.509804f, 0.780000f);
    m_style->Colors[ImGuiCol_TabActive] = ImVec4(0.588235f, 0.537255f, 0.176471f, 1.000000f);
    m_style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.239216f, 0.266667f, 0.200000f, 1.000000f);
    m_style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.349020f, 0.419608f, 0.309804f, 1.000000f);
    m_style->Colors[ImGuiCol_PlotLines] = ImVec4(0.607843f, 0.607843f, 0.607843f, 1.000000f);
    m_style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.588235f, 0.537255f, 0.176471f, 1.000000f);
    m_style->Colors[ImGuiCol_PlotHistogram] = ImVec4(1.000000f, 0.776471f, 0.278431f, 1.000000f);
    m_style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000000f, 0.600000f, 0.000000f, 1.000000f);
    m_style->Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.188235f, 0.188235f, 0.200000f, 1.000000f);
    m_style->Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.309804f, 0.309804f, 0.349020f, 1.000000f);
    m_style->Colors[ImGuiCol_TableBorderLight] = ImVec4(0.227451f, 0.227451f, 0.247059f, 1.000000f);
    m_style->Colors[ImGuiCol_TableRowBg] = ImVec4(0.000000f, 0.000000f, 0.000000f, 0.000000f);
    m_style->Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.000000f, 1.000000f, 1.000000f, 0.060000f);
    m_style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.588235f, 0.537255f, 0.176471f, 1.000000f);
    m_style->Colors[ImGuiCol_DragDropTarget] = ImVec4(0.729412f, 0.666667f, 0.239216f, 1.000000f);
    m_style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.588235f, 0.537255f, 0.176471f, 1.000000f);
    m_style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000000f, 1.000000f, 1.000000f, 0.700000f);
    m_style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000f, 0.800000f, 0.800000f, 0.200000f);
    m_style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000f, 0.800000f, 0.800000f, 0.350000f);
}