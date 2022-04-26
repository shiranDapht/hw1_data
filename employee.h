
#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include "avl_tree.h"


class Employee  
{
    private:
        int employee_id;
        int employee_company_id;
        int employee_grade;
        int employee_salary;

    public:
        Employee(int id):employee_id(id),employee_company_id(0),employee_grade(0),employee_salary(0){}; 
        //this constructor will be used to create a player with playerId only, in order
        //to send to the find function of an avl tree so we will be able to check
        //if the player exist- he will not be added to the tree!
        explicit Employee(int id, int company_id, int grade, int salary) :
        employee_id(id), employee_company_id(company_id), employee_grade(grade), employee_salary(salary){}; 
        //use in add employee function
        ~Employee()=default;
        int getEmployeeId() const;
        int getEmployeeGrade() const;
        int getEmployeeSalary() const;
        int getEmployeeCompanyId() const; 
        void setEmployeeSalary(int new_salary);
        void setEmployeeGrade(int new_grade); 
           
};


class CompareId
{
    public: 
    int operator()(const shared_ptr<Employee> p1, const shared_ptr<Employee> p2)const;
};

class CompareSalary //compare by salary, if both have the same salary then compare by id.
{
    public:
    int operator()(const shared_ptr<Employee> p1,const shared_ptr<Employee> p2)const;
};



#endif