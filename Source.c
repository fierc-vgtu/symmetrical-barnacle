#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
#define PI_CONSTANT 3.14  // значение числа Пи
#define ARRAY_SIZE 10  // размер массива
 
typedef struct {
   int functionChoice;          // Номер выбранной функции
   double inputX;               // Входное значение x
   double* values;              // Массив значений x
   double minValue;             // Минимальное значение функции
   double maxValue;             // Максимальное значение функции
   int totalCount;              // Общее количество значений
   FILE* file;                  // Файл для записи
} ProgramState;
 
double Func1(double x);  // описание функции Func1
double Func2(double x);  // описание функции Func2
double Func3(double x);  // описание функции Func3
double* Generatearray(double startX, double step);  // создает массив значений x
double* GenerateZnach(int count, double startX, double endX);  // создает уникальные случайные значения
void PrintResult(ProgramState* state, double x, double (*func)(double));  // выводит результаты в файл и на экран
double FindMin(double (*func)(double), double* arr, int size);  // находит минимальное значение функции
double FindMax(double (*func)(double), double* arr, int size);  // находит максимальное значение функции
double Integration(double (*func)(double), double start, double end, int steps);  // численное интегрирование функции
 
void ShowMenu(ProgramState* state);  // меню для взаимодействия с пользователем
 
int main() {
   setlocale(LC_CTYPE, "RUS");  // установка локали для русского языка
   ProgramState state = { 0 };
   state.file = fopen("output.csv", "w");  // открытие файла для записи
 
   if (state.file == NULL) {
       printf("Ошибка открытия файла\n");
       return 1;
   }
 
   ShowMenu(&state);  // вызов меню
   fclose(state.file);  // закрытие файла
 
   return 0;
}
 
void ShowMenu(ProgramState* state) {
   int option;
 
   while (1) {
       printf("Главное меню:\n");
       printf("1 - Вычисление функции\n");
       printf("2 - Табуляция функции\n");
       printf("3 - Генерация уникальных случайных значений\n");
       printf("4 - Поиск минимального и максимального значений\n");
       printf("5 - Интегрирование функции на заданном интервале\n");
       printf("6 - Выход\n");
       scanf("%d", &option);
 
       switch (option) {
       case 1:
           printf("Выберите функцию:\n1 - Func1\n2 - Func2\n3 - Func3\n");
           scanf("%d", &state->functionChoice);
           printf("Введите значение x:\n");
           scanf("%lf", &state->inputX);
 
           switch (state->functionChoice) {
           case 1: PrintResult(state, state->inputX, Func1); break;
           case 2: PrintResult(state, state->inputX, Func2); break;
           case 3: PrintResult(state, state->inputX, Func3); break;
           default: printf("Некорректный выбор функции.\n");
           }
           break;
 
       case 2: {
           double startX, step;
           printf("Введите начальное значение (startX):\n");
           scanf("%lf", &startX);
           printf("Введите шаг (step):\n");
           scanf("%lf", &step);
 
           state->values = Generatearray(startX, step);
           printf("|x\t\t|Func%d(x)\t|\n", state->functionChoice);
 
           for (int i = 0; i < ARRAY_SIZE; i++) {
               double result = (state->functionChoice == 1) ? Func1(state->values[i]) :
                   (state->functionChoice == 2) ? Func2(state->values[i]) : Func3(state->values[i]);
 
               printf("|%.5lf\t|%lf\t|\n", state->values[i], result);
               fprintf(state->file, "|%.5lf\t|%lf\t|\n", state->values[i], result);
           }
 
           free(state->values);
           break;
       }
 
       case 3: {
           double startX, endX;
           printf("Введите начальный и конечный диапазоны (startX, endX):\n");
           scanf("%lf %lf", &startX, &endX);
 
           printf("Введите количество значений (count):\n");
           scanf("%d", &state->totalCount);
 
           state->values = GenerateZnach(state->totalCount, startX, endX);
           printf("Сгенерированные уникальные значения:\n");
 
           for (int i = 0; i < state->totalCount; i++) {
               printf("%lf\n", state->values[i]);
               fprintf(state->file, "%lf\n", state->values[i]);
           }
 
           free(state->values);
           break;
       }
 
       case 4:
           printf("Введите количество значений (count):\n");
           scanf("%d", &state->totalCount);
 
           state->values = GenerateZnach(state->totalCount, -10, 10);
 
           state->minValue = FindMin(Func1, state->values, state->totalCount);
           state->maxValue = FindMax(Func1, state->values, state->totalCount);
 
           printf("Минимальное значение: %.5lf\n", state->minValue);
           printf("Максимальное значение: %.5lf\n", state->maxValue);
 
           fprintf(state->file, "Минимальное значение: %.5lf\n", state->minValue);
           fprintf(state->file, "Максимальное значение: %.5lf\n", state->maxValue);
 
           free(state->values);
           break;
 
       case 5: {
           double start, end;
           printf("Введите начало и конец интервала (start, end):\n");
           scanf("%lf %lf", &start, &end);
 
           printf("Введите количество шагов (steps):\n");
           scanf("%d", &state->totalCount);
 
           double integral = Integration(Func1, start, end, state->totalCount);
           printf("Интеграл функции Func1: %.5lf\n", integral);
           fprintf(state->file, "Интеграл функции Func1: %.5lf\n", integral);
           break;
       }
 
       case 6:
           return;
 
       default:
           printf("Некорректный выбор. Попробуйте снова.\n");
       }
   }
}
 
double Func1(double x) {
   return cbrt(fabs(2 * x + 1)) / ((1.0 / 3.0) * pow(sin(x), 2) - (3.0 / 7.0) * log(fabs(x)));
}
 
double Func2(double x) {
   return (x > PI_CONSTANT) ? pow(fabs(cos(x)), x) : pow(x, cos(x));
}
 
double Func3(double x) {
   int k = 1000;
   double sum = 0.0, dx = x / k;
 
   for (int i = 0; i < k; i++) {
       double t = i * dx;
       sum += sin(PI_CONSTANT / 2 * t * t);
   }
 
   return sum * dx;
}
 
double* Generatearray(double startX, double step) {
   double* values = (double*)malloc(ARRAY_SIZE * sizeof(double));
   if (values == NULL) {
       printf("Ошибка выделения памяти\n");
       exit(1);
   }
 
   for (int i = 0; i < ARRAY_SIZE; i++) {
       values[i] = startX + i * step;
   }
 
   return values;
}
 
double* GenerateZnach(int count, double startX, double endX) {
   double* values = (double*)malloc(count * sizeof(double));
   if (values == NULL) {
       printf("Ошибка выделения памяти\n");
       exit(1);
   }
 
   srand((unsigned int)time(NULL));
 
   for (int i = 0; i < count; i++) {
       double randomValue;
       int unique = 0;
 
       while (!unique) {
           randomValue = startX + (rand() / (double)RAND_MAX) * (endX - startX);
           unique = 1;
 
           for (int j = 0; j < i; j++) {
               if (fabs(values[j] - randomValue) < 1e-6) {
                   unique = 0;
                   break;
               }
           }
       }
 
       values[i] = randomValue;
   }
 
   return values;
}
 
double FindMin(double (*func)(double), double* arr, int size) {
   double minValue = func(arr[0]);
   for (int i = 1; i < size; i++) {
       double temp = func(arr[i]);
       if (temp < minValue) minValue = temp;
   }
   return minValue;
}
 
double FindMax(double (*func)(double), double* arr, int size) {
   double maxValue = func(arr[0]);
   for (int i = 1; i < size; i++) {
       double temp = func(arr[i]);
       if (temp > maxValue) maxValue = temp;
   }
   return maxValue;
}
 
double Integration(double (*func)(double), double start, double end, int steps) {
   double sum = 0.0;
   double stepSize = (end - start) / steps;
 
   for (int i = 0; i < steps; i++) {
       sum += func(start + i * stepSize) * stepSize;
   }
 
   return sum;
}
 
void PrintResult(ProgramState* state, double x, double (*func)(double)) {
   double result = func(x);
   fprintf(state->file, "|%.5lf\t|%lf\t|\n", x, result);
   printf("|%.5lf\t|%lf\t|\n", x, result);
}