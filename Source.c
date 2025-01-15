#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI_CONSTANT 3.14  // значение числа Пи
#define ARRAY_SIZE 10  // размер массива

double Func1(double x);  // описание функции Func1
double Func2(double x);  // описание функции Func2
double Func3(double x);  // описание функции Func3
double* GenerateXValues(double startX, double step);  // создает массив значений x
double* GenerateUniqueRandomValues(int count, double startX, double endX, double (*func)(double));  // создает уникальные случайные значения на основе выбранной функции
void PrintResult(FILE* file, double x, int funcNumber, double (*func)(double));  // выводит результаты в файл и на экран
double FindMinValue(double (*func)(double), double* arr, int size);  // находит минимальное значение функции
double FindMaxValue(double (*func)(double), double* arr, int size);  // находит максимальное значение функции
double NumericalIntegration(double (*func)(double), double start, double end, int steps);  // численное интегрирование функции

void ShowMenu(FILE* file);  // меню для взаимодействия с пользователем

int main() {
    setlocale(LC_CTYPE, "RUS");  // установка локали для русского языка
    FILE* outputFile = fopen("output.csv", "w");  // открытие файла для записи
    ShowMenu(outputFile);  // вызов меню
    fclose(outputFile);  // закрытие файла
    return 0;
}

void ShowMenu(FILE* file) {
    int option;
    double* values;
    double startX, endX, stepSize, inputX, epsilon = 1e-5;
    int functionChoice, totalCount;

    while (1) {
        printf("Главное меню:\n");
        printf("1 - Вычисление функции\n");
        printf("2 - Табуляция функции\n");
        printf("3 - Генерация уникальных случайных значений\n");
        printf("4 - Поиск минимального и максимального значений\n");
        printf("5 - Интегрирование функции на заданном интервале\n");
        printf("6 - Выход\n");
        scanf("%d", &option);  // выбор действия

        switch (option) {
        case 1:
            printf("Выберите функцию:\n1 - Func1\n2 - Func2\n3 - Func3\n");
            scanf("%d", &functionChoice);  // выбор функции
            printf("Введите значение x:\n");
            scanf("%lf", &inputX);  // ввод значения x
            fprintf(file, "|x\t\t|Func%d(x)\t|\n", functionChoice);  // вывод в файл
            printf("|x\t\t|Func%d(x)\t|\n", functionChoice);  // вывод в консоль
            fprintf(file, "_________________________\n");
            printf("_________________________\n");
            switch (functionChoice) {
            case 1: PrintResult(file, inputX, 1, Func1); break;  // вычисление и вывод результата для Func1
            case 2: PrintResult(file, inputX, 2, Func2); break;  // вычисление и вывод результата для Func2
            case 3: PrintResult(file, inputX, 3, Func3); break;  // вычисление и вывод результата для Func3
            default: printf("Некорректный выбор функции.\n");
            }
            break;
        case 2:
            printf("Выберите функцию:\n1 - Func1\n2 - Func2\n3 - Func3\n");
            scanf("%d", &functionChoice);  // выбор функции
            printf("Введите начальное значение (startX):\n");
            scanf("%lf", &startX);  // ввод startX
            printf("Введите шаг (stepSize):\n");
            scanf("%lf", &stepSize);  // ввод stepSize
            values = GenerateXValues(startX, stepSize);  // вызов функции для генерации значений
            fprintf(file, "|x\t\t|Func%d(x)\t|\n", functionChoice);  // вывод в файл
            printf("|x\t\t|Func%d(x)\t|\n", functionChoice);  // вывод в консоль
            fprintf(file, "_________________________\n");
            printf("_________________________\n");
            for (int i = 0; i < ARRAY_SIZE; i++) {  // вычисление и вывод значений функции
                switch (functionChoice) {
                case 1:
                    fprintf(file, "|%.5lf\t|%lf\t|\n", values[i], Func1(values[i]));
                    printf("|%.5lf\t|%lf\t|\n", values[i], Func1(values[i]));  // вывод в консоль
                    break;
                case 2:
                    fprintf(file, "|%.5lf\t|%lf\t|\n", values[i], Func2(values[i]));
                    printf("|%.5lf\t|%lf\t|\n", values[i], Func2(values[i]));  // вывод в консоль
                    break;
                case 3:
                    fprintf(file, "|%.5lf\t|%lf\t|\n", values[i], Func3(values[i]));
                    printf("|%.5lf\t|%lf\t|\n", values[i], Func3(values[i]));  // вывод в консоль
                    break;
                default:
                    printf("Некорректный выбор функции.\n");
                    break;
                }
            }
            free(values);
            break;
        case 3:
            printf("Выберите функцию:\n1 - Func1\n2 - Func2\n3 - Func3\n");
            scanf("%d", &functionChoice);  // выбор функции
            printf("Введите начальное значение (startX):\n");
            scanf("%lf", &startX);  // ввод startX
            printf("Введите конечное значение (endX):\n");
            scanf("%lf", &endX);  // ввод endX
            printf("Введите количество значений (count):\n");
            scanf("%d", &totalCount);  // ввод totalCount

            double (*selectedFunc)(double) = NULL;  // указатель на выбранную функцию
            switch (functionChoice) {
            case 1: selectedFunc = Func1; break;  // выбор Func1
            case 2: selectedFunc = Func2; break;  // выбор Func2
            case 3: selectedFunc = Func3; break;  // выбор Func3
            default:
                printf("Некорректный выбор функции.\n");
                return;
            }

            values = GenerateUniqueRandomValues(totalCount, startX, endX, selectedFunc);  // генерируем уникальные значения для выбранной функции

            fprintf(file, "Сгенерированные уникальные значения:\n");
            printf("Сгенерированные уникальные значения:\n");
            for (int i = 0; i < totalCount; i++) {
                fprintf(file, "%lf\n", values[i]);  // вывод в файл
                printf("%lf\n", values[i]);  // вывод в консоль
            }
            free(values);  // освобождение памяти
            break;
        case 4:
            printf("Выберите функцию:\n1 - Func1\n2 - Func2\n3 - Func3\n");
            scanf("%d", &functionChoice);  // выбор функции
            printf("Введите начальное значение (startX):\n");
            scanf("%lf", &startX);  // ввод startX
            printf("Введите конечное значение (endX):\n");
            scanf("%lf", &endX);  // ввод endX
            printf("Введите количество значений (count):\n");
            scanf("%d", &totalCount);  // ввод totalCount

            double (*selectedFunc2)(double) = NULL;  // указатель на выбранную функцию
            switch (functionChoice) {
            case 1: selectedFunc2 = Func1; break;  // выбор Func1
            case 2: selectedFunc2 = Func2; break;  // выбор Func2
            case 3: selectedFunc2 = Func3; break;  // выбор Func3
            default:
                printf("Некорректный выбор функции.\n");
                return;
            }
            values = GenerateUniqueRandomValues(totalCount, startX, endX, selectedFunc2);  // генерация уникальных значений для выбранной функции

            double* funcResults = (double*)malloc(totalCount * sizeof(double));  // выделение памяти для значений функции

            if (funcResults == NULL) {
                printf("Ошибка выделения памяти\n");
                exit(1);
            }

            for (int i = 0; i < totalCount; i++) {  // вычисление значений функции для каждого аргумента
                funcResults[i] = selectedFunc2(values[i]);
            }

            double minVal = FindMinValue(selectedFunc2, funcResults, totalCount);  // нахождение минимального значения
            double maxVal = FindMaxValue(selectedFunc2, funcResults, totalCount);  // нахождение максимального значения

            // Вывод результатов в файл и консоль
            fprintf(file, "Минимальное значение: %.5lf\n", minVal);  // вывод в файл
            fprintf(file, "Максимальное значение: %.5lf\n", maxVal);  // вывод в файл
            printf("Минимальное значение: %.5lf\n", minVal);  // вывод в консоль
            printf("Максимальное значение: %.5lf\n", maxVal);  // вывод в консоль

            free(funcResults);  // освобождение памяти
            free(values);  // освобождение памяти
            break;

        case 5:
            printf("Введите пределы интегрирования (start и end):\n");
            scanf("%lf %lf", &startX, &endX);  // ввод start и end для интегрирования
            printf("Введите количество шагов (steps):\n");
            scanf("%d", &totalCount);  // ввод количества шагов

            double integral = NumericalIntegration(Func1, startX, endX, totalCount);  // пример с Func1
            printf("Интеграл функции Func1: %.5lf\n", integral);  // вывод результата интегрирования
            break;
        case 6:
            return;  // выход из программы
        default:
            printf("Некорректный выбор. Попробуйте снова.\n");
        }
    }
}

// Функции для Func1, Func2 и Func3
double Func1(double x) {
    return cbrt(fabs(2 * x + 1)) / ((1.0 / 3.0) * pow(sin(x), 2) - (3.0 / 7.0) * log(fabs(x)));  // определение функции F1
}


double Func2(double x) {
    if (x > PI_CONSTANT) {
        return pow(fabs(cos(x)), x);
    }
    else {
        return pow(x, cos(x));  // определение функции F2
    }
}


double Func3(double x) {
    int k = 1000;  // Число разбиений
    double sum = 0.0;
    double dx = x / k;  // Шаг интегрирования

    for (int i = 0; i < k; i++) {
        double t = i * dx;  // Текущая точка
        sum += sin(PI_CONSTANT / 2 * t * t);  // Добавляем вклад от текущей точки
    }

    return sum * dx;  // Умножаем на шаг для получения интеграла
}


double* GenerateXValues(double startX, double step) {
    double* values = (double*)malloc(ARRAY_SIZE * sizeof(double));  // выделение памяти для массива
    if (values == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        values[i] = startX + i * step;  // генерируем значения x с шагом step
    }

    return values;
}

// Функция для генерации уникальных случайных значений для выбранной функции
double* GenerateUniqueRandomValues(int count, double startX, double endX, double (*func)(double)) {
    double* values = (double*)malloc(count * sizeof(double));
    if (values == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }

    srand((unsigned int)time(NULL));  // инициализация генератора случайных чисел

    for (int i = 0; i < count; i++) {
        double randomValue;
        int unique = 0;

        while (!unique) {
            randomValue = startX + (rand() / (double)RAND_MAX) * (endX - startX);  // генерация случайного значения
            unique = 1;

            // Проверка на уникальность значений
            for (int j = 0; j < i; j++) {
                if (values[j] == randomValue) {
                    unique = 0;
                    break;
                }
            }
        }

        values[i] = randomValue;
    }

    return values;
}

// Прочие функции для вычислений, интегрирования, нахождения минимальных и максимальных значений

void PrintResult(FILE* file, double x, int funcNum, double (*func)(double)) {
    double result = func(x);
    fprintf(file, "|%.5lf\t|%lf\t|\n", x, result);
    printf("|%.5lf\t|%lf\t|\n", x, result);
}

double FindMinValue(double (*func)(double), double* arr, int size) {
    double minValue = func(arr[0]);
    for (int i = 1; i < size; i++) {
        double temp = func(arr[i]);
        if (temp < minValue) minValue = temp;
    }
    return minValue;
}

double FindMaxValue(double (*func)(double), double* arr, int size) {
    double maxValue = func(arr[0]);
    for (int i = 1; i < size; i++) {
        double temp = func(arr[i]);
        if (temp > maxValue) maxValue = temp;
    }
    return maxValue;
}

double NumericalIntegration(double (*func)(double), double start, double end, int steps) {
    double sum = 0.0;
    double stepSize = (end - start) / steps;
    for (int i = 0; i < steps; i++) {
        sum += func(start + i * stepSize) * stepSize;  // применение метода прямоугольников
    }
    return sum;
}

