# SM64DS-ASMReference
Public repository of an ASM patching reference for SM64DS (EU)

## General
This repo should serve as an continuously updated ASM symbol reference. It includes most file from the original ASMPatchingTemplate v2 with new symbols and updates.

This repository is public, therefore feel free to add your symbols and definitions you've found yourself! Just create a pull request and it gets added to the master branch. Even if you don't have full function description and just stumbled across something interesting during debugging, you can still add it to the `unchecked` folder. Any slightest hint on what a function does is welcomed so that others could check if something similar has been discovered in order to aid their development.

**Remember: This repo contains only EU symbols. BUT if you found something interesting in other versions, you can still add it into `unchecked/non_eu.txt`.**

## Files and Folders
+ **include/**: Include files. Contains all headers.
+ **source/**: Source folder. Contains all files to be compiled. Folder name must match with Makefile settings.
+ **unchecked/**: Unchecked proposals. 
  Contains files with symbols/functions/documentation that are still in development, need to be checked/corrected, or random stuff that was too interesting to discard.
  As soon as the proposal features full definitions, it will be merged with symbols.x or the corresponding .c/.cpp file.
+ **Makefile**: Self-explanatory. Used to build your patch.
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
  As a rule of thumb, this file should only contain address locations.
