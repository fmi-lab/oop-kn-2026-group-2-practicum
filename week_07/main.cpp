#include <iostream>
#include <cassert>
#include "Vector.hpp"
#include "Student.hpp"

// ── Helpers ───────────────────────────────────────────────────────────────
static void printStudent(const Student& s) {
    std::cout << "  Student: " << s.name
              << "  | grades (" << s.gradeCount << "): ";
    for (int i = 0; i < s.gradeCount; ++i)
        std::cout << s.grades[i] << " ";
    std::cout << "\n";
}

static Student makeStudent(const char* name,
                            std::initializer_list<float> gs)
{
    Student s{};
    std::strncpy(s.name, name, sizeof(s.name) - 1);
    s.gradeCount = 0;
    for (float g : gs)
        s.grades[s.gradeCount++] = g;
    return s;
}

// ── Тест на Vector<T> ─────────────────────────────────────────────────────
void testVector() {
    std::cout << "=== Vector<int> ===\n";

    Vector<int> v;
    for (int i = 1; i <= 8; ++i) v.push_back(i * 10);

    std::cout << "size=" << v.size()
              << "  capacity=" << v.capacity() << "\n";

    // operator[]
    std::cout << "v[3] = " << v[3] << "\n";

    // at() с грешен индекс
    try {
        v.at(100);
    } catch (const std::out_of_range& e) {
        std::cout << "at(100) throw: " << e.what() << "\n";
    }

    // pop_back
    v.pop_back();
    std::cout << "After pop_back, size=" << v.size() << "\n";

    // Копиращ конструктор
    Vector<int> v2 = v;
    v2.push_back(999);
    std::cout << "v.size()=" << v.size()
              << "  v2.size()=" << v2.size() << "\n";

    // operator=
    Vector<int> v3;
    v3 = v2;
    assert(v3.size() == v2.size());
    std::cout << "operator= OK\n\n";
}

// ── Тест на двоични файлове ───────────────────────────────────────────────
void testBinaryFiles() {
    const char* FILE = "students.bin";
    std::cout << "=== Binary files ===\n";

    // 1. Запис
    Student arr[3] = {
        makeStudent("Ivam Petrov",   {5.5f, 4.0f, 6.0f}),
        makeStudent("Maria Ivanova", {4.5f, 5.0f}),
        makeStudent("Peter Georgiev",{6.0f, 5.5f, 4.5f, 3.0f}),
    };
    saveStudents(FILE, arr, 3);
    std::cout << "Saved 3 students.\n";

    // 2. Четене
    Vector<Student> loaded = loadStudents(FILE);
    std::cout << "Read " << loaded.size() << " students:\n";
    for (std::size_t i = 0; i < loaded.size(); ++i)
        printStudent(loaded[i]);

    // 3. Добавяне без презареждане
    Student newS = makeStudent("Elena Stoyanova", {5.0f, 5.5f});
    appendStudent(FILE, newS);
    std::cout << "\nAfter appendStudent:\n";
    Vector<Student> loaded2 = loadStudents(FILE);
    std::cout << "Read " << loaded2.size() << " students:\n";
    for (std::size_t i = 0; i < loaded2.size(); ++i)
        printStudent(loaded2[i]);

    // 4. Търсене по име
    Student found{};
    bool ok = findStudentInFile(FILE, "Maria Ivanova", found);
    if (ok) {
        std::cout << "\nFound student:\n";
        printStudent(found);
    } else {
        std::cout << "\nStudent not found.\n";
    }

    // Търсене на несъществуващ
    bool notOk = findStudentInFile(FILE, "Unknown", found);
    std::cout << "Searching for 'Unknown': " << (notOk ? "found" : "not found") << "\n";

    // Зареждане от несъществуващ файл → празен вектор
    Vector<Student> empty = loadStudents("no_such_file.bin");
    assert(empty.empty());
    std::cout << "loadStudents(Unknown) returned empty vector - OK\n";
}

int main() {
    testVector();
    testBinaryFiles();
    return 0;
}