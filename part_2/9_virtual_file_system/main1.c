#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    const char *filename = "output.txt";
    const char *text = "String from file";
    char ch;
    long file_size;

    // Открытие файла для записи
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка при открытии файла для записи");
        return 1;
    }

    // Запись строки в файл
    fprintf(file, "%s", text);
    fclose(file);

    // Открытие файла для чтения
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла для чтения");
        return 1;
    }

    // Определение размера файла
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);

    // Чтение файла с конца и вывод на экран
    for (long i = file_size - 1; i >= 0; i--) {
        fseek(file, i, SEEK_SET);
        ch = fgetc(file);
        putchar(ch);
    }
    putchar('\n');

    fclose(file);
    return 0;
}