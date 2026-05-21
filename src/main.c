#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SESSOES 5
#define POTENCIA_MAXIMA_GRID 22.0
#define TARIFA_BASE 0.85

typedef enum {
    DISPONIVEL,
    CARREGANDO,
    AGUARDANDO_GRID, 
    CONCLUIDO
} StatusSessao;

typedef struct {
    int id;
    char modeloVeiculo[30];
    float potenciaSolicitada; 
    float potenciaAtual;      
    float energiaConsumida;   
    float custoAtual;         
    StatusSessao status;
} SessaoCarregamento;

SessaoCarregamento postos[MAX_SESSOES];
int horaSimulada = 18; 

void inicializarSistema();
void exibirMenu();
void IniciarNovaSessao();
void monitorarSessao();
void atualizarSistemaeGerenciarPotencia();
void aplicarTarifacaoDinamica(SessaoCarregamento *s);
void simularComunicacaoModbus(SessaoCarregamento s);
void gerarRelatorio();

int main() {
    inicializarSistema();
    int opcao;

    do {
        exibirMenu();
        printf("\nEscolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida!\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (opcao) {
            case 1: IniciarNovaSessao(); break;
            case 2: atualizarSistemaeGerenciarPotencia(); break;
            case 3: gerarRelatorio(); break;
            case 4: 
                horaSimulada = (horaSimulada + 1) % 24;
                printf("[INFO] Hora avancada para: %02dh:00\n", horaSimulada);
                atualizarSistemaeGerenciarPotencia();
                break;
            case 0: printf("Encerrando ChargeGrid Intelligence...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

void inicializarSistema() {
    for (int i = 0; i < MAX_SESSOES; i++) {
        postos[i].id = i + 1;
        postos[i].status = DISPONIVEL;
        postos[i].energiaConsumida = 0.0;
        postos[i].custoAtual = 0.0;
        postos[i].potenciaAtual = 0.0;
    }
}

void exibirMenu() {
    printf("\n----------------------------------------------------\n");
    printf("   CHARGEGRID INTELLIGENCE - SPRINT 2 (FIAP 2026)     \n");
    printf("------------------------------------------------------\n");
    printf(" Hora Atual do Sistema: %02dh:00 | Limite Grid: %.1f kW\n", horaSimulada, POTENCIA_MAXIMA_GRID);
    printf("------------------------------------------------------\n");
    for (int i = 0; i < MAX_SESSOES; i++) {
        char statusStr[20];
        switch (postos[i].status) {
            case DISPONIVEL: strcpy(statusStr, "DISPONIVEL"); break;
            case CARREGANDO: strcpy(statusStr, "CARREGANDO"); break;
            case AGUARDANDO_GRID: strcpy(statusStr, "AGUARDANDO GRID"); break;
            case CONCLUIDO: strcpy(statusStr, "CONCLUIDO"); break;
        }
        printf(" Posto %d [%s] ", postos[i].id, statusStr);
        if (postos[i].status == CARREGANDO || postos[i].status == AGUARDANDO_GRID) {
            printf("- %s (Alocado: %.1f/%.1f kW)", postos[i].modeloVeiculo, postos[i].potenciaAtual, postos[i].potenciaSolicitada);
        }
        printf("\n");
    }
    printf("------------------------------------------------------\n");
    printf("1. Conectar Veiculo (Nova Sessao)\n");
    printf("2. Simular Passo de Tempo (Simula recarga e envia MODBUS)\n");
    printf("3. Gerar Relatorio de Auditoria\n");
    printf("4. Avancar Hora (+1h - Altera Tarifacao)\n");
    printf("0. Sair\n");
}

void IniciarNovaSessao() {
    int idx = -1;
    for (int i = 0; i < MAX_SESSOES; i++) {
        if (postos[i].status == DISPONIVEL) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("\n[ALERTA] Todos os postos estao ocupados no momento!\n");
        return;
    }

    printf("\n--- Nova Sessao de Recarga (Posto %d) ---\n", postos[idx].id);
    printf("Modelo do Veiculo (ex: Compass XE, BYD): ");
    scanf(" %[^\n]", postos[idx].modeloVeiculo);
    
    printf("Potencia Maxima do Carregador do Carro (kW) [Ex: 7.4 ou 22.0]: ");
    scanf("%f", &postos[idx].potenciaSolicitada);

    postos[idx].status = CARREGANDO;
    postos[idx].energiaConsumida = 0.0;
    postos[idx].custoAtual = 0.0;

    printf("\n[SUCESSO] Veiculo %s conectado com sucesso.\n", postos[idx].modeloVeiculo);
    
    atualizarSistemaeGerenciarPotencia();
}

void atualizarSistemaeGerenciarPotencia() {
    int veiculosAtivos = 0;
    float demandaTotalSolicitada = 0.0;

    for (int i = 0; i < MAX_SESSOES; i++) {
        if (postos[i].status == CARREGANDO || postos[i].status == AGUARDANDO_GRID) {
            veiculosAtivos++;
            demandaTotalSolicitada += postos[i].potenciaSolicitada;
        }
    }

    printf("\n[SMART CHARGING] Analisando Grid. Ativos: %d | Solicitado: %.2f kW / Max: %.2f kW\n", 
            veiculosAtivos, demandaTotalSolicitada, POTENCIA_MAXIMA_GRID);

    if (veiculosAtivos == 0) return;

    if (demandaTotalSolicitada > POTENCIA_MAXIMA_GRID) {
        float potenciaPorPosto = POTENCIA_MAXIMA_GRID / veiculosAtivos;
        printf("[ALERTA] Sobrecarga detectada! Aplicando Peak Shaving. Limitando postos a %.2f kW.\n", potenciaPorPosto);
        
        for (int i = 0; i < MAX_SESSOES; i++) {
            if (postos[i].status == CARREGANDO || postos[i].status == AGUARDANDO_GRID) {
                postos[i].status = CARREGANDO;
                postos[i].potenciaAtual = (postos[i].potenciaSolicitada < potenciaPorPosto) ? postos[i].potenciaSolicitada : potenciaPorPosto;
            }
        }
    } else {
        for (int i = 0; i < MAX_SESSOES; i++) {
            if (postos[i].status == CARREGANDO || postos[i].status == AGUARDANDO_GRID) {
                postos[i].status = CARREGANDO;
                postos[i].potenciaAtual = postos[i].potenciaSolicitada;
            }
        }
    }

    for (int i = 0; i < MAX_SESSOES; i++) {
        if (postos[i].status == CARREGANDO) {
            postos[i].energiaConsumida += (postos[i].potenciaAtual * 0.5); 
            aplicarTarifacaoDinamica(&postos[i]);
            
            simularComunicacaoModbus(postos[i]);

            if (postos[i].energiaConsumida >= 60.0) {
                postos[i].status = CONCLUIDO;
                postos[i].potenciaAtual = 0.0;
                printf("[NOTIFICACAO] Posto %d: Recarga do %s concluida!\n", postos[i].id, postos[i].modeloVeiculo);
            }
        }
    }
}

void aplicarTarifacaoDinamica(SessaoCarregamento *s) {
    float fatorMultiplicador = 1.0;

    if (horaSimulada >= 18 && horaSimulada <= 21) {
        fatorMultiplicador += 0.40;
    }

    int ativos = 0;
    for (int i = 0; i < MAX_SESSOES; i++) {
        if (postos[i].status == CARREGANDO) ativos++;
    }
    if (ativos >= 3) {
        fatorMultiplicador += 0.15;
    }

    float tarifaCalculada = TARIFA_BASE * fatorMultiplicador;
    s->custoAtual = s->energiaConsumida * tarifaCalculada;
}

void simularComunicacaoModbus(SessaoCarregamento s) {
    int reg_status = 30005; 
    int reg_potencia = 30012; 
    
    printf("\n  >> [MODBUS TX] Enviando Frame para Servidor Backend (Formato Hex/RTU):\n");
    printf("     [FRAME]: 01 03 %02X %02X 00 02 CA B3\n", (reg_potencia >> 8) & 0xFF, reg_potencia & 0xFF);
    printf("  << [MODBUS RX] Posto %d | Reg %d (Status=%d) | Reg %d (Potencia=%.1f kW) | Consumo: %.2f kWh\n", 
            s.id, reg_status, s.status, reg_potencia, s.potenciaAtual, s.energiaConsumida);
}

void gerarRelatorio() {
    printf("\n----------------------------------------------------\n");
    printf("          RELATORIO DE AUDITORIA E LOGS (FATE/FIAP)   \n");
    printf("------------------------------------------------------\n");
    float faturamentoTotal = 0.0;
    float energiaTotalDispensada = 0.0;

    for (int i = 0; i < MAX_SESSOES; i++) {
        if (postos[i].status != DISPONIVEL) {
            printf(" Posto %d | Veiculo: %-15s | Consumo: %5.2f kWh | Total: R$ %6.2f\n",
                    postos[i].id, postos[i].modeloVeiculo, postos[i].energiaConsumida, postos[i].custoAtual);
            faturamentoTotal += postos[i].custoAtual;
            energiaTotalDispensada += postos[i].energiaConsumida;
        }
    }