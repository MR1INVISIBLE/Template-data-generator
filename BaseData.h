#pragma once
#include "DataInfo.h"
#include "XML.h"
using namespace tinyxml2;

class BaseData : public DataInfo
{
public:
	BaseData(XMLElement* element);
	~BaseData();
private:
	char* _name;
	unsigned int _size;
	char* _value;
	char _type;
	bool _mutable;
	bool _bIsSimpleLabel;
	XMLElement* element;
	XMLAttribute* _Value;
private:
	//解析
	void XMLParser();
	//处理属性
	void HandleAttribute();
	//处理属性对应的值
	void HandleValue();
	//处理一些相同，默认的数据
	void HandleCommonInfomation();
	//处理各种标签
	void HandleText();
	void HandleINT();
	void HandleUINT();
	void HandleFLOAT();
	void HandleDOUBLE();
	void HandleLONGLONG();
	void HandleULONGLONG();
	void HandleBIT();
	void HandleBIT16();
	void HandleBIT32();
	void HandleBIT64();
public:
	//将处理好的数据封装
	BASEDATA* TraslationToBaseData();
};
