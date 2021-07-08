#include "simc.hh"

#include <imgui_internal.h>
#include <woxel_engine/debug/instrumentor.hh>

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
    editor_context_ = ed::CreateEditor();
}

void system::on_detach() {
    ZoneScoped;
    if (editor_context_) ed::DestroyEditor(editor_context_);
}

void system::on_update(const woxel::stopwatch &s) { ZoneScoped; }

void system::on_imgui_render() {
    ZoneScoped;

    auto &io = ImGui::GetIO();
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    int uniqueId = 1;

    ed::SetCurrentEditor(editor_context_);
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));

    ed::NodeId nodeA_Id         = uniqueId++;
    ed::PinId nodeA_InputPinId  = uniqueId++;
    ed::PinId nodeA_OutputPinId = uniqueId++;

    if (first_frame_) ed::SetNodePosition(nodeA_Id, ImVec2(10, 10));
    ed::BeginNode(nodeA_Id);
    ImGui::Text("Node A");
    ed::BeginPin(nodeA_InputPinId, ed::PinKind::Input);
    ImGui::Text("-> In");
    ed::EndPin();
    ImGui::SameLine();
    ed::BeginPin(nodeA_OutputPinId, ed::PinKind::Output);
    ImGui::Text("Out ->");
    ed::EndPin();
    ed::EndNode();

    ed::NodeId nodeB_Id         = uniqueId++;
    ed::PinId nodeB_InputPinId1 = uniqueId++;
    ed::PinId nodeB_InputPinId2 = uniqueId++;
    ed::PinId nodeB_OutputPinId = uniqueId++;

    if (first_frame_) ed::SetNodePosition(nodeB_Id, ImVec2(210, 60));
    ed::BeginNode(nodeB_Id);
    ImGui::Text("Node B");
    ImGuiEx_BeginColumn();
    ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
    ImGui::Text("-> In1");
    ed::EndPin();
    ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
    ImGui::Text("-> In2");
    ed::EndPin();
    ImGuiEx_NextColumn();
    ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
    ImGui::Text("Out ->");
    ed::EndPin();
    ImGuiEx_EndColumn();
    ed::EndNode();

    for (auto &linkInfo : links_)
        ed::Link(linkInfo.id, linkInfo.start, linkInfo.end, linkInfo.color);

    if (ed::BeginCreate()) {
        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId)) {
            if (inputPinId && outputPinId) {
                if (ed::AcceptNewItem()) {
                    links_.push_back({ed::LinkId(next_link_id_++), inputPinId, outputPinId});

                    ed::Link(links_.back().id, links_.back().start, links_.back().end);
                }
            }
        }
    }
    ed::EndCreate();

    if (ed::BeginDelete()) {
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId)) {
            if (ed::AcceptDeletedItem()) {
                for (auto it = std::begin(links_); it != std::end(links_); it++) {
                    auto &link = *it;
                    if (link.id == deletedLinkId) {
                        links_.erase(it);
                        break;
                    }
                }
            }
        }
    }
    ed::EndDelete();

    ed::End();

    if (first_frame_) ed::NavigateToContent(0.0f);
    ed::SetCurrentEditor(nullptr);

    first_frame_ = false;
}

void system::on_render() { ZoneScoped; }

Node *system::spawn_node1() { nodes_.emplace_back(next_link_id_++, "idk"); }

Node *system::spawn_node2() {}

} // namespace simc
