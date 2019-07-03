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

void insertionSort(int array[], int size) {
    FILE *write;
    write = fopen("../resultados/insertionSort.json", "w+");
    fprintf(write, "%s%s", "{\n\n", "  \"estados\": [\n");

    int j, key, totalComparacoes = 0, totalTrocas = 0;
    fprintf(write, "   {");
    fprintf(write, "    \"estado\": [");

    // Impressão estado inicial
    for (int f = 0; f < size; f++) {
        fprintf(write, "%d", array[f]);
        if (f != size - 1) fprintf(write, "%s", ",");
    }
    fprintf(write, "],");

    // começa
    for (int i = 1; i < size; i++) {
        key = array[i];
        j = i - 1;
        // comparação
        printf("\n");
        for (int w = j; w >= 0; w--) {
            fprintf(write, "%s%d%s%d%s", " \"comparacao\": [", i, ",", w, "]}");
            if (i == size - 1 && w == 0 &&
                !(i == size - 1 && j + 1 == size - 1)) {
                fprintf(write, "%s", "],");
            } else {
                fprintf(write, "%s", ",    \n\t {");
            }
        }

        while (j >= 0 && array[j] > key) {
            totalComparacoes++;
            printf("j->%d\t", j);
            printf("i->%d\n", i);
            array[j + 1] = array[j];
            j = j - 1;
        }
        // troca
        totalTrocas++;
        array[j + 1] = key;
        //{ "estado": [], "troca":[]},
        if (!(i == size - 1 && j + 1 == size - 1)) {
            fprintf(write, "%s", "    \"estado\": [");
            for (int z = 0; z < size; z++) {
                fprintf(write, "%d", array[z]);
                if (z != size - 1) fprintf(write, "%s", ",");
            }
            fprintf(write, "%s%d%s%d%s", "], \"troca\": [", i, ",", j + 1,
                    "]}");
        }
        if (i == size - 1 && array[size - 1] != key) {
            fprintf(write, "%s", "],\n");
        } else {
            fprintf(write, "%s", ",\n   {");
        }
    }

    fprintf(write, "%s%d%s", "\n\t\"totalComparacoes\":", totalComparacoes,
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
    printf("\n\n%d\n\n", num_elementos);
    int *values = (int *)malloc(num_elementos * sizeof(int));
    readFile(values);

    imprime_vetor(values, num_elementos);
    insertionSort(values, num_elementos);
    imprime_vetor(values, num_elementos);
    return 0;
}
