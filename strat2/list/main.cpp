﻿#include "studentIO.h"
#include "studentCompute.h"
#include "randomGenerator.h"
#include <string>
#include <list>
#include <fstream>
#include <chrono>
#include <iostream>
#include <algorithm>

using std::string; using std::list;

RandomGenerator* generator;

bool compareByName(const Student a, const Student b)
{
	return (a.name < b.name);
}

bool compareByFinalAvg(const Student a, const Student b)
{
	return (a.finalAvg < b.finalAvg);
}

bool compareByFinalMed(const Student a, const Student b)
{
	return (a.finalMed < b.finalMed);
}

void generateTestData(string path, int student_count, int grade_count, RandomGenerator* generator);
void runTests(RandomGenerator* generator);

int main()
{
	generator = new RandomGenerator();

	bool isTest = promptChoice("Paleisti testus? (t/n)\n");

	if (isTest)
	{
		runTests(generator);
	}
	else
	{
		list<Student> students;

		bool readFromFile = promptChoice("Skaityti duomenis is failo (kursiokai.txt)? (t/n)\n");

		if (readFromFile)
			getInputFromFile(students, "kursiokai.txt");
		else
			getInput(students, generator);

		computeFinals(students);

		students.sort(compareByName);

		printStudents(students);
	}

	delete generator;
	return 0;
}

void runTests(RandomGenerator* generator)
{
	const string testPath = "test.txt";
	const double gradeBound = 5.0;
	const int grade_count = 5;
	list<int> testSizes = { 1000, 10000, 100000, 1000000, 10000000 };

	std::chrono::duration<double> diff;

	std::cout << "===List testavimas===\n";
	for (auto it_size = testSizes.begin(); it_size != testSizes.end(); ++it_size)
	{ // One test

		// Generating
		list<Student> students;
		if (*it_size == 10000000)
		{
			for (int i = 0; i < 10; ++i)
			{
				genRandomStudents(students, 1000000, grade_count, generator, i * 1000);
				writeStudentsToFile(students, testPath, true);
			}
		}
		else
		{
			genRandomStudents(students, *it_size, grade_count, generator);
			writeStudentsToFile(students, testPath);
		}
		students.clear();
		
		// Reading
		auto t1 = std::chrono::high_resolution_clock::now();
		getInputFromFile(students, testPath);
		auto t2 = std::chrono::high_resolution_clock::now();
		diff = t2 - t1;
		std::cout << "Laikas " << *it_size << " studentu failo nuskaitymui: " << diff.count() << "s\n";

		// Computing finals
		computeFinals(students);

		// Sorting
		t1 = std::chrono::high_resolution_clock::now();
		students.sort(compareByFinalAvg);
		t2 = std::chrono::high_resolution_clock::now();
		diff = t2 - t1;
		std::cout << "Laikas " << *it_size << " list rusiavimui: " << diff.count() << "s\n";

		// Seperating into two groups
		t1 = std::chrono::high_resolution_clock::now();
		list<Student> studentsA;
		auto it_s = students.begin();
		while (it_s != students.end())
		{
			if (it_s->finalAvg < gradeBound) // Not epic gamers
			{
				studentsA.push_back(*it_s);
				it_s = students.erase(it_s);
			}
			else
				it_s++;
		}
		t2 = std::chrono::high_resolution_clock::now();
		diff = t2 - t1;
		std::cout << "Laikas " << *it_size << " studentu paskirstymo i atskirus list: " << diff.count() << "s\n\n";
	}
}

void generateTestData(string path, int student_count, int grade_count, RandomGenerator* generator)
{
	list<Student> students;
	genRandomStudents(students, student_count, grade_count, generator);
	writeStudentsToFile(students, path);
}