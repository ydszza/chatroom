TARGET			:= server
SUFIX			:= -lpthread -lsqlite3

INCDIRS 		:= Project \
						Component/Epoll \
						Component/List \
						Component/Socket \
						Component/Sqlite3 \
						Component/Threadpool \
						Component/UserManage 
				   			   
SRCDIRS			:= Project \
						Component/Epoll \
						Component/List \
						Component/Socket \
						Component/Sqlite3 \
						Component/Threadpool \
						Component/UserManage 



INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

CFILENDIR		:= $(notdir $(CFILES))

OBJS			:= $(patsubst %, Output/%, $(CFILENDIR:.c=.o))

#特殊变量，指定make搜索路径
VPATH			:= $(SRCDIRS)


.PYTHON : clean

$(TARGET) : $(OBJS)
	gcc -Wall -o $@ $^ $(SUFIX)

$(OBJS) : Output/%.o : %.c
	gcc -Wall -c -o $@ $< $(SUFIX)


clean :
	rm -rf $(TARGET) $(OBJS)

all:
	@echo INCLUDE = $(INCLUDE)
	@echo 
	@echo CFILES = $(CFILES)
	@echo 
	@echo CFILENDIR = $(CFILENDIR)
	@echo 
	@echo OBJS = $(OBJS)