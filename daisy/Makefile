# Project Name
TARGET = template

# Sources
# SharedDSP.cpp holds the DSP that this firmware and the VCV Rack module both use.
SHARED_DIR = ../shared
CPP_SOURCES = template.cpp $(SHARED_DIR)/SharedDSP.cpp
C_INCLUDES += -I$(SHARED_DIR)

# Library Locations
LIBDAISY_DIR = ../libDaisy
DAISYSP_DIR = ../DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
