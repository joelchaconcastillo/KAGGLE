FLAGS= -lm -O3

all: bin out evaluador 2optls nn2nodes nnpath

nn2nodes: progs/nn-to-nodes.c lib/*.c 
	gcc $(FLAGS) -lm progs/nn-to-nodes.c  lib/*.c -o bin/nn-to-nodes

evaluador: progs/eval-sol.c lib/*.c 
	gcc  $(FLAGS) progs/eval-sol.c  lib/*.c -o bin/eval-sol  

2optls: progs/2-opt-ls.c lib/*.c 
	gcc $(FLAGS) progs/2-opt-ls.c  lib/*.c -o bin/2opt-ls

nnpath: progs/nn-path.c lib/*.c 
	gcc $(FLAGS) progs/nn-path.c lib/*.c -o bin/nn-path

out: 
	mkdir -p out 


bin: 
	mkdir -p bin 
