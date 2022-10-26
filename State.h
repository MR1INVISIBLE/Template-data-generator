#pragma once
#include "DataInfo.h"
#include "XML.h"

using namespace tinyxml2;

class State :public DataInfo
{
public:
	State(XMLElement* element);
	~State();
private:
	void XMLParser();
private:
	char* _name;
	ActionData* _vec[1024];
	XMLElement* _element;
	unsigned int _NumberOfAction;
	char _bIsEmptyContainner;
private:
	void HandleAttribute();
	void HandleContainner();
public:
	StateData* TraslationToStateData();
};