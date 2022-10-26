#include "Register.h"

using namespace tinyxml2;

Register::Register(XMLElement* element)
{
	this->_element = element;
	_Reg = (RegisterData*)malloc(sizeof(RegisterData));
	if (!_Reg) return;
	memset(_Reg, 0, sizeof(RegisterData));
	XMLParser();
}

Register::~Register()
{
	free(this->_Reg);
}

void Register::XMLParser()
{
	try
	{
		HandleAttribute();
		HandleContainner();
	}
	catch (int)
	{
		printf("解析错误 ：<%s name=%s />", _element->Name(), _element->FirstAttribute()->Value());
		exit(0);
	}
}

void Register::HandleAttribute()
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
				if (strlen(Current->Value()) > 63)
				{
					printf("name 不能超过63字节\n");
					exit(1);
				}
				memcpy_s(this->_Reg->name, strlen(Current->Value()) + 1, Current->Value(), strlen(Current->Value()) + 1);
				goto END;
			}
		END:
			Current = Current->Next();
			if (!Current) break;
			Next = Current->Next();
		} while (Current != Next);
		//如果没有名字，那么是错误的，必须抛出异常
		if (this->_Reg->name[0] == NULL)
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

void Register::HandleContainner()
{
	unsigned int count;
	XMLElement* Current;
	XMLElement* Next;
	Current = XMLHandle(_element).FirstChildElement().ToElement();
	if (!Current)
	{
		printf("不允许空的<Register> \n");
		exit(1);
	}
	if(!Current->NextSibling())
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
			if (strcmp(Current->Name(), "Debugger") == 0)
			{
				//DebuggerData* debugger = Debugger(Current).TraslationToDebuggerData();
				//this->_Reg->debugger = debugger;
			}
			if (strcmp(Current->Name(), "State") == 0)
			{
				if (strcmp(Current->FirstAttribute()->Name(), "name") == 0)
				{
					const char* value = Current->FirstAttribute()->Value();
					char* buf = (char*)malloc(strlen(value) + 1);
					memcpy(buf, value, strlen(value) + 1);
					_Reg->state = buf;
				}
			}
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
}

RegisterData* Register::TraslationToRegisterData()
{
	RegisterData* reg = (RegisterData*)malloc(sizeof(RegisterData));
	if (!reg) return NULL;
	memset(reg, 0, sizeof(RegisterData));
	memcpy(reg, this->_Reg, sizeof(RegisterData));
	return reg;
}
