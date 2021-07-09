#include "simc.hh"
#include <fmt/format.h>
#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace simc {

void ImGuiEx_BeginColumn() { ImGui::BeginGroup(); }

void ImGuiEx_NextColumn() {
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
}

void ImGuiEx_EndColumn() { ImGui::EndGroup(); }

void system::on_attach() {
    ZoneScoped;
    ImNodes::CreateContext();
    ImNodes::StyleColorsDark();
    ImNodes::SetNodeGridSpacePos(1, {200.f, 200.f});

    editor_context_ = ImNodes::EditorContextCreate();

    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
    ImNodesIO &io                           = ImNodes::GetIO();
    io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
}

void system::on_detach() {
    ZoneScoped;
    ImNodes::PopAttributeFlag();
    if (editor_context_) ImNodes::EditorContextFree(editor_context_);
}

void system::on_update(const woxel::stopwatch &s) { ZoneScoped; }

void system::on_imgui_render() {
    ZoneScoped;

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImNodes::EditorContextSet(editor_context_);
    ImGui::Begin("Node editor");

    auto &io = ImGui::GetIO();
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImNodes::BeginNodeEditor();
    {
        constexpr auto get_keys = []() -> std::size_t {
            std::size_t i = 0;
            if (ImGui::IsKeyReleased(GLFW_KEY_1)) { i += 1; }
            if (ImGui::IsKeyReleased(GLFW_KEY_2)) { i += 2; }
            if (ImGui::IsKeyReleased(GLFW_KEY_3)) { i += 3; }
            if (ImGui::IsKeyReleased(GLFW_KEY_4)) { i += 4; }
            if (ImGui::IsKeyReleased(GLFW_KEY_5)) { i += 5; }
            if (ImGui::IsKeyReleased(GLFW_KEY_6)) { i += 6; }
            if (ImGui::IsKeyReleased(GLFW_KEY_7)) { i += 7; }
            if (ImGui::IsKeyReleased(GLFW_KEY_8)) { i += 8; }
            if (ImGui::IsKeyReleased(GLFW_KEY_9)) { i += 9; }
            return i;
        };
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImNodes::IsEditorHovered() &&
            get_keys() > 0) {
            const auto size   = get_keys();
            const int node_id = next_link_id_;
            next_link_id_ += 3; // 3 = node + input + output
            ImNodes::SetNodeScreenSpacePos(node_id, ImGui::GetMousePos());
            nodes_.emplace_back(node_id, size, size + (ImGui::IsKeyDown(GLFW_KEY_SPACE) ? -1 : 1));
        }

        for (auto &node : nodes_) {
            ImNodes::BeginNode(node.id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted("node");
            ImNodes::EndNodeTitleBar();

            if (node.inputs > 0) {
                ImNodes::BeginInputAttribute(node.id + 1);
                ImGui::TextUnformatted("input");
                ImNodes::EndInputAttribute();
            }

            ImNodes::BeginStaticAttribute(node.id << 16);
            ImGui::Text("%s", fmt::format("{}/{}", node.inputs, node.outputs).c_str());
            ImNodes::EndStaticAttribute();

            if (node.outputs > 0) {
                ImNodes::BeginOutputAttribute(node.id + 2);
                const float text_width = ImGui::CalcTextSize("output").x;
                ImGui::Indent(120.f + ImGui::CalcTextSize("value").x - text_width);
                ImGui::TextUnformatted("output");
                ImNodes::EndOutputAttribute();
            }

            ImNodes::EndNode();
        }

        for (const auto &link : links_) {
            ImNodes::Link(link.id, link.start, link.end);
        }
    }
    ImNodes::EndNodeEditor();

    {
        Link link;
        if (ImNodes::IsLinkCreated(&link.start, &link.end)) {
            auto start_node =
                std::find_if(nodes_.begin(), nodes_.end(),
                             [output_id = link.start](const Node &node) -> bool { return node.id == (output_id - 2); });
            auto end_node = std::find_if(nodes_.begin(), nodes_.end(), [input_id = link.end](const Node &node) -> bool {
                return node.id == (input_id - 1);
            });
            assert(start_node != nodes_.end());
            assert(end_node != nodes_.end());

            if (start_node->outputs == end_node->inputs) {
                link.id = next_link_id_++;
                links_.push_back(link);
            }
        }
    }

    {
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id)) {
            auto iter = std::find_if(links_.begin(), links_.end(),
                                     [link_id](const auto &link) -> bool { return link.id == link_id; });
            assert(iter != links_.end());
            links_.erase(iter);
        }
    }

    ImGui::End();
}

void system::on_render() { ZoneScoped; }

} // namespace simc
