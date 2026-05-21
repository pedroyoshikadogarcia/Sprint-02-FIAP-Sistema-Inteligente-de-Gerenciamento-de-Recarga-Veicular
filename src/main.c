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