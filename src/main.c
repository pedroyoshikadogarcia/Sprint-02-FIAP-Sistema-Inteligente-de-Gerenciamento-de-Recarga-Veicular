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