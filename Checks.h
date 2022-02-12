#pragma once

#include<stdio.h>
#include<stdlib.h>
#include"Employee.h"
int getCompany(Company* company, int id, char* error);
int checkFileExsistence(FILE* indexTable, FILE* database, char* error)
{
	//db is not created yet
	if (indexTable == NULL || database == NULL)
	{
		
		strcpy(error, "database files are not created");
		return 0;
	}
	return 1;
}

int checkIndexExistence(FILE* indexTable, char* error, int id)
{
	fseek(indexTable, 0, SEEK_END);
	long indexTableSize = ftell(indexTable);

	if (indexTable == 0||(id * sizeof(Indexer))>indexTableSize)
	{
		strcpy(error, "no ID in table");
		return 0;
	}
	return 1;
}

int checkRecordExistence(struct Indexer indexer, char* error)
{
	if (!indexer.exists)
	{
		strcpy(error, "this record removed");
		return 0;

	}
	return 1;
}

int checkKeyPairUniqueness(struct Company company, int employeeId)
{
	FILE* employeesDb = fopen(EMPLOYEE_DATA, "r+b"); //r+ stands for read_record and b for binary opening
	Employee employee;
	fseek(employeesDb, company.firstEmployeeAddres, SEEK_SET);
	for (int i = 0; i < company.employeesCount; i++)
	{
		fread(&employee, EMPLOYEE_SIZE, 1, employeesDb);
		fclose(employeesDb);
			if (employee.employeeId == employeeId)
			{
				return 0;
			}
			employeesDb = fopen(EMPLOYEE_DATA, "r+b");
			fseek(employeesDb, employee.nextAddress, SEEK_SET);
	}
	fclose(employeesDb);
	return 1;
}
void info() {
	
	FILE* indexTable = fopen("company.ind", "rb");

	if (indexTable == NULL)
	{
		printf("Error: DB files are not created\n");
		return;

	}
	int companyCount = 0;
	int employeeCount = 0;
	fseek(indexTable, 0, SEEK_END);
	int indAmount = ftell(indexTable) / sizeof(Indexer);
	Company company;
	char out[51];

	for (int i = 1; i <= indAmount; i++)
	{
		if (getCompany(&company, i, out))
		{
			companyCount++;
			employeeCount += company.employeesCount;
			printf("Company #%d has %d employees\n", i, company.employeesCount);
		}
	}
	fclose(indexTable);
	printf("Total companies: %d\n",companyCount);
	printf("Total employees: %d\n", employeeCount);
	

}