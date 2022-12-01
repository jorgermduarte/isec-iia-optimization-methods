
# Objetivo 

Conceber, implementar e testar métodos de otimização que encontrem soluções de boa
qualidade para diferentes instâncias do problema a seguir descrito

# Problema

Dado um grafo e um valor inteiro k, o Maximum Edge Subgraph Problem consiste em
encontrar um subconjunto de k-vértices tal que o número de arestas dentro do
subconjunto seja máximo.

Formalmente o problema é definido:

- Dados
    - um grafo não direcionado G = (V, A), composto por um conjunto V de
vértices ligados entre si por arestas A
    - um inteiro k

- Problema:
    - encontrar um subconjunto de vértices S, de tamanho k, tal que S ⊆ V, de
forma a maximizar o número de arestas desse subconjunto
    - o objetivo deste problema é, portanto, de maximização.

# Implementações a realizar sobre o problema

- 3 Métodos
    - [ ] 1. Trepa colinas
    - [ ] 2. Algoritmo evolutivo
    - [ ] 3. Método hibrido ( combinação de 1. e 2.)
- Após a implementação dos 3 métodos, deve ser efetuado um estudo comparativo aprofundado sobre o desempenho da otimização.
    - deve ser analizado os seguintes fatores:
        - [ ] no algoritmo local:
            - [ ] número de iterações
            - [ ] vizinhanças
            - [ ] aceitar soluções de custo igual
        - [ ] no algoritmo evolutivo:
            - [ ] tamanho da população
            - [ ] operadores d e recombinação / mutação
            - [ ] probabilidades do operador de recombinação / mutação
            - [ ] métodos de seleção
        - [ ] na  abordagem hibrida:
            - [ ] melhor pesquisa local + melhor evolutivo
            - [ ] comparar as duas abordagens hibridas
            - [ ] apresentar uma tabela de comparação com os resultados obtidos por:
                - [ ] melhor pesquisa local
                - [ ] melhor evolutivo
                - [ ] abordagem hibrida 1
                - [ ] abordagem hibrida 2

As experiências devem ser repetidas pelo menos **100** vezes e as conclusões do estudo
devem ser baseadas na comparação dos valores médios