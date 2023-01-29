# CH32V003-Bare-Metal
Minimal CH32V003 example makefile projects without STD, just with minimum header files.

You only need to modife these lines in makefile for each project:

TOOLCHAIN_PATH ?= /.../bin
OPENOCD_PATH   ?= /.../OpenOCD/bin

and provide path to toolchain and modifed version (WCH) of openocd.
