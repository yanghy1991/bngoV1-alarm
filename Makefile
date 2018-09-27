CFLAGS= -Wall -O2
CC=mipsel-linux-gcc
COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -lm -lszhy_get
obj += delPastalarm.o
TARGET = delPastalarm

main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)
	mipsel-linux-gcc -o doalarm messagesend.c

clean:
	rm -rf *.o doalarm delPastalarm