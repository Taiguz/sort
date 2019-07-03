#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int totalComparacoes = 0, totalTrocas = 0;
int totalComparacoesSentinel = 0, totalTrocasSentinel = 0;

void merge_sentinel(FILE *write, int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Vetores auxiliares
    int L[n1], R[n2];

    // Copiando array, primeira metade para o array esquerdo e segunda para o
    // direito (divisão)
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    // Unindo novamente o vetor separado (conquista)
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        totalComparacoesSentinel++;
        // comparação 	{	 "comparacao":[1,99]},
        if (L[i] <= R[j]) {
            totalTrocasSentinel++;
            // troca
            arr[k] = L[i];
            i++;
        } else {
            totalTrocasSentinel++;
            // troca
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        totalTrocasSentinel++;
        // troca
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        totalTrocasSentinel++;
        // troca
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort_sentinel(FILE *write, int arr[], int l, int r, int x) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_sentinel(write, arr, l, m, x);
        merge_sort_sentinel(write, arr, m + 1, r, x);
        merge_sentinel(write, arr, l, m, r);
    }
}

void merge(FILE *write, int arr[], int l, int m, int r, int x) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Vetores auxiliares
    int L[n1], R[n2];

    // Copiando array, primeira metade para o array esquerdo e segunda para o
    // direito (divisão)
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    // Unindo novamente o vetor separado (conquista)
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        // comparação
        totalComparacoes++;
        fprintf(write, "%s%d%s%d%s", "	{	 \" comparacao \":[", i, ",", j,
                "]}");

        // Verifica se esta no final
        if (totalComparacoes == totalComparacoesSentinel &&
            totalTrocas == totalTrocasSentinel) {
            fprintf(write, "%s", "],\n");
        } else {
            fprintf(write, "%s", ",\n");
        }

        if (L[i] <= R[j]) {
            // troca
            totalTrocas++;
            arr[k] = L[i];
            fprintf(write, "\t{    \"estado\": [");
            for (int j = 0; j < x; j++) {
                fprintf(write, "%d", arr[j]);
                if (j != x - 1) fprintf(write, "%s", ",");
            }
            fprintf(write, "%s%d%s%d%s", "],  \"troca\":[", k, ",", i, "]}");

            // Verifica se esta no final
            if (totalComparacoes == totalComparacoesSentinel &&
                totalTrocas == totalTrocasSentinel) {
                fprintf(write, "%s", "],\n");
            } else {
                fprintf(write, "%s", ",\n");
            }
            i++;
        } else {
            // troca
            totalTrocas++;
            arr[k] = R[j];
            fprintf(write, "\t{    \"estado\": [");
            for (int j = 0; j < x; j++) {
                fprintf(write, "%d", arr[j]);
                if (j != x - 1) fprintf(write, "%s", ",");
            }
            fprintf(write, "%s%d%s%d%s", "],  \"troca\":[", k, ",", j, "]}");

            // Verifica se esta no final
            if (totalComparacoes == totalComparacoesSentinel &&
                totalTrocas == totalTrocasSentinel) {
                fprintf(write, "%s", "],\n");
            } else {
                fprintf(write, "%s", ",\n");
            }
            j++;
        }
        k++;
    }
    while (i < n1) {
        // troca
        totalTrocas++;
        arr[k] = L[i];
        fprintf(write, "\t{    \"estado\": [");
        for (int j = 0; j < x; j++) {
            fprintf(write, "%d", arr[j]);
            if (j != x - 1) fprintf(write, "%s", ",");
        }
        fprintf(write, "%s%d%s%d%s", "],  \"troca\":[", k, ",", i, "]}");

        // Verifica se esta no final
        if (totalComparacoes == totalComparacoesSentinel &&
            totalTrocas == totalTrocasSentinel) {
            fprintf(write, "%s", "],\n");
        } else {
            fprintf(write, "%s", ",\n");
        }
        i++;
        k++;
    }
    while (j < n2) {
        // troca
        totalTrocas++;
        arr[k] = R[j];
        fprintf(write, "\t{    \"estado\": [");
        for (int j = 0; j < x; j++) {
            fprintf(write, "%d", arr[j]);
            if (j != x - 1) fprintf(write, "%s", ",");
        }
        fprintf(write, "%s%d%s%d%s", "],  \"troca\":[", k, ",", j, "]}");

        // Verifica se esta no final
        if (totalComparacoes == totalComparacoesSentinel &&
            totalTrocas == totalTrocasSentinel) {
            fprintf(write, "%s", "],\n");
        } else {
            fprintf(write, "%s", ",\n");
        }
        j++;
        k++;
    }
}

void merge_sort(FILE *write, int arr[], int l, int r, int x) {
    // Primeira execução - imprime o estado inicial do vetor no arquivo +
    // comparação 1
    if (l == 0 && r == x - 1) {
        fprintf(write, "%s%s", "{\n\n", "  \"estados\": [\n");
        fprintf(write, "\t{    \"estado\": [");
        for (int j = 0; j < x; j++) {
            fprintf(write, "%d", arr[j]);
            if (j != x - 1) fprintf(write, "%s", ",");
        }
        fprintf(write, "%s%d%s%d%s", "],  \"comparacao \":[", l, ",", r,
                "]},\n");
    }

    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(write, arr, l, m, x);
        merge_sort(write, arr, m + 1, r, x);
        merge(write, arr, l, m, r, x);
    }
}

void imprime_vetor(int *vetor, int tamanho_vetor) {
    printf("\nVETOR = {");
    for (int x = 0; x < tamanho_vetor; x++) {
        printf("%d", vetor[x]);
        if (x != tamanho_vetor - 1) printf(",");
        if (x == tamanho_vetor - 1) printf("}");
    }
    printf("\n");
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
    write_s = fopen("../resultados/mergeSort.json", "w+");
    int num_elementos_s = counter_elements();
    int *values_s = (int *)malloc(num_elementos_s * sizeof(int));
    readFile(values_s);
    merge_sort_sentinel(write_s, values_s, 0, num_elementos_s - 1,
                        num_elementos_s);

    FILE *write;
    write = fopen("../resultados/mergeSort.json", "w+");
    int num_elementos = counter_elements();
    int *values = (int *)malloc(num_elementos * sizeof(int));
    readFile(values);
    merge_sort(write, values, 0, num_elementos - 1, num_elementos);

    fprintf(write, "%s%d%s%d%s",
            "\n\t\"totalComparacoes\":", totalComparacoesSentinel,
            ",\n\t\"totalTrocas\":", totalTrocasSentinel, "\n\n}");

    free(values);
    free(values_s);
    return 0;
}