#pragma once
#include "DataInfo.h"
#include "XML.h"
using namespace tinyxml2;

class DataModelContainner :public DataInfo 
{
public:
	DataModelContainner(XMLElement* element);
	~DataModelContainner();
private:
	char* _name;
	XMLElement* _element;
	unsigned int _BlockNumber;
	char _bIsEmptyContainner;
	DataBlock* _vec[1024] = {0}; //һ��DataModel������1024�����ڵ�

private:
	virtual void XMLParser();
	void HandleAttribute();
	void HandleContainner();

public:
	DataModel* EmptyContainner();
	DataModel* TraslationToDataModel();
};
