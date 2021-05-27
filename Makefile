TARGET = main
LIB = liborder.a
LIBSO = liborder.so.1.0
DIRS = /mnt/hgfs/嵌入式/myXmrs/project_02/src
INC = -I /mnt/hgfs/嵌入式/myXmrs/project_02/include
SRCS = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
OBJ = $(patsubst %.c,%.o,$(SRCS))
CC := arm-linux-gcc
ARCR := ar -cr
RMRF:=rm -rf
SHARED = -fPIC -shared

#静态库打包
$(LIB):$(OBJ)
	$(ARCR) $@ $^
#动态库打包
# $(LIBSO):$(OBJ)
# 	$(CC) $(SHARED) $^ -o $@
# $(TARGET):$(OBJ)
# 	$(CC) $^ -o ^@
.PHONY:
arm:
	arm-linux-gcc main.c -o $(TARGET) $(LIB) -lm
gcc:
	$(CC) main.c -o $(TARGET) $(LIB)
clean:
	$(RMRF) $(OBJ)
cleanAll:
	$(RMRF) $(OBJ) $(TARGET) $(LIB)