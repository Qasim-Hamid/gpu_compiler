CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -g
CPPFLAGS ?= -Iinclude

BUILD_DIR := build
APP := gpu_comp
DAY1_TEST := $(BUILD_DIR)/test_lexer_day1
DAY2_TEST := $(BUILD_DIR)/test_lexer_day2
DAY3_TEST := $(BUILD_DIR)/test_lexer_day3
LEXER_SRC := src/lexer.cpp
LEXER_HEADER := include/lexer.h

.PHONY: all test test-day1 test-day2 test-day3 clean

all: $(APP)

$(APP): src/main.cpp $(LEXER_SRC) $(LEXER_HEADER)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) src/main.cpp $(LEXER_SRC) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DAY1_TEST): tests/test_lexer_day1.cpp $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day1.cpp $(LEXER_SRC) -o $@

$(DAY2_TEST): tests/test_lexer_day2.cpp $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day2.cpp $(LEXER_SRC) -o $@

$(DAY3_TEST): tests/test_lexer_day3.cpp $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day3.cpp $(LEXER_SRC) -o $@

test-day1: $(DAY1_TEST)
	./$(DAY1_TEST)

test-day2: $(DAY2_TEST)
	./$(DAY2_TEST)

test-day3: $(DAY3_TEST)
	./$(DAY3_TEST)

test: test-day1 test-day2 test-day3

clean:
	rm -rf $(BUILD_DIR) $(APP)
