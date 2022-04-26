
#include "company.h"

Company::Company(int id):company_id(id),
employees_of_company_by_id(),employees_of_company_by_salary(){}

int Company::getCompanyId() const
{
    return this->company_id;
}

int Company::getNumOfEmployeesInCompany() const
{
    return this->employees_of_company_by_id.getSizeOfTree();
}

void Company::insertIntoCompanyOfEmployeesById(shared_ptr<Employee> employee)
{
    this->employees_of_company_by_id.insert(employee);
}

void Company::insertIntoCompanyOfEmployeesBySalary(shared_ptr<Employee> employee)
{
    this->employees_of_company_by_salary.insert(employee);
}

void Company::removeFromCompanyOfEmployeeById(shared_ptr<Employee> employee)
{
    this->employees_of_company_by_id.remove(employee);
}
void Company::removeFromCompanyOfEmployeesBySalary(shared_ptr<Employee> employee)
{
    this->employees_of_company_by_salary.remove(employee);
}

shared_ptr<Employee> Company::getHighestEarner() const
{
    return this->employees_of_company_by_salary.getDataOfMax();
}

int CompareId::operator()(const shared_ptr<Company> g1,const shared_ptr<Company> g2)const
{
    return (g1->getCompanyId())-(g2->getCompanyId());
}


int Company::InOrderViaCompanyEmployeesById(int size,shared_ptr<Employee>* array)
{
    return this->employees_of_company_by_id.doInorder(size,array);
}

int Company::InOrderViaCompanyEmployeesBySalary(int size,shared_ptr<Employee>* array)
{
    return this->employees_of_company_by_salary.doInorder(size,array);
}

void Company::emptyCompanyOfEmployeesById()
{
    this->employees_of_company_by_id.emptyTree();

}
void Company::emptyCompanyOfEmployeesBySalary()
{
    this->employees_of_company_by_salary.emptyTree();
}

int Company::getTopEmployeeId()
{
    shared_ptr<Employee> top=this->getHighestEarner();
    return top->getEmployeeId();
}

int Company::getTopEmploeeLevel()
{
    shared_ptr<Employee> top=this->getHighestEarner();
    return top->getEmployeeSalary();
}