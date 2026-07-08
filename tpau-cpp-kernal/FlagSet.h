#ifndef HAD_TPAU_CPP_KERNAL_FLAG_SET_H
#define HAD_TPAU_CPP_KERNAL_FLAG_SET_H

/*
Copyright (C) Dieter Baron

The authors can be contacted at <tpau-cpp-kernal@tpau.group>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. The names of the authors may not be used to endorse or promote
   products derived from this software without specific prior
   written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <concepts>
#include <cstdint>
#include <type_traits>

namespace tpau::cpp_kernal {

/**
 * A set of flags. The possible flags are defined by an enum type.
 *
 * This is meant to be used with enum types that don't explicitly define the values of their members. Do not define the values as powers of two. Values must be between 0 and 63, inclusive.
 */
template <typename Flag>
    requires std::is_enum_v<Flag>
class FlagSet {
  public:
    /**
     * Create a FlagSet with all flags disabled.
     */
    FlagSet() {}


    // Constructor using Fold Expressions (C++17/20)
    // This allows: FlagSet{Flag::OptionOne, Flag::OptionTwo}
    /**
     * Create a FlagSet with the given flags enabled.
     *
     * @param args The flags to enable. Arguments can be of type Flag or other FlagSet<Flag>.
     */
    template <typename... Args>
      requires ((std::same_as<std::remove_cvref_t<Args>, Flag> || std::same_as<std::remove_cvref_t<Args>, FlagSet<Flag>>) && ...)
    constexpr FlagSet(Args... args) { ((enable(args)), ...); }

    /**
     * Create a FlagSet from a raw bitmask.
     *
     * @param flag_mask The bitmask of the flags to enable.
     */
    constexpr explicit FlagSet(uint64_t flag_mask) : flag_mask(flag_mask) {}

    /**
     * Check if a flag is enabled.
     *
     * @param flag The flag to check.
     * @return `true` if the flag is enabled, `false` otherwise.
     */
    [[nodiscard]] bool is_enabled(Flag flag) const { return (flag_mask & get_mask(flag)) != 0; }

    /**
     * Check if any flag from another FlagSet is enabled.
     *
     * @return `true` if any flag is enabled, `false` otherwise.
     */
    [[nodiscard]] bool any_enabled(const FlagSet& other) const { return (flag_mask & other.flag_mask) != 0; }

    /**
     * Check if all flags from another FlagSet are enabled.
     */
    [[nodiscard]] bool all_enabled(const FlagSet& other) const { return (flag_mask & other.flag_mask) == other.flag_mask; }

    /**
     * Check if no flags are enabled.
     *
     * @return `true` if no flags are enabled, `false` otherwise.
     */
    [[nodiscard]] bool empty() const { return flag_mask == 0; }

    /**
     * Interpret as bool: Check if any flags are enabled.
     *
     * @return `true` if any flags are enabled, `false` otherwise.
     */
    operator bool() const { return !empty(); }

    /**
     * Compare two FlagSets for equality.
     *
     * @param other The other FlagSet to compare with.
     * @return `true` if both FlagSets have the same flags enabled, `false` otherwise.
     */
    [[nodiscard]] bool operator==(const FlagSet& other) const { return flag_mask == other.flag_mask; }
 
    /**
     * Check if a flag is enabled.
     *
     * @param flag The flag to check.
     * @return `true` if the flag is enabled, `false` otherwise.
     */
    [[nodiscard]] bool operator[](Flag flag) const { return is_enabled(flag); }

    /**
     * Enable a flag.
     *
     * @param flag The flag to enable.
     */
    void enable(Flag flag) { flag_mask |= get_mask(flag); }

    /**
     * Enable all flags set in another FlagSet.
     */
    void enable(const FlagSet& other) { flag_mask |= other.flag_mask; }

    /**
     * Disable a flag.
     *
     * @param flag The flag to disable.
     */
    void disable(Flag flag) { flag_mask &= ~get_mask(flag); }

    /**
     * Disable all flags set in another FlagSet.
     *
     * @param other The FlagSet containing the flags to disable.
     */
    void disable(const FlagSet& other) { flag_mask &= ~other.flag_mask; }

    /**
     * Get the bitmask for a flag.
     *
     * @param flag The flag to get the bitmask for.
     * @return The bitmask for the flag.
     */
    static constexpr uint64_t get_mask(Flag flag) { return static_cast<uint64_t>(1) << static_cast<uint8_t>(flag); }

  private:
    uint64_t flag_mask{0};
};

} // namespace tpau::cpp_kernal

#endif // HAD_TPAU_CPP_KERNAL_FLAG_SET_H
