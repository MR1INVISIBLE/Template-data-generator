#pragma once
#include "DataInfo.h"
#include "XML.h"
#include <vector>
using namespace tinyxml2;
class Block : public DataInfo
{
public:
	Block(XMLElement* element);
	~Block();
private:
	char* _name;
	XMLElement* _element;
	unsigned int _RawDataNumber;
	char _bIsEmptyContainner;
	BASEDATA* _vec[1024] = {0};  //一个Block最多包含1024个根节点

private:
	virtual void XMLParser();
	void HandleAttribute();
	void HandleContainner();
public:
	DataBlock* EmptyContainner();
	DataBlock* TraslationToBlockData();
};
