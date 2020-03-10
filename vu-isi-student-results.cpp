#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <chrono>

using std::string; using std::vector;

struct Student
{
	string name;
	string surname;
	vector<int> hwGrades;
	int examGrade;
	float finalAvg;
	float finalMed;
};

void getInput(vector<Student>& students);
int getInputFromFile(vector<Student>& students, string path);

void handleInput(string prompt_text, int& input, bool isGrade = false);
bool promptChoice(string prompt_text);

void computeFinals(vector<Student>& students);
float getMedian(vector<int> v);
float getAvg(vector<int> v);

void printStudents(const vector<Student> students);

int main()
{
	srand(time(0)); rand();

	vector<Student> students;

	bool readFromFile = promptChoice("Skaityti duomenis is failo? (t/n)\n");

	auto start = std::chrono::high_resolution_clock::now();

	if (readFromFile)
		getInputFromFile(students, "kursiokai.txt");
	else
		getInput(students);

	computeFinals(students);

	printStudents(students);

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> diff = end - start;

	std::cout << "Vykdymo laikas: " << diff.count() << "s\n";

	return 0;
}

void getInput(vector<Student>& students)
{
	students.clear();

	int S = 0;

	handleInput("Iveskite studentu skaiciu: ", S);

	for (int i = 0; i < S; ++i)
	{
		Student student;

		std::cout << "Studento vardas: ";
		std::cin >> student.name;
		std::cout << "Studento pavarde: ";
		std::cin >> student.surname;

		bool randomGrades = promptChoice("Pasirinkti atsitiktinius pazymius? (t/n)\n");

		if (randomGrades)
		{
			int N;
			handleInput("Iveskite pazymiu skaiciu: ", N);

			for (int j = 0; j < N; ++j)
			{
				int r_grade = ceil(1.0 * rand() / RAND_MAX * 10);
				student.hwGrades.push_back(r_grade);
			}
			student.examGrade = ceil(1.0 * rand() / RAND_MAX * 10);
		}
		else
		{
			std::cout << "Iveskite studento pazymius (irasydami bet koki simboli be skaiciaus galite baigti rasyma)\n";

			do
			{
				int grade = 0;
				std::cout << "Iveskite pazymi: ";
				std::cin >> grade;
				if (std::cin)
				{
					student.hwGrades.push_back(grade);
				}
			} while (std::cin || student.hwGrades.size() == 0);
			std::cin.clear(); std::cin.ignore();

			handleInput("Iveskite egzamino rezultata: ", student.examGrade, true);
			std::cout << std::endl;
		}

		students.push_back(student);
	}
}

int getInputFromFile(vector<Student>& students, string path)
{
	std::ifstream in(path);

	if (!in.is_open())
	{
		std::cout << "Duomenu failas neegzistuoja arba jo atidaryti nepavyko" << std::endl;
		return 0;
	}

	try
	{
		string line;
		std::getline(in, line);

		while (!in.eof())
		{
			std::getline(in, line);
			std::stringstream lineStream(line);

			Student student;

			lineStream >> student.name >> student.surname;

			while (!lineStream.eof())
			{
				int grade;
				lineStream >> grade;

				student.hwGrades.push_back(grade);
			}

			student.examGrade = student.hwGrades.back();
			student.hwGrades.pop_back();

			students.push_back(student);
		}

		in.close();
	}
	catch (std::exception & e)
	{
		std::cout << "Ivyko duomenu failo skaitymo klaida" << std::endl;
	}

	return 1;
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

bool promptChoice(string prompt_text)
{
	char input = 'T';

	do
	{
		if (!std::cin || (input != 'T' && input != 't' && input != 'N' && input != 'n'))
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}

		std::cout << prompt_text;
		std::cin >> input;
	} while (!std::cin || (input != 'T' && input != 't' && input != 'N' && input != 'n'));

	if (input == 'T' || input == 't')
		return true;
	return false;
}


void computeFinals(vector<Student>& students)
{
	for (auto it_s = students.begin(); it_s != students.end(); ++it_s)
	{
		it_s->finalAvg = 0.4 * getAvg(it_s->hwGrades) + 0.6 * it_s->examGrade;
		it_s->finalMed = 0.4 * getMedian(it_s->hwGrades) + 0.6 * it_s->examGrade;
	}
}

float getMedian(vector<int> v)
{
	std::sort(v.begin(), v.end());
	if (v.size() % 2 == 1)
		return 1.0 * v.at(v.size() / 2);
	else
		return 1.0 * (v.at(v.size() / 2 - 1) + v.at(v.size() / 2)) / 2;
}

float getAvg(vector<int> v)
{
	float avg = 0;

	for (auto it = v.begin(); it != v.end(); ++it)
		avg += *it;

	avg /= v.size();

	return avg;
}


void printStudents(const vector<Student> students)
{
	size_t len_name = 0, len_surname = 0;
	for (auto it_s = students.begin(); it_s != students.end(); ++it_s)
	{
		len_name = std::max(len_name, it_s->name.length());
		len_surname = std::max(len_surname, it_s->surname.length());
	}

	std::cout
		<< std::setw(len_surname + 2) << std::left << "Pavarde"
		<< std::setw(len_name + 2) << std::left << "Vardas"
		<< "Galutinis (Vid.) Galutinis (Med.)" << std::endl;

	for (int i = 0; i < (len_name + 2) + (len_surname + 2) + 12 + 17 + 17; ++i)
		std::cout << "-";
	std::cout << std::endl;

	std::stringstream strStream;
	for (auto it_s = students.begin(); it_s != students.end(); ++it_s)
	{
		strStream
			<< std::setw(len_surname + 2) << std::left << it_s->surname
			<< std::setw(len_name + 2) << std::left << it_s->name
			<< std::setw(17) << std::left << std::fixed << std::setprecision(2) << it_s->finalAvg
			<< std::setw(17) << std::left << std::fixed << std::setprecision(2) << it_s->finalMed
			<< std::endl;
	}
	std::cout << strStream.str();
}