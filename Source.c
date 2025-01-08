#define _CRT_SECURE_NO_WARNINGS 
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14 // число пи
#define n 10 // размер массива

double F1(double x);  // определение функции F1
double F2(double x);  // определение функции F2
double F3(double x);  // определение функции F3
void A(double xmin, double dx, double* values);  // генерирует массив значений x
void B(double* array, int N, double xmin, double xmax);  // генерирует уникальные случайные значения
void result(FILE* outputFile, double x, int func_num, double (*func)(double));  // выводит результат в файл
double find_min(double (*func)(double), double* values, int N);  // находит минимальное значение
double find_max(double (*func)(double), double* values, int N);  // находит максимальное значение
double differentiate(double (*func)(double), double x, double epsilon);  // приближенное дифференцирование

void menu(FILE* outputFile);  // меню для выбора действий

int main() {
    setlocale(LC_CTYPE, "RUS");  // установка локали для русского языка
    FILE* outputFile = fopen("output.csv", "w");  // открытие файла для записи
    menu(outputFile);  // вызов меню
    fclose(outputFile);  // закрытие файла
    return 0;
}

void menu(FILE* outputFile) {
    int choice;
    double values[n], xmin, xmax, dx, x, epsilon = 1e-5;
    int func_choice, N;
    double* array;

    while (1) {
        printf("Главное меню:\n");
        printf("1. Вычисление функции\n");
        printf("2. Табуляция функции\n");
        printf("3. Генерация уникальных случайных значений\n");
        printf("4. Определение минимального и максимального значений\n");
        printf("5. Дифференцирование функции в заданной точке\n");
        printf("6. Выход\n");
        scanf("%d", &choice);  // выбор пункта меню

        switch (choice) {
        case 1:
            printf("Выберите функцию:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);  // выбор функции
            printf("Введите значение x:\n");
            scanf("%lf", &x);  // ввод значения x
            fprintf(outputFile, "|x\t\t|F%d(x)\t\t|\n", func_choice);  // вывод в файл
            fprintf(outputFile, "_________________________\n");
            switch (func_choice) {
            case 1: result(outputFile, x, 1, F1); break;  // вычисление и вывод результата для F1
            case 2: result(outputFile, x, 2, F2); break;  // вычисление и вывод результата для F2
            case 3: result(outputFile, x, 3, F3); break;  // вычисление и вывод результата для F3
            default: printf("Некорректный выбор функции.\n");
            }
            break;
        case 2:
            printf("Выберите функцию:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);  // выбор функции
            printf("Введите начальное значение (xmin):\n");
            scanf("%lf", &xmin);  // ввод xmin
            printf("Введите шаг (dx):\n");
            scanf("%lf", &dx);  // ввод dx
            A(xmin, dx, values);  // вызов функции A для генерации значений
            fprintf(outputFile, "|x\t\t|F%d(x)\t\t|\n", func_choice);  // вывод в файл
            fprintf(outputFile, "_________________________\n");
            for (int i = 0; i < n; i++) {  // перебор значений и вычисление функции
                switch (func_choice) {
                case 1: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F1(values[i])); break;
                case 2: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F2(values[i])); break;
                case 3: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F3(values[i])); break;
                default: printf("Некорректный выбор функции.\n"); break;
                }
            }
            break;
        case 3:
            printf("Введите начальное значение (xmin):\n");
            scanf("%lf", &xmin);  // ввод xmin
            printf("Введите конечное значение (xmax):\n");
            scanf("%lf", &xmax);  // ввод xmax
            printf("Введите количество значений N:\n");
            scanf("%d", &N);  // ввод N
            array = (double*)malloc(N * sizeof(double));  // выделение памяти для массива
            B(array, N, xmin, xmax);  // вызов функции B для генерации уникальных случайных значений
            fprintf(outputFile, "Сгенерированные уникальные значения:\n");
            for (int i = 0; i < N; i++) {
                fprintf(outputFile, "%lf\n", array[i]);  // вывод значений в файл
            }
            free(array);  // освобождение памяти
            break;
        case 4:
            printf("Выберите функцию:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);  // выбор функции
            printf("Введите начальное значение (xmin):\n");
            scanf("%lf", &xmin);  // ввод xmin
            printf("Введите конечное значение (xmax):\n");
            scanf("%lf", &xmax);  // ввод xmax
            printf("Введите количество значений N:\n");
            scanf("%d", &N);  // ввод N

            array = (double*)malloc(N * sizeof(double));  // выделение памяти для массива
            B(array, N, xmin, xmax);  // генерация уникальных значений

            double (*selected_func)(double) = NULL;  // указатель на выбранную функцию
            switch (func_choice) {
            case 1: selected_func = F1; break;  // выбор F1
            case 2: selected_func = F2; break;  // выбор F2
            case 3: selected_func = F3; break;  // выбор F3
            default:
                printf("Некорректный выбор функции.\n");
                free(array);
                return;
            }

            double* func_values = (double*)malloc(N * sizeof(double));  // выделение памяти для значений функции
            if (!func_values) {
                printf("Ошибка выделения памяти для массива значений функции.\n");
                free(array);
                return;
            }

            for (int i = 0; i < N; i++) {  // вычисление значений функции для каждого аргумента
                func_values[i] = selected_func(array[i]);
            }

            double min_value = find_min(selected_func, func_values, N);  // нахождение минимального значения
            double max_value = find_max(selected_func, func_values, N);  // нахождение максимального значения

            printf("Минимальное значение: %.5lf\n", min_value);  // вывод минимального значения
            printf("Максимальное значение: %.5lf\n", max_value);  // вывод максимального значения

            free(func_values);  // освобождение памяти
            free(array);  // освобождение памяти
            break;


        case 5:
            printf("Введите точку для дифференцирования:\n");
            scanf("%lf", &x);  // ввод точки
            printf("Производная функции F1 в точке %.5lf: %.5lf\n", x, differentiate(F1, x, epsilon));  // вычисление производной
            break;
        case 6:
            printf("Выход из программы.\n");
            return;
        default:
            printf("Некорректный выбор. Попробуйте снова.\n");
            break;
        }
    }
}

double F1(double x) {
    return pow(sqrt(fabs(2 * x + 1)), 3) / ((1.0 / 3.0) * pow(sin(x), 2) - (3.0 / 7.0) * log(fabs(x)));  // определение функции F1
}
double F2(double x) {
    if (x > PI) {
        return pow(fabs(cos(x)), x);
    }
    else return pow(x, cos(x));  // определение функции F2
}
double F3(double x) {
    int k = 1000;  // Число разбиений
    double sum = 0.0;
    double dx = x / k;  // Шаг интегрирования

    for (int i = 0; i < k; i++) {
        double t = i * dx;  // Текущая точка
        sum += sin(PI / 2 * t * t);  // Добавляем вклад от текущей точки
    }

    return sum * dx;  // Умножаем на шаг для получения интеграла
}
void A(double xmin, double dx, double* values) {
    for (int i = 0; i < n; i++)
        values[i] = xmin + i * dx;  // генерация массива значений с шагом dx
}
void B(double* array, int N, double xmin, double xmax) {
    int count = 0;
    while (count < N) {
        double num = xmin + (double)rand() / RAND_MAX * (xmax - xmin);  // генерация случайного числа
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (fabs(array[i] - num) < 1e-6) {  // проверка уникальности значения
                unique = 0;
                break;
            }
        }
        if (unique) array[count++] = num;  // добавление уникального значения в массив
    }
}
void result(FILE* outputFile, double x, int func_num, double (*func)(double)) {
    fprintf(outputFile, "|%.3lf\t|%lf\t|\n", x, func(x));  // вывод результата в файл
}
double find_min(double (*func)(double), double* values, int N) {
    double min = values[0];
    for (int i = 1; i < N; i++)
        if (values[i] < min)  // поиск минимального значения
            min = values[i];
    return min;
}
double find_max(double (*func)(double), double* values, int N) {
    double max = values[0];
    for (int i = 1; i < N; i++)
        if (values[i] > max)  // поиск максимального значения
            max = values[i];
    return max;
}
double differentiate(double (*func)(double), double x, double epsilon) {
    return (func(x + epsilon) - func(x)) / epsilon;  // приближенное дифференцирование
}
