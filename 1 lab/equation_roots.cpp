#include <locale>
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <exception>
#include <Windows.h>

double func1(double x);
double derfunc1(double x);
double bisection_localize_root(double func(double), double a, double b, double eps);
double newtons_localize_root(double func(double), double derfunc(double), double a, double b, double eps);
double newtons_modified_localize_root(double func(double), double derfunc(double), double a, double b, double eps);
double secant_localize_root(double func(double), double a, double b, double eps);
std::vector<std::array<double, 2>> separate_root_segments(double func(double), double a, double b, int n);


void turn_console_text_red()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_RED);
}
void turn_console_text_green()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
}
void turn_console_text_white()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


int main()
{
    setlocale(LC_CTYPE, "Russian");
    int n;
    double a, b, eps, root;

    // ��������� ������
    a = -2;
    b = 1;
    n = 100;
    eps = 1e-8;

    // ��������� ������
    turn_console_text_red();
    std::cout << "��������� ������ ������� ���������� ���������" << std::endl << std::endl;
    std::cout << "A=" << a << ", B=" << b << ", f(x)=x^2-sin(5x), " << "eps=" << eps << std::endl << std::endl;
    turn_console_text_white();

    std::vector<std::array<double, 2>> segments = separate_root_segments(func1, a, b, n);
    std::cout << "��������� ������: ����� �������� " << segments.size() << " ��������." << std::endl;
    for(int i = 0; i < segments.size(); ++i)
    {
        std::cout << i + 1 << "-� �������: [" << segments[i][0] << " " << segments[i][1] << "]." << std::endl;
    }
    std::cout << std::endl;


    // ����������� ������
    for (auto elem : segments)
    {
        turn_console_text_red();
        std::cout << "������� [" << elem[0] << " " << elem[1] << "]." << std::endl;
        turn_console_text_white();
        turn_console_text_green();
        std::cout << "����� ��������" << std::endl;
        turn_console_text_white();
        root = bisection_localize_root(func1, elem[0], elem[1], eps);
        std::cout << "����������� �����: "<< root <<"." << std::endl;

        turn_console_text_green();
        std::cout << "����� ������" << std::endl;
        turn_console_text_white();
        root = newtons_localize_root(func1, derfunc1, elem[0], elem[1], eps);
       std::cout << "����������� �����: " << root << "." << std::endl;

       turn_console_text_green();
       std::cout << "���������������� ����� �������:" << std::endl;
       turn_console_text_white();
       root = newtons_modified_localize_root(func1, derfunc1, elem[0], elem[1], eps);
       std::cout << "����������� �����: " << root << "." << std::endl;
   

       turn_console_text_green();
       std::cout << "����� �������" << std::endl;
       turn_console_text_white();
       root = secant_localize_root(func1, elem[0], elem[1], eps);
       std::cout << "����������� �����: " << root << "." << std::endl << std::endl;
       
    }

}


// ������� ��������� ������ n >= 2
std::vector<std::array<double, 2>> separate_root_segments(double func(double), double a, double b, int n)
{
    if (n < 2)
        throw(std::invalid_argument("Value should be greater than or equals 2."));
    double difference = b - a, left_border = a, right_border;
    std::vector<std::array<double, 2>> res;

    for (int i = 1; i <= n; ++i)
    {
        right_border = a + i * difference / n;
        if (func(left_border) * func(right_border) <= 0)
            res.push_back({ left_border, right_border });
        left_border = right_border;
    }
    return res;
}

double func1(double x)
{
    return x * x - std::sin(5*x);
}

double derfunc1(double x)
{
    return 2 * x - 5 * std::cos(5*x);
}


double bisection_localize_root(double func(double), double a, double b, double eps)
{
    if (func(a) * func(b) > 0)
       throw(std::invalid_argument("������� �� ������ ���� �� ���� �������."));

    if (func(a) == 0)
    {
        std::cout << "������ ����� �� ����� ������� �������." << std::endl;
        return a;
    }
        
    if (func(b) == 0)
    {
        std::cout << "������ ����� �� ������ ������� �������." << std::endl;
        return b;
    }

    int i = 0;
    double left_border = a, right_border = b, middle = (left_border + right_border) / 2;

    while (right_border - left_border > 2 * eps)
    {
        if (func(middle) == 0)
            break;

        ++i;
        if (func(left_border) * func(middle) < 0)
            right_border = middle;
        else if (func(middle) * func(right_border) < 0)
            left_border = middle;
        middle = (left_border + right_border) / 2;
    }

    std::cout << "��������� �����������: " << (a + b) / 2 << std::endl;
    std::cout << "���������� �����: " << i << std::endl;
    std::cout << "����� ���������� �������: " << right_border - left_border << std::endl;
    std::cout << "������� |f(x_m)|: " << std::abs(func(middle)) << std::endl;
    return middle;
}

double newtons_localize_root(double func(double), double derfunc(double), double a, double b, double eps)
{
    if (func(a) * func(b) > 0)
        throw(std::invalid_argument("������� �� ������ ���� �� ���� �������."));

    if (func(a) == 0)
    {
        std::cout << "������ ����� �� ����� ������� �������." << std::endl;
        return a;
    }

    if (func(b) == 0)
    {
        std::cout << "������ ����� �� ������ ������� �������." << std::endl;
        return b;
    }

    double x_k = a + (b - a) / 2;
    std::cout << "��������� �����������: " << x_k << std::endl;
    int i = 0;
    double prev_x_k;
    do
    {
        if (func(x_k) == 0)
            break;
        ++i;
        prev_x_k = x_k;
        x_k = prev_x_k - (func(prev_x_k) / derfunc(prev_x_k));
    }
    while (std::abs(prev_x_k - x_k) > eps);
    if (x_k < a || x_k > b || x_k != x_k)
    {
        std::cout << "������ ������� �� ����������� �������: " << x_k << " �� � ["<< a << " " << b << "]" << std::endl;
        return x_k;
    }
        
    else
    {
        std::cout << "���������� �����: " << i << std::endl;
        std::cout << "����� ���������� �������: " << std::abs(prev_x_k - x_k) << std::endl;
        std::cout << "������� |f(x_m)|: " << std::abs(func(x_k)) << std::endl;
    }

    return x_k;
}

double newtons_modified_localize_root(double func(double), double derfunc(double), double a, double b, double eps)
{
    if (func(a) * func(b) > 0)
        throw(std::invalid_argument("������� �� ������ ���� �� ���� �������."));

    if (func(a) == 0)
    {
        std::cout << "������ ����� �� ����� ������� �������." << std::endl;
        return a;
    }

    if (func(b) == 0)
    {
        std::cout << "������ ����� �� ������ ������� �������." << std::endl;
        return b;
    }

    double x_k = a + (b - a) / 2, der = derfunc(x_k);
    std::cout << "��������� �����������: " << x_k << std::endl;
    int i = 0;
    double prev_x_k;
    do
    {
        if (func(x_k) == 0)
            break;
        ++i;
        prev_x_k = x_k;
        x_k = prev_x_k - (func(prev_x_k) / der);
    } while (std::abs(prev_x_k - x_k) > eps);

    if (x_k < a || x_k > b || x_k != x_k)
    {
        std::cout << "������ ������� �� ����������� �������: " << x_k << "�� � [" << a << " " << b << "]" << std::endl;
        return x_k;
    }

    else
    {
        std::cout << "���������� �����: " << i << std::endl;
        std::cout << "����� ���������� �������: " << std::abs(prev_x_k - x_k) << std::endl;
        std::cout << "������� |f(x_m)|: " << std::abs(func(x_k)) << std::endl;
    }
    return x_k;
}

double secant_localize_root(double func(double), double a, double b, double eps)
{
    if (func(a) * func(b) > 0)
        throw(std::invalid_argument("������� �� ������ ���� �� ���� �������."));

    if (func(a) == 0)
    {
        std::cout << "������ ����� �� ����� ������� �������." << std::endl;
        return a;
    }

    if (func(b) == 0)
    {
        std::cout << "������ ����� �� ������ ������� �������." << std::endl;
        return b;
    }

    double x_k = a, prev_x_k = b, next_x_k;
    std::cout << "��������� �����������: " << x_k << std::endl;
    int i = 0;
    while (std::abs(prev_x_k - x_k) > eps)
    {
        if (func(x_k) == 0)
            break;
        ++i;

        next_x_k = x_k - (func(x_k) / (func(x_k) - func(prev_x_k))) * (x_k - prev_x_k);
        prev_x_k = x_k;
        x_k = next_x_k;
    }
    if (x_k < a || x_k > b || x_k != x_k)
    {
        std::cout << "������ ������� �� ����������� �������: " << x_k << "�� � [" << a << " " << b << "]" << std::endl;
        return x_k;
    }

    else
    {
        std::cout << "���������� �����: " << i << std::endl;
        std::cout << "����� ���������� �������: " << std::abs(prev_x_k - x_k) << std::endl;
        std::cout << "������� |f(x_m)|: " << std::abs(func(x_k)) << std::endl;
    }
    return x_k;
}