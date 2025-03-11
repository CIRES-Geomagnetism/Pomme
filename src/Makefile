CC = clang

########################################
# C Flags
########################################
CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra

CLIBS = -lm
LDFLAGS = -L/usr/local/lib
INC_DIRS = -Isrc

#########################################
# Target file
#########################################
PATH_BUILD = build
PATH_SRC = src
EXE_NAME = pomme_calc
TARGET_EXE = $(PATH_BUILD)/$(EXE_NAME)

SRC_CODES=$(wildcard $(PATH_SRC)/*.c)

all: $(PATH_BUILD) $(TARGET_EXE)

$(PATH_BUILD):
	mkdir -p build

$(TARGET_EXE):
	$(CC) $(CFLAGS) $(INC_DIRS) $(SRC_CODES) pomme_calc.c -o $(TARGET_EXE) $(CLIBS) $(LDFLAGS)

clean:
	rm -rf $(PATH_BUILD)


