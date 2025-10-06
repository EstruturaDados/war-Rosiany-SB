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

// Novas funções para o sistema de missões
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, const char* corJogador);
void exibirMissao(char* missao, const char* corJogador);
void liberarMissao(char* missao);

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    
    int quantidade;
    printf("=== SISTEMA WAR ESTRUTURADO COM MISSOES ===\n\n");
    
    // Solicita o número de territórios e aloca memória
    printf("Digite o numero de territorios a serem cadastrados: ");
    scanf("%d", &quantidade);
    
    struct Territorio* territorios = cadastrarTerritorios(quantidade);
    
    if (territorios == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1;
    }
    
    // Vetor de missões estratégicas
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar pelo menos 50%% dos territorios",
        "Destruir 5 tropas inimigas em um unico turno", 
        "Manter 10 tropas em seu territorio principal"
    };
    int totalMissoes = 5;
    
    // Cadastro dos jogadores e atribuição de missões
    char corJogador1[10], corJogador2[10];
    printf("\n=== CADASTRO DE JOGADORES ===\n");
    printf("Digite a cor do jogador 1: ");
    scanf("%9s", corJogador1);
    printf("Digite a cor do jogador 2: ");
    scanf("%9s", corJogador2);
    
    // Aloca e atribui missões dinamicamente
    char* missaoJogador1 = (char*)malloc(100 * sizeof(char));
    char* missaoJogador2 = (char*)malloc(100 * sizeof(char));
    
    if (missaoJogador1 == NULL || missaoJogador2 == NULL) {
        printf("Erro na alocacao de memoria para missoes!\n");
        liberarMemoria(territorios);
        return 1;
    }
    
    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);
    
    // Exibe as missões iniciais
    printf("\n=== MISSOES ATRIBUÍDAS ===\n");
    exibirMissao(missaoJogador1, corJogador1);
    exibirMissao(missaoJogador2, corJogador2);
    
    // Exibe os territórios cadastrados
    exibirTerritorios(territorios, quantidade);
    
    // Sistema de turnos com verificação de missões
    int turno = 1;
    int jogoAtivo = 1;
    char vencedor[20];
    
    while (jogoAtivo && turno <= 10) { // Máximo de 10 turnos
        printf("\n=== TURNO %d ===\n", turno);
        
        // Turno do Jogador 1
        printf("\n--- Vez do Jogador 1 (%s) ---\n", corJogador1);
        int idxAtacante1 = escolherTerritorio(territorios, quantidade, corJogador1, "atacante");
        if (idxAtacante1 != -1) {
            int idxDefensor1 = escolherTerritorio(territorios, quantidade, corJogador1, "defensor");
            if (idxDefensor1 != -1 && strcmp(territorios[idxAtacante1].cor, territorios[idxDefensor1].cor) != 0) {
                atacar(&territorios[idxAtacante1], &territorios[idxDefensor1]);
                
                // Verifica se Jogador 1 cumpriu a missão
                if (verificarMissao(missaoJogador1, territorios, quantidade, corJogador1)) {
                    strcpy(vencedor, "Jogador 1");
                    jogoAtivo = 0;
                    break;
                }
            }
        }
        
        // Turno do Jogador 2
        printf("\n--- Vez do Jogador 2 (%s) ---\n", corJogador2);
        int idxAtacante2 = escolherTerritorio(territorios, quantidade, corJogador2, "atacante");
        if (idxAtacante2 != -1) {
            int idxDefensor2 = escolherTerritorio(territorios, quantidade, corJogador2, "defensor");
            if (idxDefensor2 != -1 && strcmp(territorios[idxAtacante2].cor, territorios[idxDefensor2].cor) != 0) {
                atacar(&territorios[idxAtacante2], &territorios[idxDefensor2]);
                
                // Verifica se Jogador 2 cumpriu a missão
                if (verificarMissao(missaoJogador2, territorios, quantidade, corJogador2)) {
                    strcpy(vencedor, "Jogador 2");
                    jogoAtivo = 0;
                    break;
                }
            }
        }
        
        // Exibe situação após o turno
        printf("\n=== SITUACAO APOS O TURNO %d ===\n", turno);
        exibirTerritorios(territorios, quantidade);
        
        turno++;
    }
    
    // Final do jogo
    if (jogoAtivo == 0) {
        printf("\n🎉🎉🎉 VITORIA! 🎉🎉🎉\n");
        printf("%s (%s) cumpriu sua missao e venceu o jogo!\n", vencedor, 
               strcmp(vencedor, "Jogador 1") == 0 ? corJogador1 : corJogador2);
    } else {
        printf("\n⏰ Fim dos turnos! Nenhum jogador cumpriu sua missao a tempo.\n");
    }
    
    // Exibe missões finais
    printf("\n=== MISSOES FINAIS ===\n");
    exibirMissao(missaoJogador1, corJogador1);
    exibirMissao(missaoJogador2, corJogador2);
    
    // Libera toda a memória alocada
    liberarMissao(missaoJogador1);
    liberarMissao(missaoJogador2);
    liberarMemoria(territorios);
    
    printf("\nMemoria liberada. Programa encerrado.\n");
    return 0;
}

// Função para atribuir missão aleatória (passagem por referência)
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

// Função para verificar se a missão foi cumprida (passagem por valor para missão, referência para mapa)
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, const char* corJogador) {
    // Lógica simples de verificação baseada no texto da missão
    if (strstr(missao, "Conquistar 3 territorios seguidos") != NULL) {
        // Verifica se jogador tem pelo menos 3 territórios
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                count++;
            }
        }
        return count >= 3;
    }
    else if (strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        // Verifica se não há mais territórios vermelhos
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0 || strcmp(mapa[i].cor, "vermelha") == 0) {
                return 0;
            }
        }
        return 1;
    }
    else if (strstr(missao, "Controlar pelo menos 50%% dos territorios") != NULL) {
        // Verifica se controla metade dos territórios
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                count++;
            }
        }
        return count >= (tamanho / 2);
    }
    
    // Para outras missões, retorna falso (implementação simplificada)
    return 0;
}

// Função para exibir missão (passagem por valor)
void exibirMissao(char* missao, const char* corJogador) {
    printf("Missao do jogador %s: %s\n", corJogador, missao);
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
        printf("Territorio %d: %s (%s) - %d tropas\n", 
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

// Função para simular um ataque entre territórios usando ponteiros
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    printf("\n⚔️  INICIANDO ATAQUE ⚔️\n");
    printf("ATACANTE: %s (%s) com %d tropas\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("DEFENSOR: %s (%s) com %d tropas\n", defensor->nome, defensor->cor, defensor->tropas);
    
    // Simula a rolagem de dados (1-6 para cada lado)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("🎲 Dado do atacante: %d\n", dadoAtacante);
    printf("🎲 Dado do defensor: %d\n", dadoDefensor);
    
    // Determina o resultado do ataque
    if (dadoAtacante > dadoDefensor) {
        printf("\n✅ VITORIA DO ATACANTE!\n");
        
        // Atacante vence: defensor muda de dono e recebe metade das tropas
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= atacante->tropas / 2;
        
        printf("O territorio %s agora pertence a %s!\n", defensor->nome, atacante->cor);
        
    } else {
        printf("\n❌ VITORIA DO DEFENSOR!\n");
        
        // Defensor vence: atacante perde uma tropa
        atacante->tropas -= 1;
        printf("O atacante perdeu uma tropa!\n");
    }
    
    printf("Tropas atualizadas - %s: %d | %s: %d\n", 
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
}

// Função para auxiliar na escolha de territórios
int escolherTerritorio(struct Territorio* territorios, int quantidade, const char* corJogador, const char* tipo) {
    printf("\nEscolha o territorio %s:\n", tipo);
    
    int opcoesValidas = 0;
    // Mostra territórios disponíveis
    for(int i = 0; i < quantidade; i++) {
        if ((strcmp(tipo, "atacante") == 0 && strcmp(territorios[i].cor, corJogador) == 0) ||
            (strcmp(tipo, "defensor") == 0 && strcmp(territorios[i].cor, corJogador) != 0)) {
            printf("%d - %s (%s) - %d tropas\n", 
                   i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
            opcoesValidas++;
        }
    }
    
    if (opcoesValidas == 0) {
        printf("Nenhum territorio disponivel para %s!\n", tipo);
        return -1;
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

// Função para liberar a memória dos territórios
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}

// Função para liberar a memória das missões
void liberarMissao(char* missao) {
    free(missao);
}