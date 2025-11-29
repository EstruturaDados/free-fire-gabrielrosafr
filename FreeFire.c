#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Sistema simples de inventário inspirado em jogos de sobrevivência.
// Aqui o jogador pode registrar, remover, listar e buscar itens da mochila.
// - struct Item: armazena nome, tipo e quantidade
// - vetor de capacidade 10
// - funções: inserirItem, removerItem, listarItens, buscarItem
// Comentários e nomes em Português para facilitar manutenção.

#define CAPACIDADE 10

typedef struct {
    char nome[30];   // nome do item (até 29 caracteres + '\0')
    char tipo[20];   // tipo do item (arma, munição, cura, etc.)
    int quantidade;  // quantidade disponível
} Item;

Item mochila[CAPACIDADE];
int itemCount = 0; // número atual de itens cadastrados

// Remove o '\n' final deixado pelo fgets
void remover_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

// Exibe todos os itens cadastrados na mochila
void listarItens() {
    printf("\n--- Itens na mochila (%d/%d) ---\n", itemCount, CAPACIDADE);
    if (itemCount == 0) {
        printf("Mochila vazia.\n");
    } else {
        for (int i = 0; i < itemCount; i++) {
            printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n", i+1,
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
        }
    }
    printf("-------------------------------\n");
}

// Procura um item pelo nome. Retorna o índice ou -1 se não encontrar.
int buscarIndicePorNome(const char *nome) {
    for (int i = 0; i < itemCount; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Adiciona um item à mochila. Se o nome já existir, apenas aumenta a quantidade.
void inserirItem() {
    if (itemCount >= CAPACIDADE) {
        printf("Mochila cheia. Remova um item antes de adicionar outro.\n");
        return;
    }

    char nome[30];
    char tipo[20];
    int qtd;

    printf("Digite o nome do item: ");
    if (!fgets(nome, sizeof(nome), stdin)) return;
    remover_newline(nome);

    printf("Digite o tipo do item (ex: arma, municao, cura): ");
    if (!fgets(tipo, sizeof(tipo), stdin)) return;
    remover_newline(tipo);

    printf("Digite a quantidade: ");
    if (scanf("%d", &qtd) != 1) {
        // entrada inválida: limpar buffer e abortar
        while (getchar() != '\n');
        printf("Quantidade inválida. Operação cancelada.\n");
        return;
    }
    while (getchar() != '\n'); // limpar o '\n' restante

    if (qtd <= 0) {
        printf("Quantidade deve ser maior que zero.\n");
        return;
    }

    int idx = buscarIndicePorNome(nome);
    if (idx != -1) {
        // item existente: atualiza a quantidade
        mochila[idx].quantidade += qtd;
        printf("Item existente. Quantidade atualizada.\n");
    } else {
        // novo item
        strncpy(mochila[itemCount].nome, nome, sizeof(mochila[itemCount].nome)-1);
        mochila[itemCount].nome[sizeof(mochila[itemCount].nome)-1] = '\0';
        strncpy(mochila[itemCount].tipo, tipo, sizeof(mochila[itemCount].tipo)-1);
        mochila[itemCount].tipo[sizeof(mochila[itemCount].tipo)-1] = '\0';
        mochila[itemCount].quantidade = qtd;
        itemCount++;
        printf("Item cadastrado com sucesso.\n");
    }

    listarItens();
}

// Remove um item pelo nome e reorganiza a mochila para evitar espaços vazios.
void removerItem() {
    if (itemCount == 0) {
        printf("Mochila vazia. Nada para remover.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a ser removido: ");
    if (!fgets(nome, sizeof(nome), stdin)) return;
    remover_newline(nome);

    int idx = buscarIndicePorNome(nome);
    if (idx == -1) {
        printf("Item '%s' não encontrado na mochila.\n", nome);
    } else {
        // desloca itens para preencher o espaço
        for (int i = idx; i < itemCount - 1; i++) {
            mochila[i] = mochila[i+1];
        }
        itemCount--;
        printf("Item removido com sucesso.\n");
    }

    listarItens();
}

// Busca um item pelo nome e mostra suas informações
void buscarItem() {
    if (itemCount == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a buscar: ");
    if (!fgets(nome, sizeof(nome), stdin)) return;
    remover_newline(nome);

    int idx = buscarIndicePorNome(nome);
    if (idx == -1) {
        printf("Item '%s' não encontrado.\n", nome);
    } else {
        printf("Item encontrado:\n");
        printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
               mochila[idx].nome, mochila[idx].tipo, mochila[idx].quantidade);
    }

    // após busca, listamos para manter consistência conforme requisito
    listarItens();
}

// Menu principal que controla as ações do sistema
int main() {
    int opcao = 0;
    printf("=== Sistema de Inventario - Mochila de Loot ===\n");
    printf("Maximo de itens: %d. Use nomes únicos para facilitar a busca.\n", CAPACIDADE);

    while (1) {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item\n");
        printf("3 - Buscar item por nome\n");
        printf("4 - Listar itens\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            // entrada inválida: limpar buffer e continuar
            while (getchar() != '\n');
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }
        while (getchar() != '\n'); // limpar o '\n'

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                buscarItem();
                break;
            case 4:
                listarItens();
                break;
            case 0:
                printf("Saindo... Ate mais!\n");
                exit(0);
                break;
            default:
                printf("Opcao desconhecida. Tente novamente.\n");
        }
    }

    return 0;
}