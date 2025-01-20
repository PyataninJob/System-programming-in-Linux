#include <stdio.h>
#include <stdlib.h>

void task1()
{
    int N;
    printf("Введите размер матрицы N: ");
    scanf("%d", &N);

    if (N <= 0)
    {
        printf("Размер матрицы должен быть положительным числом.\n");
        return;
    }

    int **matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        matrix[i] = (int *)malloc(N * sizeof(int));
    }

    int value = 1;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = value++;
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void task2()
{
    int N;
    printf("Введите размер массива N: ");
    scanf("%d", &N);

    if (N <= 0)
    {
        printf("Размер массива должен быть положительным числом.\n");
        return;
    }

    int *array = (int *)malloc(N * sizeof(int));
    if (array == NULL)
    {
        printf("Ошибка выделения памяти.\n");
        return;
    }

    printf("Введите элементы массива: ");
    for (int i = 0; i < N; i++)
    {
        scanf("%d", &array[i]);
    }

    printf("Массив в обратном порядке: ");
    for (int i = N - 1; i >= 0; i--)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);
}

void task3()
{
    int N;
    printf("Введите размер матрицы N: ");
    scanf("%d", &N);

    if (N <= 0)
    {
        printf("Размер матрицы должен быть положительным числом.\n");
        return;
    }

    int **matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        matrix[i] = (int *)malloc(N * sizeof(int));
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i <= j)
            {
                matrix[i][j] = 1;
            }
            else
            {
                matrix[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("Повернутая как в примере\n");
    for (int j = 0; j < N; j++)
    {
        for (int i = N - 1; i >= 0; i--)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void task4()
{
    int N;
    printf("Введите размер матрицы N: ");
    scanf("%d", &N);

    if (N <= 0)
    {
        printf("Размер матрицы должен быть положительным числом.\n");
        return;
    }

    int **matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        matrix[i] = (int *)malloc(N * sizeof(int));
    }

    int value = 1;
    int top = 0, bottom = N - 1, left = 0, right = N - 1; // Индексы 4-х углов.
    while (value <= N * N)
    {
    // Реализация проста: Цикл идет так чтобы заполнять по прямой от одного индекса до другого. 
    // пройдемся по строчке вправо
        for (int i = left; i <= right && value <= N * N; i++)
        {
            matrix[top][i] = value++;
        }
        // перенесем начальную точку для заполнения ^ вниз
        top++;
    // пройдемся по стобцу вниз

        for (int i = top; i <= bottom && value <= N * N; i++)
        {
            matrix[i][right] = value++;
        }
        // перенесем конечную точку для заполнения ^ вверх
        right--;

        for (int i = right; i >= left && value <= N * N; i--)
        {
            matrix[bottom][i] = value++;
        }
        // аналогично для нижней границе

        bottom--;
        for (int i = bottom; i >= top && value <= N * N; i--)
        {
            matrix[i][left] = value++;
        }
        // аналогично для левой граници
        left++;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

int main()
{
    int choice;
    while (1)
    {
        printf("Выберите задание (1-4) или 0 для выхода: ");
        scanf("%d", &choice);

        if (choice == 0)
        {
            break;
        }

        switch (choice)
        {
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