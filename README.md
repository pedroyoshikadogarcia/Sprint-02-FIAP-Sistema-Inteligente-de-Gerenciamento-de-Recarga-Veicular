# ChargeGrid Intelligence - Sprint 3

Sistema de Gerenciamento de Estação de Recarga para Veículos Elétricos desenvolvido em C. O projeto evoluiu de um simulador isolado para uma plataforma completa de gerenciamento capaz de registrar, organizar, pesquisar, ordenar e analisar estatisticamente múltiplas sessões de recarga em memória.

---

**Novas Funcionalidades Implementadas (Sprint 3)**

1. **Estrutura Heterogênea de Dados (`struct` + `typedef`):** Modelagem completa da entidade `Sessao` agrupando ID, modelo do veículo, consumo (kWh), tempo (minutos), custo total (R$) e estado operacional via enumerador (`StatusSessao`).
2. **Gerenciamento de Múltiplas Sessões em Memória:** Armazenamento vetorial estático (`Sessao sessoes[100]`) com controle dinâmico de posições ocupadas (`totalSessoes`).
3. **Mapeamento de Algoritmos de Busca:**
   * **Busca Linear:** Consulta sequencial por ID em vetores desordenados.
   * **Busca Binária:** Algoritmo de divisão e conquista por ID com ordenação prévia garantida.
4. **Algoritmo de Ordenação Autoral (Insertion Sort):** Implementação manual *in-place* (sem uso de `qsort` ou bibliotecas externas) parametrizada para 4 critérios: ID, Energia Consumida, Custo Total e Tempo de Recarga.
5. **Módulo de Estatísticas e Auditoria:** Motor de cálculo que consolida o total de sessões, energia distribuída, faturamento bruto, ticket médio e picos de consumo (maior e menor kWh).
6. **Robustez de Entrada e Tratamento do Buffer:** Tratamento preventivo em todas as leituras (`scanf`) para evitar *loops* infinitos por entradas inválidas ou caracteres acidentais.

---

**Estrutura do Repositório**

```text
chargegrid-intelligence/
├── src/
│   └── main.c                                 # Código-fonte principal em C
├── docs/
│   └── documento_tecnico_sprint3_toshio_v2.pdf # Documentação técnica e análise Big-O
└── README.md                                  # Guia do projeto
```

---

**Como Compilar e Executar**

Certifique-se de ter um compilador GCC instalado em sua máquina.

1. **Clonar o repositório:**
   ```bash
   git clone [https://github.com/pedroyoshikadogarcia/Sprint-03-FIAP-Sistema-de-Gerenciamento-de-Recarga.git](https://github.com/pedroyoshikadogarcia/Sprint-03-FIAP-Sistema-de-Gerenciamento-de-Recarga.git)
   cd Sprint-03-FIAP-Sistema-de-Gerenciamento-de-Recarga
    ```

2. **Compilar o projeto:**
   ```bash
   gcc -Wall src/main.c -o chargegrid
   ```

3. **Executar a aplicação:**
   - Windows:
     ```dos
      chargegrid.exe
     ```
   - Linux/Mac
     ```bash
     ./chargegrid
     ```

---

  **Análise Teórica de Algoritmos e Complexidade (Big-O)**

- **Insertion Sort:**
  - Melhor Caso: $O(n)$ — Vetor já ordenado.
  - Pior Caso: $O(n^2)$ — Vetor em ordem invertida. Ocorre pela execução de dois laços aninhados resultando na soma $1 + 2 + \dots + (n-1) = \frac{n(n-1)}{2}$ operações.

- **Busca Binária:**
  - Melhor Caso: $O(1)$ — Elemento encontrado na primeira divisão central.
  - Pior Caso (Algoritmo Isolado): $O(\log n)$ — Redução sucessiva do espaço de busca pela metade ($\frac{n}{2^k} = 1$).
  - Operação Integrada no Sistema: $O(n^2)$ — Devido ao encadeamento da ordenação prévia obrigatória (insertionSort) antes da busca.

---

**Informações Acadêmicas**

- **Instituição: FIAP** — Faculdade de Informática e Administração Paulista
- **Curso:** Ciência da Computação
- **Matéria:** Data Structures and Algorithms
- **Professor:** Erick Toshio Yamamoto

**Grupo 03**
