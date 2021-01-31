#ifndef PHYSIM_GRAPHICS_COLORS_HPP_
#define PHYSIM_GRAPHICS_COLORS_HPP_

#include "imgui.h"
#include <map>
#include <string>

namespace graphics::colors {
enum Pallet { ONE_DARK, ONE_LIGHT };
static const std::map<std::string, std::string> one_dark = {
    {"bg0", "#282c34"},         {"bg1", "#353b45"},
    {"bg2", "#3e4451"},         {"bg3", "#545862"},
    {"bg4", "#565c64"},         {"fg4", "#abb2bf"},
    {"fg3", "#abb2bf"},         {"fg2", "#b6bdca"},
    {"fg1", "#c8ccd4"},         {"fg0", "#e6e6e6"},
    {"red", "#e05561"},         {"green", "#8cc265"},
    {"yellow", "#d19a66"},      {"blue", "#4aa5f0"},
    {"magenta", "#c162de"},     {"cyan", "#42b3c2"},
    {"orange", "#d19a66"},      {"brightRed", "#ff616e"},
    {"brightGreen", "#a5e075"}, {"brightYellow", "#f0a45d"},
    {"brightBlue", "#4dc4ff"},  {"brightMagenta", "#de73ff"},
    {"brightCyan", "#4cd1e0"},  {"brightOrange", "#f0a45d"}};
static const std::map<std::string, std::string> one_light = {
    {"bg0", "#fafafa"},         {"bg1", "#f0f0f1"},
    {"bg2", "#e5e5e6"},         {"bg3", "#e5e5e6"},
    {"bg4", "#696c77"},         {"fg4", "#696c77"},
    {"fg3", "#383a42"},         {"fg2", "#383a42"},
    {"fg1", "#202227"},         {"fg0", "#090a0b"},
    {"red", "#ca1243"},         {"green", "#50A14F"},
    {"yellow", "#986801"},      {"blue", "#4078F2"},
    {"magenta", "#A626A4"},     {"cyan", "#0184BC"},
    {"orange", "#d75f00"},      {"brightRed", "#e06c75"},
    {"brightGreen", "#98c379"}, {"brightYellow", "#e5c07b"},
    {"brightBlue", "#61afef"},  {"brightMagenta", "#c678dd"},
    {"brightCyan", "#56b6c2"},  {"brightOrange", "#d19a66"}};

enum ImTermCol_ {
    ImTermCol_Foreground,
    ImTermCol_Background,
    ImTermCol_Black,
    ImTermCol_Red,
    ImTermCol_Green,
    ImTermCol_Yellow,
    ImTermCol_Blue,
    ImTermCol_Magenta,
    ImTermCol_Cyan,
    ImTermCol_White,
    ImTermCol_BrightBlack,
    ImTermCol_BrightRed,
    ImTermCol_BrightGreen,
    ImTermCol_BrightYellow,
    ImTermCol_BrightBlue,
    ImTermCol_BrightMagenta,
    ImTermCol_BrightCyan,
    ImTermCol_BrightWhite,
    ImTermCol_COUNT
};
extern ImVec4 ImTermColors[ImTermCol_COUNT];

enum ImLogCol_ {
    ImLogCol_Trace = 0,
    ImLogCol_Debug = 1,
    ImLogCol_Info = 2,
    ImLogCol_Warning = 3,
    ImLogCol_Error = 4,
    ImLogCol_Critical = 5,
    ImLogCol_COUNT
};
extern ImVec4 ImLogColors[ImLogCol_COUNT];

void parse_hex(const std::string &hex, float &r, float &g, float &b);
void parse_hex(const std::string &hex, std::uint8_t &r, std::uint8_t &g,
               std::uint8_t &b);
void set_color_pallet(const Pallet &pallet);
void set_colors();
} // namespace graphics::colors

#endif // PHYSIM_GRAPHICS_COLORS_HPP_
