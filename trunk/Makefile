TARGET ?= RELEASE

# -Wno-ignored-qualifiers: do not warn about const in return type, because style guide rule wants it their
# -Wno-unknown-pragmas: do not warn about unknown pragmas, some code uses "#pragma region" to enable cold folding for very old IDEs

COMMON_OPTS = -march=native -mwin32 -fopenmp -D_WIN32 -Wall -Wextra -pedantic -Wno-ignored-qualifiers -Wno-unknown-pragmas
RELEASE_OPTS = $(COMMON_OPTS) -O3 -flto
DEBUG_OPTS = $(COMMON_OPTS) -Og -g2

CPP_OPTS_RELEASE =  $(RELEASE_OPTS) -DNDEBUG
LD_OPTS_RELEASE = $(RELEASE_OPTS)
CPP_OPTS_DEBUG = $(DEBUG_OPTS) -D_DEBUG
LD_OPTS_DEBUG = $(DEBUG_OPTS)

CPP_OPTS = $(CPP_OPTS_$(TARGET)) -std=c++17 -D_UNICODE -DUNICODE
LD_OPTS = $(LD_OPTS_$(TARGET)) -lgdi32 -lopengl32

CPP = @g++
ASM = @g++
LD = @g++
DISASM = @objdump -d
MKDIR = @mkdir -p

BUILD_DIR = build/$(TARGET)
EXE = Raytracer_$(TARGET).exe

VECTORIZATION_SOURCES = $(shell find vectorization/src/ -name '*.cpp')
RAYTRACING_SOURCES = $(shell find raytracing/src/ -name '*.cpp')
RAYTRACERUI_SOURCES = $(shell find raytracerui/src/ -name '*.cpp')

VECTORIZATION_OBJS = $(patsubst vectorization/src/%,$(BUILD_DIR)/vectorization/%,$(VECTORIZATION_SOURCES:%.cpp=%.obj))
RAYTRACING_OBJS = $(patsubst raytracing/src/%,$(BUILD_DIR)/raytracing/%,$(RAYTRACING_SOURCES:%.cpp=%.obj))
RAYTRACERUI_OBJS = $(patsubst raytracerui/src/%,$(BUILD_DIR)/raytracerui/%,$(RAYTRACERUI_SOURCES:%.cpp=%.obj))

VECTORIZATION_OPTS = $(CPP_OPTS) -Ivectorization/include -Ivectorization/3rdparty
RAYTRACING_OPTS = $(VECTORIZATION_OPTS) -Iraytracing/include
RAYTRACERUI_OPTS = $(RAYTRACING_OPTS) -Iraytracerui/include -Ipackages/glew.v140.1.12.0/build/native/include

.PHONY: clean build

build: $(EXE)

$(EXE): $(VECTORIZATION_OBJS) $(RAYTRACING_OBJS) $(RAYTRACERUI_OBJS)
		$(LD) $^ -o $@ $(LD_OPTS)
#		$(DISASM) -S $@ > $@.dump

VECTORIZATION_HEADERS = $(shell find vectorization/include/ -name '*.h')
$(BUILD_DIR)/vectorization/%.obj: vectorization/src/%.cpp $(VECTORIZATION_HEADERS)
		$(MKDIR) $(dir $@)
		$(CPP) -c $< -o $@ $(VECTORIZATION_OPTS)
#		$(ASM) -S $< -o $@.asm $(VECTORIZATION_OPTS)

RAYTRACING_HEADERS = $(VECTORIZATION_HEADERS) $(shell find raytracing/include/ -name '*.h')
$(BUILD_DIR)/raytracing/%.obj: raytracing/src/%.cpp $(RAYTRACING_HEADERS)
		$(MKDIR) $(dir $@)
		$(CPP) -c $< -o $@ $(RAYTRACING_OPTS)
#		$(ASM) -S $< -o $@.asm $(RAYTRACING_OPTS)

RAYTRACERUI_HEADERS = $(RAYTRACING_HEADERS) $(shell find raytracerui/include/ -name '*.h')
$(BUILD_DIR)/raytracerui/%.obj: raytracerui/src/%.cpp $(RAYTRACERUI_HEADERS)
		$(MKDIR) $(dir $@)
		$(CPP) -c $< -o $@ $(RAYTRACERUI_OPTS)
#		$(ASM) -S $< -o $@.asm $(RAYTRACERUI_OPTS)

clean:
		rm -fr $(BUILD_DIR)/
		rm -f $(EXE)
