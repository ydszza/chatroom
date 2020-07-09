objects = main.o server.o deal_client.o op_sqlite3.o

main: $(objects)
	gcc -o main $(objects) -pthread -lsqlite3

%.o: %.c
	gcc -c $<




.PYTHON:clean

clean :
	rm *.o
	rm main