#pragma once

struct Company
{
	int id;
	char name[20];
	/*int status;*/
	long firstEmployeeAddres;
	int employeesCount;
};

struct Employee
{
	int companyId;
	int employeeId;
	char name[20];
	int exist;
	int selfAddress;
	int nextAddress;

};

struct Indexer
{
	int id;
	int addres;
	int exists;
};