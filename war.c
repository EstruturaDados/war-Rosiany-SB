#include <stdio.h>
#include <string.h>

// Definição da struct Território
// Agrupa dados relacionados sobre cada território
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas[10];
};

int main() {
    // Declaração do vetor de structs com capacidade para 5 territórios
    struct Territorio territorios[5];

    printf("=== SISTEMA DE CADASTRO DE TERRITORIOS ===\n\n");

    // Entrada de dados: cadastro dos 5 territótios
    // Laço for para percorrer e prencher cada posição do vetor
    for(int i = 0; i < 5; i++) {
        printf("--- Cadastro do Territorio %d ---\n", i + 1);

        //Entrada do nome do território
        printf("Digite o nome do territorio: ");
        scanf("%29s", territorios[i].nome);

        //Entrada da cor do exército
        printf("Digite a cor do exército: ");
        scanf("%9s", &territorios[i].cor);

        // Entrada da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n"); //Linha em branco para melhor organização
    }

    // Exibição dos dados: mostra todos os territórios cadastrados
    printf("=== DADOS DOS TERRITORIOS CADASTRADOS ===\n\n");

    //Laço for para percorrer e exibir cada território
    for(int i = 0; i< 5; i++) {
        printf("Territorio %d: \n", i + 1);
        printf("Nome: %s\n", territorios[i]. nome);
        printf("Cor do exército:%s\n", territorios[i].cor);
        printf("Tropas:%d\n", territorios[i].tropas);
        printf("----------------------\n");
    }

    return 0;
}
