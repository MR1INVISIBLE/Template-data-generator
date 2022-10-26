#include "BaseData.h"


BaseData::BaseData(XMLElement* element)
{
	this->element = element;
	this->_name = 0;
	this->_bIsSimpleLabel = 0;
	XMLParser();
}

BaseData::~BaseData()
{

}

void BaseData::XMLParser()
{
	try
	{
		HandleCommonInfomation();
		HandleAttribute();
		HandleValue();
	}
	catch(int)
	{
		printf("处理标签出现异常\n");
		exit(1);
	}
}

void BaseData::HandleAttribute()
{
	const XMLAttribute* Current;
	const XMLAttribute* Next;
	Current= element->FirstAttribute();
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
		if (strcmp(Current->Name(), "value") == 0)
		{
			this->_Value = (XMLAttribute*)Current;
			_bIsSimpleLabel = 1;
		}
		if (strcmp(Current->Name(), "type") == 0)
		{
			if (!this->_type) {
				if (strcmp(Current->Value(), "ascii") == 0)
					this->_type = 0;
				if (strcmp(Current->Value(), "utf8") == 0)
					this->_type = 1;
				if (strcmp(Current->Value(), "hex") == 0)
					this->_type = 2;
			}
			goto END;
		}
		if (strcmp(Current->Name(), "size") == 0)
		{
			if (!(this->_size)) {
				this->_size = Current->IntValue();
			}
			goto END;
		}
		if (strcmp(Current->Name(), "mutable") == 0)
		{
			this->_mutable = (char)Current->BoolValue();
			goto END;
		}
END:
		Current = Current->Next();
		if (!Current) break;
		Next = Current->Next();
	} while (Current != Next);
}

void BaseData::HandleValue()
{
	if (strcmp(this->element->Name(), "Text") == 0)
	{
		HandleText();
	}
	else if (strcmp(this->element->Name(), "INT") == 0)
	{
		HandleINT();
	}
	else if (strcmp(this->element->Name(), "UINT") == 0)
	{
		HandleUINT();
	}
	else if (strcmp(this->element->Name(), "FLOAT") == 0)
	{
		HandleFLOAT();
	}
	else if (strcmp(this->element->Name(), "DOUBLE") == 0)
	{
		HandleDOUBLE();
	}
	else if (strcmp(this->element->Name(), "BIT") == 0)
	{
		HandleBIT();
	}
	else if (strcmp(this->element->Name(), "BIT16") == 0)
	{
		HandleBIT16();
	}
	else if (strcmp(this->element->Name(), "BIT32") == 0)
	{
		HandleBIT32();
	}
	else if (strcmp(this->element->Name(), "BIT64") == 0)
	{
		HandleBIT64();
	}
	else if (strcmp(this->element->Name(), "LONGLONG") == 0)
	{
		HandleLONGLONG();
	}
	else if (strcmp(this->element->Name(), "ULONGLONG") == 0)
	{
		HandleULONGLONG();
	}
}

void BaseData::HandleCommonInfomation()
{
	//这里需要处理一下type属性
	//具体思路如下，如果是Text,将值设置为0，在下面的函数中处理，否则INT UINT这类别的统统在此处赋值。
	//size处理，如果是均以一字节对齐。Text额外处理，其他在此处处理
	if (strcmp(this->element->Name(), "Text") == 0)
	{
		this->_type = _type::assci;
		this->_size = 0;
	}
	else if (strcmp(this->element->Name(), "INT") == 0)
	{
		this->_type = _type::INT;
		this->_size = sizeof(int);
	}
	else if (strcmp(this->element->Name(), "UINT") == 0)
	{
		this->_type = _type::UINT;
		this->_size = sizeof(unsigned int);
	}
	else if (strcmp(this->element->Name(), "FLOAT") == 0)
	{
		this->_type = _type::FLOAT;
		this->_size = sizeof(float);
	}
	else if (strcmp(this->element->Name(), "DOUBLE") == 0)
	{
		this->_type = _type::DOUBLE;
		this->_size = sizeof(double);
	}
	else if (strcmp(this->element->Name(), "BIT") == 0)
	{
		this->_type = _type::BIT;
		this->_size = sizeof(char);
	}
	else if (strcmp(this->element->Name(), "BIT16") == 0)
	{
		this->_type = _type::BIT16;
		this->_size = sizeof(short);
	}
	else if (strcmp(this->element->Name(), "BIT32") == 0)
	{
		this->_type = _type::BIT32;
		this->_size = sizeof(int);
	}
	else if (strcmp(this->element->Name(), "BIT64") == 0)
	{
		this->_type = _type::BIT64;
		this->_size = sizeof(long long);
	}
	else if (strcmp(this->element->Name(), "LONGLONG") == 0)
	{
		this->_type = _type::LONGLONG;
		this->_size = sizeof(long long);
	}
	else if (strcmp(this->element->Name(), "ULONGLONG") == 0)
	{
		this->_type = _type::ULONGLONG;
		this->_size = sizeof(unsigned long long);
	}
}

void BaseData::HandleText()
{
	try
		{
		if (!this->_bIsSimpleLabel) {
			int SizeToCopy;
				const char* tempbuf = element->GetText();
				if (!this->_size || strlen(tempbuf) == 1)
				{
					SizeToCopy = strlen(tempbuf) + 1;
					this->_size = SizeToCopy;
				}
				else {
					SizeToCopy = this->_size;
				}
				if (strlen(tempbuf) < SizeToCopy)
				{
					SizeToCopy = strnlen(tempbuf, this->_size);
				}
				char* buf = (char*)malloc(SizeToCopy + 1);
				if (!buf) return;
				memset(buf, 0, SizeToCopy + 1);
				memcpy(buf, tempbuf, SizeToCopy);
				this->_value = buf;
		}
		else
		{
			int SizeToCopy;
			const char* data = this->_Value->Value();
			if (!this->_size || strlen(data) == 1)
			{
				SizeToCopy = strlen(data) + 1;
				this->_size = SizeToCopy;
			}
			else {
				SizeToCopy = this->_size;
			}
			if (strlen(data) < SizeToCopy)
			{
				SizeToCopy = strnlen(data, this->_size);
			}
			char* buffer2 = (char*)malloc(strlen(this->_Value->Value()) + 1);
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy + 1);
			memcpy(buffer2, this->_Value->Value(), strlen(this->_Value->Value()) + 1);
			this->_value = buffer2;
		}
	}
	catch (...)
	{
		printf("HandleText() 出错 -->%d\n",element->Name());
		return;
	}
}

void BaseData::HandleINT()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			int tempbuf = element->IntText();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			int* buf = (int*)malloc(sizeof(int));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			int data = this->_Value->IntValue();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			int* buffer2 = (int*)malloc(sizeof(int));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleINT() 出错\n --->%s",element->Name());
		return;
	}
}

void BaseData::HandleUINT()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			unsigned int tempbuf = element->UnsignedText();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			unsigned int* buf = (unsigned int*)malloc(sizeof(unsigned int));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			unsigned int data = this->_Value->UnsignedValue();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			unsigned int* buffer2 = (unsigned int*)malloc(sizeof(unsigned int));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleUINT() 出错\n --->%s", element->Name());
		return;
	}
}

void BaseData::HandleFLOAT()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			float tempbuf = element->FloatText();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			float* buf = (float*)malloc(sizeof(float));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			float data = this->_Value->FloatValue();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			float* buffer2 = (float*)malloc(sizeof(float));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleFLOAT() 出错\n --->%s", element->Name());
		return;
	}
}

void BaseData::HandleDOUBLE()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			double tempbuf = element->DoubleText();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			double* buf = (double*)malloc(sizeof(double));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			double data = this->_Value->DoubleValue();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			double* buffer2 = (double*)malloc(sizeof(double));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleDOUBLE() 出错\n");
		return;
	}
}

void BaseData::HandleBIT()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			//获取标签内的内容
			char tempbuf = element->Bit8Text();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			char* buf = (char*)malloc(sizeof(char));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			//获取属性的值
			char data = this->_Value->Bit8Value();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			char* buffer2 = (char*)malloc(sizeof(char));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleBIT() 出错\n");
		return;
	}
}

void BaseData::HandleBIT16()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			//获取标签内的内容
			short tempbuf = element->Bit16Text();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			short* buf = (short*)malloc(sizeof(short));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			//获取属性的值
			short data = this->_Value->Bit16Value();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			short* buffer2 = (short*)malloc(sizeof(short));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleBIT16() 出错\n");
		return;
	}
}

void BaseData::HandleBIT32()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			//获取标签内的内容
			int tempbuf = element->Bit32Text();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			int* buf = (int*)malloc(sizeof(int));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			//获取属性的值
			int data = this->_Value->Bit32Value();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			int* buffer2 = (int*)malloc(sizeof(int));

			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleBIT32() 出错\n");
		return;
	}
}

void BaseData::HandleBIT64()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			//获取标签内的内容
			long long  tempbuf = element->Bit64Text();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			long long* buf = (long long *)malloc(sizeof(long long));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			//获取属性的值
			long long data = this->_Value->Bit64Value();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			long long* buffer2 = (long long*)malloc(sizeof(long long));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleBIT64() 出错\n");
		return;
	}
}

void BaseData::HandleLONGLONG()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			long long  tempbuf = element->Int64Text();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			long long* buf = (long long*)malloc(sizeof(long long));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			long long data = this->_Value->Int64Value();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			long long* buffer2 = (long long*)malloc(sizeof(long long));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleLONGLONG() 出错\n");
		return;
	}
}

void BaseData::HandleULONGLONG()
{
	try
	{
		if (!this->_bIsSimpleLabel) {
			unsigned long long  tempbuf = element->Unsigned64Text();
			int SizeToCopy = this->_size;
			if (sizeof(tempbuf) < SizeToCopy)
			{
				SizeToCopy = sizeof(tempbuf);
			}
			unsigned long long* buf = (unsigned long long*)malloc(sizeof(unsigned long long));
			if (!buf) return;
			memset(buf, 0, SizeToCopy);
			*buf = tempbuf;
			this->_value = (char*)buf;
		}
		else
		{
			unsigned long long data = this->_Value->Unsigned64Value();
			int SizeToCopy = this->_size;
			if (sizeof(data) < SizeToCopy)
			{
				SizeToCopy = sizeof(data);
			}
			unsigned long long* buffer2 = (unsigned long long*)malloc(sizeof(unsigned long long));
			if (!buffer2) return;
			memset(buffer2, 0, SizeToCopy);
			*buffer2 = data;
			this->_value = (char*)buffer2;
		}
	}
	catch (...)
	{
		printf("HandleULONGLONG() 出错\n");
		return;
	}
}

BASEDATA* BaseData::TraslationToBaseData()
{
	unsigned int usespace;
	//计算整个解析后的数据的大小，记得字符串得额外+1
	usespace = sizeof(BASEDATA);

	if (this->_name && strlen(this->_name) > 63)
	{
		printf("name 不能超过63字节\n");
		return NULL;
	}
	//原始数据解析
	BASEDATA* raw = (BASEDATA*)malloc(usespace);
	//这里将内存全部置为0
	memset(raw, 0, usespace);
	//拷贝数据
	if (this->_name) {
		memcpy(raw->name, this->_name, strlen(this->_name));
	}
	else
	{
		memset(raw->name, 0, 64);
	}
	//这里拷贝数据到Data中
	raw->Data.info = this->_value;
	raw->Data.length = this->_size;

	raw->Mutable = this->_mutable;
	raw->type = this->_type;
	return raw;
	//printf("%s\n %s\n %d %d %d", raw->name, raw->value, raw->Mutable, raw->size, raw->type);
}
