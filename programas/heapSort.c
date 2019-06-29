#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int totalTrocas = 0;
int totalComparacoes = 0;
FILE *write;

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
    totalTrocas++;
}
void writeVetor(int arr[], int valA, int valB, int tamanho)
{
    fprintf(write, "\n{\"estado\": [");
    for (int i = 0; i < tamanho; i++)
    {
        if (i == tamanho - 1)
        {
            fprintf(write, "%i", arr[i]);
        }
        else
        {
            fprintf(write, "%i%s", arr[i], ",");
        }
    }
    if (valA == 0 && valB == 0)
    {
        fprintf(write, "%s%i%s%i%s", "], \"troca\":[", valA, ",", valB, "]}");
    }
    else
    {
        fprintf(write, "%s%i%s%i%s", "], \"troca\":[", valA, ",", valB, "]},");
    }
}
void heapify(int arr[], int n, int i, int tamanho)
{
    int maior = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[maior])
    {
        fprintf(write, "%s%i%s%i%s", "\n{\"comparacao\":[", l, ",", maior, "]},");
        totalComparacoes++;
        maior = l;
    }

    if (r < n && arr[r] > arr[maior])
    {
        fprintf(write, "%s%i%s%i%s", "\n{\"comparacao\":[", r, ",", maior, "]},");
        totalComparacoes++;
        maior = r;
    }

    if (maior != i)
    {
        swap(&arr[i], &arr[maior]);
        writeVetor(arr, i, maior, tamanho);
        heapify(arr, n, maior, tamanho);
    }
}

void heapSort(int arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, n);

    for (int i = n - 1; i >= 0; i--)
    {
        swap(&arr[0], &arr[i]);
        writeVetor(arr, 0, i, n);
        heapify(arr, i, 0, n);
    }
}
void printArray(int arr[], int n)
{
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("%i, ", arr[i]);
    }
    printf("\n");
}
void readFile(int *values)
{
    // Abrindo arquivo
    FILE *archive;
    archive = fopen("dados.dat", "r");

    if (!archive)
    {
        printf("Houve um erro ao abrir o arquivo");
    }
    else
    {
        char c;
        char aux[30];
        int i = 0, w = 0;

        // Leitura e armazenamento dos valores
        while ((c = fgetc(archive)) != EOF)
        {
            if (c != ',')
            {
                aux[w++] = c;
            }
            else
            {
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
int counter_elements()
{
    int cont_elements = 1;

    // Abrindo arquivo 'dados.dat'
    FILE *archive;
    archive = fopen("dados.dat", "r");

    // Contagem dos números
    if (!archive)
    {
        printf("Houve um erro ao abrir o arquivo.\n");
    }
    else
    {
        char c;
        while ((c = fgetc(archive)) != EOF)
        {
            if (c == ',')
            {
                cont_elements++;
            }
        }
    }
    fclose(archive);
    return cont_elements;
}
int main()
{

    int num_elementos = counter_elements();
    int *values = (int *)malloc(num_elementos * sizeof(int));
    readFile(values);
    write = fopen("../resultados/heapSort.json", "w+");
    fprintf(write, "%s%s", "{\n\n", "  \"estados\": [\n{ \"estado\":[");

    for (int i = 0; i < num_elementos; i++)
    {
        if (i == num_elementos - 1)
        {
            fprintf(write, "%i", values[i]);
        }
        else
        {

            fprintf(write, "%i,", values[i]);
        }
    }
    fprintf(write, "],\"comparacao\":[0,0]},");

    printArray(values, num_elementos);
    heapSort(values, num_elementos);
    fprintf(write, "%s%i%s%i%s", "],\n \"totalComparacoes\":", totalComparacoes, ",\n \"totalTrocas\":", totalTrocas, "\n }");
    printArray(values, num_elementos);
}