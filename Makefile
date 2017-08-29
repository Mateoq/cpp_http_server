CC = ~/gcc-7.1/root/bin/g++

BUILD_DIR := ./build
BIN_DIR := ./bin
# DEPS_LIB_DIR := ./dependencies/lib
DEPS_INC_DIR := ./dependencies/includes
INCLUDES := -I$(DEPS_INC_DIR)
CFLAGS := -std=c++1z -Wall -m64 -g

BOOST_PATH = /Users/mquintero/boost/root

INCLUDE_PATHS = -I/Users/mquintero/gcc-7.1/root/include -I$(BOOST_PATH)/include $(INCLUDES)
LIBRARY_PATHS = -L/Users/mquintero/gcc-7.1/root/lib -L$(BOOST_PATH)/lib
OUTPUT := -o $(BIN_DIR)/HttpServer
LINKER_FLAGS = -lboost_system -m64
BUILD_DEPS = $(BUILD_DIR)/main.o \
    $(BUILD_DIR)/request_handler.o \
    $(BUILD_DIR)/request_parser.o \
    $(BUILD_DIR)/response.o \
    $(BUILD_DIR)/mime_types.o \
	$(BUILD_DIR)/server.o \
    $(BUILD_DIR)/connection.o \
    $(BUILD_DIR)/connection_manager.o
SRC_DIR = ./src

start:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)
	make clean
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cpp $(INCLUDE_PATHS) -o $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/server.cpp $(INCLUDE_PATHS) -o $(BUILD_DIR)/server.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/connection.cpp $(INCLUDE_PATHS) -o $(BUILD_DIR)/connection.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/connection_manager.cpp $(INCLUDE_PATHS) -o $(BUILD_DIR)/connection_manager.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/request_handler.cpp $(INCLUDE_PATHS) -o $(BUILD_DIR)/request_handler.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/request_parser.cpp $(INCLUDE_PATHS) -o $(BUILD_DIR)/request_parser.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/response.cpp $(INCLUDE_PATHS) -o $(BUILD_DIR)/response.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mime_types.cpp $(INCLUDE_PATHS) -o $(BUILD_DIR)/mime_types.o
	$(CC) $(CFLAGS) $(BUILD_DEPS) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(OUTPUT)

cleanBuild:
	rm -rf $(BUILD_DIR)/*

clean:
	rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*
