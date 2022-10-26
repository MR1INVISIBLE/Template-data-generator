
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "XML.h"
#include "BaseData.h"
#include "Block.h"
#include "DataModel.h"
#include "Action.h"
#include "State.h"
#include "utils.h"
using namespace tinyxml2;

int main()
{
	XMLDocument* doc = new XMLDocument();
	if (doc->LoadFile("C:\\Users\\invis\\Desktop\\XMLParser\\raw2.xml") != XML_SUCCESS)
	{
		getchar();
		printf("����ʧ��\n");
	}
	DataModel* data = DataModelContainner(doc->FirstChildElement()).TraslationToDataModel();
	SeedCombiner* sc = new SeedCombiner(data);
	char* buf = sc->Extractant();
	sc->WriteFile("D:\\11\\seek.txt");
	free(buf);
}
