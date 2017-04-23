CC = gcc

pipeline: simulate.o memory.o regfile.o piperpt.o
	$(CC) -o $@ simulate.o memory.o regfile.o piperpt.o
simulate.o: simulate.c piperpt.c piperpt.h memory.c memory.h regfile.c regfile.h
	$(CC) -c -g simulate.c
memory.o: memory.c memory.h regfile.c regfile.h
	$(CC) -c -g memory.c
regfile.o: regfile.c regfile.h memory.c memory.h
	$(CC) -c -g regfile.c
piperpt.o: piperpt.c piperpt.h regfile.c regfile.h
	$(CC) -c -g piperpt.c

clean:
	rm -f simulate.o memory.o regfile.o piperpt.o
