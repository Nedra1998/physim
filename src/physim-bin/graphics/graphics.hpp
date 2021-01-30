#ifndef PHYSIM_GRAPHICS_HPP_
#define PHYSIM_GRAPHICS_HPP_

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "exit_code.hpp"
namespace graphics {
extern GLFWwindow *window;
ExitCode run();
ExitCode initialize(const std::string& title, unsigned width, unsigned height);
ExitCode terminate();
} // namespace graphics

#endif // PHYSIM_GRAPHICS_HPP_
