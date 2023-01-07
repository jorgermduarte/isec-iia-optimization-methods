clean:
	$(RM) hill_climbing
	$(RM) evolutionary
	$(RM) hybrid_algorithm
	$(RM) hybrid_algorithm2

all:
	make hill_climbing
	make evolutionary
	make hybrid_algorithm
	make hybrid_algorithm2

hill_climbing:
	gcc -o hill_climbing algorithms/hill-climbing.c utils/utils.c -lm

hybrid_algorithm:
	gcc -o hybrid_algorithm algorithms/hybrid.c utils/utils.c -lm

evolutionary:
	gcc -o evolutionary algorithms/evolutionary.c utils/utils.c -lm

hybrid_algorithm2:
	gcc -o hybrid_algorithm2 algorithms/hybrid2.c utils/utils.c -lm