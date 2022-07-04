CC = gcc
SRC = src
OBJDIR = obj
BUILD = build

OBJECTS = $(addprefix $(OBJDIR)/, cell-logger.o processor.o network.o \
	  				clpacket.o log.o)

all: build cell-logger

build:
	[ -d $(OBJDIR) ] || mkdir $(OBJDIR);	\
	if [ -d $(BUILD) ]; then 		\
		rm -rf $(BUILD); 		\
	fi;					\
	mkdir $(BUILD) 755;

cell-logger: $(OBJECTS)
	$(CC) -o $(BUILD)/cell-logger $(OBJECTS)

$(OBJDIR)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $<

clean:
	rm $(OBJDIR)/*.o
