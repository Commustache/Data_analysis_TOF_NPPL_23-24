//Questo programma è in grado di leggere le ottuple fornite dal run1, legge il primo valore, lo salva su un file
//chiamato grc_tof_run1.txt e scarta i successivi 7


#include <stdio.h>

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[] = "grc_tof_run1.asc"; //just for example
    char outputFileName[] = "grc_tof_run1.txt"; //just for example
    float value;
    int i;

    // Apro il file di input in modalità lettura
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Errore nell'apertura del file di input.");
        return 1;
    }

    // Apro il file di output in modalità scrittura
    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("Errore nell'apertura del file di output.");
        return 1;
    }

    // Leggo e scarto i successivi 7 valori di ogni ottupla e scrivo il primo valore nel file di output
    while (fscanf(inputFile, "%f", &value) == 1) {
        fprintf(outputFile, "%f\n", value);
        for (i = 0; i < 7; i++) {
            if (fscanf(inputFile, "%f", &value) != 1) {
                printf("Errore nella lettura del file di input.");
                return 1;
            }
        }
    }

    // Chiudo i file
    fclose(inputFile);
    fclose(outputFile);

    printf("Conversione completata con successo.\n");

    return 0;
}
