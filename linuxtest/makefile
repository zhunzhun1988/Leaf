INC := -I../driver/ -I../common/
LIB := ../out/lout/libdriver.so \

SRC := test.cpp \

test: $(SRC) $(LIB)
	gcc $(SRC) $(INC) -o test $(LIB) -DCUR_OS_LINUX


clean:
	-rm test
