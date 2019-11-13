# SM64DS-Symbols
Public repository where discovered symbols from SM64DS (EU) including documentation can be found.

## General
This repo contains all symbols that have been discovered in the EU version of SM64DS. 
Its purpose is to help other people to find the right functions without spending lots of time conducting their own research.
For beginners it can be very frustrating finding information on the game's internals.
Although there exists an incredibly useful ASM Patching Template with many general-purpose functions, there's a lack of documentation on how to properly use certain methods.
I hope it helps others to save time. To everybody who found something new or discovered a mistake, feel free to propose your discovery so we can add it to the master branch.

**Remember: This repo contains only EU symbols.**

## Files
+ **symbols.x**: Contains all necessary linker symbols for functions you use in C/C++/ASM code. It's important to outline the syntax of this file:
  ```C
  C_Function_Name = 0x0add1e55; /* Comments are like in standard C */
  ```
  In case a symbol is referenced from C++ code, it is mandatory to leave the name mangled, e.g.:
  ```C++
  _ZN5Class7ExampleEv = 0xdeadbeef; /* Class::Example() */
  ```
  Name mangling scheme follows the Itanium ABI, used by e.g. GCC but not MSVC.
  All C symbols must be referenced with `extern "C"` when compiled with a C++ compiler.
  This file does **NOT** contain constants. This is important since this may inflict linker errors. 
  Additionally, debuggers which offer symbol name display (like no$gba) could treat constants as real symbols, creating confusion for the user.
  As a rule of thumb, only addresses can be found here.
+ **asm-sym-export.exe**: Program that converts symbols.x to symbols.sym
  It turned out to be useful to automatically convert the linker file to a .sym file debuggers can utilize.
  Call it with `asm-sym-export.exe infile` to create a .sym file of the same name. Comments from the original .x file are ignored.
  Demangles any C++ symbols for readability and replaces spaces with underscores (no$gba refuses to load spaces).
+ **asm-sym-export.bat**: Batch file that automatically calls `asm-sym-export.exe` and renames the resulting .sym to SM64DS.sym. 
  Adjust the output file name to match your game's file name, otherwise no$gba won't load it.
  **Warning**: The previous .sym file will be deleted, so make sure you back it up in case you want to keey the previous .sym.
+ **src/**: Source folder. Contains all files to be compiled. Folder name must match with Makefile settings.
+ **unchecked/**: Unchecked proposals. 
  Contains files with symbols/functions/documentation that is still in development, needs to be checked or corrected, or only vague definitons but was too interesting to discard it.
  As soon as the proposal features full definitions, it will be merged with symbols.x or the corresponding .c/.cpp file.
