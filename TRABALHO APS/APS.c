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

/*
 * Função: bubbleSort
 * Complexidade de Tempo: O(n^2) em todos os casos (pior, médio e melhor prático).
 * Lógica: Percorre o vetor n vezes. Em cada iteração, compara elementos vizinhos
 * e realiza o "swap" (troca) se estiverem fora de ordem. Os maiores elementos
 * "flutuam" para o final do array.
 */
void bubbleSort(int v[], int n) {
    int i, j, temp;
    // O laço externo controla as passagens. A cada passagem, o maior item é posicionado no final.
    for (i = 0; i < n - 1; i++) {
        // O laço interno evita comparar os elementos já ordenados no final (n - i - 1).
        for (j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                // Swap: Troca de valores utilizando uma variável temporária.
                temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

/*
 * Função: insertionSort
 * Complexidade de Tempo: O(n^2) no pior caso, mas atinge O(n) no melhor caso (dados já ordenados).
 * Lógica: Constrói a lista ordenada inserindo um elemento de cada vez na sua 
 * posição correta através de deslocamentos à direita (shift), evitando "swaps" repetitivos.
 */
void insertionSort(int v[], int n) {
    int i, j, chave;
    // Inicia no índice 1, assumindo que o elemento no índice 0 já é uma sub-lista ordenada de tamanho 1.
    for (i = 1; i < n; i++) {
        chave = v[i]; // Guarda o valor atual a ser inserido.
        j = i - 1;
        // Desloca os elementos maiores que a 'chave' uma posição para a direita.
        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        // Insere a 'chave' na lacuna correta encontrada.
        v[j + 1] = chave;
    }
}

/*
 * Função: particionar (Função auxiliar estrita do Quick Sort)
 * Propósito: Escolher um pivô (neste caso, o último elemento) e organizar o vetor
 * de forma que os menores que o pivô fiquem à sua esquerda, e os maiores à direita.
 * Retorno: O índice definitivo onde o pivô foi posicionado.
 */
int particionar(int v[], int inicio, int fim) {
    int pivo = v[fim]; // O último elemento atua como pivô lógico.
    int i = (inicio - 1); // Índice do menor elemento atual.
    int j, temp;
    
    // Varredura linear comparando todos os elementos com o pivô.
    for (j = inicio; j < fim; j++) {
        if (v[j] <= pivo) {
            i++;
            // Move elementos menores ou iguais ao pivô para a janela inferior do array.
            temp = v[i];
            v[i] = v[j];
            v[j] = temp;
        }
    }
    // Posiciona o pivô exatamente após a janela dos elementos menores.
    temp = v[i + 1];
    v[i + 1] = v[fim];
    v[fim] = temp;
    
    return i + 1; // Retorna o centro da partição.
}

/*
 * Função: quickSort
 * Complexidade de Tempo: O(n log n) no caso médio. O(n^2) no pior caso patológico.
 * Lógica: Aplica o paradigma Divisão e Conquista. Particiona o vetor recursivamente
 * em sub-vetores menores, garantindo altíssima escalabilidade e desempenho em Big Data.
 */
void quickSort(int v[], int inicio, int fim) {
    int p;
    // Condição de parada da recursão: quando o sub-vetor tem 1 ou 0 elementos.
    if (inicio < fim) {
        p = particionar(v, inicio, fim); // Encontra o eixo divisório (pivô).
        quickSort(v, inicio, p - 1);     // Chama a ordenação para a metade esquerda.
        quickSort(v, p + 1, fim);        // Chama a ordenação para a metade direita.
    }
}

/* ==============================================================================
 * FUNÇÕES UTILITÁRIAS E MANIPULAÇÃO DE DADOS I/O
 * ============================================================================== */

/*
 * Função: copiarVetor
 * Propósito: Os algoritmos de ordenação operam "in-place" (modificam o vetor original).
 * Para testar justamente três algoritmos distintos com os mesmos dados, precisamos
 * restaurar a desordem original copiando os dados do backup a cada iteração de teste.
 */
void copiarVetor(int origem[], int destino[], int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) {
        destino[i] = origem[i];
    }
}

/*
 * Função: salvarVetorEmArquivo
 * Propósito: Serializa o estado atual do vetor na memória para o disco (arquivo de texto).
 * Lógica: Utiliza iteração linear associada à escrita em bloco (fprintf).
 */
void salvarVetorEmArquivo(const char* nomeArquivo, int v[], int n) {
    FILE* arquivo = fopen(nomeArquivo, "w"); // Abre em modo de escrita ('w'). Substitui se existir.
    int i;
    if (arquivo == NULL) { // Programação defensiva contra falhas de permissão no SO.
        printf("Erro ao criar o arquivo de relatorio %s\n", nomeArquivo);
        return;
    }
    for (i = 0; i < n; i++) {
        fprintf(arquivo, "%d\n", v[i]); // Serializa um identificador por linha.
    }
    fclose(arquivo); // Libera o ponteiro do arquivo no Sistema Operacional.
}

/*
 * Função: gerarArquivoDados
 * Propósito: Simular o catálogo de imagens do satélite populando um arquivo externo
 * com 'n' identificadores numéricos aleatórios, servindo como base de dados primária.
 */
void gerarArquivoDados(const char* nomeArquivo, int quantidade) {
    FILE* arquivo;
    int i;
    
    arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s\n", nomeArquivo);
        return;
    }
    
    srand(time(NULL)); // Inicializa a semente geradora com o timestamp atual da máquina.
    for (i = 0; i < quantidade; i++) {
        // rand() % 1000000 restringe os códigos das imagens a números entre 0 e 999.999.
        fprintf(arquivo, "%d\n", rand() % 1000000);
    }
    fclose(arquivo);
    printf("Catalogo '%s' gerado com %d imagens simuladas.\n", nomeArquivo, quantidade);
}

/*
 * Função: lerArquivoDados
 * Retorno: Um ponteiro para a struct 'CatalogoSatelite' instanciada no Heap.
 * Propósito: Realizar a extração dos dados em disco (I/O) carregando-os integralmente 
 * para a memória RAM, gerenciando a alocação de forma segura.
 */
CatalogoSatelite* lerArquivoDados(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r"); // Abre em modo leitura estrita ('r').
    int capacidade = 200000; // Define o teto máximo de processamento estabelecido pela arquitetura.
    CatalogoSatelite* catalogo;
    int cont = 0;

    if (arquivo == NULL) {
        printf("Erro ao ler o arquivo %s. Certifique-se de gerar os dados primeiro.\n", nomeArquivo);
        return NULL;
    }
    
    // Alocação dinâmica de 2 blocos independentes: A estrutura(TAD) e o array interno.
    catalogo = (CatalogoSatelite*)malloc(sizeof(CatalogoSatelite));
    catalogo->imagens = (int*)malloc(capacidade * sizeof(int));
    
    // Tratamento rigoroso de erro caso a RAM do sistema esteja sobrecarregada (Defensive Programming).
    if (catalogo->imagens == NULL) {
        printf("Erro fatal: Falha na alocacao de memoria no Heap.\n");
        fclose(arquivo);
        free(catalogo); // Evita memory leak liberando a primeira alocação antes de abortar.
        return NULL;
    }
    
    // Extrai linha por linha até o fim do arquivo (EOF) ou até atingir o limite de capacidade.
    while (fscanf(arquivo, "%d", &catalogo->imagens[cont]) != EOF) {
        cont++;
        if (cont >= capacidade) break;
    }
    fclose(arquivo);
    catalogo->quantidade = cont; // Atualiza a cardinalidade oficial no TAD.
    return catalogo;
}

/*
 * Função: executarTestesDesempenho
 * Dependências: clock(), quickSort, insertionSort, bubbleSort, salvarVetorEmArquivo.
 * Propósito: Executar o benchmark controlando rigorosamente os cronômetros
 * para extrair puramente os tempos de ciclo de CPU (CPU time) dedicados à ordenação matemática.
 */
void executarTestesDesempenho(CatalogoSatelite* catalogo, int salvarListagem) {
    // Aloca um array espelho para operar as ordenações sem destruir o catálogo original.
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

    /* ----------------------------------------------------------------------
     * TESTE 1: QUICK SORT (Alta eficiência / Divisão e Conquista)
     * ---------------------------------------------------------------------- */
    copiarVetor(catalogo->imagens, dadosParaOrdenar, catalogo->quantidade); // Restaura cenário inicial
    inicio = clock(); // Captura o tick exato do clock da CPU.
    quickSort(dadosParaOrdenar, 0, catalogo->quantidade - 1);
    fim = clock(); // Congela o tick imediatamente após retornar.
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC; // Conversão de ticks para segundos em ponto flutuante.
    printf("[Quick Sort     - %6d imagens] Tempo: %f segundos\n", catalogo->quantidade, tempo_gasto);
    if (salvarListagem) salvarVetorEmArquivo("relatorio_depois_quick.txt", dadosParaOrdenar, catalogo->quantidade);

    /* ----------------------------------------------------------------------
     * TESTE 2: INSERTION SORT (Abordagem iterativa / Deslocamento de blocos)
     * ---------------------------------------------------------------------- */
    copiarVetor(catalogo->imagens, dadosParaOrdenar, catalogo->quantidade);
    inicio = clock();
    insertionSort(dadosParaOrdenar, catalogo->quantidade);
    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("[Insertion Sort - %6d imagens] Tempo: %f segundos\n", catalogo->quantidade, tempo_gasto);
    if (salvarListagem) salvarVetorEmArquivo("relatorio_depois_insertion.txt", dadosParaOrdenar, catalogo->quantidade);

    /* ----------------------------------------------------------------------
     * TESTE 3: BUBBLE SORT (Gargalo esperado, Complexidade Quadrática pura)
     * ---------------------------------------------------------------------- */
    printf("[Bubble Sort] Processando... (pode demorar para muitos dados)\n");
    copiarVetor(catalogo->imagens, dadosParaOrdenar, catalogo->quantidade);
    inicio = clock();
    bubbleSort(dadosParaOrdenar, catalogo->quantidade);
    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("[Bubble Sort    - %6d imagens] Tempo: %f segundos\n", catalogo->quantidade, tempo_gasto);
    if (salvarListagem) salvarVetorEmArquivo("relatorio_depois_bubble.txt", dadosParaOrdenar, catalogo->quantidade);

    printf("======================================================\n\n");
    
    // Libera a memória do vetor espelho para evitar memory leak local.
    free(dadosParaOrdenar); 
}

/* ==============================================================================
 * FUNÇÃO MAIN (Ponto de entrada e Menu de Interface de Usuário)
 * ============================================================================== */
int main() {
    // Variáveis de controle de fluxo de estado e entrada de dados
    int opcao = 0;
    char nomeArquivoBase[100];
    char nomeArquivoCompleto[110];
    int qtdCoord;
    int salvarListagem;

    // Cabeçalho da aplicação (Identidade visual da APS)
    printf("========================================================\n");
    printf("  SISTEMA DE GEOPROCESSAMENTO - FLORESTA AMAZONICA (APS)\n");
    printf("========================================================\n");

    // Loop infinito do menu (rompido quando opcao == 4). Mantém a CLI ativa.
    do {
        printf("\n--- MENU DE CATALOGACAO E ORDENACAO ---\n");
        printf("1. Gerar imagens EXTERNAS de satelite (Salvar em .txt)\n");
        printf("2. Ler e Ordenar catalogo EXTERNO (Tempo de execucao)\n");
        printf("3. Gerar e Ordenar imagens INTERNAS (Direto na memoria)\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        
        // Tratamento seguro contra buffer overflows de leitura em inteiros.
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpa a entrada defeituosa do stream.
            continue; // Reinicia a visualização do menu.
        }
        getchar(); // Remove o '\n' residual do pressionamento de 'Enter'.

        /* ==================== OPÇÃO 1 ==================== 
         * Fluxo de Criação da Base de Dados Física (.txt)
         */
        if (opcao == 1) {
            printf("\nDigite o nome do arquivo a ser criado (ex: satelite_amz): ");
            fgets(nomeArquivoBase, 100, stdin); // fgets previne buffer overflow ao limitar a string a 100 caracteres.
            nomeArquivoBase[strcspn(nomeArquivoBase, "\n")] = 0; // Localiza e substitui o "\n" residual por "\0" de terminação.
            
            // Concatena a extensão formatando a string final.
            sprintf(nomeArquivoCompleto, "%s.txt", nomeArquivoBase);

            printf("Digite a quantidade de imagens catalogadas pelo satelite (maximo 200000): ");
            // Proteção contra entradas não numéricas, negativas ou que ultrapassem o limite do ponteiro lógico.
            while (scanf("%d", &qtdCoord) != 1 || qtdCoord <= 0 || qtdCoord > 200000) {
                printf("Entrada invalida. Digite um numero entre 1 e 200000: ");
                while (getchar() != '\n'); 
            }
            getchar();

            gerarArquivoDados(nomeArquivoCompleto, qtdCoord);

        /* ==================== OPÇÃO 2 ==================== 
         * Fluxo de Carga de Dados Físicos e Processamento (I/O -> RAM -> CPU)
         */
        } else if (opcao == 2) {
            CatalogoSatelite* meuCatalogo;
            
            printf("\nDigite o nome do catalogo para ler (ex: satelite_amz): ");
            fgets(nomeArquivoBase, 100, stdin);
            nomeArquivoBase[strcspn(nomeArquivoBase, "\n")] = 0; 
            
            sprintf(nomeArquivoCompleto, "%s.txt", nomeArquivoBase);

            printf("Deseja gerar arquivos TXT para o relatorio? (1 - Sim, 0 - Nao): ");
            scanf("%d", &salvarListagem);
            getchar();

            // Realiza a I/O, instanciando o TAD no Heap e retornando o endereço de memória.
            meuCatalogo = lerArquivoDados(nomeArquivoCompleto);
            
            if (meuCatalogo != NULL) {
                executarTestesDesempenho(meuCatalogo, salvarListagem);
                
                // Desalocação em cascata. Primeiro o vetor interno, depois a casca estrutural(TAD).
                free(meuCatalogo->imagens);
                free(meuCatalogo);
            }

        /* ==================== OPÇÃO 3 ==================== 
         * Fluxo de Alta Velocidade: Mock de Dados Direto na Memória RAM
         */
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

            // Alocação manual do TAD ignorando I/O no disco.
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
                
                // Prevenção estrita contra memory leaks antes de reiniciar o loop.
                free(catalogoInterno->imagens);
                free(catalogoInterno);
            }

        /* ==================== AVISO DE EXCEÇÃO ==================== */
        } else if (opcao != 4) {
            printf("\nOpcao invalida!\n\n");
        }

    } while (opcao != 4);

    // Encerramento da thread principal devolvendo 0 para a chamada do Sistema Operacional.
    printf("\nSistema de Geoprocessamento encerrado.\n");
    return 0;
}
