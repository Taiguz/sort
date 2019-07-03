#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int totalComparacoes = 0, totalTrocas = 0;
int totalComparacoesSentinel = 0, totalTrocasSentinel = 0;
int sentinela = 0;

void imprime_vetor(int *vetor, int tamanho_vetor) {
    printf("\nVETOR = {");
    for (int x = 0; x < tamanho_vetor; x++) {
        printf("%d", vetor[x]);
        if (x != tamanho_vetor - 1) printf(",");
        if (x == tamanho_vetor - 1) printf("}");
    }
    printf("\n");
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(FILE *write, int *vetor, int p, int r, int tamanho_vetor) {
    int pivo = vetor[r], i = (p - 1);
    if (p == 0 && r == (tamanho_vetor - 1)) {
        fprintf(write, "%s%s", "{\n\n", "  \"estados\": [\n");
        fprintf(write, "\t{    \"estado\": [");
        for (int x = 0; x < tamanho_vetor; x++) {
            fprintf(write, "%d", vetor[x]);
            if (x != tamanho_vetor - 1) fprintf(write, "%s", ",");
        }
        fprintf(write, "],");
    }  // termina print

    for (int j = p; j <= r - 1; j++) {
        // comparacao
        totalComparacoes++;
        fprintf(write, "%s%d%s%d%s", " \"comparacao\":[", j, ",", r, "]}");
        if (!((totalComparacoes + totalTrocas) ==
              (totalComparacoesSentinel + totalTrocasSentinel))) {
            fprintf(write, "%s", ", \n\t{\t");
        } else {
            if (j + 1 > r - 1) {
                if (!(vetor[j] <= pivo)) {
                    fprintf(write, "%s", ", \n\t{\t");
                } else {
                    fprintf(
                        write, "%s%d%s%d%s", "],\n\n\t\"totalComparacoes\":",
                        totalComparacoesSentinel,
                        ",\n\t\"totalTrocas\":", totalTrocasSentinel, "\n\n}");
                }

            } else {
                fprintf(write, "%s", ", \n\t{\t");
            }
        }

        if (vetor[j] <= pivo) {
            i++;
            int t = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = t;
        }
    }
    int t = vetor[i + 1];
    vetor[i + 1] = vetor[r];
    vetor[r] = t;

    if (i + 1 != r) {
        totalTrocas++;
        fprintf(write, "    \"estado\": [");
        for (int x = 0; x < tamanho_vetor; x++) {
            fprintf(write, "%d", vetor[x]);
            if (x != tamanho_vetor - 1) fprintf(write, "%s", ",");
        }
        fprintf(write, "%s%d%s%d%s", "], \"troca\":[", i + 1, ",", r, "]}");
        if (!((totalComparacoes + totalTrocas) ==
              (totalComparacoesSentinel + totalTrocasSentinel))) {
            fprintf(write, "%s", ",\n\t{");
        } else {
            fprintf(write, "%s%d%s%d%s",
                    "],\n\n\t\"totalComparacoes\":", totalComparacoesSentinel,
                    ",\n\t\"totalTrocas\":", totalTrocasSentinel, "\n\n}");
        }
    }
    return (i + 1);
}

void quick_sort(FILE *write, int *vetor, int p, int r, int tamanho_vetor) {
    if (p < r) {
        int q = partition(write, vetor, p, r, tamanho_vetor);
        quick_sort(write, vetor, p, q - 1, tamanho_vetor);
        quick_sort(write, vetor, q + 1, r, tamanho_vetor);
    }
}

int partition_sentinel(FILE *write, int *vetor, int p, int r,
                       int tamanho_vetor) {
    int pivo = vetor[r], i = (p - 1);
    for (int j = p; j <= r - 1; j++) {
        totalComparacoesSentinel++;
        if (vetor[j] <= pivo) {
            i++;
            int t = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = t;
        }
    }
    int t = vetor[i + 1];
    vetor[i + 1] = vetor[r];
    vetor[r] = t;
    if (i + 1 != r) {
        totalTrocasSentinel++;
    }
    return (i + 1);
}

void quick_sort_sentinel(FILE *write, int *vetor, int p, int r,
                         int tamanho_vetor) {
    if (p < r) {
        int q = partition_sentinel(write, vetor, p, r, tamanho_vetor);
        quick_sort_sentinel(write, vetor, p, q - 1, tamanho_vetor);
        quick_sort_sentinel(write, vetor, q + 1, r, tamanho_vetor);
    }
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
    FILE *write_s;
    write_s = fopen("../resultados/quickSort.json", "w+");
    int num_elementos_s = counter_elements();
    int *values_s = (int *)malloc(num_elementos_s * sizeof(int));
    readFile(values_s);
    quick_sort_sentinel(write_s, values_s, 0, num_elementos_s - 1,
                        num_elementos_s);

    FILE *write;
    write = fopen("../resultados/quickSort.json", "w+");
    int num_elementos = counter_elements();
    int *values = (int *)malloc(num_elementos * sizeof(int));
    readFile(values);
    quick_sort(write, values, 0, num_elementos - 1, num_elementos);
    return 0;
}