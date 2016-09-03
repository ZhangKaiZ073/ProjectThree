CC=gcc
CPPFLAGS=-I../inc 
CFLAGS=-Wall

#找到str目录下的所有.c文件
str = $(wildcard *.c)

#将str目录下的所有.c 文件转换成.o给obj
obj = $(patsubst %.c, $(src))

ALL:../exec_fdfs.out

#生成所有的.o文件
../exec_fdfs.out:fastdfs_client_test01.c make_log.c
	$(CC) $^ -o $@ $(CPPFLAGS) $(CFLAGS)

#clean 

clean:
	-rm -rf ../exec_fdfs.out

.PHONY: clean ALL distclean
