#include "log.hpp"

#include <fmt/chrono.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <magic_enum.hpp>

#include "graphics/colors.hpp"
#include "logging.hpp"

graphics::gui::SpdlogWindow::SpdlogWindow(std::size_t buffer)
    : Window("\uf529 Logs", 0, true), buffer_size(buffer),
      sink(physim::logging::_buffer_sink) {}
graphics::gui::SpdlogWindow::~SpdlogWindow() {}
void graphics::gui::SpdlogWindow::draw() {

  if (!regex_filter.has_value() && !compiled_regex_str.empty())
    ImGui::PushStyleColor(ImGuiCol_Text,
                          colors::ImLogColors[spdlog::level::level_enum::err]);
  ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.7f);
  ImGui::InputTextWithHint("##Filter", "Regex log filter...",
                           &regex_filter_str);
  if (!regex_filter.has_value() && !compiled_regex_str.empty())
    ImGui::PopStyleColor();

  ImGui::SameLine();
  const char *log_level_names[] = {"Trace", "Debug",    "Info", "Warning",
                                   "Error", "Critical", "Off"};
  ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25f);
  ImGui::Combo("##Level", &level_filter, log_level_names,
               sizeof(log_level_names) / sizeof(*log_level_names));

  if (ImGui::BeginTable(
          "log_table", 8,
          ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
              ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable |
              ImGuiTableFlags_SortMulti | ImGuiTableFlags_ScrollY,
          ImVec2(0.0f, 0.0f), 0.0f)) {
    ImGui::TableSetupColumn("Timestamp",
                            ImGuiTableColumnFlags_DefaultSort |
                                ImGuiTableColumnFlags_WidthFixed,
                            0.0f, TIMESTAMP);
    ImGui::TableSetupColumn("Logger",
                            ImGuiTableColumnFlags_DefaultSort |
                                ImGuiTableColumnFlags_WidthFixed,
                            0.0f, LOGGER);
    ImGui::TableSetupColumn("Level",
                            ImGuiTableColumnFlags_DefaultSort |
                                ImGuiTableColumnFlags_WidthFixed,
                            0.0f, LEVEL);
    ImGui::TableSetupColumn("File",
                            ImGuiTableColumnFlags_DefaultSort |
                                ImGuiTableColumnFlags_WidthFixed,
                            0.0f, FILENAME);
    ImGui::TableSetupColumn("Line",
                            ImGuiTableColumnFlags_DefaultSort |
                                ImGuiTableColumnFlags_WidthFixed,
                            0.0f, LINE);
    ImGui::TableSetupColumn("Function",
                            ImGuiTableColumnFlags_DefaultSort |
                                ImGuiTableColumnFlags_WidthFixed,
                            0.0f, FUNCNAME);
    ImGui::TableSetupColumn("Thread",
                            ImGuiTableColumnFlags_DefaultSort |
                                ImGuiTableColumnFlags_WidthFixed,
                            0.0f, THREAD);
    ImGui::TableSetupColumn("Message",
                            ImGuiTableColumnFlags_DefaultSort |
                                ImGuiTableColumnFlags_WidthStretch,
                            0.0f, MSG);
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableHeadersRow();

    if (!sink->buffer.empty()) {
      const std::lock_guard<std::mutex> lock(sink->buffer_mutex);
      for (auto &msg : sink->buffer) {
        std::vector<physim::logging::LogMsg>::iterator it = std::upper_bound(
            buffer.begin(), buffer.end(), msg,
            [&](const physim::logging::LogMsg &a,
                const physim::logging::LogMsg &b) { return this->sort(a, b); });
        if (it != buffer.end())
          buffer.insert(it, msg);
        else
          buffer.push_back(msg);
      }
      sink->buffer.clear();
    }

    if (buffer.size() > buffer_size) {
      for (std::size_t i = 0; i < buffer.size() - buffer_size; ++i) {
        std::vector<physim::logging::LogMsg>::iterator it =
            std::min_element(buffer.begin(), buffer.end(),
                             [](const physim::logging::LogMsg &lhs,
                                const physim::logging::LogMsg &rhs) {
                               return lhs.time.time_since_epoch().count() <
                                      rhs.time.time_since_epoch().count();
                             });
        if (it != buffer.end())
          buffer.erase(it);
      }
    }

    ImGuiTableSortSpecs *sort_specs = ImGui::TableGetSortSpecs();
    if (sort_specs != nullptr && sort_specs->SpecsDirty) {
      current_sort_specs = sort_specs;
      std::sort(
          buffer.begin(), buffer.end(),
          [&](const physim::logging::LogMsg &a,
              const physim::logging::LogMsg &b) { return this->sort(a, b); });
      sort_specs->SpecsDirty = false;
    }

    if (regex_filter_str.size() != 0) {
      if (regex_filter_str != compiled_regex_str.data()) {
        compiled_regex_str = regex_filter_str;
        regex_filter = std::optional<std::regex>();
        try {
          regex_filter = std::regex(compiled_regex_str,
                                    std::regex::optimize | std::regex::nosubs);
        } catch (std::regex_error err) {
          SPDLOG_DEBUG("Invalid RegEx \"{}\"", compiled_regex_str);
        }
      }
    } else if (regex_filter.has_value()) {
      compiled_regex_str = std::string();
      regex_filter = std::optional<std::regex>();
    }

    for (std::size_t i = 0; i < buffer.size(); ++i) {
      physim::logging::LogMsg *msg = &buffer[i];

      if (msg->level < level_filter)
        continue;

      if (regex_filter.has_value()) {
        if (!std::regex_search(msg->filename, regex_filter.value()) &&
            !std::regex_search(msg->funcname, regex_filter.value()) &&
            !std::regex_search(msg->payload, regex_filter.value()))
          continue;
      }

      ImGui::PushID(static_cast<int>(i));
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      std::time_t time = spdlog::log_clock::to_time_t(msg->time);
      ImGui::Text(
          "%s", fmt::format(
                    "{:%Y-%m-%d %H:%M:%S}.{:03}", fmt::localtime(time),
                    detail::time_fraction<std::chrono::milliseconds>(msg->time)
                        .count())
                    .c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%s", msg->logger_name.c_str());
      ImGui::TableNextColumn();
      ImGui::TextColored(
          colors::ImLogColors[msg->level], "%s",
          fmt::format("{}", spdlog::level::to_string_view(msg->level)).c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%s", msg->filename.c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%d", msg->line);
      ImGui::TableNextColumn();
      ImGui::Text("%s", msg->funcname.c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%lu", msg->thread_id);
      ImGui::TableNextColumn();
      ImGui::Text("%s", msg->payload.c_str());
      ImGui::PopID();
    }

    ImGui::EndTable();
  }
}

bool graphics::gui::SpdlogWindow::sort(const physim::logging::LogMsg &a,
                                       const physim::logging::LogMsg &b) const {
  if (current_sort_specs != nullptr) {
    for (int i = 0; i < current_sort_specs->SpecsCount; ++i) {
      const ImGuiTableColumnSortSpecs *spec = &current_sort_specs->Specs[i];
      int delta = 0;
      switch (spec->ColumnUserID) {
      case TIMESTAMP:
        delta = ((a.time - b.time).count() > 0) ? +1 : -1;
        break;
      case LEVEL:
        delta = static_cast<int>(a.level) - static_cast<int>(b.level);
        break;
      case FILENAME:
        delta = a.filename.compare(b.filename);
        break;
      case FUNCNAME:
        delta = a.funcname.compare(b.funcname);
        break;
      case LINE:
        delta = (a.line - b.line);
        break;
      case THREAD:
        delta = static_cast<int>(a.thread_id) - static_cast<int>(b.thread_id);
        break;
      case LOGGER:
        delta = a.logger_name.compare(b.logger_name);
        break;
      case MSG:
        delta = a.payload.compare(b.payload);
        break;
      }
      if (delta > 0)
        return (spec->SortDirection == ImGuiSortDirection_Ascending) ? true
                                                                     : false;
      else if (delta < 0)
        return (spec->SortDirection == ImGuiSortDirection_Ascending) ? false
                                                                     : true;
    }
  }
  return (a.time - b.time).count() > 0;
}
