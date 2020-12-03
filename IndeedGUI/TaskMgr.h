#pragma once
#include "global.h"
class TaskMgr
{

	Model_TasksTable model;

public:


	TaskMgr()
	{
		if (!EnsureTaskFolderCreated())
		{
			QMessageBox::critical(NULL, "Error", pConfig->SoftwareName +
				" Failed to Start: Can not create Tasks Folder");
			exit(0);
		}

		model.LoadTasksFromDisk();
	}

	void BindModelToTable(QTableView* tableToBind)
	{
		tableToBind->setModel(&model);
	}


	void UpdateRunningTasksInfo(QString runningTasksJson)
	{
		model.UpdateRunningTasksInfo(runningTasksJson);
	}
	
	bool EnsureTaskFolderCreated();
	void StartTask(string taskName, string url);
};

