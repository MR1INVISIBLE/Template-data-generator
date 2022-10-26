
#include "utils.h"
#include <vector>
#include <stdio.h>

using namespace std;
#define TableSize 3
const char* dictStr[TableSize] = { "\\n","\\r","\\t" };
char dictHex[TableSize] = { '\n','\r','\t'};
SeedCombiner::SeedCombiner()
{
	printf("��֧���޲εĹ��캯��\n");
}
SeedCombiner::SeedCombiner(DataModel* data)
{
	this->_seekSize = 0;
	this->_data = data;
	this->_SeekBuffer = 0;
	this->arrays = new std::vector<UnSureCharInformation*>();
	Resolver();
}

SeedCombiner::~SeedCombiner()
{

}
int  SeedCombiner::Counter()
{
	int count = 0;
	for (auto iter = this->arrays->begin(); iter != this->arrays->end(); iter++)
	{
		//��ȥĩβ��0x0
		count = count + (*iter)->length - 1;
	}
	return count;
}
char* SeedCombiner::Extractant()
{
	int SizeToSeek;
	char* mm_seek = NULL;
	//ת�������������
	Translator();
	SizeToSeek = Counter();
	mm_seek = (char*)malloc(SizeToSeek);
	if (!mm_seek) return NULL;
	memset(mm_seek, 0, SizeToSeek);
	DEBUGPRINT("Seek: {\n");
	char* temp = mm_seek;
	for (auto iter = this->arrays->begin(); iter != this->arrays->end(); iter++)
	{
		memcpy(temp, (*iter)->info, (*iter)->length - 1);
		temp = temp + (*iter)->length - 1;
	}
	DEBUGPRINT("\n}\n");
	this->_seekSize = SizeToSeek;
	this->_SeekBuffer = mm_seek;
	return mm_seek;
}

void SeedCombiner::Resolver()
{
	DEBUGPRINT("DataModel %s: {\n", _data->name.info);
	for (auto iter = _data->obj->begin(); iter != _data->obj->end(); iter++)
	{
		DEBUGPRINT("	Block %s: { ",(*iter)->name.info);
		for (auto iter2 = (*iter)->obj->begin(); iter2 != (*iter)->obj->end(); iter2++)
		{
			DEBUGPRINT("%s ", (*iter2)->name);
			if ((*iter2)->Mutable)
			{
				//����б��졣��ô���ñ���
				UnSureCharInformation* info = Mutant((*iter2)->Data, (*iter2)->type);
				this->arrays->push_back(info);
			}
			else
			{
				this->arrays->push_back(&((*iter2)->Data));
			}
		}
		DEBUGPRINT("}\n ");
	}
	DEBUGPRINT("}\n ");
}

UnSureCharInformation* SeedCombiner::Mutant(UnSureCharInformation& data, char MutantType)
{
	//������ʱ���ż�д���첿��
	return &data;
}
int SeedCombiner::Replace(char* data, int SizeOfRawData, char** buffers)
{
	//ֱ�ӿ���һ���¿ռ为��洢����
	char* buffer = (char*)malloc(SizeOfRawData);
	if (!buffer) return -1;
	memset(buffer, 0, SizeOfRawData);
	char* temp = buffer;
	char* temp2 = data;
	int sizeOfCopy = 0;
	int currentOffset = 0;
	int lastCopyLoc = 0;
	//������������
	for (int i = 0; i < SizeOfRawData; i++)
	{
		for (int x = 0; x < TableSize; x++)
		{
			//������ֵ��д���
			if (memcmp(dictStr[x], &data[i], 2) == 0)
			{
				//��¼��ǰƥ���λ��
				currentOffset = i;
				//�����/r/n���ŵ�
				if ((currentOffset - lastCopyLoc) != 2) {
					//����ǰ�������
					memcpy(temp, temp2, currentOffset - lastCopyLoc);
					//׼�����ƶ�Ӧ�Ķ�����
					buffer[currentOffset] = dictHex[x];
					//���һ�θ��Ƶص�
					temp = temp + currentOffset - lastCopyLoc + 1;
					temp2 = temp2 + currentOffset - lastCopyLoc + 2;
					lastCopyLoc = currentOffset;
					sizeOfCopy = sizeOfCopy + currentOffset - lastCopyLoc + 1;
				}
				else
				{
					//����/R/N����
					buffer[lastCopyLoc + 1] = dictHex[x];
					temp = temp + 1;
					temp2 = temp2 + 2;
					sizeOfCopy = sizeOfCopy + 1;
				}
			}
		}
	}
	if ((data + SizeOfRawData - temp2) > 0 && (data + SizeOfRawData - temp2) < 0x10000000)
	{
		memcpy(temp, temp2, (int)(SizeOfRawData - currentOffset -2));
		sizeOfCopy = sizeOfCopy + (SizeOfRawData - currentOffset - 2);
	}
	*buffers = buffer;
	return sizeOfCopy;
}

void SeedCombiner::Translator() 
{
	char* buffer = NULL;
	int resize = 0;
	for (auto iter = this->arrays->begin(); iter != this->arrays->end(); iter++)
	{
		for(int i = 0 ; i < (*iter)->length ; i++)
		{
			if (memcmp("\\", (const char *)&(*iter)->info[i],1) == 0)
			{
				resize = Replace((*iter)->info, (*iter)->length, &buffer);
				//�ͷžɵ�//�����µ�
				free((*iter)->info);
				(*iter)->info = buffer;
				(*iter)->length = resize;
			}
		}
	}
}
int SeedCombiner::GetSeekSize()
{
	return this->_seekSize;
}

void SeedCombiner::WriteFile(const char* url)
{
	FILE* fp = fopen(url, "wb");
	if (fp == NULL)
	{
		printf("д���ļ�ʧ��\n");
		return;
	}
	fwrite(this->_SeekBuffer, 1, GetSeekSize(), fp);;
}