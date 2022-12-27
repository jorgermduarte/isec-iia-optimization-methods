clean:
	$(RM) breadth-first-s
	$(RM) depth-first-s

hill_climbing:
	gcc -o hill_climbing algorithms/hill-climbing.c utils/utils.c -lm