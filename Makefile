CXX := g++
NVCC := nvcc

CXXFLAGS := \
    -Wall -pedantic -Wdisabled-optimization \
    -pedantic-errors -Wextra -Wcast-align \
    -Wcast-qual -Wchar-subscripts -Wcomment \
    -Wconversion -Werror -Wfloat-equal -Wformat \
    -Wformat=2 -Wformat-nonliteral -Wformat-security \
    -Wformat-y2k -Wimport -Winit-self -Winvalid-pch \
    -Wunsafe-loop-optimizations -Wlong-long \
    -Wmissing-braces -Wmissing-field-initializers \
    -Wmissing-format-attribute -Wmissing-include-dirs \
    -Weffc++ -Wpacked -Wparentheses -Wpointer-arith \
    -Wredundant-decls -Wreturn-type -Wsequence-point \
    -Wshadow -Wsign-compare -Wstack-protector \
    -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch \
    -Wswitch-default -Wswitch-enum -Wtrigraphs \
    -Wuninitialized -Wunknown-pragmas -Wunreachable-code \
    -Wunused -Wunused-function -Wunused-label \
    -Wunused-parameter -Wunused-value -Wunused-variable \
    -Wvariadic-macros -Wvolatile-register-var -Wwrite-strings \
    -pipe -Ofast -g -s -std=c++17 -fopenmp \
    -msse -msse2 -msse3 -mfpmath=sse -flto=auto \
    -march=native -funroll-loops -ffast-math \
    -DNDEBUG

INCLUDES := -Iinclude
OUT_DIR := dist
OBJ_DIR := $(OUT_DIR)/obj

SRC_DIR := src
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

zhivo-x86: $(OBJ_DIR)/zhivo-x86

$(OBJ_DIR)/zhivo-x86: $(OBJS)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(OUT_DIR)/zhivo-x86

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

zhivo-cuda: $(OBJ_DIR)/zhivo-cuda

$(OBJ_DIR)/zhivo-cuda: $(SRCS)
	@mkdir -p $(OUT_DIR)
	$(NVCC) $(INCLUDES) $(SRCS) -o $(OUT_DIR)/zhivo-cuda

clean:
	rm -rf $(OUT_DIR)

.PHONY: clean zhivo-x86 zhivo-cuda
