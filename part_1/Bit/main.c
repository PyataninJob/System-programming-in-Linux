#include <stdio.h>

void print_binary(unsigned int num) {
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--) {
        printf("%u", (num >> i) & 1);
    }
    printf("\n");
}

void print_binary_signed(int num) {
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--) {
        printf("%u", (num >> i) & 1);
    }
    printf("\n");
}

void task1() {
    unsigned int num;
    printf("Введите целое положительное число: ");
    scanf("%u", &num);

    printf("Двоичное представление числа %u:\n", num);
    print_binary(num);
}

void task2() {
    int num;
    printf("Введите целое отрицательное число: ");
    scanf("%d", &num);

    printf("Двоичное представление числа %d:\n", num);
    print_binary_signed(num);
}

void task3() {
    unsigned int num;
    printf("Введите целое положительное число: ");
    scanf("%u", &num);
    printf("Двоичное представление числа %u:\n", num);
    print_binary(num);

    int count = 0;
    unsigned int temp = num;
    while (temp) {
        count += temp & 1;
        temp >>= 1;
    }

    printf("Количество единиц в двоичном представлении: %d\n", count);
}

void task4() {
    int num;
    unsigned int new_byte;
    printf("Введите целое положительное число: ");
    scanf("%d", &num);
    print_binary((unsigned int)num);

    printf("Введите новое значение для третьего байта (0-255): ");
    scanf("%u", &new_byte);
    // В задании не написано что такого не может быть. так что поставим заглушку. 
    // Проверка, что значение new_byte не превышает 255
    if (new_byte > 255) {
        printf("Ошибка: значение для третьего байта должно быть в диапазоне 0-255.\n");
        return;
    }

    print_binary((unsigned int)new_byte);

    // Обнуляем третий байт
    // 0xFF = 00000000 00000000 00000000 11111111, сдвигаем на 16 битов и получаем 00000000 11111111 00000000 00000000
    // Инвертируем чтобы получить 11111111 00000000 11111111 11111111 и так очищаем 3 байт через И
    num &= ~(0xFF << 16);
    printf("Двоичное представление числа после обнуления третьего байта:\n");
    print_binary((unsigned int)num);

    // Устанавливаем новое значение для третьего байта
    // new_byte это что-то состоящее из 8 битов. сдвигаем на 16 чтобы получить 00000000 {new_byte} 00000000 00000000
    num |= (new_byte << 16);

    printf("Новое число: %d\n", num);
    printf("Двоичное представление нового числа:\n");
    print_binary((unsigned int)num);
}

int main() {
    int choice;
    while (1) {
        printf("Выберите задание (1-4) или 0 для выхода: ");
        scanf("%d", &choice);

        if (choice == 0) {
            break;
        }

        switch (choice) {
            case 1:
                task1();
                break;
            case 2:
                task2();
                break;
            case 3:
                task3();
                break;
            case 4:
                task4();
                break;
            default:
                printf("Неверный выбор.\n");
                break;
        }
    }

    return 0;
}