FLAGS= -lm -O3

all: bin out evaluador 2optls nn2nodes nnpath

nn2nodes: progs/nn-to-nodes.c lib/*.c 
	gcc -lm progs/nn-to-nodes.c  lib/*.c -o bin/nn-to-nodes $(FLAGS) 

evaluador: progs/eval-sol.c lib/*.c 
	gcc  $(FLAGS) progs/eval-sol.c  lib/*.c -o bin/eval-sol  $(FLAGS) 


2optls: progs/2-opt-ls.c lib/*.c 
	gcc progs/2-opt-ls.c  lib/*.c -o bin/2opt-ls $(FLAGS)

nnpath: progs/nn-path.c lib/*.c 
	gcc progs/nn-path.c lib/*.c -o bin/nn-path $(FLAGS)

out: 
	mkdir -p out 


bin: 
	mkdir -p bin 
