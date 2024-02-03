#include "Students.h"

uint16_t Students::snums_idx = 0;

Students::Students(/* args */)
{
}

Students::~Students()
{
}

Student Students::isStudentValid(String snum) {
    bool found = false;

    uint16_t i;

    for (i = 0; i < snums_idx; i++)
        if (snums[i].snum == snum){
            found = true;
            break;
        }
    
    if (!found)
        return {"0", "0", "0"};
    
    return snums[i];
}

void Students::addStudent(Student s) {
    snums[snums_idx++] = s;
}