CC = gcc
SRC = src
BUILD = build

OBJECTS = $(addprefix $(SRC)/, cell-logger.o processor.o network.o clpacket.o log.o)

all: build cell-logger

build:
	if [ -d $(BUILD) ]; then 		\
		rm -rf $(BUILD); 		\
	fi;					\
	mkdir $(BUILD) -m755;

cell-logger: $(OBJECTS)
	$(CC) -o $(BUILD)/cell-logger $(OBJECTS)

$(SRC)/%.o: $(SRC)/%.c
	$(CC) -g -c -o $@ $<

clean:
	rm $(SRC)/*.o
