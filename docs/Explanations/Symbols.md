# Symbols — Advantages and Limitations

Especially when writing parsers, it is often useful to have a representation of strings that is cheap to compare and copy, can be used as keys in hash tables, and can be used in global initializers.

 The [`Symbol`](../Reference/Symbol) type is designed for this purpose, It contains a raw pointer. When the same string is converted into a symbol, it will always produce the same pointer. The corresponding string can be retrieved from the symbol cheaply. The drawback is that the compiler can't keep track of who has a reference to a symbol, so symbols can not be removed from the symbol table. 

Currently, there is one global symbol table, so all symbols are stored for the entire duration of the program. This is fine for build tools that do their work and then exit. It could be a problem for long running programs, especially if they create symbols based on user input.

One solution to this problem would be to have multiple symbol tables, possibly chained together: A global symbol table for symbols that are used throughout the program, and a local symbol per task, that would be deallocated at the end of the task. However, this would violate C++'s RAII principle, as it is the program's responsibility to make sure that local symbols are not used after their table has been deallocated. This is currently not implemented, but if the need arises, it could be added in the future.
