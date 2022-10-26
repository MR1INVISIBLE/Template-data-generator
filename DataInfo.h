#include <vector>
#pragma once

#pragma pack(1)
#define NAME_CONFLICT 0
#define NO_NAME 1
#define UNKOWN_ERROR 2
#define ACTION_LOSS_ATTRI 3
#define NO_VALUE 4
#define MALLOC_ERROR 5
typedef struct _UnSureCharInformation
{
	unsigned int length;	//内容的长度
	char* info;	//内容数据
}UnSureCharInformation, * pUnSureCharInformation;
typedef struct _BASEDATA
{
	char name[64];
	char type;
	char Mutable;
	UnSureCharInformation Data;
}BASEDATA, * pBASEDATA;
typedef struct _DataBlock
{
	UnSureCharInformation name;
	//这里生成生成一个vector容器，用于储存所有的BaseData
	std::vector<BASEDATA* > *obj;
}DataBlock, * pDataBlock;


//typedef struct _RawDataBlock
//{
//	int size;		//所有RAWDATA块大小
//	char name[64];	//每个容器类的对象只需要一个名字，不需要其他的，方便定位就可以了
//	unsigned int NumberOfRawData;	//记录了原始数据块的数量
//	pDataBlock address;	//记录了第一个RawData的地址
//}RawDataBlock, * pRawDataBlock;


//原始的设计思路与，在DataModel下不允许出现直接是TEXT等标签，只能将其封装成块<Block>
typedef struct _DataModel
{
	UnSureCharInformation name; //DataModel的名字
	//这里生成生成一个vector容器，用于储存所有的DataBlock
	std::vector<DataBlock* > *obj;
}DataModel, * pDataModel;

typedef struct _ActionData
{
	unsigned int mode;	//模式，三种，Input,Output,ChangeState
	char name[64];			//Action名字
	union
	{
		char ChangedTo[64];	//要切换的状态名
		char UseData[64];	//使用的DataModel名
	};
}ActionData, * pActionData;

typedef struct _StateData
{
	char name[64];	//状态的名字
	unsigned int NumberOfAction;	//一个状态下有多少个动作
	ActionData* Address;	//第一个动作的地址

}StateData, * pStateData;


typedef struct _DebuggerData
{
	UnSureCharInformation* name;	//名字
	UnSureCharInformation* path;	//路径
}DebuggerData, * pDebuggerData;

typedef struct _RegisterData
{
	char name[64];	//名字
	pDebuggerData debugger;	//调试器
	char* state; //这里只需要一个名字即可
	//带扩展的//
}RegisterData, * pRegisterData;

typedef struct _GetMainContainner
{
	DataModel* datamodel[1024];
	StateData* statedata[1024];
	RegisterData* Register;
}GetMainContainner, * pGetMainContainner;

enum _type
{
	assci,
	utf8,
	hex,
	INT,
	UINT,
	FLOAT,
	DOUBLE,
	LONGLONG,
	BIT,
	BIT16,
	BIT32,
	BIT64,
	ULONGLONG
};

enum _ActionMode
{
	Error,
	Input,
	Output,
	ChangeState
};
class DataInfo
{
private: 
	const char* name;
public:
	DataInfo();
	~DataInfo();
private:
	virtual void XMLParser() = 0;
};
