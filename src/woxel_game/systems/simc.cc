#include "simc.hh"
#include <fmt/format.h>
#include <numeric>
#include <taskflow/taskflow.hpp>
#include <unordered_map>
#include <woxel_engine/core/log.hh>
#include <woxel_engine/debug/instrumentor.hh>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace simc {

inline int next_id = 0;
int get_next_id() { return next_id++; }

void Node::add_output(size_t size) { outputs.emplace_back(get_next_id(), size); }

void Node::add_input(size_t size) { inputs.emplace_back(get_next_id(), size); }

Node create_source_node() {
    Node n{get_next_id()};
    n.add_output(1);
    n.function = [](double time, const std::vector<Signal> &inputs) -> std::vector<Signal> {
        // woxel::log::trace("Generating {}", std::sin(10. * time));
        return {{std::sin(10. * time)}};
    };

    return n;
}

Node create_sink_node() {
    Node n{get_next_id()};
    n.add_input(1);
    n.function = [](double time, const std::vector<Signal> &inputs) -> std::vector<Signal> {
        // Log current value
        // woxel::log::trace("Sinking {}", inputs.front().front());
        return {};
    };

    return n;
}

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
    ImGui::BeginGroup();
    if (ImGui::Button("Traverse")) { simulate(); }
    ImGui::PushItemWidth(150);
    ImGui::InputDouble("Timestep", &timestep_, 0.001, 0., "%.3f");
    ImGui::SameLine();
    ImGui::InputDouble("Final time", &final_time_, 1., 0., "%.3f");
    ImGui::PopItemWidth();
    ImGui::EndGroup();

    auto &io = ImGui::GetIO();
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImNodes::BeginNodeEditor();
    {
        enum class NodeType
        {
            None,
            Source,
            Sink
        };

        constexpr auto get_keys = []() -> NodeType {
            if (ImGui::IsKeyReleased(GLFW_KEY_1)) { return NodeType::Source; }
            if (ImGui::IsKeyReleased(GLFW_KEY_2)) { return NodeType::Sink; }
            return NodeType::None;
        };
        const auto keys = get_keys();
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImNodes::IsEditorHovered() &&
            keys != NodeType::None) {
            if (keys == NodeType::Source) {
                nodes_.push_back(create_source_node());
            } else if (keys == NodeType::Sink) {
                nodes_.push_back(create_sink_node());
            }
            ImNodes::SetNodeScreenSpacePos(nodes_.back().id, ImGui::GetMousePos());
        }

        for (auto &node : nodes_) {
            ImNodes::BeginNode(node.id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(fmt::format("Node {}", node.id).c_str());
            ImNodes::EndNodeTitleBar();

            ImGui::BeginGroup();
            for (auto &input : node.inputs) {
                ImNodes::BeginInputAttribute(input.id);
                ImGui::TextUnformatted(fmt::format("Vector{}: {}", input.size, input.id).c_str());
                ImNodes::EndInputAttribute();
            }
            ImGui::EndGroup();

            /*
            ImNodes::BeginStaticAttribute(node.id << 16);
            ImGui::Text("%s", fmt::format("{}/{}", node.inputs.size(), node.outputs.size()).c_str());
            ImNodes::EndStaticAttribute();
            */

            ImGui::SameLine();
            ImGui::BeginGroup();
            for (auto &output : node.outputs) {
                ImNodes::BeginOutputAttribute(output.id);
                ImGui::TextUnformatted(fmt::format("Vector{}: {}", output.size, output.id).c_str());
                /*const float text_width = ImGui::CalcTextSize("output").x;
                ImGui::Indent(120.f + ImGui::CalcTextSize("value").x - text_width);
                ImGui::TextUnformatted("output");*/
                ImNodes::EndOutputAttribute();
            }
            ImGui::EndGroup();

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
            std::size_t output_size = 0, input_size = 0;
            for (const auto &node : nodes_) {
                const auto start_pin = std::find_if(node.outputs.begin(), node.outputs.end(),
                                                    [start = link.start](const Pin &p) { return p.id == start; });
                if (start_pin != node.outputs.end()) { output_size = start_pin->size; }
                const auto end_pin = std::find_if(node.inputs.begin(), node.inputs.end(),
                                                  [end = link.end](const Pin &p) { return p.id == end; });
                if (end_pin != node.inputs.end()) { input_size = end_pin->size; }
            }
            // Verify the link.end doesn't already appear in links_
            auto link_it = std::find_if(links_.begin(), links_.end(),
                                        [end = link.end](const Link &link) { return link.end == end; });

            if (link_it == links_.end() && output_size == input_size && input_size != 0 && output_size != 0) {
                link.id = get_next_id();
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

void system::simulate() {
    ZoneScopedN("simulate");
    // Compile the current graph
    tf::Taskflow taskflow;
    tf::Executor executor(1);

    std::unordered_map<int, tf::Task> tasks;
    std::unordered_map<int, std::vector<Signal>> data;
    std::unordered_map<int, int> input_data_node_id;

    double time = 0.;

    // Create tasks with no work
    for (auto &node : nodes_) {
        tasks[node.id] = taskflow.placeholder();
    }

    // Create dependencies between tasks depending on node links
    for (auto &link : links_) {
        ZoneScopedN("for each link");
        // Find the start and end node index in nodes_ from each link's pin ids
        auto nodeit    = nodes_.begin();
        int node_start = -1, node_end = -1;
        {
            ZoneScopedN("find start/end node");
            while (nodeit != nodes_.end() && (node_start == -1 || node_end == -1)) {
                auto &node = *nodeit;
                // Find start node (match output pin)
                {
                    auto it_start = std::find_if(node.outputs.begin(), node.outputs.end(),
                                                 [start = link.start](const Pin &p) { return start == p.id; });
                    if (it_start != node.outputs.end()) { node_start = node.id; }
                }
                // Find end node (match input pin)
                {
                    auto it_end = std::find_if(node.inputs.begin(), node.inputs.end(),
                                               [end = link.end](const Pin &p) { return end == p.id; });
                    if (it_end != node.inputs.end()) { node_end = node.id; }
                }
                nodeit++;
            }
        }
        {
            ZoneScopedN("set tasks and input data");
            assert(node_start != -1 && node_end != -1); // assert we found both nodes
            input_data_node_id[node_end] = node_start;
            tasks[node_start].precede(tasks[node_end]);
        }
    }

    // Add the work (traversal task) for each task/node
    for (auto &node : nodes_) {
        ZoneScopedN("add work");
        tasks[node.id]
            .work([&] {
                ZoneScopedN("execute task");
                // woxel::log::trace("Executing node {} at t={}", node.id, time);
                node.traversed = true;
                if (input_data_node_id[node.id] == -1) {
                    data[node.id] = node.function(time, {});
                } else {
                    data[node.id] = node.function(time, data[input_data_node_id[node.id]]);
                }
            })
            .name(fmt::format("Node {}", node.id));
    }

    woxel::log::trace(taskflow.dump());

    // Traverse graph
    const auto t1 = std::chrono::high_resolution_clock::now();
    /*while (time < final_time_) {
        ZoneScopedN("execute 1");
        executor.run(taskflow).wait();
        time += timestep_;
    }*/
    executor.run_n(taskflow, final_time_ / timestep_).wait();
    const auto t2 = std::chrono::high_resolution_clock::now();
    woxel::log::trace("time {} ns per iteration", (t2 - t1).count() / (final_time_ / timestep_));
}

} // namespace simc
