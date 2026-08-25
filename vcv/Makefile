# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= ../..

# If DAISYSP_DIR is not defined when calling the Makefile, default to the sibling DaisySP checkout
DAISYSP_DIR ?= ../DaisySP

# The DSP this module shares with the Daisy firmware
SHARED_DIR ?= ../shared

# FLAGS will be passed to both the C and C++ compiler
FLAGS += -I$(DAISYSP_DIR)/Source
FLAGS += -I$(SHARED_DIR)
CFLAGS +=
CXXFLAGS +=

# DaisySP's own Makefile builds for the Daisy's ARM Cortex-M7, so its libdaisysp.a
# can't be linked into an x86-64 plugin. Build a native copy here instead, using
# this plugin's compiler flags so the ABI matches. Left in build/ so `make clean`
# takes it with everything else, and so DaisySP/build/ stays reserved for firmware.
DAISYSP_LIB := build/libdaisysp.a
DAISYSP_SOURCES := $(wildcard $(DAISYSP_DIR)/Source/*/*.cpp)
DAISYSP_OBJECTS := $(patsubst $(DAISYSP_DIR)/%.cpp, build/daisysp/%.o, $(DAISYSP_SOURCES))
DAISYSP_DEPS := $(DAISYSP_OBJECTS:.o=.d)
DAISYSP_INCLUDES := $(addprefix -I, $(DAISYSP_DIR)/Source $(sort $(dir $(DAISYSP_SOURCES))))

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
# $(DAISYSP_LIB) is linked by naming it as a prerequisite of $(TARGET) below, since
# compile.mk's link rule passes $^ to the linker -- no -l flag needed.

# Add .cpp files to the build
SOURCES += $(wildcard src/*.cpp)
SOURCES += $(wildcard src/dsp/*.cpp)

# ../shared can't go in SOURCES: compile.mk would derive build/../shared/... and
# write the objects outside build/. Compile them here and append to OBJECTS,
# which compile.mk places after its own objects and links into $(TARGET).
SHARED_SOURCES := $(wildcard $(SHARED_DIR)/*.cpp)
SHARED_OBJECTS := $(patsubst $(SHARED_DIR)/%.cpp, build/shared/%.o, $(SHARED_SOURCES))
OBJECTS += $(SHARED_OBJECTS)
SHARED_DEPS := $(SHARED_OBJECTS:.o=.d)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

# TARGET is defined by plugin.mk, so these rules have to come after the include.
$(TARGET): $(DAISYSP_LIB)

$(DAISYSP_LIB): $(DAISYSP_OBJECTS)
	$(AR) rcs $@ $^

# -std=c++14 (DaisySP's standard) and -w come last so they override the C++11 and
# warning flags Rack's compile.mk put in CXXFLAGS. DaisySP is a vendored dependency;
# its warnings aren't ours to fix.
build/daisysp/%.o: $(DAISYSP_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DAISYSP_INCLUDES) -std=c++14 -w -c -o $@ $<

# Our own code, so it keeps the plugin's warning flags.
build/shared/%.o: $(SHARED_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

-include $(DAISYSP_DEPS) $(SHARED_DEPS)
