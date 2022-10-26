#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "DataModel.h"
#include <vector>
#define DEBUG
#ifdef DEBUG
	#define DEBUGPRINT printf
#else
	#define DEBUGPRINT 
#endif // DEBUG



class SeedCombiner
{
public:
	SeedCombiner();
	SeedCombiner(DataModel* data);
	~SeedCombiner();
private:
	DataModel* _data;
	int _seekSize;
	char* _SeekBuffer;
	std::vector<UnSureCharInformation*> *arrays;
	//��ԭʼ����������
	void Resolver();
	//��������
	UnSureCharInformation* Mutant(UnSureCharInformation& data, char MutantType);
	//��������
	void GarbageCollection();
	//���������ַ�
	void Translator();
	//�滻����
	int Replace(char* data, int SizeOfRawData,char ** buffers);
	//������,�������ӵĴ�С
	int Counter();
public:
	//�������յ�����
	char* Extractant();
	//��ȡ�������Ӵ�С
	int GetSeekSize();
	//д�ļ�
	void WriteFile(const char* url);

};