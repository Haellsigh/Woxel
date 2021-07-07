#pragma once

#include <imgui_node_editor.h>
#include <vector>
#include <woxel_engine/scene/system.hh>

struct LinkInfo {
    ax::NodeEditor::LinkId Id;
    ax::NodeEditor::PinId InputId;
    ax::NodeEditor::PinId OutputId;
};

class simc : public woxel::system {
  public:
    void on_attach() final;
    void on_detach() final;

    void on_update(woxel::stopwatch const &s) final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    ax::NodeEditor::EditorContext *editor_context_ = nullptr;
    std::vector<LinkInfo> links_;
    bool first_frame_ = true;
    int next_link_id_ = 100;
};
