#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int totalComparacoes = 0, totalTrocas = 0;
int totalComparacoesSentinel = 0, totalTrocasSentinel = 0;

void imprime_vetor(int *vetor, int tamanho_vetor) {
    printf("\nVETOR = {");
    for (int x = 0; x < tamanho_vetor; x++) {
        printf("%d", vetor[x]);
        if (x != tamanho_vetor - 1) printf(",");
        if (x == tamanho_vetor - 1) printf("}");
    }
    printf("\n");
}

void shell_sort_sentinela(int *vetor, int tamanho_vetor) {
    int chave = 0, j = 1, h = 1, i = 1, aux = 0, aux2 = 0;
    while (h < tamanho_vetor) {
        h = 3 * h + 1;
    }
    while (h > 1) {
        h = (h - 1) / 3;
        i = h;
        while (i < tamanho_vetor) {
            aux = vetor[i];
            j = i - h;
            while (j >= 0 && aux < vetor[j]) {
                aux2++;
                // comparacao
                totalComparacoesSentinel++;
                vetor[j + h] = vetor[j];
                j = j - h;
            }
            vetor[j + h] = aux;
            // troca
            totalTrocasSentinel++;
            i++;
        }
    }
}

void shell_sort(int *vetor, int tamanho_vetor) {
    FILE *write;
    write = fopen("../resultados/shellSort.json", "w+");
    fprintf(write, "%s%s", "{\n\n", "  \"estados\": [\n");
    int chave = 0, j = 1, h = 1, i = 1, aux = 0, aux2 = 0;
    while (h < tamanho_vetor) {
        h = 3 * h + 1;
    }
    while (h > 1) {
        h = (h - 1) / 3;
        i = h;
        while (i < tamanho_vetor) {
            aux = vetor[i];
            j = i - h;
            while (j >= 0 && aux < vetor[j]) {
                aux2++;
                // comparacao
                totalComparacoes++;
                fprintf(write, "    {");
                if (aux2 == 1) {
                    fprintf(write, "    \"estado\": [");
                    for (int x = 0; x < tamanho_vetor; x++) {
                        fprintf(write, "%d", vetor[x]);
                        if (x != tamanho_vetor - 1) fprintf(write, "%s", ",");
                    }
                    fprintf(write, "],");
                }
                fprintf(write, "%s%d%s%d%s", "\"comparacao\":[", i, ",", j,
                        "]}");
                if ((totalComparacoes + totalTrocas) ==
                    (totalComparacoesSentinel + totalTrocasSentinel)) {
                    fprintf(write, "%s", "],\n");
                } else {
                    fprintf(write, "%s", ",\n");
                }
                vetor[j + h] = vetor[j];
                j = j - h;
            }
            vetor[j + h] = aux;
            // troca
            totalTrocas++;
            fprintf(write, "    {\"estado\": [");
            for (int x = 0; x < tamanho_vetor; x++) {
                fprintf(write, "%d", vetor[x]);
                if (x != tamanho_vetor - 1) fprintf(write, "%s", ",");
            }
            fprintf(write, "%s%d%s%d%s", "], \"troca\":[", j + h, ",", i, "]}");

            if ((totalComparacoes + totalTrocas) ==
                (totalComparacoesSentinel + totalTrocasSentinel)) {
                fprintf(write, "%s", "],\n");
            } else {
                fprintf(write, "%s", ",\n");
            }
            i++;
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
    int num_elementos_s = counter_elements();
    int *values_s = (int *)malloc(num_elementos_s * sizeof(int));
    readFile(values_s);
    shell_sort_sentinela(values_s, num_elementos_s);

    int num_elementos = counter_elements();
    int *values = (int *)malloc(num_elementos * sizeof(int));
    readFile(values);
    shell_sort(values, num_elementos);

    free(values);
    free(values_s);

    // for (int i = 100; i > 0; i--) {
    //     printf("%d,", i);
    // }

    return 0;
}