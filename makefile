clean:
	$(RM) hill_climbing
	$(RM) evolutionary

all:
	make hill_climbing
	make evolutionary

hill_climbing:
	gcc -o hill_climbing algorithms/hill-climbing.c utils/utils.c -lm


evolutionary:
	gcc -o evolutionary algorithms/evolutionary.c utils/utils.c -lm