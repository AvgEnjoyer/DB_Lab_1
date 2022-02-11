#pragma once
#include<stdio.h>
#include"Company.h"
#include"Structures.h"

void printCompany(Company company)
{
	printf("Company's name: %s\n", company.name);
	printf("Company's status: %d\n", company.status);
}
void printEmployee(Employee employee)
{
	printf("Employee's name: %s\n", employee.name);
}