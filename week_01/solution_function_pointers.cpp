#include <iostream>

// Указател към функция: <връщан тип> (* <име (optional)>) (<тип на параметър 1>, <тип на параметър 2>, ..., <тип на параметър n>)
// С using: 
//      using MyFunctionPointer = <връщан тип> (*) (<тип на параметър 1>, <тип на параметър 2>, ..., <тип на параметър n>)
//      Използваме така: 
//          void MyFunc(MyFunctionPointer f) 
//          f(<параметър 1>, ..., <параметър n>);
// ! Ако функцията няма параметри пишем <връщан_тип> (* <име (optional)>)(void) !

// Задача 5:
using VoidFunctionPointer = void (*) (void);
void Loop(VoidFunctionPointer f, const unsigned int n)
{
    for(unsigned int i = 0; i < n; i++)
        f();
}
// Ако не използвахме using декларацията щеше да изглежда така:
//      void Loop(void (* f) (void), const unsigned int n)

// Задача 6:
using IntToIntFunctionPointer = int (*) (int);
int Repeat(IntToIntFunctionPointer f, int x0, unsigned int n)
{
    for(unsigned int i = 0; i < n; i++)
        x0 = f(x0);
    return x0;
}

// Задача 7:
using BoolFunctionPointer = bool (*) (int, int);
void Sort(int* arr, const size_t size, BoolFunctionPointer f)
{
    for(int i = 0; i < size - 1; i++)
    {
        for(int j = i + 1; j < size; j++)
        {
            if(f(arr[i], arr[j]))
                std::swap(arr[i], arr[j]);
        }
    }
}

// Задача 8:
template <class T>
using TtoTFunctionPointer = T (*) (T);

template <class T>
T ApplyInOrder(const TtoTFunctionPointer<T>* fs, const size_t* arr, const size_t arr_size, int x)
{
    for(size_t i = 0; i < arr_size; i++)
        x = fs[arr[i]](x);
    
    return x;
}

// Тестови функции

void test_func1()
{
    std::cout << "a ";
}

int test_func2(int a)
{
    return a + 1;
}

bool test_func3(int a, int b)
{
    return a > b;
}

// copied from solution_templates
template <class T>
void Print(const T* arr, const size_t size)
{
    for(size_t i = 0; i < size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

int inc(int x) { return x + 1; }
int mul2(int x) { return x * 2; }
int dec(int x) { return x - 1; }

int main()
{
    Loop(test_func1, 5);
    std::cout << std::endl;

    std::cout << Repeat(test_func2, 0, 5) << std::endl;

    int test_arr[] = {1, 3, 2, 0, 5};
    Sort(test_arr, 5, test_func3);
    Print(test_arr, 5);

    int (*funcs[])(int) = {inc, mul2, dec};
    size_t order1[] = {2, 0, 1};
    size_t order2[] = {0, 1, 2};
    std::cout << ApplyInOrder<int>(funcs, order1, 3, 3) << std::endl; // ((3 - 1) + 1) * 2 = 6
    std::cout << ApplyInOrder<int>(funcs, order2, 3, 3) << std::endl; // ((3 + 1) * 2) - 1 = 7

    return 0;
}