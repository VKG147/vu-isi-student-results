#include "studentCompute.h"
#include <algorithm>
#include <string>

void computeFinals(deque<Student>& students)
{
	for (auto it_s = students.begin(); it_s != students.end(); ++it_s)
	{
		it_s->finalAvg = 0.4 * getAvg(it_s->hwGrades) + 0.6 * it_s->examGrade;
	}
}

float getAvg(deque<int> v)
{
	float avg = 0;

	for (auto it = v.begin(); it != v.end(); ++it)
		avg += *it;

	avg /= v.size();

	return avg;
}

void genRandomStudents(deque<Student>& students, int student_count, int grade_count, RandomGenerator* generator, int start_index)
{
	students.clear();
	
	string dName = "Vardas";
	string dSurname = "Pavarde";

	for (int i = 0; i < student_count; ++i)
	{
		string suffix = std::to_string(start_index+i);
		
		Student student;
		student.name = dName + suffix;
		student.surname = dSurname + suffix;

		for (int j = 0; j < grade_count; ++j)
		{
			int r_grade = generator->getRandom(1, 10);
			student.hwGrades.push_back(r_grade);
		}

		int exam_grade = generator->getRandom(1, 10);
		student.examGrade = exam_grade;

		students.push_back(student);
	}
}
