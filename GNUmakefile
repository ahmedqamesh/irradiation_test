# $Id: GNUmakefile 78655 2014-01-14 11:13:41Z gcosmo $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------
# GNUmakefile Updated for examples module.  Ahmed Qamesh, 06/05/2018.
#-------------------------------------------------------------

name := Xray_Test
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

include $(G4INSTALL)/config/binmake.gmk
