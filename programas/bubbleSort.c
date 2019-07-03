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

void bubble_sort(int *vetor, int tamanho_vetor) {
    FILE *write;
    write = fopen("../resultados/bubbleSort.json", "w+");
    fprintf(write, "%s%s", "{\n\n", "  \"estados\": [\n");
    int aux = 0, totalComparacoes = 0, totalTrocas = 0;
    for (int i = 0; i < tamanho_vetor; i++) {
        for (int j = (tamanho_vetor - 1); j > i; j--) {
            // comparacao
            fprintf(write, "    {");
            if (i == 0 && j == (tamanho_vetor - 1)) {
                fprintf(write, "    \"estado\": [");
                for (int x = 0; x < tamanho_vetor; x++) {
                    fprintf(write, "%d", vetor[x]);
                    if (x != tamanho_vetor - 1) fprintf(write, "%s", ",");
                }
                fprintf(write, "],");
            }
            fprintf(write, "%s%d%s%d%s", "\"comparacao\":[", j, ",", (j - 1),
                    "]}");
            if (!(vetor[j] < vetor[j - 1]) && i == tamanho_vetor - 2) {
                fprintf(write, "%s", "],\n");
            } else {
                fprintf(write, "%s", ",\n");
            }

            totalComparacoes++;
            // fim comparacao
            printf("\n[j] -> %d", j);
            printf("\t[j - 1] -> %d\n", j - 1);
            if (vetor[j] < vetor[j - 1]) {
                printf("i -> %d \n", i);
                aux = vetor[j];
                vetor[j] = vetor[j - 1];
                vetor[j - 1] = aux;
                // troca
                fprintf(write, "    {\"estado\": [");
                for (int x = 0; x < tamanho_vetor; x++) {
                    fprintf(write, "%d", vetor[x]);
                    if (x != tamanho_vetor - 1) fprintf(write, "%s", ",");
                }
                fprintf(write, "%s%d%s%d%s", "], \"troca\":[", j, ",", (j - 1),
                        "]}");

                if (i == tamanho_vetor - 2) {
                    fprintf(write, "%s", "],\n");
                } else {
                    fprintf(write, "%s", ",\n");
                }
                totalTrocas++;
                // fim troca
            }
        }
    }
    fprintf(write, "%s%d%s", "\t\"totalComparacoes\":", totalComparacoes,
            ",\n");
    fprintf(write, "%s%d%s", "\t\"totalTrocas\":", totalTrocas, "\n");
    fprintf(write, "\n}");
}

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
    imprime_vetor(values, num_elementos);
    bubble_sort(values, num_elementos);
    imprime_vetor(values, num_elementos);
    return 0;
}