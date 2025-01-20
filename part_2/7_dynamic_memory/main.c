#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h> // Добавлено для использования функции exit

struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
};

void add_abonent(struct abonent **directory, int *count);
void delete_abonent(struct abonent **directory, int *count);
void search_abonent(struct abonent *directory, int count);
void display_all(struct abonent *directory, int count);

int main() {
    struct abonent *directory = NULL;
    int count = 0;
    char choice;

    while (1) {
        printf("\nМеню:\n");
        printf("1) Добавить абонента\n");
        printf("2) Удалить абонента\n");
        printf("3) Поиск абонентов по имени\n");
        printf("4) Вывод всех записей\n");
        printf("5) Выход\n");
        printf("Введите пункт меню: ");
        scanf(" %c", &choice);

        switch (choice) {
            case '1':
                add_abonent(&directory, &count);
                break;
            case '2':
                delete_abonent(&directory, &count);
                break;
            case '3':
                search_abonent(directory, count);
                break;
            case '4':
                display_all(directory, count);
                break;
            case '5':
                free(directory);
                return 0;
            default:
                printf("Неверный пункт меню. Попробуйте снова.\n");
        }
    }
}

void add_abonent(struct abonent **directory, int *count) {
    *directory = realloc(*directory, (*count + 1) * sizeof(struct abonent));
    if (*directory == NULL) {
        printf("Ошибка выделения памяти.\n");
        exit(1);
    }

    printf("Введите имя: ");
    scanf("%s", (*directory)[*count].name);
    printf("Введите фамилию: ");
    scanf("%s", (*directory)[*count].second_name);
    printf("Введите телефон: ");
    scanf("%s", (*directory)[*count].tel);

    (*count)++;
    printf("Абонент добавлен.\n");
}

void delete_abonent(struct abonent **directory, int *count) {
    char name[10];
    printf("Введите имя абонента для удаления: ");
    scanf("%s", name);

    for (int i = 0; i < *count; i++) {
        if (strcmp((*directory)[i].name, name) == 0) {
            // Перемещение удаляемого элемента в конец массива
            struct abonent temp = (*directory)[i];
            (*directory)[i] = (*directory)[*count - 1];
            (*directory)[*count - 1] = temp;

            // Уменьшение размера массива
            *directory = realloc(*directory, (*count - 1) * sizeof(struct abonent));
            if (*directory == NULL && *count - 1 > 0) {
                printf("Ошибка выделения памяти.\n");
                exit(1);
            }

            (*count)--;
            printf("Абонент удален.\n");
            return;
        }
    }
    printf("Абонент с таким именем не найден.\n");
}

void search_abonent(struct abonent *directory, int count) {
    char name[10];
    printf("Введите имя для поиска: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(directory[i].name, name) == 0) {
            printf("Имя: %s, Фамилия: %s, Телефон: %s\n", directory[i].name, directory[i].second_name, directory[i].tel);
            found = 1;
        }
    }
    if (!found) {
        printf("Абоненты с таким именем не найдены.\n");
    }
}

void display_all(struct abonent *directory, int count) {
    if (count == 0) {
        printf("Справочник пуст.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("Имя: %s, Фамилия: %s, Телефон: %s\n", directory[i].name, directory[i].second_name, directory[i].tel);
    }
}