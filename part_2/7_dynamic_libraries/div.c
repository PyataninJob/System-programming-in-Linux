#include "div.h"

int divide(int a, int b) {
    if (b == 0) {
        return 0; // Обработка деления на ноль
    }
    return a / b;
}