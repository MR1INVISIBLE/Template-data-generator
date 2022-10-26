#include "Action.h"

Action::Action(XMLElement* element)
{
	this->_element = element;
	XMLParser();
}

Action::~Action()
{
	free(this->_name);
	free(this->_UseData);
}

void Action::XMLParser()
{
	try
	{
		HandleAttribute();
	}catch(int)
	{
		printf("�������� ��<%s name=%s .../>", _element->Name(), _element->FirstAttribute()->Value());
		exit(1);
	}
}

void Action::HandleAttribute()
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
				memcpy(buffer, Current->Value(), strlen(Current->Value()) + 1);
				this->_name = buffer;
				goto END;
			}
			if (strcmp(Current->Name(), "mode") == 0)
			{
				if (strcmp(Current->Value(), "input") == 0)
				{
					this->_mode = _ActionMode::Input;
				}
				if (strcmp(Current->Value(), "output") == 0)
				{
					this->_mode = _ActionMode::Output;
				}
				if (strcmp(Current->Value(), "changestate") == 0)
				{
					this->_mode = _ActionMode::ChangeState;
				}
				goto END;
			}
			if (strcmp(Current->Name(), "changeto") == 0 )
			{
				char* buffer = (char*)malloc(strlen(Current->Value()) + 1);
				if (!buffer) return;
				memset(buffer, 0, strlen(Current->Value()) + 1);
				memcpy(buffer, Current->Value(), strlen(Current->Value()) + 1);
				this->_ChangedTo = buffer;
				this->_UseData = buffer;
				goto END;
			}
			if (strcmp(Current->Name(), "datamodel") == 0)
			{
				char* buffer = (char*)malloc(strlen(Current->Value()) + 1);
				if (!buffer) return;
				memset(buffer, 0, strlen(Current->Value()) + 1);
				memcpy(buffer, Current->Value(), strlen(Current->Value()) + 1);
				this->_ChangedTo = buffer;
				this->_UseData = buffer;
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
	if (!this->_mode || !this->_ChangedTo || !this->_UseData)
	{
		throw ACTION_LOSS_ATTRI;
	}
}

ActionData* Action::TraslationToActionData()
{
	unsigned int usespace;
	unsigned int nameSize;
	//������������������ݵĴ�С���ǵ��ַ����ö���+1
	usespace = sizeof(this->_mode) + sizeof(char[64]) * 2;
	if (strlen(this->_name) > 63)
	{
		printf("name ���ܳ���63�ֽ�\n");
		return NULL;
	}
	//ԭʼ���ݽ���
	ActionData* raw = (ActionData*)malloc(usespace);
	//���ｫ�ڴ�ȫ����Ϊ0
	memset(raw, 0, usespace);
	//��������
	memcpy(raw->name, this->_name, strlen(this->_name));
	memcpy(raw->ChangedTo, this->_ChangedTo, strlen(this->_ChangedTo));
	raw->mode = this->_mode;
	return raw;
}
