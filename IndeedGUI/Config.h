#pragma once
#include "global.h"
static class Config
{
public:
	QString TaskFolderName;
	QString SoftwareName;

	Config()
	{
		TaskFolderName = "./Tasks/";
		SoftwareName = "IndeedJobAnalyser";
	}
};

