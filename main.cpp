// DB_Lab_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include<stdio.h>
#include"Structures.h"
#include "Company.h"
#include"Employee.h"
#include "Input.h"
#include"Output.h"


int main()
{
	Company company;
	Employee employee;

	while (1)
	{
		int choice;
		int id;
		char error[51];
		printf("Choose option:\n0 - Quit\n1 - Insert Company\n2 - Get Company\n3 - Update company\n4 - Delete Company\n5 - Insert Employee\n 6 - Get Employee\n7 - Update Eployee\n8 - Delete Employee\n9- Get Info\n");
		scanf("%d", &choice);
		switch (choice)
		{
		case 0: return 0;
		case 1:
			readCompany(&company);
			insertCompany(company);
			break;
		case 2:
			printf("Company ID: ");
			scanf("%d", &id);
			getCompany(&company, id, error) ? printCompany(company) : printf("Error: %s\n", error);
			break;
		case 3:
			printf("Enter ID: ");
			scanf("%d", &id);
			company.id = id;

			readCompany(&company);
			updateCompany(company, error) ? printf("Updated\n") : printf("Error: %s\n", error);
			break;
		case 4:
			printf("Enter ID: ");
			scanf("%d", &id);
			deleteCompany(id, error) ? printf("Deleted\n") : printf("Error: %s\n", error);
		case 5:
			printf("Enter Company\'s ID: ");
			scanf("%d", id);
			if (getCompany(&company, id, error))
			{
				employee.companyId = id;
				printf("Enter Employee ID: ");
				scanf("%d", &id);

				employee.employeeId = id;
				readEmployee(&employee);

				insertEmployee(company, employee, error);
				printf("Inserted. To access, use company\'s and employee\'s IDs\n"); \
			}
			else {
				printf("Error: %s", error);
			}
			break;
		case 6:
			printf("Enter company\'s ID: ");
			scanf("%d", &id);
			if (getCompany(&company, id, error))
			{
				printf("Enter employee\'s ID: ");
				scanf("%d", &id);
				getEmployee(company, &employee, id, error) ? printEmployee(employee, company) : printf("Error: %s\n", error);

			}
			else
			{
				printf("Error: %s\n", error);
			}
			break;
		case 7:
			printf("Enter company\'s ID: ");
			scanf("%d", &id);

			if (getCompany(&company, id, error))
			{
				printf("Enter employee\'s ID: ");
				scanf("%d", &id);
				if (getEmployee(company, &employee, id, error))
				{
					readEmployee(&employee);
					updateEmployee(employee, id, error);
					printf("Updated\n");
				}
				else
				{
					printf("Error: %s\n", error);
				}
			}
			else
			{
				printf("Error: %s\n", error);
			}
			break;
		case 8:
			printf("Enter company\'s ID: ");
			scanf("%d", &id);
			if (getCompany(&company, id, error))
			{
				printf("Enter employee\'s ID: ");
				scanf("%d", &id);
				if (getEmployee(company, &employee, id, error))
				{
					deleteEmployee(&employee);
					printf("Deleted\n");
				}
				else
				{
					printf("Error: %s\n", error);
				}
			}
			else
			{
				printf("Error: %s\n", error);
			}
			break;

		case 9:
			info();
			break;
		default:printf("Invalid input\n");
		}
		printf("——————————————————");

		}
		return 0;
	}


