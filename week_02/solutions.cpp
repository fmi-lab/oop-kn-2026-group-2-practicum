#include <cstring>
#include <iostream>

template <class NumberType>
class Student {
	char		 name[32];
	NumberType	 number;
	float		 grades[8][32];
	unsigned int gradesCount[8];

   public:
	Student() : gradesCount{0} {
		name[0] = '\0';
		number	= NumberType();
	}

	Student(const char *name, NumberType number) : gradesCount{0} {
		strcpy(this->name, name);
		this->number = number;
	}
	
	/// среден успех за даден семестър
	float GetTermAverage(int semester) {
		if (semester < 0 || semester >= 8) {
			std::cerr << "Invalid semester." << std::endl;
			return 0.0f;
		}
		float sum = 0.0f;
		for (unsigned int i = 0; i < gradesCount[semester]; i++) {
			sum += grades[semester][i];
		}
		return gradesCount[semester] > 0 ? sum / gradesCount[semester] : 0.0f;
	}

	/// среден успех за всички семестри
	float GetAverage() {
		float		 sum		 = 0.0f;
		unsigned int totalGrades = 0;
		for (int semester = 0; semester < 8; semester++) {
			for (unsigned int i = 0; i < gradesCount[semester]; i++) {
				sum += grades[semester][i];
				totalGrades++;
			}
		}
		return totalGrades > 0 ? sum / totalGrades : 0.0f;
	}
	
	/// добавя оценка за даден семестър
	void AddGrade(float grade, int semester) {
		if (semester < 0 || semester >= 8) {
			std::cerr << "Invalid semester." << std::endl;
			return;
		}
		if (gradesCount[semester] >= 32) {
			std::cerr << "Grade count exceeded for semester " << semester << "." << std::endl;
			return;
		}

		grades[semester][gradesCount[semester]] = grade;
		gradesCount[semester]++;
	}

	NumberType	GetNumber() const { return number; }
	const char *GetName() const { return name; }
};

// ще счетем, че както се иска в задачата, студентите, които са семестриално завършили имат 
// факултетни номера от тип char, а тези, които са редовни - от тип int

using SemestriallyGradedStudent = Student<char>;
using RegularStudent			= Student<int>;

class University {
	RegularStudent students[100];
	unsigned int   studentCount;
	char		   name[32];

   public:
	University(const char *name) : studentCount(0) { strcpy(this->name, name); }

	University(const char *name, RegularStudent *students, unsigned int count) : studentCount(count) {
		// считаме, че дължината на името е подходяща.
		strcpy(this->name, name);
		for (unsigned int i = 0; i < count && i < 100; i++) {
			this->students[i] = students[i];
		}
	}

	/// среден успех за даден семестър на всички студенти
	float GetTermAverage(int semester) {
		float sum = 0.0f;
		for (unsigned int i = 0; i < studentCount; i++) {
			sum += students[i].GetTermAverage(semester);
		}
		return studentCount > 0 ? sum / studentCount : 0.0f;
	}
	
	/// среден успех за всички семестри на всички студенти
	float GetAverage() {
		float sum = 0.0f;
		for (unsigned int i = 0; i < studentCount; i++) {
			sum += students[i].GetAverage();
		}
		return studentCount > 0 ? sum / studentCount : 0.0f;
	}
};

int main() {
	RegularStudent students[2] = {RegularStudent("Alice", 12345), RegularStudent("Bob", 67890)};
	students[0].AddGrade(85.0f, 0);
	students[0].AddGrade(90.0f, 1);
	students[1].AddGrade(78.0f, 0);
	students[1].AddGrade(82.0f, 1);

	University university("Tech University", students, 2);
	std::cout << "Term Average for Semester 0: " << university.GetTermAverage(0) << std::endl;
	std::cout << "Overall Average: " << university.GetAverage() << std::endl;

	// raboti
	Student<const char *> s = Student<const char *>("Charlie", "ID123");

	Student<const char *> *ptr = nullptr;
	{ ptr = new Student<const char *>("Eve", "ID789"); }
	const char str[] = "hello there aaaa";
	std::cout << str << std::endl;

	std::cout << "Student Name: " << ptr->GetName() << " Number: " << ptr->GetNumber() << std::endl;

	delete ptr;

	return 0;
}
