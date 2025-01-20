#include <stdio.h>

#define MAX_ABONENTS 100

struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
};

void add_abonent(struct abonent *directory, int *count);
void delete_abonent(struct abonent *directory, int *count);
void search_abonent(struct abonent *directory, int count);
void display_all(struct abonent *directory, int count);
int my_strcmp(const char *str1, const char *str2);
void my_memset(void *ptr, int value, size_t num);

int main() {
    struct abonent directory[MAX_ABONENTS] = {0};
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
                add_abonent(directory, &count);
                break;
            case '2':
                delete_abonent(directory, &count);
                break;
            case '3':
                search_abonent(directory, count);
                break;
            case '4':
                display_all(directory, count);
                break;
            case '5':
                return 0;
            default:
                printf("Неверный пункт меню. Попробуйте снова.\n");
        }
    }
}

void add_abonent(struct abonent *directory, int *count) {
    if (*count >= MAX_ABONENTS) {
        printf("Справочник переполнен. Невозможно добавить нового абонента.\n");
        return;
    }

    printf("Введите имя: ");
    scanf("%s", directory[*count].name);
    printf("Введите фамилию: ");
    scanf("%s", directory[*count].second_name);
    printf("Введите телефон: ");
    scanf("%s", directory[*count].tel);

    (*count)++;
    printf("Абонент добавлен.\n");
}

void delete_abonent(struct abonent *directory, int *count) {
    char name[10];
    printf("Введите имя абонента для удаления: ");
    scanf("%s", name);

    for (int i = 0; i < *count; i++) {
        if (my_strcmp(directory[i].name, name) == 0) {
            // Удаление абонента путем заполнения нулями
            my_memset(&directory[i], 0, sizeof(struct abonent));
            printf("Абонент удален.\n");
            // (*count)--; // Сам напаролся

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
        if (my_strcmp(directory[i].name, name) == 0) {
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
        if (directory[i].name[0] != '\0') { // Проверка, что запись не удалена
            printf("Имя: %s, Фамилия: %s, Телефон: %s\n", directory[i].name, directory[i].second_name, directory[i].tel);
        }
    }
}

// Функция для сравнения строк
int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Функция для заполнения памяти
void my_memset(void *ptr, int value, size_t num) {
    unsigned char *p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
}