#include "simc.hh"
#include <fmt/format.h>
#include <woxel_engine/debug/instrumentor.hh>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

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

    ed::SetCurrentEditor(editor_context_);

    ed::SetNodePosition(spawn_node1()->id, ImVec2(0, 0));
    ed::SetNodePosition(spawn_node2()->id, ImVec2(100, 0));

    build_nodes();
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

    ed::SetCurrentEditor(editor_context_);

    ed::Begin("Node editor");
    {
        for (auto &node : nodes_) {
            ed::BeginNode(node.id);
            for (auto &input : node.inputs) {
                // if(!CanCreateLink) => set alpha
                ed::BeginPin(input.id, input.kind);
                {
                    if (!input.name.empty()) {
                        ImGui::TextUnformatted(input.name.c_str());
                        ImGui::SameLine();
                    }
                    if (input.type > 0) {
                        ImGui::TextUnformatted(fmt::format("Pin type: {}", input.type).c_str());
                        ImGui::SameLine();
                    }
                }
                ed::EndPin();
            }
            ImGui::TextUnformatted(node.name.c_str());
            for (auto &output : node.outputs) {
                // if(!CanCreateLink) => set alpha
                ed::BeginPin(output.id, output.kind);
                if (!output.name.empty()) {
                    ImGui::TextUnformatted(output.name.c_str());
                    ImGui::SameLine();
                }
                ed::EndPin();
            }
            ed::EndNode();
        }

        for (auto &link : links_) {
            ed::Link(link.id, link.start, link.end, link.color, 2.0f);
        }

        if (ed::BeginCreate(ImColor{255, 255, 255}, 2.0f)) {
            auto showLabel = [](const char *label, ImColor color) {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
                auto size = ImGui::CalcTextSize(label);

                auto padding = ImGui::GetStyle().FramePadding;
                auto spacing = ImGui::GetStyle().ItemSpacing;

                ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

                auto rectMin = ImGui::GetCursorScreenPos() - padding;
                auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

                auto drawList = ImGui::GetWindowDrawList();
                drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
                ImGui::TextUnformatted(label);
            };

            ed::PinId startPinId = 0, endPinId = 0;
            if (ed::QueryNewLink(&startPinId, &endPinId)) {
                auto startPin = find_pin(startPinId);
                auto endPin   = find_pin(endPinId);

                new_link_pin_ = startPin ? startPin : endPin;

                if (startPin->kind == PinKind::Input) {
                    std::swap(startPin, endPin);
                    std::swap(startPinId, endPinId);
                }

                if (startPin && endPin) {
                    if (endPin == startPin) {
                        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                    } else if (endPin->kind == startPin->kind) {
                        showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
                        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                    } else if (endPin->node == startPin->node) {
                        showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
                        ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
                    } else if (endPin->type != startPin->type) {
                        showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
                        ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                    } else {
                        showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                        if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f)) {
                            links_.emplace_back(next_link_id_++, startPinId, endPinId);
                            // links_.back().color = GetIconColor(startPin->type);
                        }
                    }
                }
            }

            // Create a new node if link to empty space
            /*
            ed::PinId pinId = 0;
            if (ed::QueryNewNode(&pinId)) {
                new_link_pin_ = find_pin(pinId);
                if (new_link_pin_) showLabel("+ Create Node", ImColor(32, 45, 32, 180));

                if (ed::AcceptNewItem()) {
                    createNewNode  = true;
                    newNodeLinkPin = find_pin(pinId);
                    new_link_pin_  = nullptr;
                    ed::Suspend();
                    ImGui::OpenPopup("Create New Node");
                    ed::Resume();
                }
            }
            */
        } else {
            new_link_pin_ = nullptr;
        }
        ed::EndCreate();

        //    if (ed::BeginCreate()) {
        //        ed::PinId inputPinId, outputPinId;
        //        if (ed::QueryNewLink(&inputPinId, &outputPinId)) {
        //            if (inputPinId && outputPinId) {
        //                if (ed::AcceptNewItem()) {
        //                    links_.push_back({ed::LinkId(next_link_id_++), inputPinId, outputPinId});

        //                    ed::Link(links_.back().id, links_.back().start, links_.back().end);
        //                }
        //            }
        //        }
        //    }
        //    ed::EndCreate();

        //    if (ed::BeginDelete()) {
        //        ed::LinkId deletedLinkId;
        //        while (ed::QueryDeletedLink(&deletedLinkId)) {
        //            if (ed::AcceptDeletedItem()) {
        //                for (auto it = std::begin(links_); it != std::end(links_); it++) {
        //                    auto &link = *it;
        //                    if (link.id == deletedLinkId) {
        //                        links_.erase(it);
        //                        break;
        //                    }
        //                }
        //            }
        //        }
        //    }
        //    ed::EndDelete();

        static ed::NodeId contextNodeId = 0;
        static ed::LinkId contextLinkId = 0;
        static ed::PinId contextPinId   = 0;

        auto openPopupPosition = ImGui::GetMousePos();
        ed::Suspend();
        if (ed::ShowNodeContextMenu(&contextNodeId))
            ImGui::OpenPopup("Node Context Menu");
        else if (ed::ShowPinContextMenu(&contextPinId))
            ImGui::OpenPopup("Pin Context Menu");
        else if (ed::ShowLinkContextMenu(&contextLinkId))
            ImGui::OpenPopup("Link Context Menu");
        /*else if (ed::ShowBackgroundContextMenu()) {
            ImGui::OpenPopup("Create New Node");
            newNodeLinkPin = nullptr;
        }*/
        ed::Resume();
    }
    ed::End();

    if (first_frame_) ed::NavigateToContent(0.0f);
    ed::SetCurrentEditor(nullptr);

    first_frame_ = false;
}

void system::on_render() { ZoneScoped; }

Node *system::spawn_node1() {
    nodes_.emplace_back(next_link_id_++, "node1");
    nodes_.back().inputs.emplace_back(next_link_id_++, "Vector of 1", 1);
    nodes_.back().outputs.emplace_back(next_link_id_++, "Vector of 5", 5);
    nodes_.back().build();

    return &nodes_.back();
}

Node *system::spawn_node2() {
    nodes_.emplace_back(next_link_id_++, "node2");
    nodes_.back().inputs.emplace_back(next_link_id_++, "Vector of 5", 5);
    nodes_.back().outputs.emplace_back(next_link_id_++, "Vector of 1", 1);
    nodes_.back().build();

    return &nodes_.back();
}

void system::build_nodes() {
    for (auto &node : nodes_)
        node.build();
}

Pin *system::find_pin(ed::PinId id) {
    if (!id) return nullptr;

    for (auto &node : nodes_) {
        for (auto &pin : node.inputs)
            if (pin.id == id) return &pin;

        for (auto &pin : node.outputs)
            if (pin.id == id) return &pin;
    }

    return nullptr;
}

bool system::can_create_link(Pin *a, Pin *b) const {
    if (!a || !b || a == b || a->kind == b->kind || a->type != b->type || a->node == b->node) return false;

    return true;
}

} // namespace simc
