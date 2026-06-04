# Symbols — Advantages and Limitations

## What Is a Symbol?

In compiler and programming language implementations, a *symbol* refers to a representation of a string that is cheap to compare and copy. This is usually achieved by representing each symbol as a unique integer or pointer. Conversion from string to symbol is done using a symbol table that stores the mapping between strings and symbols.

## Symbols in the T'Pau C++ Kernal

The [Symbol](../Reference/Symbol) type implements this concept. It contains a raw pointer to a `std::string` stored in the symbol table. The global symbol table will be automatically allocated on first use, which allows the `Symbol` constructor to be used in global initializers, even though C++ does not guarantee the order of initialization of global variables.

## Lifetime and Deallocation of Symbols

The drawback of using a raw pointer is that the C++ compiler can't keep track of where a symbol is stored, which means a symbol cannot be safely deallocated, therefore deallocation is not implemented: There is no way to remove a symbol from the symbol table, or to deallocate the global symbol table, which exists for the entire duration of the program.

This is fine for build tools that do their work and then exit. It would be a problem for long running programs, if they continually create new symbols.

## Possible Solutions

### Unsafe but Efficient

To allow deallocation of symbols, we could use multiple symbol tables, possibly linked together: A global symbol table for symbols that are used throughout the program, and a local symbol per task, that would be deallocated at the end of the task. If a symbol is not found in the local table, it would be looked up in the global table before creating a new symbol.

However, since Symbols are represented as raw pointers, the compiler can't guarantee that local symbols are not used after their table has been deallocated, thus violating C++'s RAII principle. 

### Safe but Less Efficient

Another solution would be to use smart pointers as the underlying representation of symbols and use weak pointers in the symbol table, thus automatically deallocating symbols when they are no longer used.

However, this would make symbols use more memory and be less efficient to copy. Maybe we could use a hybrid approach, where global symbols are represented as raw pointers, and local symbols are represented as smart pointers. This would allow us to have the best of both worlds, but it would also make the implementation more complex.

### Conclusion

Neither of these solutions is currently implemented, as it is not needed for our use cases, but they are worth considering if the need arises.
