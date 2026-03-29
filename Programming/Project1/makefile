CXX       = g++
CXXFLAGS  = -std=c++20 -O2 
INCLUDES  = -Iinclude
CFLAGS = -ljsoncpp -llapacke -llapack -lblas -lgfortran -fPIC

# set directories and variables
SRC_DIR   = src
BUILD_DIR = build
TARGET    = $(BUILD_DIR)/main
SRCS      = $(wildcard $(SRC_DIR)/*.cpp)
OBJS      = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS      = $(OBJS:.o=.d)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(CFLAGS)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
# create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $@
-include $(DEPS)

all: $(TARGET)
	make run
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f $(BUILD_DIR)/*.d $(BUILD_DIR)/*.o
# 	rm -f $(TARGET)	

.PHONY: all clean run