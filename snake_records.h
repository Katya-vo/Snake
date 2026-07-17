#pragma once
#include <iostream>
#define NAME_MAX_SIZE 50 
struct rec
{
	char name[NAME_MAX_SIZE]{};
	short score{};
};
rec* addRecord(rec* records, int& size);
rec* readRecords(int& size);
bool saveRecords(rec* records, int& size);
void updateRecord(const char* playerName, int final_score);