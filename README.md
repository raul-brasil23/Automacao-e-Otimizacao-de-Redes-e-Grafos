# 🌐 Automação de Roteamento de Redes (Algoritmo de Prim)

Este projeto implementa uma solução automatizada para o problema de roteamento de infraestrutura em redes complexas (fibra óptica), determinando a Árvore Geradora Mínima (AGM) para conectar múltiplos servidores com o menor custo possível de propagação.

## 🎯 O Problema e o Foco em Dados
Em cenários de infraestrutura de TI e Data Centers, conectar dezenas de unidades produtivas exige a análise de grandes volumes de dados de topologia de rede. Calcular a rota mais eficiente manualmente é inviável e sujeito a erros. 

Este projeto automatiza essa tomada de decisão através da leitura e processamento de dados topológicos (`.txt`), aplicando validações rigorosas de integridade antes da execução do roteamento.

## ⚙️ Arquitetura e Tecnologias
O sistema foi desenvolvido em **C** com forte ênfase em performance, modularização e gestão de memória. 

* **Algoritmo Guloso:** Implementação do Algoritmo de Prim.
* **Estrutura de Dados Avançada:** Fila de prioridade utilizando **Heap Binário Mínimo** para garantir eficiência $O(E \log V)$ na extração do menor custo.
* **Modularidade e Interface Opaca:** O código central (`ep1.c`) interage com os grafos através de uma interface genérica, permitindo que a base de dados subjacente mude dinamicamente entre **Lista de Adjacência** ou **Matriz de Adjacência** em tempo de compilação, sem alterar a lógica de negócios.
* **Pipeline de Validação de Dados:** Antes de qualquer roteamento, os dados de entrada passam por uma esteira de validação crítica, identificando e isolando dados corrompidos (vértices inválidos, auto-laços, arestas paralelas e pesos físicos impossíveis).

## 🚀 Como Executar

O projeto utiliza diretivas de compilação (`#ifdef`) para alternar a estrutura de dados em memória.

**Compilando com Lista de Adjacência:**
```bash
gcc -o ep1 ep1.c -lm
```

**Compilando com Matriz de Adjacência:**
```bash
gcc -DMATRIZ -o ep1 ep1.c -lm
```

**Executando a Automação:**
```bash
./ep1 entrada.txt saida.txt
```

## 🧪 Testes
Para garantir a integridade da solução, o repositório conta com uma série de testes de borda (edge cases) na pasta `/testes`. Eles validam a resiliência do algoritmo contra:
* Vértices inexistentes ou corrompidos.
* Pesos físicos impossíveis (negativos ou nulos).
* Grafos desconexos (ilhas de infraestrutura).
* Arestas paralelas.
* Auto-laços.