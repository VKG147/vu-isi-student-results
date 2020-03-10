#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using std::string; using std::vector;

#define MAX_N 100
#define MAX_G 100

struct Student
{
	string name;
	string surname;
	int n_grades;
	int hwGrades[MAX_G];
	int examGrade;
	float final;
};

void getInput(Student students[], int& n, bool& isMedian);
void handleInput(string prompt_text, int& input, bool isGrade = false);

void computeFinals(Student students[], int n, bool isMedian);
float getMedian(int arr[], int n);
float getAvg(int arr[], int n);

void printStudents(Student students[], int n, bool isMedian);

int main()
{
	srand(time(0)); rand();
	
	int n;
	Student students[MAX_N];
	bool isMedian;
	
	getInput(students, n, isMedian);

	computeFinals(students, n, isMedian);

	printStudents(students, n, isMedian);
	
	return 0;
}

void getInput(Student students[], int& n, bool& isMedian)
{
	handleInput("Iveskite studentu skaiciu: ", n);

	for (int i = 0; i < n; ++i)
	{
		Student student;

		std::cout << "Studento vardas: ";
		std::cin >> student.name;
		std::cout << "Studento pavarde: ";
		std::cin >> student.surname;

		char input = ' ';
		
		do
		{
			if (!std::cin || (input != 't' && input != 'n' && input != 'T' && input != 'N'))
			{
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
			}

			std::cout << "Parinkti atsitiktinius pazymius? (t/n)\n";
			std::cin >> input;
		} while (!std::cin || (input != 't' && input != 'n' && input != 'T' && input != 'N'));

		if (input == 't' || input == 'T')
		{
			handleInput("Iveskite pazymiu skaiciu: ", student.n_grades);

			for (int j = 0; j < student.n_grades; ++j)
			{
				int r_grade = ceil(1.0*rand() / RAND_MAX * 10);
				student.hwGrades[j] = r_grade;
			}
			student.examGrade = ceil(1.0 * rand() / RAND_MAX * 10);
		}
		else
		{
			std::cout << "Iveskite studento pazymius (irasydami bet koki simboli be skaiciaus galite baigti rasyma)\n";

			int j = 0;

			do
			{
				int grade = 0;
				std::cout << "Iveskite pazymi: ";
				std::cin >> grade;
				if (std::cin)
				{
					student.hwGrades[j] = grade;
					j++;
				}
			} while (std::cin || j == 0);
			std::cin.clear(); std::cin.ignore();

			handleInput("Iveskite egzamino rezultata: ", student.examGrade, true);
			std::cout << std::endl;
		}

		students[i] = student;
	}

	char input = ' ';
	do
	{
		if (!std::cin || (input != 't' && input != 'n' && input != 'T' && input != 'N'))
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
		
		std::cout << "Vidurkius skaiciuoti pagal mediana? (t/n)\n";
		std::cin >> input;
	} while (!std::cin || (input != 't' && input != 'n' && input != 'T' && input != 'N'));

	if (input == 't' || input == 'T')
		isMedian = true;
	else
		isMedian = false;
}

void handleInput(string prompt_text, int& input, bool isGrade)
{
	input = 5;

	do
	{
		if (!std::cin || input <= 0 || (isGrade && (input < 1 || input > 10)))
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}

		std::cout << prompt_text;
		std::cin >> input;
	} while (!std::cin || input <= 0 || (isGrade && (input < 1 || input > 10)));
}


void computeFinals(Student students[], int n, bool isMedian)
{
	for (int i = 0; i < n; ++i)
	{
		if (isMedian)
			students[i].final = 0.4 * getMedian(students[i].hwGrades, students[i].n_grades) + 0.6 * students[i].examGrade;
		else
			students[i].final = 0.4 * getAvg(students[i].hwGrades, students[i].n_grades) + 0.6 * students[i].examGrade;
	}
}

float getMedian(int arr[], int n)
{
	std::sort(arr, arr + n - 1);
	if (n % 2 == 1)
		return 1.0 * arr[n / 2];
	else
		return 1.0 * (arr[((int)(n / 2) - 1)] + arr[(int)(n / 2)]) / 2;
}

float getAvg(int arr[], int n)
{
	float avg = 0;

	for (int i = 0; i < n; ++i)
		avg += arr[i];

	avg /= n;

	return avg;
}


void printStudents(Student students[], int n, bool isMedian)
{
	size_t len_name = 0, len_surname = 0;
	for (int i = 0; i < n; ++i)
	{
		len_name = std::max(len_name, students[i].name.length());
		len_surname = std::max(len_surname, students[i].surname.length());
	}

	std::cout
		<< std::setw(len_surname + 2) << std::left << "Pavarde"
		<< std::setw(len_name + 2) << std::left << "Vardas"
		<< std::setw(12) << std::left << (isMedian ? "Galutinis (Med.)" : "Galutinis (Vid.)") << std::endl;
	
	for (int i = 0; i < (len_name + 2) + (len_surname + 2) + 12; ++i)
		std::cout << "-";
	std::cout << std::endl;

	for (int i = 0; i < n; ++i)
	{
		std::cout
			<< std::setw(len_surname + 2) << std::left << students[i].surname
			<< std::setw(len_name + 2) << std::left << students[i].name
			<< std::fixed << std::setprecision(2) << students[i].final
			<< std::endl;
	}
}



