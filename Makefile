CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -g
CPPFLAGS ?= -Iinclude

BUILD_DIR := build
APP := gpu_comp
DAY1_TEST := $(BUILD_DIR)/test_lexer_day1
DAY2_TEST := $(BUILD_DIR)/test_lexer_day2
DAY3_TEST := $(BUILD_DIR)/test_lexer_day3
DAY4_TEST := $(BUILD_DIR)/test_lexer_day4
DAY5_TEST := $(BUILD_DIR)/test_lexer_day5
DAY6_TEST := $(BUILD_DIR)/test_lexer_day6
DAY7_TEST := $(BUILD_DIR)/test_lexer_day7
DAY8_TEST := $(BUILD_DIR)/test_lexer_day8
DAY9_TEST := $(BUILD_DIR)/test_lexer_day9
DAY10_TEST := $(BUILD_DIR)/test_lexer_day10
LEXER_SRC := src/lexer.cpp
LEXER_HEADER := include/lexer.h

.PHONY: all test test-day1 test-day2 test-day3 test-day4 test-day5 test-day6 test-day7 test-day8 test-day9 test-day10 clean

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

$(DAY4_TEST): tests/test_lexer_day4.cpp $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day4.cpp $(LEXER_SRC) -o $@

$(DAY5_TEST): tests/test_lexer_day5.cpp tests/test_helpers.h $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day5.cpp $(LEXER_SRC) -o $@

$(DAY6_TEST): tests/test_lexer_day6.cpp tests/test_helpers.h $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day6.cpp $(LEXER_SRC) -o $@

$(DAY7_TEST): tests/test_lexer_day7.cpp tests/test_helpers.h $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day7.cpp $(LEXER_SRC) -o $@

test-day1: $(DAY1_TEST)
	./$(DAY1_TEST)

test-day2: $(DAY2_TEST)
	./$(DAY2_TEST)

test-day3: $(DAY3_TEST)
	./$(DAY3_TEST)

test-day4: $(DAY4_TEST)
	./$(DAY4_TEST)

test-day5: $(DAY5_TEST)
	./$(DAY5_TEST)

test-day6: $(DAY6_TEST)
	./$(DAY6_TEST)

$(DAY8_TEST): tests/test_lexer_day8.cpp tests/test_helpers.h $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day8.cpp $(LEXER_SRC) -o $@

test-day7: $(DAY7_TEST)
	./$(DAY7_TEST)

test-day8: $(DAY8_TEST)
	./$(DAY8_TEST)

test-day9: $(DAY9_TEST)
	./$(DAY9_TEST)

$(DAY9_TEST): tests/test_lexer_day9.cpp tests/test_helpers.h $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day9.cpp $(LEXER_SRC) -o $@

$(DAY10_TEST): tests/test_lexer_day10.cpp tests/test_helpers.h $(LEXER_SRC) $(LEXER_HEADER) | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/test_lexer_day10.cpp $(LEXER_SRC) -o $@

test-day10: $(DAY10_TEST)
	./$(DAY10_TEST)

test: test-day1 test-day2 test-day3 test-day4 test-day5 test-day6 test-day7 test-day8 test-day9 test-day10

clean:
	rm -rf $(BUILD_DIR) $(APP)
