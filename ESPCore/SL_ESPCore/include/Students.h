#ifndef STUDENTS_H
#define STUDENTS_H

#include <Arduino.h>
#include <vector>

struct Student {
    String fname;
    String lname;
    String snum;
};

class Students
{
private:
    static Student snums[100];
    static std::vector<Student> sittedStudents;
    static uint16_t snums_idx;
public:
    Students(/* args */);
    ~Students();
    static Student isStudentValid(String snum);
    static void addStudent(Student s);
    static void addSittedStudent(Student s);
};

#endif