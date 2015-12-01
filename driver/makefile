GCC := gcc
LD := ld

PWD = $(shell pwd)
LDFLAGS := -shared -fpic
CFLAGS := -g -Wall -fpic

LIB_NAME := libdriver

DIR_INC := \

DIR_OBJ := ./out/tmp/driver/obj
DIR_LIB := ./out/tmp/driver/lib
DIR_OUT := ./out/lout/

SHARE_LIB := $(LIB_NAME).so



THREAD_SRC_FILES := \
    $(PWD)/driver/thread/thread.cpp \



SRC_FILES := \
   $(THREAD_SRC_FILES) \


OBJ := $($(notdir $(SRC_FILES)))
INC := -I${DIR_INC}

$(warning $(SRC_FILES))

all:$(OBJ)
	-mkdir -p $(DIR_LIB)
	$(GCC) $(SRC_FILES) $(LDFLAGS)  -o $(DIR_LIB)/$(SHARE_LIB)
	-cp $(DIR_LIB)/$(SHARE_LIB) $(DIR_OUT)
#	$(LD) $(LDFLAGS) -o $(DIR_LIB)/$(SHARE_LIB) $(OBJ)
#	${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
#	@echo Compiling $(OBJ) ...  
#	$(GCC) $(CFLAGS) -o $@ -c $< ${INC} -g

	  



