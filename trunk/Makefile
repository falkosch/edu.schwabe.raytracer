TARGET ?= RELEASE

# -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wno-error=suggest-attribute=const -Wno-error=suggest-attribute=pure -Wno-error=suggest-attribute=noreturn -Wno-error=suggest-attribute=format -Wpadded -Weffc++ -Wfloat-equal 

WARNING_OPTS = -pedantic -pedantic-errors -Wall -Wextra -Wno-unknown-pragmas -Wno-ignored-qualifiers -Wformat=2 -Wstrict-overflow=5 -Wnormalized=nfkc

WARNING_OPTS_EXT = -Wabi-tag -Wc++0x-compat -Wcast-align -Wcast-qual -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wdate-time -Wdelete-non-virtual-dtor -Wdisabled-optimization -Wdouble-promotion -Winit-self -Winline -Wlogical-op -Wmissing-declarations -Wnoexcept -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpacked -Wredundant-decls -Wreorder -Wshadow -Wsign-promo -Wstack-protector -Wstrict-null-sentinel -Wswitch-default -Wswitch-enum -Wsynth -Wtrampolines -Wunsafe-loop-optimizations -Wunused-macros -Wuseless-cast -Wvector-operation-performance -Wwrite-strings -Wzero-as-null-pointer-constant

COMMON_OPTS = -march=native -mtune=native -fopenmp -D_WIN32
RELEASE_OPTS = $(COMMON_OPTS) -Ofast -flto -funroll-loops
DEBUG_OPTS = $(COMMON_OPTS) -Og -g2

CPP_OPTS_RELEASE = $(WARNING_OPTS) $(WARNING_OPTS_EXT) $(RELEASE_OPTS) -DNDEBUG
LD_OPTS_RELEASE = $(RELEASE_OPTS)
CPP_OPTS_DEBUG = $(WARNING_OPTS) $(WARNING_OPTS_EXT) $(DEBUG_OPTS) -D_DEBUG
LD_OPTS_DEBUG = $(DEBUG_OPTS)

CPP_OPTS = $(CPP_OPTS_$(TARGET)) -std=gnu++17
#LD_OPTS = $(LD_OPTS_$(TARGET)) -static-libgcc -static -lgdi32 -lopengl32
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

VECTORIZATION_OPTS = $(CPP_OPTS) -Ivectorization/include
RAYTRACING_OPTS = $(VECTORIZATION_OPTS) -Iraytracing/include
RAYTRACERUI_OPTS = $(RAYTRACING_OPTS) -Iraytracerui/include -Ipackages/glew.v140.1.12.0/build/native/include

.PHONY: clean

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
