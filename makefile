PWD := $(shell pwd)
DIR_CUR := ${shell pwd}
DIR_OUT := ${PWD}/out/lout
DIR_OUT_TMP := ${PWD}/out/tmp


include $(DIR_CUR)/driver/makefile


cleanall: clean
	-rm $(DIR_OUT)/*

clean:
	-rm -rf $(DIR_OUT_TMP)
