TARGET = iop

all:$(TARGET)

UTL_func.o:UTL_func.c UTL_func.h
	gcc -Wall -c -o $@ $<

iop:iop.c UTL_func.o
	gcc -Wall -o $@ $^ -g

clean:
	rm -rf $(TARGET)

