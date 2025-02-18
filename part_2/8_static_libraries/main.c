#include <stdio.h>
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"

void display_menu() {
    printf("\nМеню:\n");
    printf("1) Сложение\n");
    printf("2) Вычитание\n");
    printf("3) Умножение\n");
    printf("4) Деление\n");
    printf("5) Выход\n");
    printf("Введите пункт меню: ");
}

int main() {
    int choice, a, b, result;

    while (1) {
        display_menu();
        scanf("%d", &choice);

        if (choice == 5) {
            break;
        }

        printf("Введите два числа: ");
        scanf("%d %d", &a, &b);

        switch (choice) {
            case 1:
                result = add(a, b);
                printf("Результат: %d\n", result);
                break;
            case 2:
                result = sub(a, b);
                printf("Результат: %d\n", result);
                break;
            case 3:
                result = mul(a, b);
                printf("Результат: %d\n", result);
                break;
            case 4:
                result = divide(a, b);
                if (b == 0) {
                    printf("Ошибка: деление на ноль.\n");
                } else {
                    printf("Результат: %d\n", result);
                }
                break;
            default:
                printf("Неверный пункт меню. Попробуйте снова.\n");
        }
    }

    return 0;
}