#pragma once
#include "XML.h"
#include "DataInfo.h"

using namespace tinyxml2;


class Action :public DataInfo 
{
public:
	Action(XMLElement* element);
	~Action();
private:
	void XMLParser();
private:
	char* _name;
	unsigned int _mode;
	char* _ChangedTo;
	char* _UseData;
	XMLElement* _element;
private:
	void HandleAttribute();
public:
	ActionData* TraslationToActionData();
};