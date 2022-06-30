CC = gcc
SRC = src
BUILD = build

all: build cell-logger

build:
	$(shell if [ -d $(BUILD) ]; then rm -rf $(BUILD) fi)
	$(shell mkdir $(BUILD))
	
cell-logger: cell-logger.o connmgr.o network.o
	$(CC) -o $(BUILD)/cell-logger cell-logger.o connmgr.o network.o

cell-logger.o: $(SRC)/cell-logger.c
	$(CC) -c $(SRC)/cell-logger.c

connmgr.o: $(SRC)/connmgr.c
	$(CC) -c $(SRC)/connmgr.c

network.o: $(SRC)/network.c
	$(CC) -c $(SRC)/network.c

clean:
	rm $(SRC)/*.o
