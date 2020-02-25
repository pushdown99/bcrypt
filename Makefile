all: bcrypt

CFLAGS=-Icrypt_blowfish -I/usr/include/mysql
LFLAGS=-L. -lbcrypt -L/usr/lib -lmysqlclient

SRC= bcrypt.c
OBJ=$(SRC:.c=.o)

bcrypt: $(OBJ)
	$(CC) -o $@ $< $(LFLAGS)

clean:
	rm -f $(OBJ)
