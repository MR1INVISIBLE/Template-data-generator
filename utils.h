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
	//对原始数据做解析
	void Resolver();
	//变异数据
	UnSureCharInformation* Mutant(UnSureCharInformation& data, char MutantType);
	//回收数据
	void GarbageCollection();
	//翻译特殊字符
	void Translator();
	//替换数据
	int Replace(char* data, int SizeOfRawData,char ** buffers);
	//计数器,计算种子的大小
	int Counter();
public:
	//导出最终的种子
	char* Extractant();
	//获取生成种子大小
	int GetSeekSize();
	//写文件
	void WriteFile(const char* url);

};