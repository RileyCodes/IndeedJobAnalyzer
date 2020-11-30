#pragma once
#include "global.h"
class TaskMgr
{

	struct TaskInfo
	{
		QString TaskName;
		QString Url;
		int JobCount;
		int PagesAt;
	};
	std::vector<TaskInfo> taskInfos;

public:
	void LoadTasksFromDisk()
	{
		const QDir directory(config.TaskFolderName);
		auto tasks = directory.entryList(QDir::Files);
		foreach(QString filename, tasks)
		{
			QFile file(config.TaskFolderName + filename);
			if (!file.open(QIODevice::ReadOnly))
			{
				continue;
			}

			QTextStream in(&file);
			auto qTaskJsonStr = in.readAll();
			auto jsonDoc = QJsonDocument::fromJson(qTaskJsonStr.toUtf8());
			auto objTaskInfo = jsonDoc.object();

			TaskInfo taskInfo;

			if (!objTaskInfo["TaskName"].isString())
				continue;
			taskInfo.TaskName = objTaskInfo["TaskName"].toString();

			if (!objTaskInfo["Url"].isString())
				continue;
			taskInfo.Url = objTaskInfo["Url"].toString();

			if (objTaskInfo["JobCount"].isNull())
				continue;
			taskInfo.JobCount = objTaskInfo["JobCount"].toInt();

			if (objTaskInfo["PagesAt"].isNull())
				continue;
			taskInfo.PagesAt = objTaskInfo["PagesAt"].toInt();

			taskInfos.emplace_back(taskInfo);
		}
	}

	TaskMgr()
	{
		if (!EnsureTaskFolderCreated())
		{
			QMessageBox::critical(NULL, "Error", config.SoftwareName +
				" Failed to Start: Can not create Tasks Folder");
			exit(0);
		}

		LoadTasksFromDisk();
	}

	std::vector<TaskInfo> GetTasksInfo()
	{
		return taskInfos;
	}

	bool EnsureTaskFolderCreated();
	void StartTask(string taskName, string url);
};

