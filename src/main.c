#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SESSOES 100
#define TARIFA_BASE 0.85

typedef enum {
    DISPONIVEL,
    CARREGANDO,
    CONCLUIDO
} StatusSessao;

typedef struct {
    int id;
    char modeloVeiculo[30];
    float energiaConsumida; // kWh
    float tempoRecarga;     // Minutos
    float custoTotal;       // R$
    StatusSessao status;
} Sessao;

void exibirMenu();
void cadastrarSessao(Sessao sessoes[], int *total);
void listarSessoes(const Sessao sessoes[], int total);
void buscarSessao(Sessao sessoes[], int total);
int buscaLinearPorID(const Sessao sessoes[], int total, int idBusca);
int buscaBinariaPorID(Sessao sessoes[], int total, int idBusca);
void ordenarSessoes(Sessao sessoes[], int total);
void insertionSort(Sessao sessoes[], int total, int criterio);
void mostrarEstatisticas(const Sessao sessoes[], int total);
const char* obterStatus(StatusSessao status);
void limparBuffer();

int main() {
    Sessao sessoes[MAX_SESSOES];
    int totalSessoes = 0;
    int opcao;

    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Entrada invalida! Digite apenas numeros.\n");
            limparBuffer();
            continue;
        }

        switch (opcao) {
            case 1: cadastrarSessao(sessoes, &totalSessoes); break;
            case 2: listarSessoes(sessoes, totalSessoes); break;
            case 3: buscarSessao(sessoes, totalSessoes); break;
            case 4: ordenarSessoes(sessoes, totalSessoes); break;
            case 5: mostrarEstatisticas(sessoes, totalSessoes); break;
            case 6: printf("\nEncerrando o ChargeGrid Management System...\n"); break;
            default: printf("\n[ERRO] Opcao invalida!\n");
        }
    } while (opcao != 6);

    return 0;
}

void limparBuffer() {
    while (getchar() != '\n');
}

const char* obterStatus(StatusSessao status) {
    switch (status) {
        case DISPONIVEL: return "DISPONIVEL";
        case CARREGANDO: return "CARREGANDO";
        case CONCLUIDO:  return "CONCLUIDO";
        default:         return "DESCONHECIDO";
    }
}

void exibirMenu() {
    printf("\n=====================================\n");
    printf("   CHARGEGRID - ESTACAO DE RECARGA   \n");
    printf("=====================================\n");
    printf("1 - Nova sessao de recarga\n");
    printf("2 - Listar sessoes\n");
    printf("3 - Buscar sessao\n");
    printf("4 - Ordenar sessoes\n");
    printf("5 - Estatisticas\n");
    printf("6 - Encerrar\n");
    printf("=====================================\n");
}

void cadastrarSessao(Sessao sessoes[], int *total) {
    if (*total >= MAX_SESSOES) {
        printf("\n[ERRO] Limite maximo de sessoes atingido (%d)!\n", MAX_SESSOES);
        return;
    }

    Sessao nova;
    nova.id = *total + 1;

    printf("\n--- Nova Sessao de Recarga (ID: %d) ---\n", nova.id);
    printf("Modelo do Veiculo: ");
    limparBuffer();
    scanf(" %[^\n]", nova.modeloVeiculo);

    printf("Energia Consumida (kWh): ");
    while (scanf("%f", &nova.energiaConsumida) != 1 || nova.energiaConsumida <= 0) {
        printf("[ERRO] Valor invalido! Digite a energia consumida em kWh (numero positivo): ");
        limparBuffer();
    }

    printf("Tempo de Recarga (minutos): ");
    while (scanf("%f", &nova.tempoRecarga) != 1 || nova.tempoRecarga <= 0) {
        printf("[ERRO] Valor invalido! Digite o tempo em minutos (numero positivo): ");
        limparBuffer();
    }

    nova.custoTotal = nova.energiaConsumida * TARIFA_BASE;
    nova.status = CONCLUIDO;

    sessoes[*total] = nova;
    (*total)++;

    printf("\n[SUCESSO] Sessao ID %d cadastrada com sucesso!\n", nova.id);
}

void listarSessoes(const Sessao sessoes[], int total) {
    if (total == 0) {
        printf("\n[INFO] Nenhuma sessao registrada ate o momento.\n");
        return;
    }

    printf("\n----------------------------------------------------------------------------------\n");
    printf("ID  | Modelo             | Energia (kWh) | Tempo (min) | Custo (R$) | Status\n");
    printf("----------------------------------------------------------------------------------\n");
    for (int i = 0; i < total; i++) {
        printf("%-3d | %-18s | %-13.2f | %-11.1f | R$ %-7.2f | %s\n",
               sessoes[i].id, sessoes[i].modeloVeiculo, sessoes[i].energiaConsumida,
               sessoes[i].tempoRecarga, sessoes[i].custoTotal, obterStatus(sessoes[i].status));
    }
    printf("----------------------------------------------------------------------------------\n");
}

int buscaLinearPorID(const Sessao sessoes[], int total, int idBusca) {
    for (int i = 0; i < total; i++) {
        if (sessoes[i].id == idBusca) {
            return i;
        }
    }
    return -1;
}

int buscaBinariaPorID(Sessao sessoes[], int total, int idBusca) {
    insertionSort(sessoes, total, 1);

    int inicio = 0, fim = total - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        if (sessoes[meio].id == idBusca) return meio;
        if (sessoes[meio].id < idBusca) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

void buscarSessao(Sessao sessoes[], int total) {
    if (total == 0) {
        printf("\n[INFO] Nenhuma sessao para buscar.\n");
        return;
    }

    int id, pos, metodo;
    printf("\nDigite o ID da sessao procurada: ");
    while (scanf("%d", &id) != 1) {
        printf("[ERRO] ID invalido! Digite apenas numeros: ");
        limparBuffer();
    }

    printf("Escolha o algoritmo de busca (1- Linear | 2- Binaria): ");
    while (scanf("%d", &metodo) != 1 || (metodo != 1 && metodo != 2)) {
        printf("[ERRO] Opcao invalida! Digite 1 para Linear ou 2 para Binaria: ");
        limparBuffer();
    }

    if (metodo == 2) pos = buscaBinariaPorID(sessoes, total, id);
    else pos = buscaLinearPorID(sessoes, total, id);

    if (pos != -1) {
        printf("\n[ENCONTRADO] Dados da Sessao ID %d:\n", sessoes[pos].id);
        printf("  Modelo: %s\n", sessoes[pos].modeloVeiculo);
        printf("  Energia: %.2f kWh\n", sessoes[pos].energiaConsumida);
        printf("  Tempo: %.1f min\n", sessoes[pos].tempoRecarga);
        printf("  Custo: R$ %.2f\n", sessoes[pos].custoTotal);
        printf("  Status: %s\n", obterStatus(sessoes[pos].status));
    } else {
        printf("\n[ERRO] Sessao com ID %d nao encontrada!\n", id);
    }
}

void insertionSort(Sessao sessoes[], int total, int criterio) {
    for (int i = 1; i < total; i++) {
        Sessao chave = sessoes[i];
        int j = i - 1;

        int condicao = 0;
        while (j >= 0) {
            switch (criterio) {
                case 1: condicao = (sessoes[j].id > chave.id); break;
                case 2: condicao = (sessoes[j].energiaConsumida > chave.energiaConsumida); break;
                case 3: condicao = (sessoes[j].custoTotal > chave.custoTotal); break;
                case 4: condicao = (sessoes[j].tempoRecarga > chave.tempoRecarga); break;
            }

            if (!condicao) break;
            sessoes[j + 1] = sessoes[j];
            j--;
        }
        sessoes[j + 1] = chave;
    }
}

void ordenarSessoes(Sessao sessoes[], int total) {
    if (total < 2) {
        printf("\n[INFO] Registro insuficiente de sessoes para ordenacao.\n");
        return;
    }

    int criterio;
    printf("\nEscolha o criterio de ordenacao (Insertion Sort):\n");
    printf("1 - ID\n2 - Energia Consumida\n3 - Custo Total\n4 - Tempo de Recarga\nOpcao: ");
    while (scanf("%d", &criterio) != 1 || criterio < 1 || criterio > 4) {
        printf("[ERRO] Opcao invalida! Digite um numero de 1 a 4: ");
        limparBuffer();
    }

    insertionSort(sessoes, total, criterio);
    printf("\n[SUCESSO] Sessoes ordenadas com sucesso!\n");
    listarSessoes(sessoes, total);
}

void mostrarEstatisticas(const Sessao sessoes[], int total) {
    if (total == 0) {
        printf("\n[INFO] Nenhuma sessao registrada para gerar estatisticas.\n");
        return;
    }

    float energiaTotal = 0.0, faturamentoTotal = 0.0;
    float maiorConsumo = sessoes[0].energiaConsumida;
    float menorConsumo = sessoes[0].energiaConsumida;

    for (int i = 0; i < total; i++) {
        energiaTotal += sessoes[i].energiaConsumida;
        faturamentoTotal += sessoes[i].custoTotal;

        if (sessoes[i].energiaConsumida > maiorConsumo) maiorConsumo = sessoes[i].energiaConsumida;
        if (sessoes[i].energiaConsumida < menorConsumo) menorConsumo = sessoes[i].energiaConsumida;
    }

    float ticketMedio = faturamentoTotal / total;

    printf("\n========= ESTATISTICAS DE RECARGA =========\n");
    printf("Sessoes realizadas: %d\n", total);
    printf("Energia fornecida : %.2f kWh\n", energiaTotal);
    printf("Faturamento       : R$ %.2f\n", faturamentoTotal);
    printf("Ticket medio      : R$ %.2f\n", ticketMedio);
    printf("Maior consumo     : %.2f kWh\n", maiorConsumo);
    printf("Menor consumo     : %.2f kWh\n", menorConsumo);
    printf("===========================================\n");
}