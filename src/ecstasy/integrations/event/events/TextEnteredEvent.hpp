///
/// @file TextEnteredEvent.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_INTEGRATIONS_EVENT_EVENTS_TEXTENTEREDEVENT_HPP_
#define ECSTASY_INTEGRATIONS_EVENT_EVENTS_TEXTENTEREDEVENT_HPP_

#include <cstdint>

namespace ecstasy::integration::event
{
    ///
    /// @brief Event describing a text (character) entered.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-17)
    ///
    struct TextEnteredEvent {
        /// @brief Unicode of character entered.
        std::uint32_t unicode;

        ///
        /// @brief Construct a new TextEnteredEvent object.
        ///
        /// @param[in] pUnicode Unicode character entered.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-17)
        ///
        constexpr TextEnteredEvent(std::uint32_t pUnicode) noexcept : unicode(pUnicode){};
    };
} // namespace ecstasy::integration::event

#endif /* !ECSTASY_INTEGRATIONS_EVENT_EVENTS_TEXTENTEREDEVENT_HPP_ */
