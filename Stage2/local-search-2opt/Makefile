all: out evaluador 2optls nn2nodes

nn2nodes: 
	gcc -lm progs/nn-to-nodes.c  lib/*.c -o bin/nn-to-nodes

evaluador: 
	gcc -lm progs/eval-sol.c  lib/*.c -o bin/eval-sol  

2optls:
	gcc -lm progs/2-opt-ls.c  lib/*.c -o bin/2opt-ls

out: 
	mkdir -p out 

