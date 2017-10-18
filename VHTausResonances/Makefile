# Package information
LIBRARY = VHTausResonances
OBJDIR  = obj
DEPDIR  = $(OBJDIR)/dep
SRCDIR  = src
INCDIR  = include

# Include the generic compilation rules
include $(SFRAME_DIR)/Makefile.common


#LHAPDF
LHAPDFINCS = -I$(shell lhapdf-config --prefix)/include
LHAPDFINCS += $(shell lhapdf-config --cflags)
LHAPDFDIR  = $(shell lhapdf-config --prefix)/lib
LHAPDFLIBS = -L$(LHAPDFDIR) -lLHAPDF

USERCXXFLAGS += $(LHAPDFINCS)
INCLUDES += $(LHAPDFLIBS)
USERLDFLAGS += $(shell lhapdf-config --ldflags)