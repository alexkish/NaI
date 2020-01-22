
name := 3inch_noDent
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
   G4INSTALL = /app/cern/geant_4.9.3.2/
endif

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)

 EXTRALIBS += $(ROOTLIBS)
 EXTRALIBS += $(CLHEPLIBS)

CPPFLAGS += $(ROOTCFLAGS)

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk
