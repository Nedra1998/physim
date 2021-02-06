#ifndef WINDOW_BASE_HPP_
#define WINDOW_BASE_HPP_

#include <string>

#include <imgui.h>

#include "logging.hpp"

namespace graphics::gui {
template <typename T> struct ReservedQueue {
public:
  ReservedQueue(std::size_t size) : offset(0), _size(size) {
    _data.reserve(size);
  }
  inline std::vector<T> &data() { return _data; }
  inline const std::vector<T> &data() const { return _data; }

  inline T &operator[](std::size_t i) { return _data[(i + offset) % _size]; }
  inline const T &operator[](std::size_t i) const {
    return _data[(i + offset) % _size];
  }

  inline T &front() { return _data[offset]; }
  inline const T &front() const { return _data[offset]; }
  inline T &back() { return _data[((offset == 0) ? _size : offset) - 1]; }
  inline const T &back() const {
    return _data[((offset == 0) ? _size : offset) - 1];
  }
  inline T &prev() { return _data[((offset == 0) ? _size : offset) - 2]; }
  inline const T &prev() const {
    return _data[((offset == 0) ? _size : offset) - 2];
  }

  inline void push_back(const T &v) {
    if (_data.size() < _size)
      _data.push_back(v);
    else
      _data[offset] = v;
    offset = (offset + 1) % _size;
  }
  inline void clear() {
    _data.clear();
    offset = 0;
  }

  inline std::size_t size() const { return _data.size(); }
  inline std::size_t capacity() const { return _size; }

  inline void resize(const std::size_t &new_size) {
    if (new_size < _size) {
      std::vector<T> cpy = _data;
      _data.clear();
      _data.reserve(new_size);
      for (std::size_t i = 0; i < cpy.size(); ++i) {
        if (i < new_size)
          _data.push_back(cpy[(i + offset) % _size]);
        else
          _data[i % new_size] = cpy[(i + offset) % _size];
      }
      _size = new_size;
    } else {
      _size = new_size;
      _data.reserve(new_size);
    }
  }

  std::size_t offset = 0;

private:
  std::size_t _size;
  std::vector<T> _data;
};

class Window {
public:
  Window(const std::string &title, const ImGuiWindowFlags &flags = 0,
         const bool &show = false)
      : show_window(show), window_title(title), window_flags(flags) {}
  virtual ~Window() {}

  inline virtual void render() {
    if (show_window) {
      if (ImGui::Begin(window_title.c_str(), &show_window, window_flags)) {
        this->draw();
      }
      ImGui::End();
    }
  }
  virtual void draw(){};
  virtual void update(){};

  bool show_window = true;
  std::string window_title = "";
  ImGuiWindowFlags window_flags = 0;
};
} // namespace graphics::gui

#endif /* end of include guard: WINDOW_BASE_HPP_ */
