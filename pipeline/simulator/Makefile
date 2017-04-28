CC = gcc

pipeline: simulate.o piperpt.o memory.o regfile.o pipereg.o pipeline.o instruction.o
	$(CC) -o $@ simulate.o piperpt.o memory.o regfile.o pipereg.o pipeline.o instruction.o
simulate.o: simulate.c piperpt.c piperpt.h memory.c memory.h regfile.c regfile.h pipereg.c pipereg.h pipeline.c pipeline.h
	$(CC) -c -g simulate.c
piperpt.o: piperpt.c piperpt.h regfile.c regfile.h pipereg.c pipereg.h
	$(CC) -c -g piperpt.c
memory.o: memory.c memory.h regfile.c regfile.h pipereg.c pipereg.h
	$(CC) -c -g memory.c
regfile.o: regfile.c regfile.h memory.c memory.h
	$(CC) -c -g regfile.c
instruction.o: instruction.c instruction.h memory.c memory.h
	$(CC) -c -g instruction.c
pipereg.o: pipereg.c pipereg.h instruction.c instruction.h regfile.c regfile.h
	$(CC) -c -g pipereg.c
pipeline.o: pipeline.c pipeline.h piperpt.c piperpt.h memory.c memory.h regfile.c regfile.h
	$(CC) -c -g pipeline.c

clean:
	rm -f simulate.o piperpt.o memory.o regfile.o instruction.o pipereg.o pipeline.o
