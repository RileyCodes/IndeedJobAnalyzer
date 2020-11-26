#pragma once
#include "global.h"
class TaskMgr
{
	std::vector<QString> taskNames;
	
public:
	TaskMgr()
	{
		if (!EnsureTaskFolderCreated())
		{
			QMessageBox::critical(NULL, "Error", config.SoftwareName + " Failed to Start: Can not create Tasks Folder");
			exit(0);
		}

		const QDir directory(config.TaskFolderName);
		QStringList tasks = directory.entryList(QStringList() << "*.txt" << "*.TXT", QDir::Files);
		foreach(QString filename, tasks)
		{
			taskNames.emplace_back(filename);
		}
	}
	
	std::vector<QString> GetTaskNames()
	{
		return taskNames;
	}
	
	bool EnsureTaskFolderCreated();
	void StartTask(string taskName, string url);
};

