#pragma once

#include "woxel_engine/core/log.hh"
#include "woxel_engine/debug/instrumentor.hh"

#include <ctti/type_id.hpp>
#include <entt/signal/delegate.hpp>

#include <any>
#include <vector>

namespace woxel {

/**
 * Problems of this solution:
 *  - A layer cannot send an event to the other layers.
 *    This can be solved easily but it's probably not the best solution.
 *  - Maybe we need a more global event system with one instance per application, and
 *    application-global events.
 */

class message_handler {
  public:
    /**
     * \brief Listens to message_type messages using candidate.
     */
    template <typename message_type, auto candidate, typename candidate_type> void listen(candidate_type *instance) {
        ZoneScoped;
        get_create_delegate<message_type>()->template connect<candidate>(instance);
    }

    /**
     * \brief Send the message to the registered handlers.
     */
    template <typename message_type> bool send(const message_type &message) {
        ZoneScoped;
        auto *delegate = get_delegate<message_type>();
        return delegate ? (*delegate)(message) : false;
    }

  private:
    /**
     * \brief Gets the existing delegate corresponding to a type of message, or nothing if the message_type isn't
     * handled.
     */
    template <typename message_type> entt::delegate<bool(message_type)> *get_delegate() {
        using delegate_type = entt::delegate<bool(message_type)>;

        constexpr auto id = ctti::unnamed_type_id<message_type>().hash();

        auto it = std::lower_bound(delegates_types_ids_.begin(), delegates_types_ids_.end(), id);
        if (it != delegates_types_ids_.end() && *it == id) {
            return std::any_cast<delegate_type>(&delegates_[std::distance(delegates_types_ids_.begin(), it)]);
        } else {
            return nullptr;
        }
    }

    /**
     * \brief Gets or create the delegate corresponding to a type of message.
     */
    template <typename message_type> entt::delegate<bool(message_type)> *get_create_delegate() {
        using delegate_type = entt::delegate<bool(message_type)>;

        constexpr auto id = ctti::unnamed_type_id<message_type>().hash();

        auto it = std::lower_bound(delegates_types_ids_.begin(), delegates_types_ids_.end(), id);
        // found a delegate
        if (it != delegates_types_ids_.end() && *it == id) {
            return std::any_cast<delegate_type>(&delegates_[std::distance(delegates_types_ids_.begin(), it)]);
        }
        // create a delegate
        else {
            auto index =
                std::distance(delegates_types_ids_.begin(),
                              delegates_types_ids_.insert(
                                  std::upper_bound(delegates_types_ids_.begin(), delegates_types_ids_.end(), id), id));
            return std::any_cast<delegate_type>(
                &*delegates_.emplace(std::next(delegates_.begin(), index), delegate_type{}));
        }
    }

  private:
    std::vector<std::size_t> delegates_types_ids_;
    std::vector<std::any> delegates_;
};

} // namespace woxel
