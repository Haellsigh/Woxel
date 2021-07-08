#pragma once

#include <imgui_node_editor.h>
#include <vector>
#include <woxel_engine/scene/system.hh>

namespace simc {

namespace ed = ax::NodeEditor;

using PinType = std::size_t;

enum class PinKind
{
    Input,
    Output
};

struct Node;
struct Pin;
struct Link;

struct Pin {
    ed::PinId id;
    Node *node;
    std::string name;
    PinKind kind;

    Pin(Node *parent) : node(parent) {}
};

struct Node {
    ed::NodeId id;
    std::string name;
    std::vector<Pin> inputs;
    std::vector<Pin> outputs;
    ImVec2 size;

    ImColor color{255, 255, 255};

    Node(ed::NodeId id, std::string_view name) : id(id), name(name) {}
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

  private:
    ax::NodeEditor::EditorContext *editor_context_ = nullptr;
    std::vector<Node> nodes_;
    std::vector<Link> links_;
    bool first_frame_ = true;
    int next_link_id_ = 100;
};

} // namespace simc
