#include <iostream>

// Задача 0: 
template <class T>
void Print(const T* arr, const size_t size)
{
    for(size_t i = 0; i < size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

// Задача 1:
template <class T>
T Sum(const T* arr, const size_t size)
{
    if(size == 0)
        return T();

    T res = arr[0];
    for(size_t i = 1; i < size; i++)
        res += arr[i];
    
    return res;
}
// Ако Т няма оператор + -> грешка!

// Задача 3:
template <class T>
T MaxElement(const T* arr, const size_t size)
{
    if(size == 0)
        return T();

    T res = arr[0];
    for(size_t i = 1; i < size; i++)
    {
        if(arr[i] > res)
            res = arr[i];
    }
    
    return res;
}

// Задача 4:
template <class T>
int PositiveElementsCount(const T* arr, const size_t size)
{
    int count = 0;
    T zero = T(0);
    for(size_t i = 0; i < size; i++)
    {
        if(arr[i] >= zero)
            count++;
    }
    return count;
}

// Задача 5:
template <class T>
T* FilterMoreThan(const T* arr, const size_t size, const T k, T& filtered_size) 
{
    filtered_size = 0; // Добре е да се подсигурим, че filtered_size има правилна начална стойности
    for(size_t i = 0; i < size; i++)
    {
        if(arr[i] > k)
            filtered_size++;
    }

    T* filtered = new T[filtered_size]();
    size_t idx = 0;

    for(size_t i = 0; i < size; i++)
    {
        if(arr[i] > k)
            filtered[idx++] = arr[i];
    }

    return filtered;
}

int main()
{
    int test_arr[] = {1,2,3,4,5};
    Print(test_arr, 5);

    std::cout << "Sum: " << Sum(test_arr, 5) << std::endl;

    std::cout << "Max element: " << MaxElement(test_arr, 5) << std::endl;

    int test_arr2[] = {1,-2,0,-4,5};

    std::cout << "Positive elements: " << PositiveElementsCount(test_arr2, 5) << std::endl;

    int filtered_size;
    int *filtered_arr = FilterMoreThan(test_arr, 5, 3, filtered_size);
    std::cout << "Filtered size: " << filtered_size << " Filtered array: ";
    Print(filtered_arr, filtered_size);

    delete[] filtered_arr;
    return 0;
}