#include <stdio.h>

// Функция для вычисления длины строки
size_t my_strlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

// Функция для сравнения строк
int my_strncmp(const char *str1, const char *str2, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (str1[i] != str2[i])
        {
            return str1[i] - str2[i];
        }
        if (str1[i] == '\0')
        {
            return 0;
        }
    }
    return 0;
}

// Функция для поиска подстроки
char *find_substring(const char *str, const char *substr)
{
    const char *p = str;
    size_t len = my_strlen(substr);

    while (*p)
    {
        if (my_strncmp(p, substr, len) == 0)
        {
            return (char *)p;
        }
        p++;
    }
    return NULL;
}

int main()
{
    char str[100], substr[100];

    printf("Введите строку: ");
    fgets(str, sizeof(str), stdin);
    // Удаление символа новой строки
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
            break;
        }
    }

    printf("Введите подстроку: ");
    fgets(substr, sizeof(substr), stdin);
    // Удаление символа новой строки
    for (int i = 0; substr[i] != '\0'; i++)
    {
        if (substr[i] == '\n')
        {
            substr[i] = '\0';
            break;
        }
    }

    // Проверка, что подстрока короче основной строки
    if (my_strlen(substr) > my_strlen(str))
    {
        printf("Ошибка: подстрока длиннее основной строки.\n");
        return 1;
    }

    char *result = find_substring(str, substr);

    if (result)
    {
        printf("Подстрока найдена: %s\n", result);
    }
    else
    {
        printf("Подстрока не найдена.\n");
    }

    return 0;
}