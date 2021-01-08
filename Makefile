APP = scalpa

SHELL = /bin/bash
CC = gcc
CLEX = lex
CBISON = bison
CFLAGS = -g -Wall -Werror -Wextra -Wpedantic
CLIBS = -lfl -ly

BIN_DIR = bin/

SRC_DIR = src/
SRC_FILES = $(wildcard $(SRC_DIR)*.c) $(wildcard $(SRC_DIR)*/*.c) \
	$(SRC_DIR)$(APP).tab.c $(SRC_DIR)$(APP).yy.c

LEX_FILES = $(wildcard $(SRC_DIR)*.l)	
YACC_FILES = $(wildcard $(SRC_DIR)*.y)

OBJ_DIR = obj/
OBJ_FILES = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES))
OBJ_SUBDIRS = $(patsubst $(SRC_DIR)%, $(OBJ_DIR)%, $(dir $(SRC_FILES)))

DOC_DIR = doc/
DOXYFILE = Doxyfile

OUT_DIR = out/

TESTS_DIR = test/

vpath %.c src
vpath %.h src
vpath %.o obj

.PHONY: all clean cleanOut test

# Compile project
all: createDirectories $(SRC_DIR)$(APP).tab.c $(SRC_DIR)$(APP).yy.c \
	$(BIN_DIR)$(APP)

createDirectories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ) $(OBJ_SUBDIRS)

$(SRC_DIR)$(APP).tab.c : $(YACC_FILES)
	@echo -e "\e[1m\e[36m$(CBISON): $@\e[0m\e[39m"
	$(CBISON) -d -Wall -o $@ $<
	@echo -e

$(SRC_DIR)$(APP).yy.c : $(LEX_FILES)
	@echo -e "\e[1m\e[36m$(CLEX): $@\e[0m\e[39m"
	$(CLEX) -o $@ $<
	@echo -e

$(BIN_DIR)$(APP): $(OBJ_FILES)
	@echo -e "\n\e[1m\e[32m$(CC): $@\e[0m\e[39m"
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(CLIBS) -o $@ $^
	@echo -e

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo -e "\e[1m\e[36m$(CC): $@\e[0m\e[39m"
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CLIBS) $(INCLUDE) -c $< -o $@

# Clean project
clean: cleanSrc
	@echo -e "\e[1m\e[31mCleaning project\e[0m\e[39m"
	@rm -rf $(BIN_DIR) $(OBJ_DIR) $(DOC_DIR) $(OUT_DIR) scalpa.tar.gz
	@echo -e

cleanSrc:
	@rm -rf $(SRC_DIR)$(APP).tab.* $(SRC_DIR)$(APP).yy.c

cleanOut:
	@echo -e "\e[1m\e[31mCleaning output\e[0m\e[39m"
	@rm -rf $(OUT_DIR)
	@echo -e

# Document project
doc:
	@echo -e "\e[1m\e[32mCreating and opening documentation\e[0m\e[39m"
	@doxygen $(DOXYFILE)
	@xdg-open $(DOC_DIR)html/index.html

# Tests
test: all
	@./test.sh

# Create archive
archive:
	@tar -czf scalpa.tar.gz $(SRC_DIR) $(TESTS_DIR) Doxyfile Makefile Rapport.pdf README.md test.sh
