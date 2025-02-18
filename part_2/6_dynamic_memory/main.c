#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура абонента с указателями на предыдущий и следующий элементы
struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
    struct abonent *prev;
    struct abonent *next;
};

// Прототипы функций
void add_abonent(struct abonent **head);
void delete_abonent(struct abonent **head);
void search_abonent(struct abonent *head);
void display_all(struct abonent *head);
void free_list(struct abonent *head);

int main() {
    struct abonent *head = NULL; // Изначально список пуст
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
                add_abonent(&head);
                break;
            case '2':
                delete_abonent(&head);
                break;
            case '3':
                search_abonent(head);
                break;
            case '4':
                display_all(head);
                break;
            case '5':
                free_list(head);
                return 0;
            default:
                printf("Неверный пункт меню. Попробуйте снова.\n");
        }
    }
}

// Функция добавления абонента
void add_abonent(struct abonent **head) {
    struct abonent *new_abonent = (struct abonent *)malloc(sizeof(struct abonent));
    if (new_abonent == NULL) {
        printf("Ошибка выделения памяти.\n");
        exit(1);
    }

    printf("Введите имя: ");
    scanf("%s", new_abonent->name);
    printf("Введите фамилию: ");
    scanf("%s", new_abonent->second_name);
    printf("Введите телефон: ");
    scanf("%s", new_abonent->tel);

    new_abonent->next = *head; // Новый элемент указывает на текущую голову
    new_abonent->prev = NULL;  // У нового элемента нет предыдущего, так как он станет головой

    if (*head != NULL) {
        (*head)->prev = new_abonent; // У текущей головы предыдущий элемент теперь новый элемент
    }

    *head = new_abonent; // Новый элемент становится головой списка
    printf("Абонент добавлен.\n");
}

// Функция удаления абонента
void delete_abonent(struct abonent **head) {
    char name[10];
    printf("Введите имя абонента для удаления: ");
    scanf("%s", name);

    struct abonent *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next; // У предыдущего элемента следующий теперь текущий следующий
            } else {
                *head = current->next; // Если удаляется голова, то новая голова это следующий элемент
            }
            if (current->next != NULL) {
                current->next->prev = current->prev; // У следующего элемента предыдущий теперь текущий предыдущий
            }
            free(current);
            printf("Абонент удален.\n");
            return;
        }
        current = current->next;
    }
    printf("Абонент с таким именем не найден.\n");
}

// Функция поиска абонента
void search_abonent(struct abonent *head) {
    char name[10];
    printf("Введите имя для поиска: ");
    scanf("%s", name);

    struct abonent *current = head;
    int found = 0;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("Имя: %s, Фамилия: %s, Телефон: %s\n", current->name, current->second_name, current->tel);
            found = 1;
        }
        current = current->next;
    }
    if (!found) {
        printf("Абоненты с таким именем не найдены.\n");
    }
}

// Функция вывода всех абонентов
void display_all(struct abonent *head) {
    if (head == NULL) {
        printf("Справочник пуст.\n");
        return;
    }

    struct abonent *current = head;
    while (current != NULL) {
        printf("Имя: %s, Фамилия: %s, Телефон: %s\n", current->name, current->second_name, current->tel);
        current = current->next;
    }
}

// Функция освобождения памяти списка
void free_list(struct abonent *head) {
    struct abonent *current = head;
    while (current != NULL) {
        struct abonent *next = current->next;
        free(current);
        current = next;
    }
}