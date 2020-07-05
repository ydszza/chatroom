objects = main.o server.o deal_client.o

main: $(objects)
	gcc -o main $(objects) -pthread

%.o: %.c
	gcc -c $<




.PYTHON:clean

clean :
	rm *.o
	rm main