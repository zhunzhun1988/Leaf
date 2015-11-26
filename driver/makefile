GCC := gcc
LD := ld


LDFLAGS := -shared -fpic
CFLAGS := -g -Wall -fpic

LIB_NAME := libdriver

DIR_INC := \

DIR_OBJ := ../out/tmp/driver/obj
DIR_LIB := ../out/tmp/driver/lib

SHARE_LIB := $(LIB_NAME).so



THREAD_SRC_FILES := \
    ./thread/thread.cpp \


SRC_FILES := \
   $(THREAD_SRC_FILES) \


OBJ := $($(notdir $(SRC_FILES)))
INC := -I${DIR_INC}

$(warning $(SRC_FILES))

all:$(OBJ)
	$(LD) $(LDFLAGS) -o $(DIR_LIB)/$(SHARE_LIB) $(OBJ)
	${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	@echo Compiling $(OBJ) ...  
	$(GCC) $(CFLAGS) -o $@ -c $< ${INC} -g

clean:
	  



