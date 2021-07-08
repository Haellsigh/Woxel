#pragma once

#include <imgui_node_editor.h>
#include <vector>
#include <woxel_engine/scene/system.hh>

namespace simc {

namespace ed = ax::NodeEditor;

using PinType = std::size_t;

using PinKind = ed::PinKind;

struct Node;
struct Pin;
struct Link;

struct Pin {
    ed::PinId id;
    Node *node = nullptr;
    std::string name;
    PinType type = 0;
    PinKind kind = PinKind::Input;

    Pin(ed::PinId id, std::string_view name, PinType type) : id(id), name(name), type(type) {}
};

struct Node {
    ed::NodeId id;
    std::string name;
    std::vector<Pin> inputs;
    std::vector<Pin> outputs;
    ImVec2 size;

    ImColor color{255, 255, 255};

    Node(ed::NodeId id, std::string_view name, ImColor color = ImColor{255, 255, 255}) : id(id), name(name) {}

    void build() {
        for (auto &pin : inputs) {
            pin.node = this;
            pin.kind = PinKind::Input;
        }
        for (auto &pin : outputs) {
            pin.node = this;
            pin.kind = PinKind::Output;
        }
    }
};

struct Link {
    ed::LinkId id;
    ed::PinId start, end;

    ImColor color{255, 255, 255};
};

class system : public woxel::system {
  public:
    void on_attach() final;
    void on_detach() final;

    void on_update(woxel::stopwatch const &s) final;
    void on_imgui_render() final;
    void on_render() final;

  private:
    Node *spawn_node1();
    Node *spawn_node2();
    void build_nodes();
    Pin *find_pin(ed::PinId id);

    bool can_create_link(Pin *a, Pin *b) const;

  private:
    ax::NodeEditor::EditorContext *editor_context_ = nullptr;
    std::vector<Node> nodes_;
    std::vector<Link> links_;
    bool first_frame_  = true;
    int next_link_id_  = 100;
    Pin *new_link_pin_ = nullptr;
};

} // namespace simc
