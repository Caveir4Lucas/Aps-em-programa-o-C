#include <stdio.h>   // Fornece funções de entrada e saída, como printf, scanf, fopen e fprintf.
#include <stdlib.h>  // Fornece funções de alocação de memória (malloc, free) e geração de números aleatórios (srand, rand).
#include <string.h>  // Utilizado para manipulação de strings, especificamente a função strcspn para limpar a quebra de linha (\n).
#include <time.h>    // Essencial para a medição de desempenho (clock_t, clock()) e para inicializar a semente de aleatoriedade (time(NULL)).

/* ==============================================================================
 * TAD - TIPO ABSTRATO DE DADOS (Encapsulamento estrutural)
 * ------------------------------------------------------------------------------
 * Propósito: Agrupar o ponteiro do arranjo dinâmico e o seu respectivo tamanho
 * em uma única entidade lógica. 
 * Vantagem: Permite a passagem por referência de blocos massivos de dados entre
 * funções, economizando processamento e prevenindo estouro na memória Stack.
 * ============================================================================== */
typedef struct {
    int *imagens;    // Ponteiro que guardará o endereço base do vetor alocado no Heap.
    int quantidade;  // Cardinalidade (número de elementos) armazenada no vetor.
} CatalogoSatelite;

/* ==============================================================================
 * ALGORITMOS DE ORDENAÇÃO (In-place sorting)
 * ============================================================================== */

void bubbleSort(int v[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

void insertionSort(int v[], int n) {
    int i, j, chave;
    for (i = 1; i < n; i++) {
        chave = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = chave;
    }
}

int particionar(int v[], int inicio, int fim) {
    int pivo = v[fim];
    int i = (inicio - 1);
    int j, temp;
    
    for (j = inicio; j < fim; j++) {
        if (v[j] <= pivo) {
            i++;
            temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
    temp = v[i + 1];
    v[i + 1] = v[fim];
    v[fim] = temp;
    
    return i + 1;
}

void quickSort(int v[], int inicio, int fim) {
    int p;
    if (inicio < fim) {
        p = particionar(v, inicio, fim);
        quickSort(v, inicio, p - 1);
        quickSort(v, p + 1, fim);
    }
}

/* ==============================================================================
 * FUNÇÕES UTILITÁRIAS E MANIPULAÇÃO DE DADOS I/O
 * ============================================================================== */

void copiarVetor(int origem[], int destino[], int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) {
        destino[i] = origem[i];
    }
}

void salvarVetorEmArquivo(const char* nomeArquivo, int v[], int n) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    int i;
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de relatorio %s\n", nomeArquivo);
        return;
    }
    for (i = 0; i < n; i++) {
        fprintf(arquivo, "%d\n", v[i]);
    }
    fclose(arquivo);
}

void gerarArquivoDados(const char* nomeArquivo, int quantidade) {
    FILE* arquivo;
    int i;
    
    arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s\n", nomeArquivo);
        return;
    }
    
    srand(time(NULL));
    for (i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d\n", rand() % 1000000);
    }
    fclose(arquivo);
    printf("Catalogo '%s' gerado com %d imagens simuladas.\n", nomeArquivo, quantidade);
}

CatalogoSatelite* lerArquivoDados(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    int capacidade = 200000;
    CatalogoSatelite* catalogo;
    int cont = 0;

    if (arquivo == NULL) {
        printf("Erro ao ler o arquivo %s. Certifique-se de gerar os dados primeiro.\n", nomeArquivo);
        return NULL;
    }
    
    catalogo = (CatalogoSatelite*)malloc(sizeof(CatalogoSatelite));
    catalogo->imagens = (int*)malloc(capacidade * sizeof(int));
    
    if (catalogo->imagens == NULL) {
        printf("Erro fatal: Falha na alocacao de memoria no Heap.\n");
        fclose(arquivo);
        free(catalogo);
        return NULL;
    }
    
    while (fscanf(arquivo, "%d", &catalogo->imagens[cont]) != EOF) {
        cont++;
        if (cont >= capacidade) break;
    }
    fclose(arquivo);
    catalogo->quantidade = cont;
    return catalogo;
}

void executarTestesDesempenho(CatalogoSatelite* catalogo, int salvarListagem) {
    int* dadosParaOrdenar = (int*)malloc(catalogo->quantidade * sizeof(int));
    clock_t inicio, fim;
    double tempo_gasto;

    if (dadosParaOrdenar == NULL) {
        printf("Erro fatal: Falha na alocacao de memoria para ordenacao.\n");
        return;
    }

    printf("\n=== INICIANDO ORDENACAO DO CATALOGO (%d imagens) ===\n", catalogo->quantidade);

    if (salvarListagem) {
        salvarVetorEmArquivo("relatorio_antes_ordenacao.txt", catalogo->imagens, catalogo->quantidade);
        printf("[Info] Catalogo original salvo em 'relatorio_antes_ordenacao.txt'\n");
    }

    /* TESTE 1: QUICK SORT */
    copiarVetor(catalogo->imagens, dadosParaOrdenar, catalogo->quantidade);
    inicio = clock();
    quickSort(dadosParaOrdenar, 0, catalogo->quantidade - 1);
    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("[Quick Sort     - %6d imagens] Tempo: %f segundos\n", catalogo->quantidade, tempo_gasto);
    if (salvarListagem) salvarVetorEmArquivo("relatorio_depois_quick.txt", dadosParaOrdenar, catalogo->quantidade);

    /* TESTE 2: INSERTION SORT */
    copiarVetor(catalogo->imagens, dadosParaOrdenar, catalogo->quantidade);
    inicio = clock();
    insertionSort(dadosParaOrdenar, catalogo->quantidade);
    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("[Insertion Sort - %6d imagens] Tempo: %f segundos\n", catalogo->quantidade, tempo_gasto);
    if (salvarListagem) salvarVetorEmArquivo("relatorio_depois_insertion.txt", dadosParaOrdenar, catalogo->quantidade);

    /* TESTE 3: BUBBLE SORT */
    printf("[Bubble Sort] Processando... (pode demorar para muitos dados)\n");
    copiarVetor(catalogo->imagens, dadosParaOrdenar, catalogo->quantidade);
    inicio = clock();
    bubbleSort(dadosParaOrdenar, catalogo->quantidade);
    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("[Bubble Sort    - %6d imagens] Tempo: %f segundos\n", catalogo->quantidade, tempo_gasto);
    if (salvarListagem) salvarVetorEmArquivo("relatorio_depois_bubble.txt", dadosParaOrdenar, catalogo->quantidade);

    printf("======================================================\n\n");
    free(dadosParaOrdenar); 
}

int main() {
    int opcao = 0;
    char nomeArquivoBase[100];
    char nomeArquivoCompleto[110];
    int qtdCoord;
    int salvarListagem;

    printf("========================================================\n");
    printf("  SISTEMA DE GEOPROCESSAMENTO - FLORESTA AMAZONICA (APS)\n");
    printf("========================================================\n");

    do {
        printf("\n--- MENU DE CATALOGACAO E ORDENACAO ---\n");
        printf("1. Gerar imagens EXTERNAS de satelite (Salvar em .txt)\n");
        printf("2. Ler e Ordenar catalogo EXTERNO (Tempo de execucao)\n");
        printf("3. Gerar e Ordenar imagens INTERNAS (Direto na memoria)\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            continue;
        }
        getchar();

        if (opcao == 1) {
            printf("\nDigite o nome do arquivo a ser criado (ex: satelite_amz): ");
            fgets(nomeArquivoBase, 100, stdin);
            nomeArquivoBase[strcspn(nomeArquivoBase, "\n")] = 0;
            
            sprintf(nomeArquivoCompleto, "%s.txt", nomeArquivoBase);

            printf("Digite a quantidade de imagens catalogadas pelo satelite (maximo 200000): ");
            while (scanf("%d", &qtdCoord) != 1 || qtdCoord <= 0 || qtdCoord > 200000) {
                printf("Entrada invalida. Digite um numero entre 1 e 200000: ");
                while (getchar() != '\n'); 
            }
            getchar();

            gerarArquivoDados(nomeArquivoCompleto, qtdCoord);

        } else if (opcao == 2) {
            CatalogoSatelite* meuCatalogo;
            
            printf("\nDigite o nome do catalogo para ler (ex: satelite_amz): ");
            fgets(nomeArquivoBase, 100, stdin);
            nomeArquivoBase[strcspn(nomeArquivoBase, "\n")] = 0; 
            
            sprintf(nomeArquivoCompleto, "%s.txt", nomeArquivoBase);

            printf("Deseja gerar arquivos TXT para o relatorio? (1 - Sim, 0 - Nao): ");
            scanf("%d", &salvarListagem);
            getchar();

            meuCatalogo = lerArquivoDados(nomeArquivoCompleto);
            
            if (meuCatalogo != NULL) {
                executarTestesDesempenho(meuCatalogo, salvarListagem);
                free(meuCatalogo->imagens);
                free(meuCatalogo);
            }

        } else if (opcao == 3) {
            CatalogoSatelite* catalogoInterno;
            int i;

            printf("\nDigite a quantidade de imagens simuladas na memoria (maximo 200000): ");
            while (scanf("%d", &qtdCoord) != 1 || qtdCoord <= 0 || qtdCoord > 200000) {
                printf("Entrada invalida. Digite um numero entre 1 e 200000: ");
                while (getchar() != '\n'); 
            }
            getchar();

            printf("Deseja gerar arquivos TXT para o relatorio? (1 - Sim, 0 - Nao): ");
            scanf("%d", &salvarListagem);
            getchar();

            catalogoInterno = (CatalogoSatelite*)malloc(sizeof(CatalogoSatelite));
            catalogoInterno->quantidade = qtdCoord;
            catalogoInterno->imagens = (int*)malloc(qtdCoord * sizeof(int));
            
            if (catalogoInterno->imagens == NULL) {
                printf("Erro fatal: Falha na alocacao de memoria no Heap.\n");
            } else {
                srand(time(NULL));
                for (i = 0; i < qtdCoord; i++) {
                    catalogoInterno->imagens[i] = rand() % 1000000;
                }
                printf("\nImagens geradas com sucesso na memoria RAM.\n");
                
                executarTestesDesempenho(catalogoInterno, salvarListagem);
                
                free(catalogoInterno->imagens);
                free(catalogoInterno);
            }

        } else if (opcao != 4) {
            printf("\nOpcao invalida!\n\n");
        }

    } while (opcao != 4);

    printf("\nSistema de Geoprocessamento encerrado.\n");
    return 0;
}