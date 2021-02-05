#include "colors.hpp"

#include <imgui.h>

static graphics::colors::Pallet color_pallet = graphics::colors::ONE_DARK;

namespace graphics::colors {
ImVec4 ImTermColors[ImTermCol_COUNT];
ImVec4 ImLogColors[ImLogCol_COUNT];
} // namespace graphics::colors

static const std::map<std::string, std::string> *pallet =
    &graphics::colors::one_dark;

void graphics::colors::parse_hex(const std::string &hex, float &r, float &g,
                                 float &b) {
  std::uint8_t ur, ug, ub;
  parse_hex(hex, ur, ug, ub);
  r = static_cast<float>(ur) / 255.0f;
  g = static_cast<float>(ug) / 255.0f;
  b = static_cast<float>(ub) / 255.0f;
}
void graphics::colors::parse_hex(const std::string &hex, std::uint8_t &r,
                                 std::uint8_t &g, std::uint8_t &b) {
  unsigned ur, ug, ub;
  if (hex.size() == 7) {
    std::sscanf(hex.c_str(), "#%02x%02x%02x", &ur, &ug, &ub);
  } else {
    std::sscanf(hex.c_str(), "%02x%02x%02x", &ur, &ug, &ub);
  }
  r = static_cast<std::uint8_t>(ur);
  g = static_cast<std::uint8_t>(ug);
  b = static_cast<std::uint8_t>(ub);
}
void graphics::colors::set_color_pallet(const Pallet &new_pallet) {
  color_pallet = new_pallet;
  switch (color_pallet) {
  default:
  case ONE_DARK:
    pallet = &one_dark;
    break;
  case ONE_LIGHT:
    pallet = &one_light;
    break;
  }
  set_colors();
}
ImVec4 hex(const std::string &hex) {
  float r, g, b;
  if (hex[0] == '#') {
    graphics::colors::parse_hex(hex, r, g, b);
  } else {
    graphics::colors::parse_hex(pallet->at(hex), r, g, b);
  }
  return ImVec4(r, g, b, 1.00f);
}
ImVec4 darken(const ImVec4 &rgba, float v) {
  return ImVec4(rgba.x * v, rgba.y * v, rgba.z * v, rgba.w);
}

void graphics::colors::set_colors() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;
  colors[ImGuiCol_Text] = hex("fg1");
  colors[ImGuiCol_TextDisabled] = hex("fg3");
  colors[ImGuiCol_WindowBg] = hex("bg0");
  colors[ImGuiCol_ChildBg] = colors[ImGuiCol_WindowBg];
  colors[ImGuiCol_PopupBg] = hex("bg1");
  colors[ImGuiCol_Border] = hex("bg4");
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = colors[ImGuiCol_WindowBg];
  colors[ImGuiCol_FrameBgHovered] = hex("bg2");
  colors[ImGuiCol_FrameBgActive] = colors[ImGuiCol_FrameBgHovered];
  colors[ImGuiCol_TitleBg] = colors[ImGuiCol_WindowBg];
  colors[ImGuiCol_TitleBgActive] = darken(hex("blue"), 0.5);
  colors[ImGuiCol_TitleBgCollapsed] = hex("brightBlue");
  colors[ImGuiCol_MenuBarBg] = colors[ImGuiCol_PopupBg];
  colors[ImGuiCol_ScrollbarBg] = colors[ImGuiCol_WindowBg];
  colors[ImGuiCol_ScrollbarGrab] = colors[ImGuiCol_PopupBg];
  colors[ImGuiCol_ScrollbarGrabHovered] = colors[ImGuiCol_FrameBgHovered];
  colors[ImGuiCol_ScrollbarGrabActive] = colors[ImGuiCol_FrameBgHovered];
  colors[ImGuiCol_CheckMark] = hex("blue");
  colors[ImGuiCol_SliderGrab] = colors[ImGuiCol_CheckMark];
  colors[ImGuiCol_SliderGrabActive] = colors[ImGuiCol_CheckMark];
  colors[ImGuiCol_Button] = darken(hex("blue"), 0.5);
  colors[ImGuiCol_ButtonHovered] = colors[ImGuiCol_CheckMark];
  colors[ImGuiCol_ButtonActive] = darken(hex("blue"), 0.7f);
  colors[ImGuiCol_Header] = colors[ImGuiCol_PopupBg];
  colors[ImGuiCol_HeaderHovered] = colors[ImGuiCol_FrameBgHovered];
  colors[ImGuiCol_HeaderActive] = hex("bg3");
  colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = colors[ImGuiCol_Button];
  colors[ImGuiCol_ResizeGripHovered] = colors[ImGuiCol_ButtonHovered];
  colors[ImGuiCol_ResizeGripActive] = colors[ImGuiCol_ButtonActive];
  colors[ImGuiCol_Tab] = colors[ImGuiCol_PopupBg];
  colors[ImGuiCol_TabHovered] = colors[ImGuiCol_FrameBgHovered];
  colors[ImGuiCol_TabActive] = darken(hex("blue"), 0.7f);
  colors[ImGuiCol_TabUnfocused] = colors[ImGuiCol_Tab];
  colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabActive];
  colors[ImGuiCol_DockingPreview] = colors[ImGuiCol_CheckMark];
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotLines] = hex("red");
  colors[ImGuiCol_PlotLinesHovered] = hex("brightRed");
  colors[ImGuiCol_PlotHistogram] = hex("yellow");
  colors[ImGuiCol_PlotHistogramHovered] = hex("brightYellow");
  colors[ImGuiCol_TableHeaderBg] = colors[ImGuiCol_FrameBgHovered];
  colors[ImGuiCol_TableBorderStrong] = hex("fg4");
  colors[ImGuiCol_TableBorderLight] = colors[ImGuiCol_FrameBgHovered];
  colors[ImGuiCol_TableRowBg] = colors[ImGuiCol_WindowBg];
  colors[ImGuiCol_TableRowBgAlt] = colors[ImGuiCol_PopupBg];
  colors[ImGuiCol_TextSelectedBg] = colors[ImGuiCol_FrameBgHovered];
  colors[ImGuiCol_DragDropTarget] = hex("brightBlue");
  colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = hex("brightBlue");
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_ModalWindowDimBg] = colors[ImGuiCol_WindowBg];

  ImTermColors[ImTermCol_Foreground] = hex("fg1");
  ImTermColors[ImTermCol_Background] = hex("bg0");
  ImTermColors[ImTermCol_Black] = hex("bg1");
  ImTermColors[ImTermCol_Red] = hex("red");
  ImTermColors[ImTermCol_Green] = hex("green");
  ImTermColors[ImTermCol_Yellow] = hex("yellow");
  ImTermColors[ImTermCol_Blue] = hex("blue");
  ImTermColors[ImTermCol_Magenta] = hex("magenta");
  ImTermColors[ImTermCol_Cyan] = hex("cyan");
  ImTermColors[ImTermCol_White] = hex("fg4");
  ImTermColors[ImTermCol_BrightBlack] = hex("bg4");
  ImTermColors[ImTermCol_BrightRed] = hex("brightRed");
  ImTermColors[ImTermCol_BrightGreen] = hex("brightGreen");
  ImTermColors[ImTermCol_BrightYellow] = hex("brightYellow");
  ImTermColors[ImTermCol_BrightBlue] = hex("brightBlue");
  ImTermColors[ImTermCol_BrightMagenta] = hex("brightMagenta");
  ImTermColors[ImTermCol_BrightCyan] = hex("brightCyan");
  ImTermColors[ImTermCol_BrightWhite] = hex("fg0");

  ImLogColors[ImLogCol_Trace] = ImTermColors[ImTermCol_Magenta];
  ImLogColors[ImLogCol_Debug] = ImTermColors[ImTermCol_Cyan];
  ImLogColors[ImLogCol_Info] = ImTermColors[ImTermCol_Green];
  ImLogColors[ImLogCol_Warning] = ImTermColors[ImTermCol_Yellow];
  ImLogColors[ImLogCol_Error] = ImTermColors[ImTermCol_Red];
  ImLogColors[ImLogCol_Critical] = ImTermColors[ImTermCol_Red];
}
