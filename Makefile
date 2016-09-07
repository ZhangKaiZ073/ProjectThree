CC=gcc
LIB=-lhiredis
INC=-I./inc
CFLAG=-Wall
SRC=./src/redis_op_test01.c ./src/make_log.c ./src/redis_op.c

ORG=$(patsubst %.c, %.o, $(SRC))

OUT=redis_op_test01

$(OUT):	

$(OUT):$(ORG)
	$(CC) $^ -o $@ $(LIB) $(INC) $(CFLAG)

$(ORG):$(SRC)
	$(CC) -c $< -o $@ $(INC) $(CFLAG)

demo:

demo:./src/fcgi_test01.c
	$(CC) $^ -o $@ -lfcgi $(CFLAG)

upload:./src/upload_test01.c ./src/make_log.c
	gcc $^ -o $@ -lfcgi $(CFIAG) $(INC) $(LIB)

clean:
	-rm -rf $(OUT) $(ORG) demo upload
	  
.PHONY: clean ALL distclean   
