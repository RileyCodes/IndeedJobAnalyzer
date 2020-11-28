#pragma once
#include "global.h"
class TaskMgr
{
	std::vector<QString> taskNames;
	
public:
	void LoadTasksFromDisk()
	{
		const QDir directory(config.TaskFolderName);
		QStringList tasks = directory.entryList(QStringList() << "*.txt" << "*.TXT", QDir::Files);
		foreach(QString filename, tasks)
		{
			QFile file(config.TaskFolderName + filename);
			if (!file.open(QIODevice::ReadOnly)) 
			{
				continue;
			}
			;
			QTextStream in(&file);

			auto qTaskJsonStr = in.readAll();
			

		}
	}

	TaskMgr()
	{
		if (!EnsureTaskFolderCreated())
		{
			QMessageBox::critical(NULL, "Error", config.SoftwareName + " Failed to Start: Can not create Tasks Folder");
			exit(0);
		}

		LoadTasksFromDisk();
	}
	
	std::vector<QString> GetTaskNames()
	{
		return taskNames;
	}
	
	bool EnsureTaskFolderCreated();
	void StartTask(string taskName, string url);
};

