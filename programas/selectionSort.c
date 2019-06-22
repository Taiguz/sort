#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprime_vetor(int *vetor, int tamanho_vetor) {
    printf("\nVETOR = {");
    for (int x = 0; x < tamanho_vetor; x++) {
        printf("%d", vetor[x]);
        if (x != tamanho_vetor - 1) printf(",");
        if (x == tamanho_vetor - 1) printf("}");
    }
    printf("\n");
}

void selection_sort(int *vetor, int tamanho_vetor) {
    FILE *write;
    write = fopen("../resultados/selectionSort.json", "w+");
    fprintf(write, "%s%s", "{\n\n", "  \"estados\": [\n");
    int menor = 0, totalComparacoes = 0, totalTrocas = 0;
    for (int i = 0; i < tamanho_vetor; i++) {
        menor = i;
        // comparacao
        for (int j = i; j < tamanho_vetor; j++) {
            fprintf(write, "    {");
            if (i == 0 && j == 0) {
                fprintf(write, "    \"estado\": [");
                for (int x = 0; x < tamanho_vetor; x++) {
                    fprintf(write, "%d", vetor[x]);
                    if (x != tamanho_vetor - 1) fprintf(write, "%s", ",");
                }
                fprintf(write, "],");
            }
            fprintf(write, "%s%d%s%d%s", "\"comparacao\":[", j, ",", menor,
                    "]}");
            fprintf(write, "%s", ",\n");
            // file management end

            totalComparacoes++;
            if (vetor[j] < vetor[menor]) {
                menor = j;
            }
        }

        int aux = vetor[i];
        vetor[i] = vetor[menor];
        vetor[menor] = aux;
        totalTrocas++;

        // troca
        fprintf(write, "    {\"estado\": [");
        for (int x = 0; x < tamanho_vetor; x++) {
            fprintf(write, "%d", vetor[x]);
            if (x != tamanho_vetor - 1) fprintf(write, "%s", ",");
        }
        fprintf(write, "%s%d%s%d%s", "], \"troca\":[", i, ",", menor, "]}");
        if (i != tamanho_vetor - 1) {
            fprintf(write, "%s", ",\n");
        } else {
            fprintf(write, "%s", "],\n");
        }
        // file management end
    }
    fprintf(write, "%s%d%s", "\t\"totalComparacoes\":", totalComparacoes,
            ",\n");
    fprintf(write, "%s%d%s", "\t\"totalTrocas\":", totalTrocas, "\n");
    fprintf(write, "\n}");
}

// int busca_binaria(int *vetor, int inicio, int fim, float chave) {
//     if (inicio < fim) {
//         int meio = (inicio + fim) / 2;
//         if (chave == vetor[meio]) {
//             return meio + 1;
//         }
//         if (fim - inicio == 1) {
//             return -1;
//         }
//         if (chave < vetor[meio]) {
//             busca_binaria(vetor, inicio, meio, chave);
//         } else {
//             busca_binaria(vetor, meio + 1, fim, chave);
//         }
//     }
// }

void readFile(int *values) {
    // Abrindo arquivo
    FILE *archive;
    archive = fopen("dados.dat", "r");

    if (!archive) {
        printf("Houve um erro ao abrir o arquivo");
    } else {
        char c;
        char aux[30];
        int i = 0, w = 0;

        // Leitura e armazenamento dos valores
        while ((c = fgetc(archive)) != EOF) {
            if (c != ',') {
                aux[w++] = c;
            } else {
                values[i] = atoi(aux);
                i++;
                w = 0;
                strcpy(aux, "            ");
            }
        }
        values[i] = atoi(aux);
        strcpy(aux, "            ");
        pclose(archive);
    }
}

int counter_elements() {
    int cont_elements = 1;

    // Abrindo arquivo 'dados.dat'
    FILE *archive;
    archive = fopen("dados.dat", "r");

    // Contagem dos números
    if (!archive) {
        printf("Houve um erro ao abrir o arquivo.\n");
    } else {
        char c;
        while ((c = fgetc(archive)) != EOF) {
            if (c == ',') {
                cont_elements++;
            }
        }
    }
    fclose(archive);
    return cont_elements;
}

int main(int argc, char const *argv[]) {
    int num_elementos = counter_elements();
    int *values = (int *)malloc(num_elementos * sizeof(int));
    readFile(values);
    selection_sort(values, num_elementos);
    imprime_vetor(values, num_elementos);
    return 0;
}