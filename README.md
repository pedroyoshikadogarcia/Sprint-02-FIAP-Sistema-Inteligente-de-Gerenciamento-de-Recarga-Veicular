# ChargeGrid Intelligence - Sprint 2

Sistema Inteligente de Gerenciamento de Recarga para Veículos Elétricos desenvolvido em C. O projeto simula o ecossistema de gerenciamento de múltiplos eletropostos com foco em eficiência energética, tarifação dinâmica e comunicação baseada em protocolos industriais.

---

**Funcionalidades Implementadas** (Critérios de Avaliação)

1. **Gerenciamento de Múltiplas Sessões:** Suporte para até 5 postos de carregamento simultâneos controlados via estruturas (`structs`) dinâmicas em memória.
2. **Controle de Demanda Inteligente (Smart Charging):** Algoritmo de *Peak Shaving* que monitora a carga total do grid (limite de 22.0 kW) e redistribui a potência proporcionalmente em caso de sobrecarga.
3. **Tarifação Dinâmica:** Motor de cálculo que aplica acréscimos baseados no Horário de Pico (18h às 21h) e por densidade de alta demanda (3 ou mais VEs conectados).
4. **Simulação Modbus RTU:** Geração de logs de pacotes e frames hexadecimais simulando a leitura e escrita nos registradores reais baseados no mapa de comunicação do carregador *GoodWe HCA G2*.

---

**Estrutura de Pasta**
```
chargegrid-intelligence/
├── src/
│   └── main.c
├── docs/
│   ├── documento_tecnico.md 
│   └── modbus_mapping.pdf
└── README.md
```

---

**Como Compilar e Executar**

Certifique-se de ter um compilador GCC instalado em sua máquina.

### Clonando o repositório:
```bash
git clone [https://github.com/SEU_USUARIO/chargegrid-intelligence.git](https://github.com/SEU_USUARIO/chargegrid-intelligence.git)
cd chargegrid-intelligence
