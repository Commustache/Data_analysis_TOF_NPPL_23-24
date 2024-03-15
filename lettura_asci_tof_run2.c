//Questo programma è in grado di leggere le ottuple fornite dal run1, legge il primo valore, lo salva su un file
//chiamato grc_tof_run1.txt e scarta i successivi 7


#include <stdio.h>

int main() {
    FILE *inputFile, *outputFile1, *outputFile2;
    char inputFileName[] = "grc_tof_run5.asc";
    char outputFileName1[] = "grc_tof_run5_ch1.txt";
    char outputFileName2[] = "grc_tof_run5_ch2.txt";
    float value, value1, value2;
    int i;

    // Apro il file di input in modalità lettura
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Errore nell'apertura del file di input.");
        return 1;
    }

    // Apro i files di output in modalità scrittura
    outputFile1 = fopen(outputFileName1, "w");
    if (outputFile1 == NULL) {
        printf("Errore nell'apertura del file di output.");
        return 1;
    }
    outputFile2 = fopen(outputFileName2, "w");
    if (outputFile2 == NULL) {
        printf("Errore nell'apertura del file di output.");
        return 1;
    }

    // Leggo e scarto i successivi 7 valori di ogni ottupla e scrivo il primo valore nel file di output
    while (fscanf(inputFile, "%f", &value1) == 1) {
        fprintf(outputFile1, "%f\n", value1);
        fscanf(inputFile, "%f", &value2);
        fprintf(outputFile2, "%f\n", value2);
        for (i = 0; i < 6; i++) {
            if (fscanf(inputFile, "%f", &value) != 1) {
                printf("Errore nella lettura del file di input.");
                return 1;
            }
        }
    }

    // Chiudo i file
    fclose(inputFile);
    fclose(outputFile1);
    fclose(outputFile2);


    printf("Conversione completata con successo.\n");

    return 0;
}
