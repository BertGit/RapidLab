GTEST_DIR = ../googletest/googletest
GMOCK_DIR = ../googletest/googlemock

USER_DIR = .

CXX = g++

CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GMOCK_DIR)/include

CXXFLAGS += -g -Wall -Wextra -std=c++11 -O3

TESTS = interval_test

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
                $(GMOCK_DIR)/include/gmock/internal/*.h \
                $(GTEST_HEADERS)

OBJ_DIR = obj

all : directories $(TESTS)
	@echo All done
clean :
	rm -rf $(OBJ_DIR)
	rm -f $(TESTS)
	@echo Clean done

directories: $(OBJ_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)

$(OBJ_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GTEST_DIR)/src/gtest-all.cc -o $@

$(OBJ_DIR)/gmock-all.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock-all.cc -o $@

$(OBJ_DIR)/gmock_main.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock_main.cc -o $@

$(OBJ_DIR)/gmock.a : $(OBJ_DIR)/gmock-all.o $(OBJ_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/gmock_main.a : $(OBJ_DIR)/gmock-all.o $(OBJ_DIR)/gtest-all.o $(OBJ_DIR)/gmock_main.o
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/interval.test.o : $(USER_DIR)/interval.test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/interval.test.cpp -o $@ -I..

$(OBJ_DIR)/optimizer.test.o : $(USER_DIR)/optimizer.test.cpp $(GMOCK_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/optimizer.test.cpp -o $@ -I..

interval_test : $(OBJ_DIR)/interval.test.o $(OBJ_DIR)/optimizer.test.o $(OBJ_DIR)/gmock_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
