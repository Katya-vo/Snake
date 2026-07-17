#define _CRT_SECURE_NO_WARNINGS

#include "snake_records.h"
#include <fstream>
#include <cstring>     
#include <iostream>    

using namespace std;

rec* addRecord(rec* records, int& size)
{
	rec* rec2 = new rec[size + 1];

	for (int i = 0; i < size; i++)
		rec2[i] = records[i];

	size++;
	if (records != nullptr)
		delete[] records;

	return rec2;
}
rec* readRecords(int& size)
{
	rec* records = nullptr;
	size = 0;
	fstream f("records.txt", ios::in);

	if (!f.is_open())
	{
		f.open("records.txt", ios::out);
		f.close();
		return nullptr;
	}
	rec current_rec;
	while (f >> current_rec.name >> current_rec.score)
	{
		records = addRecord(records, size);
		records[size - 1] = current_rec;
	}

	f.close();
	return records;
}
bool saveRecords(rec* records, int& size)
{
	fstream f;

	f.open("records.txt", ios_base::out | ios::trunc);
	if (!f.is_open())
		return false;

	for (int i = 0; i < size; i++)
	{
		f << records[i].name << " ";
		f << records[i].score;
		if (i < size - 1) f << "\n";
	}

	f.close();
	return true;
}
void updateRecord(const char* playerName, int final_score)
{
	int records_size = 0;
	rec* records = readRecords(records_size);

	bool found = false;
	int i;
	for (i = 0; i < records_size; i++)
	{
		if (strcmp(records[i].name, playerName) == 0)
		{
			if (final_score > records[i].score) {
				records[i].score = (short)final_score;
			}
			found = true;
			break;
		}
	}
	if (!found)
	{
		records = addRecord(records, records_size);
		int new_index = records_size - 1;
		strcpy(records[new_index].name, playerName);
		records[new_index].score = (short)final_score;
	}
	if (!saveRecords(records, records_size))
	{
		cerr << "Error: Failed to save records.\n";
	}
	if (records != nullptr)
		delete[] records;
}