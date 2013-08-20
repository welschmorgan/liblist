ARCH32=x86
ARCH64=x64
INC_DIR=./inc
SRC_DIR=./src
BIN_DIR=./lib
BIN_DIR_X86=$(BIN_DIR)/$(ARCH32)
BIN_DIR_X64=$(BIN_DIR)/$(ARCH64)
OBJ_DIR=./obj
OBJ_DIR_X86=$(OBJ_DIR)/$(ARCH32)
OBJ_DIR_X64=$(OBJ_DIR)/$(ARCH64)

TARGET_NAME=liblist.a
TARGET_DIR_X86=$(BIN_DIR_X86)
TARGET_PATH_X86=$(TARGET_DIR_X86)/$(TARGET_NAME)
TARGET_DIR_X64=$(BIN_DIR_X64)
TARGET_PATH_X64=$(TARGET_DIR_X64)/$(TARGET_NAME)

all: all_x86 all_x64

clean_x86:
	rm -rf $(OBJ_DIR_X86)/*.o
mrproper_x86:
	rm -rf $(TARGET_PATH_X86)
clean_x64:
	rm -rf $(OBJ_DIR_X64)/*.o
mrproper_x64:
	rm -rf $(TARGET_PATH_X64)
.PHONY: mrproper_x86 mrproper_x64 clean_x86 clean_x64

CC=gcc
AR=ar rcs
DEFINES=-D NDEBUG
CFLAGS_X86=-g -Wall -Werror -Wextra -pedantic -ansi -m32 $(DEFINES)
CFLAGS_X64=-g -Wall -Werror -Wextra -pedantic -ansi -m64 $(DEFINES)

$(OBJ_DIR_X86)/list.o:
	$(CC) $(CFLAGS_X86) -c -I$(INC_DIR) $(SRC_DIR)/list.c -o $(OBJ_DIR_X86)/list.o
$(OBJ_DIR_X64)/list.o:
	$(CC) $(CFLAGS_X64) -c -I$(INC_DIR) $(SRC_DIR)/list.c -o $(OBJ_DIR_X64)/list.o


$(OBJ_DIR_X86)/main.o:
	$(CC) $(CFLAGS_X86) -c -I$(INC_DIR) $(SRC_DIR)/main.c -o $(OBJ_DIR_X86)/main.o
$(OBJ_DIR_X64)/main.o:
	$(CC) $(CFLAGS_X64) -c -I$(INC_DIR) $(SRC_DIR)/main.c -o $(OBJ_DIR_X64)/main.o


$(TARGET_PATH_X86): $(OBJ_DIR_X86)/list.o $(OBJ_DIR_X86)/main.o
	$(AR) $(TARGET_PATH_X86) $(OBJ_DIR_X86)/list.o
	$(CC) $(CFLAGS_X86) -static $(OBJ_DIR_X86)/main.o -L$(BIN_DIR_X86) -llist -o $(BIN_DIR_X86)/main.out

$(TARGET_PATH_X64): $(OBJ_DIR_X64)/list.o $(OBJ_DIR_X64)/main.o
	$(AR) $(TARGET_PATH_X64) $(OBJ_DIR_X64)/list.o
	$(CC) $(CFLAGS_X64) -static $(OBJ_DIR_X64)/main.o -L$(BIN_DIR_X64) -llist -o $(BIN_DIR_X64)/main.out

all_x86: clean_x86 mrproper_x86 $(TARGET_PATH_X86)
all_x64: clean_x64 mrproper_x64 $(TARGET_PATH_X64)

