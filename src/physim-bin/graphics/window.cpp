#include "window.hpp"

#include <algorithm>
#include <chrono>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "colors.hpp"
#include "gui.hpp"
#include "logging.hpp"
#include "magic_enum.hpp"
#include "spdlog/common.h"

void graphics::gui::ImGuiDemoWindow::render() {
  if (show_window) {
    SPDLOG_TRACE("Rendering ImGui window \"{}\"", window_title);
    ImGui::ShowDemoWindow(&show_window);
  }
}
void graphics::gui::ImPlotDemoWindow::render() {
  if (show_window) {
    SPDLOG_TRACE("Rendering ImGui window \"{}\"", window_title);
    ImPlot::ShowDemoWindow(&show_window);
  }
}

// const ImGuiTableSortSpecs *graphics::gui::SpdlogWindow::current_sort_specs =
// nullptr;
graphics::gui::SpdlogWindow::SpdlogWindow(std::size_t buffer)
    : Window("\uf529 Logs", 0, true), buffer_size(buffer),
      sink(std::make_shared<logging::buffer_sink_mt>(buffer)) {
  logging::default_logger->sinks().push_back(sink);
}
graphics::gui::SpdlogWindow::~SpdlogWindow() {
  auto sinks = logging::default_logger->sinks();
  sinks.erase(std::remove(sinks.begin(), sinks.end(), sink), sinks.end());
}
void graphics::gui::SpdlogWindow::draw() {
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

    // bool should_sort = false;

    if (!sink->buffer.empty()) {
      const std::lock_guard<std::mutex> lock(sink->buffer_mutex);
      for (auto &msg : sink->buffer) {
        // std::vector<logging::LogMsg>::iterator it = std::upper_bound(
        //     buffer.begin(), buffer.end(), msg, compare_with_sort);
        // if (it != buffer.end())
        //   buffer.insert(it, msg);
        // else
        buffer.push_back(msg);
      }
      sink->buffer.clear();
    }

    if (buffer.size() > buffer_size) {
      buffer.erase(buffer.begin(),
                   buffer.begin() +
                       static_cast<long>(buffer.size() - buffer_size));
    }

    // ImGuiTableSortSpecs *sort_specs = ImGui::TableGetSortSpecs();
    // if (sort_specs && sort_specs->SpecsDirty) {
    //   current_sort_specs = sort_specs;
    //   should_sort = true;
    // }
    // if (should_sort && buffer.size() > 1) {
    //   std::sort(buffer.begin(), buffer.end(), compare_with_sort);
    //   if (sort_specs)
    //     sort_specs->SpecsDirty = false;
    // }

    for (std::size_t i = 0; i < buffer.size(); ++i) {
      logging::LogMsg *msg = &buffer[buffer.size() - 1 - i];
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
    // static bool items_need_sort = false;
    // if (items.Size != static_cast<int>(sink->buffer.size())) {
    //   items.resize(static_cast<int>(sink->buffer.size()), logging::LogMsg());
    //   for(int n = 0; n < items.Size; ++n) {
    //     logging::LogMsg& msg = items[n];
    //
    //   }
    // }
  }
}

// bool graphics::gui::SpdlogWindow::compare_with_sort(
//     const logging::LogMsg &lhs, const logging::LogMsg &rhs) {
//   if (current_sort_specs != nullptr)
//     for (int n = 0; n < current_sort_specs->SpecsCount; ++n) {
//       const ImGuiTableColumnSortSpecs *sort_spec =
//           &current_sort_specs->Specs[n];
//       long delta = 0;
//       switch (sort_spec->ColumnUserID) {
//       case TIMESTAMP:
//         delta = (lhs.time - rhs.time);
//         break;
//       case LEVEL:
//         delta = (lhs.level - rhs.level);
//         break;
//       case MSG:
//         delta = (strcmp(lhs.buffer, rhs.buffer));
//         break;
//       default:
//         break;
//       }
//       if (delta > 0)
//         return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ?
//         +1
//                                                                           :
//                                                                           -1;
//       return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1
//                                                                         : +1;
//     }
//
//   return ((lhs.time - rhs.time) > 0) ? +1 : -1;
// }
