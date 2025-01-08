#define _CRT_SECURE_NO_WARNINGS 
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14 // ����� ��
#define n 10 // ������ �������

double F1(double x);  // ����������� ������� F1
double F2(double x);  // ����������� ������� F2
double F3(double x);  // ����������� ������� F3
void A(double xmin, double dx, double* values);  // ���������� ������ �������� x
void B(double* array, int N, double xmin, double xmax);  // ���������� ���������� ��������� ��������
void result(FILE* outputFile, double x, int func_num, double (*func)(double));  // ������� ��������� � ����
double find_min(double (*func)(double), double* values, int N);  // ������� ����������� ��������
double find_max(double (*func)(double), double* values, int N);  // ������� ������������ ��������
double differentiate(double (*func)(double), double x, double epsilon);  // ������������ �����������������

void menu(FILE* outputFile);  // ���� ��� ������ ��������

int main() {
    setlocale(LC_CTYPE, "RUS");  // ��������� ������ ��� �������� �����
    FILE* outputFile = fopen("output.csv", "w");  // �������� ����� ��� ������
    menu(outputFile);  // ����� ����
    fclose(outputFile);  // �������� �����
    return 0;
}

void menu(FILE* outputFile) {
    int choice;
    double values[n], xmin, xmax, dx, x, epsilon = 1e-5;
    int func_choice, N;
    double* array;

    while (1) {
        printf("������� ����:\n");
        printf("1. ���������� �������\n");
        printf("2. ��������� �������\n");
        printf("3. ��������� ���������� ��������� ��������\n");
        printf("4. ����������� ������������ � ������������� ��������\n");
        printf("5. ����������������� ������� � �������� �����\n");
        printf("6. �����\n");
        scanf("%d", &choice);  // ����� ������ ����

        switch (choice) {
        case 1:
            printf("�������� �������:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);  // ����� �������
            printf("������� �������� x:\n");
            scanf("%lf", &x);  // ���� �������� x
            fprintf(outputFile, "|x\t\t|F%d(x)\t\t|\n", func_choice);  // ����� � ����
            fprintf(outputFile, "_________________________\n");
            switch (func_choice) {
            case 1: result(outputFile, x, 1, F1); break;  // ���������� � ����� ���������� ��� F1
            case 2: result(outputFile, x, 2, F2); break;  // ���������� � ����� ���������� ��� F2
            case 3: result(outputFile, x, 3, F3); break;  // ���������� � ����� ���������� ��� F3
            default: printf("������������ ����� �������.\n");
            }
            break;
        case 2:
            printf("�������� �������:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);  // ����� �������
            printf("������� ��������� �������� (xmin):\n");
            scanf("%lf", &xmin);  // ���� xmin
            printf("������� ��� (dx):\n");
            scanf("%lf", &dx);  // ���� dx
            A(xmin, dx, values);  // ����� ������� A ��� ��������� ��������
            fprintf(outputFile, "|x\t\t|F%d(x)\t\t|\n", func_choice);  // ����� � ����
            fprintf(outputFile, "_________________________\n");
            for (int i = 0; i < n; i++) {  // ������� �������� � ���������� �������
                switch (func_choice) {
                case 1: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F1(values[i])); break;
                case 2: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F2(values[i])); break;
                case 3: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F3(values[i])); break;
                default: printf("������������ ����� �������.\n"); break;
                }
            }
            break;
        case 3:
            printf("������� ��������� �������� (xmin):\n");
            scanf("%lf", &xmin);  // ���� xmin
            printf("������� �������� �������� (xmax):\n");
            scanf("%lf", &xmax);  // ���� xmax
            printf("������� ���������� �������� N:\n");
            scanf("%d", &N);  // ���� N
            array = (double*)malloc(N * sizeof(double));  // ��������� ������ ��� �������
            B(array, N, xmin, xmax);  // ����� ������� B ��� ��������� ���������� ��������� ��������
            fprintf(outputFile, "��������������� ���������� ��������:\n");
            for (int i = 0; i < N; i++) {
                fprintf(outputFile, "%lf\n", array[i]);  // ����� �������� � ����
            }
            free(array);  // ������������ ������
            break;
        case 4:
            printf("�������� �������:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);  // ����� �������
            printf("������� ��������� �������� (xmin):\n");
            scanf("%lf", &xmin);  // ���� xmin
            printf("������� �������� �������� (xmax):\n");
            scanf("%lf", &xmax);  // ���� xmax
            printf("������� ���������� �������� N:\n");
            scanf("%d", &N);  // ���� N

            array = (double*)malloc(N * sizeof(double));  // ��������� ������ ��� �������
            B(array, N, xmin, xmax);  // ��������� ���������� ��������

            double (*selected_func)(double) = NULL;  // ��������� �� ��������� �������
            switch (func_choice) {
            case 1: selected_func = F1; break;  // ����� F1
            case 2: selected_func = F2; break;  // ����� F2
            case 3: selected_func = F3; break;  // ����� F3
            default:
                printf("������������ ����� �������.\n");
                free(array);
                return;
            }

            double* func_values = (double*)malloc(N * sizeof(double));  // ��������� ������ ��� �������� �������
            if (!func_values) {
                printf("������ ��������� ������ ��� ������� �������� �������.\n");
                free(array);
                return;
            }

            for (int i = 0; i < N; i++) {  // ���������� �������� ������� ��� ������� ���������
                func_values[i] = selected_func(array[i]);
            }

            double min_value = find_min(selected_func, func_values, N);  // ���������� ������������ ��������
            double max_value = find_max(selected_func, func_values, N);  // ���������� ������������� ��������

            printf("����������� ��������: %.5lf\n", min_value);  // ����� ������������ ��������
            printf("������������ ��������: %.5lf\n", max_value);  // ����� ������������� ��������

            free(func_values);  // ������������ ������
            free(array);  // ������������ ������
            break;


        case 5:
            printf("������� ����� ��� �����������������:\n");
            scanf("%lf", &x);  // ���� �����
            printf("����������� ������� F1 � ����� %.5lf: %.5lf\n", x, differentiate(F1, x, epsilon));  // ���������� �����������
            break;
        case 6:
            printf("����� �� ���������.\n");
            return;
        default:
            printf("������������ �����. ���������� �����.\n");
            break;
        }
    }
}

double F1(double x) {
    return pow(sqrt(fabs(2 * x + 1)), 3) / ((1.0 / 3.0) * pow(sin(x), 2) - (3.0 / 7.0) * log(fabs(x)));  // ����������� ������� F1
}
double F2(double x) {
    if (x > PI) {
        return pow(fabs(cos(x)), x);
    }
    else return pow(x, cos(x));  // ����������� ������� F2
}
double F3(double x) {
    int k = 1000;  // ����� ���������
    double sum = 0.0;
    double dx = x / k;  // ��� ��������������

    for (int i = 0; i < k; i++) {
        double t = i * dx;  // ������� �����
        sum += sin(PI / 2 * t * t);  // ��������� ����� �� ������� �����
    }

    return sum * dx;  // �������� �� ��� ��� ��������� ���������
}
void A(double xmin, double dx, double* values) {
    for (int i = 0; i < n; i++)
        values[i] = xmin + i * dx;  // ��������� ������� �������� � ����� dx
}
void B(double* array, int N, double xmin, double xmax) {
    int count = 0;
    while (count < N) {
        double num = xmin + (double)rand() / RAND_MAX * (xmax - xmin);  // ��������� ���������� �����
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (fabs(array[i] - num) < 1e-6) {  // �������� ������������ ��������
                unique = 0;
                break;
            }
        }
        if (unique) array[count++] = num;  // ���������� ����������� �������� � ������
    }
}
void result(FILE* outputFile, double x, int func_num, double (*func)(double)) {
    fprintf(outputFile, "|%.3lf\t|%lf\t|\n", x, func(x));  // ����� ���������� � ����
}
double find_min(double (*func)(double), double* values, int N) {
    double min = values[0];
    for (int i = 1; i < N; i++)
        if (values[i] < min)  // ����� ������������ ��������
            min = values[i];
    return min;
}
double find_max(double (*func)(double), double* values, int N) {
    double max = values[0];
    for (int i = 1; i < N; i++)
        if (values[i] > max)  // ����� ������������� ��������
            max = values[i];
    return max;
}
double differentiate(double (*func)(double), double x, double epsilon) {
    return (func(x + epsilon) - func(x)) / epsilon;  // ������������ �����������������
}
