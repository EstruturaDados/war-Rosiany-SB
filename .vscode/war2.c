#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Protótipos das funções
struct Territorio* cadastrarTerritorios(int quantidade);
void exibirTerritorios(struct Territorio* territorios, int quantidade);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);
int escolherTerritorio(struct Territorio* territorios, int quantidade, const char* corJogador, const char* tipo);

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    
    int quantidade;
    printf("=== SISTEMA WAR ESTRUTURADO ===\n\n");
    
    // Solicita o número de territórios e aloca memória
    printf("Digite o numero de territorios a serem cadastrados: ");
    scanf("%d", &quantidade);
    
    struct Territorio* territorios = cadastrarTerritorios(quantidade);
    
    if (territorios == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1;
    }
    
    // Exibe os territórios cadastrados
    exibirTerritorios(territorios, quantidade);
    
    // Realiza um ataque de exemplo
    printf("\n=== SIMULACAO DE ATAQUE ===\n");
    char corJogador[10];
    printf("Digite a cor do jogador atacante: ");
    scanf("%9s", corJogador);
    
    // Escolher território atacante
    int idxAtacante = escolherTerritorio(territorios, quantidade, corJogador, "atacante");
    if (idxAtacante == -1) {
        printf("Nenhum territorio valido para ataque!\n");
        liberarMemoria(territorios);
        return 1;
    }
    
    // Escolher território defensor
    int idxDefensor = escolherTerritorio(territorios, quantidade, corJogador, "defensor");
    if (idxDefensor == -1) {
        printf("Nenhum territorio valido para defesa!\n");
        liberarMemoria(territorios);
        return 1;
    }
    
    // Verifica se não está atacando próprio território
    if (strcmp(territorios[idxAtacante].cor, territorios[idxDefensor].cor) == 0) {
        printf("Erro: Nao pode atacar um territorio da mesma cor!\n");
        liberarMemoria(territorios);
        return 1;
    }
    
    // Realiza o ataque
    atacar(&territorios[idxAtacante], &territorios[idxDefensor]);
    
    // Exibe situação final
    printf("\n=== SITUACAO FINAL DOS TERRITORIOS ===\n");
    exibirTerritorios(territorios, quantidade);
    
    // Libera a memória alocada
    liberarMemoria(territorios);
    printf("\nMemoria liberada. Programa encerrado.\n");
    
    return 0;
}

// Função para cadastrar territórios com alocação dinâmica
struct Territorio* cadastrarTerritorios(int quantidade) {
    // Aloca memória para o vetor de territórios usando calloc
    struct Territorio* territorios = (struct Territorio*)calloc(quantidade, sizeof(struct Territorio));
    
    if (territorios == NULL) {
        return NULL;
    }
    
    printf("\n=== CADASTRO DE %d TERRITORIOS ===\n", quantidade);
    
    for(int i = 0; i < quantidade; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);
        printf("Nome: ");
        scanf("%29s", territorios[i].nome);
        printf("Cor do exercito: ");
        scanf("%9s", territorios[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }
    
    return territorios;
}

// Função para exibir todos os territórios
void exibirTerritorios(struct Territorio* territorios, int quantidade) {
    printf("\n=== MAPA DE TERRITORIOS ===\n");
    for(int i = 0; i < quantidade; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
    }
    printf("------------------------\n");
}

// Função para simular um ataque entre territórios usando ponteiros
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    printf("\n=== INICIANDO ATAQUE ===\n");
    printf("ATACANTE: %s (%s) com %d tropas\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("DEFENSOR: %s (%s) com %d tropas\n", defensor->nome, defensor->cor, defensor->tropas);
    
    // Simula a rolagem de dados (1-6 para cada lado)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("\nDado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);
    
    // Determina o resultado do ataque
    if (dadoAtacante > dadoDefensor) {
        printf("\n>>> VITORIA DO ATACANTE! <<<\n");
        
        // Atacante vence: defensor muda de dono e recebe metade das tropas
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= atacante->tropas / 2;
        
        printf("O territorio %s agora pertence a %s!\n", defensor->nome, atacante->cor);
        printf("Novas tropas em %s: %d\n", defensor->nome, defensor->tropas);
        printf("Tropas restantes em %s: %d\n", atacante->nome, atacante->tropas);
        
    } else {
        printf("\n>>> VITORIA DO DEFENSOR! <<<\n");
        
        // Defensor vence: atacante perde uma tropa
        atacante->tropas -= 1;
        printf("O atacante perdeu uma tropa!\n");
        printf("Tropas restantes em %s: %d\n", atacante->nome, atacante->tropas);
        printf("Tropas restantes em %s: %d\n", defensor->nome, defensor->tropas);
    }
}

// Função para auxiliar na escolha de territórios
int escolherTerritorio(struct Territorio* territorios, int quantidade, const char* corJogador, const char* tipo) {
    printf("\n=== Escolha o territorio %s ===\n", tipo);
    
    // Mostra territórios disponíveis
    for(int i = 0; i < quantidade; i++) {
        if ((strcmp(tipo, "atacante") == 0 && strcmp(territorios[i].cor, corJogador) == 0) ||
            (strcmp(tipo, "defensor") == 0 && strcmp(territorios[i].cor, corJogador) != 0)) {
            printf("%d - %s (%s) - %d tropas\n", 
                   i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
        }
    }
    
    int escolha;
    printf("Digite o numero do territorio: ");
    scanf("%d", &escolha);
    
    // Valida a escolha
    if (escolha < 1 || escolha > quantidade) {
        printf("Escolha invalida!\n");
        return -1;
    }
    
    int index = escolha - 1;
    
    // Verifica se o território é válido para o tipo escolhido
    if ((strcmp(tipo, "atacante") == 0 && strcmp(territorios[index].cor, corJogador) != 0) ||
        (strcmp(tipo, "defensor") == 0 && strcmp(territorios[index].cor, corJogador) == 0)) {
        printf("Territorio invalido para %s!\n", tipo);
        return -1;
    }
    
    return index;
}

// Função para liberar a memória alocada
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}