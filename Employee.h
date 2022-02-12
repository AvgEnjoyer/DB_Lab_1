#pragma once
#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include "Structures.h"
#include "Checks.h"
#include "Company.h"

#define EMPLOYEE_DATA "employee.fl"
#define EMPLOYEE_GARBAGE "employee_garbage.txt"
#define EMPLOYEE_SIZE sizeof(Employee)

void reopenDatabase(FILE* database)
{
	fclose(database);
	database = fopen(EMPLOYEE_DATA,"r+b");
}
void linkAdresses(FILE* database, Company company, Employee employee)
{
	reopenDatabase(database);
	Employee prev;
	fseek(database, company.firstEmployeeAddres, SEEK_SET);
	for (int i = 0; i < company.employeesCount; i++)
	{
		fread(&prev, EMPLOYEE_SIZE, 1, database);
		fseek(database, prev.nextAddress, SEEK_SET);
	}
	prev.nextAddress = employee.selfAddress;
	fwrite(&prev, EMPLOYEE_SIZE, 1, database);
}
void relinkAddresses(FILE* database, Employee prev, Employee employee, Company* company)
{
	if (employee.selfAddress == company->firstEmployeeAddres)
	{
		if (employee.selfAddress == employee.nextAddress)
		{
			company->firstEmployeeAddres = -1;
		}
		else 
		{
			company->firstEmployeeAddres = employee.nextAddress;

		}
	}
	else
	{
		if (employee.selfAddress == employee.nextAddress)
		{
			prev.nextAddress = prev.selfAddress;
		}
		else
		{
			prev.nextAddress = employee.nextAddress;
		}
		fseek(database, prev.selfAddress, SEEK_SET);
		fwrite(&prev, EMPLOYEE_SIZE,1, database);
	}
}
void overwriteGarbageAddress(int garbageCount, FILE* garbageZone, Employee* record)
{
	long* delIds =(long*) malloc(garbageCount * sizeof(long));	

	for (int i = 0; i < garbageCount; i++)
	{
		fscanf(garbageZone, "%d", delIds + i);			
	}

	record->selfAddress = delIds[0];					
	record->nextAddress = delIds[0];

	fclose(garbageZone);								
	fopen(EMPLOYEE_GARBAGE, "wb");							  
	fprintf(garbageZone, "%d", garbageCount - 1);		

	for (int i = 1; i < garbageCount; i++)
	{
		fprintf(garbageZone, " %d", delIds[i]);			
	}

	free(delIds);										
	fclose(garbageZone);								
}
void noteDeletedEmployee(long address)
{
	FILE* garbageZone = fopen(EMPLOYEE_GARBAGE, "rb");		         

	int garbageCount;
	fscanf(garbageZone, "%d", &garbageCount);

	long* delAddresses = (long*)malloc(garbageCount * sizeof(long)); 

	for (int i = 0; i < garbageCount; i++)
	{  
		fscanf(garbageZone, "%ld", delAddresses + i);		         
	}														         
															         
	fclose(garbageZone);								             
	garbageZone = fopen(EMPLOYEE_GARBAGE, "wb");			         
	fprintf(garbageZone, "%ld", garbageCount + 1);		             

	for (int i = 0; i < garbageCount; i++)
	{
		fprintf(garbageZone, " %ld", delAddresses[i]);	             
	}													             
														             
	fprintf(garbageZone, " %d", address);				             
	free(delAddresses);									             
	fclose(garbageZone);								             
}


int insertEmployee( Company company, Employee employee, char* error)
{
	employee.exist = 1;
	FILE* database = fopen(EMPLOYEE_DATA, "a+b");
	FILE* garbageZone = fopen(EMPLOYEE_GARBAGE, "rb");
	int garbbageCount;
	
	fscanf(garbageZone, "%d", &garbbageCount);
	if (garbbageCount)
	{
		overwriteGarbageAddress(garbbageCount, garbageZone, &employee);
		reopenDatabase(database);								
		fseek(database, employee.selfAddress, SEEK_SET);
	}
	else
	{
		fseek(database, 0, SEEK_END);
		int dbSize = ftell(database);
		employee.selfAddress = dbSize;
		employee.nextAddress = dbSize;	
	}
	fwrite(&employee, EMPLOYEE_SIZE, 1, database);
	if (!company.employeesCount)								  
	{
		company.firstEmployeeAddres = employee.selfAddress;
	}
	else
	{
		linkAdresses(database, company, employee);
	}
	company.employeesCount++;
	updateCompany(company, error);
	return 1;


}
int getEmployee(Company company, Employee* employee, int employeeId, char* error)
{
	if (!company.employeesCount)				
	{
		strcpy(error, "Company hasn't employees");
		return 0;
	}
	
	FILE* database = fopen(EMPLOYEE_DATA, "rb");

	fseek(database, company.firstEmployeeAddres, SEEK_SET);		
	fread(employee, EMPLOYEE_SIZE, 1, database);
	for (int i = 0; i < company.employeesCount; i++)
	{
		if (employee->employeeId == employeeId)
		{
			fclose(database);
			return 1;
		}
		fseek(database, employee->nextAddress , SEEK_SET);
		fread(employee, EMPLOYEE_SIZE, 1, database);

	}
	strcpy(error, "No such employee in database");
	fclose(database);
	return 0;
}
int updateEmployee(Employee employee, int productId)
{
	FILE* database = fopen(EMPLOYEE_DATA, "r+b"); 
	fseek(database, employee.selfAddress, SEEK_SET);
	fwrite(&employee, EMPLOYEE_SIZE, 1, database);
	fclose(database);
	return 1;
}

int deleteEmployee(Company company, Employee employee, int employeeId, char* error)
{
	FILE* database = fopen(EMPLOYEE_DATA, "r+b");
	Employee prev;

	fseek(database, company.firstEmployeeAddres, SEEK_SET);

	do		                                                                                                   
	{														                                                   
		fread(&prev, EMPLOYEE_SIZE, 1, database);		                                                       
		fseek(database, prev.nextAddress, SEEK_SET);
	} while (prev.nextAddress != employee.selfAddress && employee.selfAddress != company.firstEmployeeAddres);

	relinkAddresses(database, prev, employee, &company);
	noteDeletedEmployee(employee.selfAddress);					                                               
															                                                   
	employee.exist = 0;										                                                   
															                                                   
	fseek(database, employee.selfAddress, SEEK_SET);		                                                   
	fwrite(&employee, EMPLOYEE_SIZE, 1, database);			                                                   
	fclose(database);										                                                   
															                                                   
	company.employeesCount--;								                                                   
	updateCompany(company, error);

}