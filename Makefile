CC = gcc
CXX = g++
CFLAGS = -O2 -Wall --std=c++17
LFLAGS = -lssl -lcrypto
OUTPUT_DIR = output

SRCS = 	\
main.cpp	\
download_file/dl_file.cpp	\
clean_file/clean_file.cpp	\
verify_file/verify_file.cpp	\
checker/checker.cpp	\
file_loader/file_loader.cpp

INCS = 	\
.	\
download_file	\
clean_file	\
verify_file	\
checker	\
file_loader

OBJECTS = $(addprefix $(OUTPUT_DIR)/,$(notdir $(SRCS:.cpp=.o)))
vpath %.cpp $(sort $(dir $(SRCS)))
INCLUDES = $(addprefix -I,$(INCS))

all: prepare $(OBJECTS)
	$(CXX) $(OBJECTS) $(LFLAGS) -o $(OUTPUT_DIR)/downloader.out
#	cp genshin-downloader.py $(OUTPUT_DIR)/

$(OUTPUT_DIR)/%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@ $(INCLUDES)

prepare:
	mkdir -p $(OUTPUT_DIR)

clean:
	rm -rf $(OUTPUT_DIR)
