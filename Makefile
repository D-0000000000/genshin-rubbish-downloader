CC = gcc
CXX = g++
CFLAGS = -O2 -Wall --std=c++17
LFLAGS =
OUTPUT_DIR = output

SRCS = 	\
main.cpp	\
download_file/dl_file.cpp	\
clean_file/clean_file.cpp

INCS = 	\
.	\
download_file	\
clean_file

OBJECTS = $(addprefix $(OUTPUT_DIR)/,$(notdir $(SRCS:.cpp=.o)))
vpath %.cpp $(sort $(dir $(SRCS)))

all: prepare $(OBJECTS)
	$(CXX) $(OBJECTS) $(LFLAGS) -o $(OUTPUT_DIR)/downloader.out
	cp genshin-downloader.py $(OUTPUT_DIR)/

$(OUTPUT_DIR)/%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

prepare:
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf $(OUTPUT_DIR)
