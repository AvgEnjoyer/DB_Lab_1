#pragma once
#include<stdio.h>
#include<string.h>
#include"Structures.h"
#include"Company.h"

void readCompany(Company* company)
{
	char name[20];
	int status;
	name[0] = '\0';
	printf("Enter company\'s name: ");
	scanf("%s", name);

	strcpy(company->name,name);
	printf("Enter company\'s status: ");
	scanf("%d", status);
	company->status = status;
}
void readEmployee(Employee* employee)
{
	char name[20];
	printf("Enter employee\'s name: ");
		scanf("%s", name);
	strcpy(employee->name, name);
}