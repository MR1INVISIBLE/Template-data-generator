#pragma once
#include "XML.h"
#include "DataInfo.h"

using namespace tinyxml2;

class Register : public DataInfo
{
public:
	Register(XMLElement* element);
	~Register();
private:
	XMLElement* _element;
	RegisterData* _Reg;
private:
	void XMLParser();
	void HandleAttribute();
	void HandleContainner();
public:
	RegisterData* TraslationToRegisterData();

};