CXX = g++
CXXFLAGS = -g -O3 -MMD -MP -Isrc -Iext

SRC_DIR = src
BUILD_DIR = build
TARGET = raytracer

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
