

#ifndef COMPANY_H
#define COMPANY_H
#include "avl_tree.h"
#include "employee.h"


class Company   
{
    private:
    int company_id;
    AVLTree<shared_ptr<Employee>,CompareId> employees_of_company_by_id;
    AVLTree<shared_ptr<Employee>,CompareSalary> employees_of_company_by_salary;
    

    public:
    Company(int id);
    ~Company()
    {
      employees_of_company_by_id.emptyTree();
      employees_of_company_by_salary.emptyTree();
    }
    int getCompanyId()const;
    int getNumOfEmployeesInCompany() const;
    void insertIntoCompanyOfEmployeesById(shared_ptr<Employee> employee);
    void insertIntoCompanyOfEmployeesBySalary(shared_ptr<Employee> employee);
    void removeFromCompanyOfEmployeeById(shared_ptr<Employee> employee);
    void removeFromCompanyOfEmployeesBySalary(shared_ptr<Employee> employee);
    shared_ptr<Employee> getHighestEarner() const;
    int InOrderViaGroupPlayersById(int size,shared_ptr<Employee>* array);
    int InOrderViaGroupPlayersBySalary(int size,shared_ptr<Employee>* array);
    void emptyCompanyOfEmployeesById();
    void emptyCompanyOfEmployeesBySalary();
    int getTopEmployeeId();
    int getTopEmploeeLevel();

};

class CompareId
{
    public:
    int operator()(const shared_ptr<Company> g1,const shared_ptr<Company> g2)const;
};

#endif  