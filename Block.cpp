#include "BaseData.h"
#include "Block.h"

#define ReturnDataBlockSize(block) (sizeof(BASEDATA))


Block::Block(XMLElement* element)
{
	this->_element = element;
	this->_RawDataNumber = 1024;
	this->_name = { 0 };
	XMLParser();
}

Block::~Block()
{

}

void Block::XMLParser()
{
	try
	{
		HandleAttribute();
		HandleContainner();
	}
	catch (int)
	{
		printf("处理标签出现异常\n");
		exit(1);
	}
}

void Block::HandleAttribute()
{
	try
	{
		const XMLAttribute* Current;
		const XMLAttribute* Next;
		Current = _element->FirstAttribute();
		Next = Current->Next();
		do {
			if (memcmp(Current->Name(), "name", strlen("name")) == 0)
			{
				char* buffer = (char*)malloc(strlen(Current->Value()) + 1);
				if (!buffer) return;
				memcpy(buffer, Current->Value(), strlen(Current->Value()) + 1);
				this->_name = buffer;
				goto END;
			}
		END:
			Current = Current->Next();
			if (!Current) break;
			Next = Current->Next();
		} while (Current != Next);
		//如果没有名字，那么是错误的，必须抛出异常
		if (this->_name == NULL)
		{
			throw NO_NAME;
		}
	}
	catch (int)
	{
		printf("HandleAttribute() 出现异常 No Name\n");
		exit(1);
	}
}

void Block::HandleContainner()
{
	pBASEDATA block;
	unsigned int count;
	XMLElement* Current;
	XMLElement* Next;
	Current = _element->FirstChildElement();
	if (!Current)
	{
		this->_bIsEmptyContainner = 1;
		return;
	}
	if (!Current->NextSiblingElement()) 
	{
		Next = NULL;
	}
	else
	{
		Next = Current->NextSiblingElement();
	}
	count = 0;
	do {
		try
		{
			if (count >= 1024)
			{
				printf("已包含最大数据块，无法添加，丢弃[1025]\n");
				return;
			}
			block = BaseData(Current).TraslationToBaseData();
			try {
				for (int i = 0; i < count; i++)
				{
					if ( block->name[0] && strcmp(this->_vec[i]->name, block->name) == 0)
					{
					throw NAME_CONFLICT;//如果有同名的属性标签，那么异常。
					}
				}
			}
			catch (int)
			{
				printf("错误：%s.%s 名字重复定义", Current->Name(), block->name);
				exit(1);
			}
			this->_vec[count] = block;
			count++;
		}
		catch (int)
		{
			printf("解析Containner出错\n");
			exit(1);
		}
	END:
		Current = Next;
		if (!Current) break;
		Next = Next->NextSiblingElement();
	} while ((Current != Next) || Next);
	this->_RawDataNumber = count;
}
DataBlock* Block::EmptyContainner()
{
	unsigned int TotalMemoryToKeepRawData;
	void* MemoryToKeepRawData;
	DataBlock* usespace = (DataBlock*)malloc(sizeof(DataBlock));
	if (!usespace)
	{
		printf("EmptyContainner::malloc() error\n");
		return NULL;
	}
	//初始化空间
	memset(usespace, 0, sizeof(DataBlock));
	//拷贝name
	memcpy(usespace->name.info, this->_name,64);
	usespace->name.length = 64;
	//拷贝整个BASEDATA到容器
	usespace->obj = new std::vector<BASEDATA* >();
	for(int i =0; this->_vec[i]; i++)
	{
		usespace->obj->push_back(this->_vec[i]);
	}
	return usespace;
}

DataBlock* Block::TraslationToBlockData()
{
	if (!this->_bIsEmptyContainner)
	{
		unsigned int TotalMemoryToKeepRawData;
		void* MemoryToKeepRawData;
		DataBlock* usespace = (DataBlock*)malloc(sizeof(DataBlock));
		if (!usespace)
		{
			printf("TraslationToBlockData::malloc() error\n");
			return NULL;
		}
		//初始化空间
		memset(usespace, 0, sizeof(DataBlock));
		//拷贝name
		usespace->name.info = this->_name;
		usespace->name.length = 64;
		//拷贝整个BASEDATA到容器
		usespace->obj = new std::vector<BASEDATA* >();
		for(int i =0; this->_vec[i]; i++)
		{
			usespace->obj->push_back(this->_vec[i]);
		}
		return usespace;
	}
	else
	{
		return EmptyContainner();
	}

}
