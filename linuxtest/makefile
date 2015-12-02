INC := -I../driver/ -I../common/
LIB := ../out/lout/libdriver.so \

SRC := test.cpp \

test: $(SRC) $(LIB)
	gcc $(SRC) $(INC) -o test $(LIB)


clean:
	-rm test
