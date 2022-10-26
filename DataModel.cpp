#include "DataModel.h"

#include "Block.h"

#define ReturnDataModelSize(PASS) sizeof(RawDataBlock)
DataModelContainner::DataModelContainner(XMLElement* element)
{
	this->_element = element;
	this->_BlockNumber = 1024;
	this->_name = { 0 };
	this->_bIsEmptyContainner = 0;
	XMLParser();
}

DataModelContainner::~DataModelContainner()
{
	
}

void DataModelContainner::XMLParser()
{
	try
	{
		HandleAttribute();
		HandleContainner();
	}
	catch (int)
	{
		printf("�����ǩ�����쳣: %s-%s\n",this->_element->Name(),this->_element->FirstAttribute()->Value());
		exit(1);
	}
}

void DataModelContainner::HandleAttribute()
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
				memset(buffer, 0, strlen(Current->Value()) + 1);
				memcpy(buffer, Current->Value(), strlen(Current->Value()) + 1);
				this->_name = buffer;
				goto END;
			}
		END:
			Current = Current->Next();
			if (!Current) break;
			Next = Current->Next();
		} while (Current != Next);
		//���û�����֣���ô�Ǵ���ģ������׳��쳣
		if (this->_name == NULL)
		{
			throw NO_NAME;
		}
	}
	catch (int)
	{
		printf("HandleAttribute() �����쳣 No Name\n");
		exit(1);
	}
}

void DataModelContainner::HandleContainner()
{
	DataBlock* block;
	unsigned int count;
	XMLElement* Current;
	XMLElement* Next = NULL;
	Current = _element->FirstChildElement();
	if (!Current)
	{
		this->_bIsEmptyContainner = 1;
		return;
	}
	if (!Current->NextSiblingElement())
	{
		Next == NULL;
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
				printf("�Ѱ���������ݿ飬�޷���ӣ�����[1025]\n");
				return;
			}
			block = Block(Current).TraslationToBlockData();
			try {
				for (int i = 0; i < count; i++)
				{

					if (strcmp(this->_vec[i]->name.info, block->name.info) == 0)
					{
						throw NAME_CONFLICT;//�����ͬ�������Ա�ǩ����ô�쳣��
					}
				}
			}
			catch (int)
			{
				printf("����%s.%s �����ظ�����", Current->Name(), block->name);
				exit(1);
			}
			this->_vec[count] = block;
			count++;
		}
		catch (int)
		{
			printf("����Containner����\n");
			exit(1);
		}
	END:
		Current = Next;
		if (!Current) break;
		Next = Next->NextSiblingElement();
	} while ((Current != Next) || Next);
	this->_BlockNumber = count;
}

DataModel* DataModelContainner::EmptyContainner()
{
	unsigned int TotalMemoryToKeepRawData;
	void* MemoryToKeepRawData;
	DataModel* usespace = (DataModel*)malloc(sizeof(DataModel));
	memset(usespace, 0, sizeof(usespace));
	if (!usespace)
	{
		printf("TraslationToRawDataBlock::malloc() error\n");
		return NULL;
	}
	//��ʼ���ռ�
	memset(usespace, 0, sizeof(DataModel));
	//����name
	memcpy(usespace->name.info, this->_name, 64);
	usespace->name.length = 64;

	usespace->obj = new std::vector<DataBlock* >();
	//����
	for(int i =0; this->_vec[i]; i++)
	{
		usespace->obj->push_back(this->_vec[i]);
	}
	return usespace;
}

DataModel* DataModelContainner::TraslationToDataModel()
{
	if (!this->_bIsEmptyContainner) {
		unsigned int TotalMemoryToKeepRawData;
		void* MemoryToKeepRawData;
		DataModel* usespace = (DataModel*)malloc(sizeof(DataModel));
		memset(usespace, 0, sizeof(usespace));
		if (!usespace)
		{
			printf("TraslationToRawDataBlock::malloc() error\n");
			return NULL;
		}
		//��ʼ���ռ�
		memset(usespace, 0, sizeof(DataModel));
		//����name
		usespace->name.info = this->_name;
		usespace->name.length = 64;

		usespace->obj = new std::vector<DataBlock* >();
		//����
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
