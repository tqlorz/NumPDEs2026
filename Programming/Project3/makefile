# set compiler and flags
CXX       = g++
CXXFLAGS  = -std=c++20 -g
INCLUDES  = -Iinclude
CFLAGS = -ljsoncpp -fPIC
# set directories
SRC_DIR   = src
BUILD_DIR = build
OUTPUT_DATA_DIR = output/data
OUTPUT_FIG_DIR = output/figure
# set target and sources of cpp
TARGET    = $(BUILD_DIR)/main
SRCS      = $(wildcard $(SRC_DIR)/*.cpp)
OBJS      = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS      = $(OBJS:.o=.d)
# set the list of result files
TXT_FILES = $(wildcard $(OUTPUT_DATA_DIR)/*.txt)
FIG_FILES = $(TXT_FILES:.txt=.pdf)

.PHONY: clean run plot gdb story
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
	make plot
story:
	$(MAKE) -C doc
plot: 
	$(MAKE) do-plot
do-plot: $(patsubst $(OUTPUT_DATA_DIR)/%.txt,$(OUTPUT_FIG_DIR)/%.pdf, \
         $(wildcard $(OUTPUT_DATA_DIR)/*.txt))
$(OUTPUT_FIG_DIR)/%.pdf: $(OUTPUT_DATA_DIR)/%.txt | $(OUTPUT_FIG_DIR)
	python3 python/plot.py $<
gdb: $(TARGET)
	gdb $(TARGET)
clean:
	rm -f $(BUILD_DIR)/*.d $(BUILD_DIR)/*.o
	rm -f $(TARGET)	