#ifndef PHYSIM_GUI_HPP_
#define PHYSIM_GUI_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <implot.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "exit_code.hpp"
#include "graphics/windows/base.hpp"

namespace graphics::gui {
extern std::map<std::string,
                std::vector<std::shared_ptr<graphics::gui::Window>>>
    window_map;
extern std::map<std::string, std::map<std::string, ImFont*>> font_map;
ExitCode initialize(GLFWwindow *window);
ExitCode terminate();

void register_windows();

void render();
void start_frame();
void end_frame();

void render_menu_bar();

} // namespace graphics::gui

#endif // PHYSIM_GUI_HPP_
