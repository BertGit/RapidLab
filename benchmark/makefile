BIN = benchmark

CXX = g++ -std=c++11
CXXFLAGS += -g -Wall -Wextra -O3

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

OBJ_DIR := obj

all : directories $(BIN)
$(BIN) : $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -g -o $@ $^
	@echo All done

directories: $(OBJ_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/benchmark.o : benchmark.cpp
	$(CXX) $(CXXFLAGS) -g -o $@ -c $< -I../

clean :
	rm -rf $(OBJ_DIR)
	rm -f $(BIN)
	@echo Clean done
