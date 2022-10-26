#include <exception>
#include "State.h"
#include "XML.h"
#include "Action.h"

#define ReturnStateSize(PASS) sizeof(_ActionData)

State::State(XMLElement* element)
{
	this->_element = element;
	XMLParser();
}

State::~State()
{
	free(this->_name);
}

void State::XMLParser()
{
	try
	{
		HandleAttribute();
		HandleContainner();
	}
	catch (int)
	{
		printf("�������� ��<%s name=%s />", _element->Name(), _element->FirstAttribute()->Value());
		exit(0);
	}
}

void State::HandleAttribute()
{
	try
	{
		const XMLAttribute* Current;
		const XMLAttribute* Next;
		Current = _element->FirstAttribute();
		Next = Current->Next();
		do {
			if (strcmp(Current->Name(), "name") == 0)
			{
				char* buffer = (char*)malloc(strlen(Current->Value()) + 1);
				if (!buffer) return;
				memset(buffer, 0, strlen(Current->Value()) + 1);
				memcpy(buffer,  Current->Value(), strlen(Current->Value()) + 1);
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

void State::HandleContainner()
{
	pActionData action;
	unsigned int count;
	XMLElement* Current;
	XMLElement* Next = NULL;
	Current = XMLHandle(_element).FirstChildElement().ToElement();
	if (!Current)
	{
		printf("������յ�<State> \n");
		exit(1);
	}
	if (!Current->NextSibling())
	{
		Next = NULL;
	}
	else
	{
		Next = Current->NextSibling()->ToElement();
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
			action = Action(Current).TraslationToActionData();
			try {
				for (int i = 0; i < count; i++)
				{
					if (strcmp(this->_vec[i]->name, action->name) == 0)
					{
						throw NAME_CONFLICT;//�����ͬ�������Ա�ǩ����ô�쳣��
					}
				}
			}
			catch (int)
			{
				printf("����%s.%s �����ظ�����", Current->Name(), action->name);
				exit(1);
			}
			this->_vec[count] = action;
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
	this->_NumberOfAction = count;
}

StateData* State::TraslationToStateData()
{
	unsigned int TotalMemoryToKeepRawData;
	void* MemoryToKeepRawData;
	StateData* usespace = (StateData*)malloc(sizeof(StateData));
	if (!usespace)
	{
		printf("TraslationToStateData::malloc() error\n");
		return NULL;
	}
	//��ʼ���ռ�
	memset(usespace, 0, sizeof(StateData));
	//����name
	memcpy(usespace->name, this->_name, 64);
	//����
	usespace->NumberOfAction = this->_NumberOfAction;
	TotalMemoryToKeepRawData = 0;
	//����RawDataNumber��С
	for (int i = 0; i < this->_NumberOfAction; i++)
	{
		TotalMemoryToKeepRawData += ReturnStateSize(this->_vec[i]);
		//printf("%s \n",this->_vec[i]->data.name);
	}
	//��ʼ����Actiondata
	MemoryToKeepRawData = malloc(TotalMemoryToKeepRawData);
	memset(MemoryToKeepRawData, 0, TotalMemoryToKeepRawData);
	void* temp = MemoryToKeepRawData;
	for (int i = 0; i < this->_NumberOfAction; i++)
	{
		memcpy(temp, \
			this->_vec[i], \
			ReturnStateSize(this->_vec[i]));
		temp = ((char*)temp) + ReturnStateSize(this->_vec[i]);
	}
	usespace->Address = (ActionData*)MemoryToKeepRawData;
	return usespace;
}
