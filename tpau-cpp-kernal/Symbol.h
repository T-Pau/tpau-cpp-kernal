#ifndef HAD_TPAU_CPP_KERNAL_SYMBOL_H
#define HAD_TPAU_CPP_KERNAL_SYMBOL_H

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

#include <format>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

namespace tpau::cpp_kernal {

class SymbolTable;

/**
 * A Symbol represents a string in a way that allows copying and comparison in constant time.
 *
 * Even though it uses the global symbol table and C++ does not guarantee the order of initialization of global variables, it is safe to use the `Symbol` constructor in global initializers.
 */
class Symbol {
  public:
    /// @brief Create an empty symbol.
    Symbol() = default;

    /**
     * Create a symbol from a string.
     *
     * @param name The string to create the symbol from.
     * @return The created symbol.
     */
    Symbol(const char* name);

    /**
     * Create a symbol from a string.
     *
     * @param name The string to create the symbol from.
     * @return The created symbol.
     */
    Symbol(const std::string& name);

    /**
     * Create a symbol from a string view.
     *
     * @param name The string to create the symbol from.
     * @return The created symbol.
     */
    Symbol(std::string_view name);

    /**
     * Assign the symbol corresponding to a string.
     *
     * @param name The string to create the symbol from.
     * @return The created symbol.
     */
    Symbol& operator=(const char* name);

    /**
     * Assign the symbol corresponding to a string.
     *
     * @param name The string to create the symbol from.
     * @return The created symbol.
     */
    Symbol& operator=(const std::string& name);

    /**
     * Assign the symbol corresponding to a string view.
     *
     * @param name The string to create the symbol from.
     * @return The created symbol.
     */
    Symbol& operator=(std::string_view name);

    /**
     * Get the string represented by the symbol.
     *
     * @return The string represented by the symbol.
     */
    [[nodiscard]] const std::string& str() const { return *id; }

    /**
     * Check if the symbol is empty, i. e. it represents the empty string.
     *
     * @return `true` if the symbol is empty, `false` otherwise.
     */
    [[nodiscard]] bool empty() const { return id == &empty_string; }

    /**
     * Check if two symbols are equal.
     *
     * @param other The symbol to compare with.
     * @return `true` if the symbols are equal, `false` otherwise.
     */
    bool operator==(const Symbol& other) const { return id == other.id; }

    /**
     * Check if two symbols are not equal.
     *
     * @param other The symbol to compare with.
     * @return `true` if the symbols are not equal, `false` otherwise.
     */
    bool operator!=(const Symbol& other) const { return id != other.id; }

    /**
     * Check if this symbol comes before another symbol in lexicographical order.
     *
     * @param other The symbol to compare with.
     * @return `true` if this symbol comes before the other symbol, `false` otherwise.
     */
    bool operator<(const Symbol& other) const { return *id < *other.id; }

    /**
     * Check if this symbol comes before or is equal to another symbol in lexicographical order.
     *
     * @param other The symbol to compare with.
     * @return `true` if this symbol comes before or is equal to the other symbol, `false` otherwise.
     */
    bool operator<=(const Symbol& other) const { return *id <= *other.id; }

    /**
     * Check if this symbol comes after another symbol in lexicographical order.
     *
     * @param other The symbol to compare with.
     * @return `true` if this symbol comes after the other symbol, `false` otherwise.
     */
    bool operator>(const Symbol& other) const { return *id > *other.id; }

    /**
     * Check if this symbol comes after or is equal to another symbol in lexicographical order.
     *
     * @param other The symbol to compare with.
     * @return `true` if this symbol comes after or is equal to the other symbol, `false` otherwise.
     */
    bool operator>=(const Symbol& other) const { return *id >= *other.id; }

    /**
     * Check if the symbol is valid (not empty).
     *
     * @return `true` if the symbol is valid, `false` otherwise.
     */
    operator bool() const { return !empty(); } // NOLINT(*-explicit-constructor)

  private:
    /// @brief A table of all symbols, used to ensure that each symbol is only stored once.
    struct Table {
      public:
        const std::string* intern(std::string_view string);

      private:
        /// @brief This table maps raw pointers of interned strings to unique pointers of the same strings.
        std::unordered_map<std::string_view, std::unique_ptr<std::string>> symbols;
    };

    /// @brief Pointer to the interned representation by the symbol.
    const std::string* id{&empty_string};

    /// @brief The canonical representation of the empty string, used for empty symbols.
    static const std::string empty_string;

    /// @brief Initialize the global symbol table.
    static void init_global();

    /**
     * The global symbol table.
     *
     * It is a pointer to an object so we can force initialization before we use it in constructing symbols.
     */
    static Table* global;
};


/**
 * Output a symbol to a stream.
 *
 * @param stream The stream to output to.
 * @param symbol The symbol to output.
 * @return The stream that was output to.
 */
std::ostream& operator<<(std::ostream& stream, const Symbol& symbol);

} // namespace tpau::cpp_kernal

template <> struct std::hash<tpau::cpp_kernal::Symbol> {
    std::size_t operator()(tpau::cpp_kernal::Symbol const& symbol) const noexcept { return std::hash<std::string_view>{}(symbol.str()); }
};

template <> struct std::formatter<tpau::cpp_kernal::Symbol> : std::formatter<std::string_view> {
    auto format(const tpau::cpp_kernal::Symbol& symbol, format_context& ctx) const {
        // We delegate the actual rendering to the base class.
        // It will use the options parsed by the inherited parse() method.
        return std::formatter<std::string_view>::format(symbol.str(), ctx);
    }
};

#endif // HAD_TPAU_CPP_KERNAL_SYMBOL_H
