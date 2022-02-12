#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Checks.h"
#include"Employee.h"

#define COMPANY_IND "master.ind"
#define COMPANY_DATA "company.fl"
#define COMPANY_GARBAGE "company_garbage.txt"
#define INDEXER_SIZE sizeof(Indexer)
#define COMPANY_SIZE sizeof(Company)

void overwriteGarbageId(int garbageCount, FILE* garbageZone, Company* record)
{
	int* delIds = (int*) malloc(garbageCount * sizeof(int)); //+mem
	for (int i = 0; i < garbageCount; i++)
	{
		fscanf(garbageZone, "%d", delIds + i);//mem filling
	}
	record->id = delIds[0];
	fclose(garbageZone);
	fopen(COMPANY_GARBAGE, "wb");//clear
	fprintf(garbageZone, "%d", garbageCount - 1);//new garbage_ind count
	for (int i = 1; i < garbageCount; i++)
	{
		fprintf(garbageZone, "%d", delIds[i]); //fill new garbage_ind
	}
	free(delIds);//-mem
	fclose(garbageZone);

}

void noteDeletedCompany(int id)
{
	FILE* garbageZone = fopen(COMPANY_GARBAGE, "rb");

	int garbageCount;
	fscanf(garbageZone, "%d", &garbageCount);
	int* delIds = (int*)malloc(garbageCount * sizeof(int));//+mem
	for (int i = 0; i < garbageCount; i++)
	{
		fscanf(garbageZone, "%d", delIds + i);//mem filling
	}
	fclose(garbageZone);
	fopen(COMPANY_GARBAGE, "wb");//clear
	fprintf(garbageZone, "%d", garbageCount +1);
	for (int i = 0; i < garbageCount; i++)
	{
		fprintf(garbageZone, "%d", delIds[i]); //fill new garbage_ind
	}
	fprintf(garbageZone, "%d", id);						// add garbage_ind
	free(delIds);											//-mem
	fclose(garbageZone);
}




int insertCompany(Company record)
{
	FILE* indexTable = fopen(COMPANY_IND, "a+b");
	FILE* database = fopen(COMPANY_DATA, "a+b");
	FILE* garbageZone = fopen(COMPANY_GARBAGE, "rb");

	struct Indexer indexer;
	int garbageCount;
	fscanf(garbageZone, "%d", &garbageCount);
	if (garbageCount)
	{
		overwriteGarbageId(garbageCount, garbageZone, &record);
		fclose(database);
		fclose(indexTable);

		indexTable = fopen(COMPANY_IND, "r+b");
		database = fopen(COMPANY_DATA, "r+b");

		fseek(indexTable, (record.id - 1) * INDEXER_SIZE, SEEK_SET);
		fread(&indexer, INDEXER_SIZE, 1, indexTable);
		fseek(database, indexer.addres, SEEK_SET);
	}
	else
	{
		long indexerSize = INDEXER_SIZE;
		fseek(indexTable, 0, SEEK_END);

		if (ftell(indexTable))
		{
			fseek(indexTable, -indexerSize, SEEK_END);
			fread(&indexer, INDEXER_SIZE, 1, indexTable); 
			
			record.id = indexer.id + 1;
		}
		else
		{
			record.id = 1;//ind_table clear
		}

	}
	record.firstEmployeeAddres = -1;
	record.employeesCount = 0;
	fwrite(&record, COMPANY_SIZE, 1, database);
	indexer.id = record.id;
	indexer.addres = (record.id-1)*COMPANY_SIZE;
	indexer.exists = 1;

	printf("Company\'s id is %d\n", record.id);

	fseek(indexTable, (record.id - 1) * INDEXER_SIZE, SEEK_SET);
	fwrite(&indexer, INDEXER_SIZE, 1, indexTable);			
	fclose(indexTable);									
	fclose(database);

	return 1;


}
int getCompany(Company* company, int id, char* error)
{
	FILE* indexTable = fopen(COMPANY_IND, "rb");
	FILE* database = fopen(COMPANY_DATA, "rb");
	if (!checkFileExsistence(indexTable, database, error))
	{
		return 0;
	}
	Indexer indexer;
	if (!checkIndexExistence(indexTable,error,id))
	{
		return 0;
	}
	fseek(indexTable,(id-1)*INDEXER_SIZE,SEEK_SET);
	fread(&indexer, INDEXER_SIZE, 1, indexTable);

	if(!checkRecordExistence(indexer,error))
	{
		return 0;
	}
	fseek(database, indexer.addres, SEEK_SET);				
	fread(company, sizeof(Company), 1, database);		
	fclose(indexTable);										
	fclose(database);

	return 1;
}
int updateCompany(Company company, char* error)
{
	FILE* indexTable = fopen(COMPANY_IND, "r+b");		
	FILE* database = fopen(COMPANY_DATA, "r+b");

	if (!checkFileExsistence(indexTable, database, error))
	{
		return 0;
	}
	Indexer indexer;
	int id = company.id;
	if (!checkIndexExistence(indexTable, error, id))
	{
		return 0;
	}

	fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
	fread(&indexer, INDEXER_SIZE, 1, indexTable);

	if (!checkRecordExistence(indexer, error))
	{
		return 0;
	}
	fseek(database, indexer.addres, SEEK_SET);
	fwrite(&company, COMPANY_SIZE, 1, database);
	fclose(indexTable);										
	fclose(database);

	return 1;

}
int deleteCompany(int id, char* error)
{
	FILE* indexTable = fopen(COMPANY_IND, "r+b");
	if (indexTable == NULL)
	{
		strcpy(error, "database is not created");

		return 0;
	}
	if (!checkIndexExistence(indexTable, error, id))
	{
		return 0;
	}
	Company company;
	getCompany(&company, id, error);

	Indexer indexer;
	fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
	fread(&indexer, INDEXER_SIZE, 1, indexTable);
	indexer.exists = 0;
	fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
	fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
	fclose(indexTable);

	noteDeletedCompany(id);

	if (company.employeesCount)
	{
		FILE* employeeDb = fopen(EMPLOYEE_DATA,"r+b");
		Employee employee;	
		fseek(employeeDb, company.firstEmployeeAddres, SEEK_SET);
		for (int i = 0; i < company.employeesCount; i++)
		{
			fread(&employee, EMPLOYEE_SIZE, 1, employeeDb);
			fclose(employeeDb);
			deleteEmployee(company, employee, employee.employeeId, error);
			employeeDb=fopen(EMPLOYEE_DATA, "r+b");
			fseek(employeeDb, employee.nextAddress, SEEK_SET);
		}
		fclose(employeeDb);
	}
	return 1;
}

