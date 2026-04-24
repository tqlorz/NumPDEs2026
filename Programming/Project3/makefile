# set compiler and flags
CXX       = g++
CXXFLAGS  = -std=c++20 -g
INCLUDES  = -Iinclude
CFLAGS = -ljsoncpp -llapacke -llapack -lblas -lgfortran -fPIC
# set directories and variables
SRC_DIR   = src
BUILD_DIR = build
OUTPUT_DATA_DIR = output/data
OUTPUT_FIG_DIR = output/figure
TARGET    = $(BUILD_DIR)/main
SRCS      = $(wildcard $(SRC_DIR)/*.cpp)
OBJS      = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS      = $(OBJS:.o=.d)
# compile and link
$(TARGET): $(OBJS) | $(BUILD_DIR) $(OUTPUT_DATA_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(CFLAGS)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR) $(OUTPUT_DATA_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
-include $(DEPS)
# create directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $@
$(OUTPUT_DATA_DIR):
	mkdir -p $@
$(OUTPUT_FIG_DIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)
# 	make plot
story:
	$(MAKE) -C doc
plot: $(OUTPUT_DATA_DIR) $(OUTPUT_FIG_DIR)
	python3 python/plot.py $(OUTPUT_DATA_DIR)/*.txt
gdb: $(TARGET)
	gdb $(TARGET)
clean:
	rm -f $(BUILD_DIR)/*.d $(BUILD_DIR)/*.o
	rm -f $(TARGET)	

.PHONY: all clean run plot gdb