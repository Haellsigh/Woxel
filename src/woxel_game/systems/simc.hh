#pragma once

#include <imnodes/imnodes.h>
#include <string>
#include <string_view>
#include <vector>
#include <woxel_engine/scene/system.hh>

namespace simc {

// Signal dimension is the size of the vector
using Signal = std::vector<double>;

struct Pin {
    int id;
    std::size_t size;

    constexpr Pin(int id, std::size_t size) : id(id), size(size) {}
};

struct Node {
    int id;
    std::vector<Pin> inputs, outputs;
    bool traversed = false;

    std::function<std::vector<Signal>(double, const std::vector<Signal> &)> function;

    Node(int id) : id(id) {}

    void add_output(std::size_t dimension);
    void add_input(std::size_t dimension);
};

struct Link {
    int id;
    int start, end;
};

/*
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

    Link(ed::LinkId id, ed::PinId start, ed::PinId end) : id(id), start(start), end(end) {}
};
*/

class system : public woxel::system {
  public:
    void on_attach() final;
    void on_detach() final;

    void on_update(woxel::stopwatch const &s) final;
    void on_imgui_render() final;
    void on_render() final;

  public:
    void simulate();

  private:
    ImNodesEditorContext *editor_context_ = nullptr;
    std::vector<Node> nodes_;
    std::vector<Link> links_;

    double timestep_ = 0.1, final_time_ = 1.;
    /*
  private:
    Node *spawn_node1();
    Node *spawn_node2();
    void build_nodes();
    Pin *find_pin(ed::PinId id);

    bool can_create_link(Pin *a, Pin *b) const;

  private:
    ax::NodeEditor::EditorContext *editor_context_ = nullptr;
    bool first_frame_  = true;
    int next_link_id_  = 100;
    Pin *new_link_pin_ = nullptr;
    */
};

} // namespace simc
