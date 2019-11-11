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
  _ZN5ActorEv
