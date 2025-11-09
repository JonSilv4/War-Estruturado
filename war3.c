#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_STRING 100

// Estrutura dos territórios
struct Territorio {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
};

// Função para limpar o buffer
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Leitura segura de inteiros dentro de intervalo
int lerInteiroIntervalo(const char *mensagem, int max) {
    int valor;
    do {
        printf("%s", mensagem);
        scanf("%d", &valor);
        limparBufferEntrada();
        if (valor < 1 || valor > max)
            printf("Valor inválido! Digite um número entre 1 e %d.\n", max);
    } while (valor < 1 || valor > max);
    return valor;
}

// Cadastrar territórios (passagem por referência)
void cadastrarTerritorios(struct Territorio *mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, TAM_STRING, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].cor, TAM_STRING, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

// Exibir territórios (passagem por valor, apenas leitura)
void listarTerritorios(const struct Territorio *mapa, int qtd) {
    printf("\n===== LISTA DE TERRITÓRIOS =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("\nTERRITÓRIO %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

// Sistema de ataque (usa ponteiros = passagem por referência)
void atacar(struct Territorio *mapa, int qtd) {
    char continuar = 's';

    while (continuar == 's' || continuar == 'S') {
        listarTerritorios(mapa, qtd);

        int atacante = lerInteiroIntervalo("\nNúmero do território ATACANTE: ", qtd);
        int defensor = lerInteiroIntervalo("Número do território DEFENSOR: ", qtd);

        if (atacante == defensor) {
            printf("\nUm território não pode atacar a si mesmo!\n");
        } else {
            struct Territorio *ptrAtacante = &mapa[atacante - 1];
            struct Territorio *ptrDefensor = &mapa[defensor - 1];

            if (ptrAtacante->tropas <= 0) {
                printf("\n%s não tem tropas para atacar!\n", ptrAtacante->nome);
            } else {
                int dadoAtacante = (rand() % 6) + 1;
                int dadoDefensor = (rand() % 6) + 1;

                printf("\nRolagem de dados:\n");
                printf("%s (atacante) rolou: %d\n", ptrAtacante->nome, dadoAtacante);
                printf("%s (defensor) rolou: %d\n", ptrDefensor->nome, dadoDefensor);

                ptrDefensor->tropas -= dadoAtacante;
                if (ptrDefensor->tropas < 0) ptrDefensor->tropas = 0;

                ptrAtacante->tropas -= dadoDefensor;
                if (ptrAtacante->tropas < 0) ptrAtacante->tropas = 0;

                printf("\nResultado do combate:\n");
                printf("%s agora tem %d tropas.\n", ptrAtacante->nome, ptrAtacante->tropas);
                printf("%s agora tem %d tropas.\n", ptrDefensor->nome, ptrDefensor->tropas);

                if (ptrDefensor->tropas == 0) {
                    printf("\n%s foi conquistado por %s!\n", ptrDefensor->nome, ptrAtacante->nome);
                }
            }
        }

        printf("\nDeseja atacar novamente? (s/n): ");
        scanf(" %c", &continuar);
        limparBufferEntrada();
    }
}

// Função para verificar missão (passagem por valor, só leitura)
int checarMissao(const struct Territorio *mapa, int qtd, const char *corAlvo) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(mapa[i].cor, corAlvo) == 0 && mapa[i].tropas > 0)
            return 0; // ainda existe inimigo
    }
    return 1; // missão completa!
}

int main() {
    srand(time(NULL));

    struct Territorio *mapa = NULL;
    int qtdTerritorios = 0;
    int jaAlocado = 0;
    int opcao;

    char corJogador[TAM_STRING];
    char corMissao[TAM_STRING];

    do {
        printf("\n====== JOGO WAR ======\n");
        printf("1 - Cadastrar territórios\n");
        printf("2 - Listar territórios\n");
        printf("3 - Atacar território\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                if (!jaAlocado) {
                    printf("\nQuantos territórios deseja cadastrar? ");
                    scanf("%d", &qtdTerritorios);
                    limparBufferEntrada();

                    mapa = calloc(qtdTerritorios, sizeof(struct Territorio));
                    if (mapa == NULL) {
                        printf("Erro ao alocar memória!\n");
                        exit(1);
                    }

                    cadastrarTerritorios(mapa, qtdTerritorios);

                    printf("\nEscolha a cor do seu exército: ");
                    fgets(corJogador, TAM_STRING, stdin);
                    corJogador[strcspn(corJogador, "\n")] = '\0';

                    printf("Escolha a cor do exército inimigo (sua missão será eliminá-lo): ");
                    fgets(corMissao, TAM_STRING, stdin);
                    corMissao[strcspn(corMissao, "\n")] = '\0';

                    jaAlocado = 1;
                } else {
                    printf("\nOs territórios já foram cadastrados!\n");
                }
                break;

            case 2:
                if (!jaAlocado) printf("\nNenhum território cadastrado ainda!\n");
                else listarTerritorios(mapa, qtdTerritorios);
                break;

            case 3:
                if (!jaAlocado) printf("\nVocê precisa cadastrar os territórios primeiro!\n");
                else {
                    atacar(mapa, qtdTerritorios);

                    if (checarMissao(mapa, qtdTerritorios, corMissao)) {
                        printf("\nMISSÃO CONCLUÍDA! O exército %s foi eliminado!\n", corMissao);
                    } else {
                        printf("\nA missão ainda não foi concluída. Continue lutando!\n");
                    }
                }
                break;

            case 0:
                printf("\nSaindo do jogo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    free(mapa);
    return 0;
}