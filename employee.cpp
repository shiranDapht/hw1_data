#include "employee.h"

int Employee::getEmployeeId() const
{
	return this->employee_id;
}

int Employee::getEmployeeGrade() const
{
	return this->employee_grade;
}

int Employee::getEmployeeSalary() const
{
	return this->employee_salary;
}

int Employee::getEmployeeCompanyId() const
{
	return this->employee_company_id;
}

void Employee::setEmployeeSalary(int new_salary)
{
	this->employee_salary = new_salary;
} 

void Employee::setEmployeeGrade(int new_grade)
{
	this->employee_grade = new_grade;
}

int CompareId::operator() (const shared_ptr<Employee> p1,const shared_ptr<Employee> p2) const
{
	return p1->getEmployeeId()-p2->getEmployeeId();
}


int CompareSalary::operator()(const shared_ptr<Employee> p1,const shared_ptr<Employee> p2)const
{
	int sum = p1->getEmployeeSalary() - p2->getEmployeeSalary(); 
	if (sum == 0)
	{
		return CompareId()(p1,p2); 
	}
	return sum;
}








