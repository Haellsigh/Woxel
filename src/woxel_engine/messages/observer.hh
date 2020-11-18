#pragma once

#include "woxel_engine/debug/instrumentor.hh"

#include <ctti/type_id.hpp>
#include <entt/signal/delegate.hpp>

#include <any>
#include <vector>

namespace woxel {

template <class> struct types { using type = types; };

template <class Sig> struct args;
template <class R, class message_type> struct args<R(message_type)> : types<message_type> {};
template <class Sig> using args_t = typename args<Sig>::type;

/**
 * Problems of this solution:
 *  - A layer cannot send an event to the other layers.
 *    This can be solved easily but it's probably not the best solution.
 *  - Maybe we need a more global event system with one instance per application, and
 *    application-global events.
 */
class observer {
    template <typename message_type> using delegate_type = entt::delegate<bool(message_type const &)>;
    // template <typename message_type> using function_type = typename delegate_type<message_type>::function_type;
    template <typename message_type> using function_type = bool (*)(message_type const &);

  public:
    /**
     * \brief Subscribes to message_type messages using a free function.
     */
    /*template <typename message_type, auto candidate> void subscribe(delegate_type<message_type> instance) {
        ZoneScoped;
        get_create_delegate<message_type>()->template connect<candidate>(instance);
    }*/

    /**
     * \brief Subscribes to message_type messages using a free function.
     */
    template <typename message_type, bool (*candidate)(message_type const &)> void subscribe() {
        ZoneScoped;
        get_create_delegate<message_type>()->template connect<candidate>();
    }

    /**
     * \brief Subscribe to message_type messages using a free function with payload or a bound member to a delegate.
     */
    template <typename message_type, auto candidate, typename type> void subscribe(type *value_or_instance) {
        ZoneScoped;
        get_create_delegate<message_type>()->template connect<candidate>(value_or_instance);
    }

    /**
     * \brief Send the message to all the subscribed observers.
     *
     * \returns true if the message was handled.
     * \returns false if the message wasn't handled.
     */
    template <typename message_type> bool publish(message_type const &message, bool pass_through = false) {
        ZoneScoped;
        auto *delegate = get_delegate<message_type>();

        if (!delegate) return false;

        const auto &ret = (*delegate)(message);
        return pass_through ? false : ret;
    }

    /*
    template <typename message_type, typename... Args> bool publish(Args &&...args, bool pass_through = false) {
        ZoneScoped;
        auto *delegate = get_delegate<message_type>();

        if (!delegate) return false;

        const auto &ret = (*delegate)(message_type{std::forward<Args>(args)...});
        return pass_through ? false : ret;
    }
    */

  private:
    /**
     * \brief Gets the existing delegate corresponding to a type of message, or nothing if the message_type isn't
     * handled.
     */
    template <typename message_type> entt::delegate<bool(message_type const &)> *get_delegate() {
        constexpr auto id = ctti::unnamed_type_id<message_type>().hash();

        auto it = std::lower_bound(delegates_types_ids_.begin(), delegates_types_ids_.end(), id);
        if (it != delegates_types_ids_.end() && *it == id) {
            return std::any_cast<delegate_type<message_type>>(
                &delegates_[std::distance(delegates_types_ids_.begin(), it)]);
        } else {
            return nullptr;
        }
    }

    /**
     * \brief Gets or create the delegate corresponding to a type of message.
     */
    template <typename message_type> entt::delegate<bool(message_type const &)> *get_create_delegate() {
        constexpr auto id = ctti::unnamed_type_id<message_type>().hash();

        auto it = std::lower_bound(delegates_types_ids_.begin(), delegates_types_ids_.end(), id);
        // found a delegate
        if (it != delegates_types_ids_.end() && *it == id) {
            return std::any_cast<delegate_type<message_type>>(
                &delegates_[std::distance(delegates_types_ids_.begin(), it)]);
        }
        // create a delegate
        else {
            auto index =
                std::distance(delegates_types_ids_.begin(),
                              delegates_types_ids_.insert(
                                  std::upper_bound(delegates_types_ids_.begin(), delegates_types_ids_.end(), id), id));
            return std::any_cast<delegate_type<message_type>>(
                &*delegates_.emplace(std::next(delegates_.begin(), index), delegate_type<message_type>{}));
        }
    }

  private:
    std::vector<std::size_t> delegates_types_ids_;
    std::vector<std::any> delegates_;
};

} // namespace woxel
