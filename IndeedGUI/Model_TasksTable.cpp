#include "global.h"
#include "Model_TasksTable.h"

#define COL_COUNT_TASKS 4

bool Model_TasksTable::BuildTaskRowDataFromJson(QJsonObject objTaskInfo, TaskRowData& taskInfo)
{
	if (!objTaskInfo["TaskName"].isString())
		return true;
	taskInfo.TaskName = objTaskInfo["TaskName"].toString();

	if (!objTaskInfo["Url"].isString())
		return true;
	taskInfo.Url = objTaskInfo["Url"].toString();

	if (objTaskInfo["JobCount"].isNull())
		return true;
	taskInfo.JobCount = QString::number(objTaskInfo["JobCount"].toInt());

	if (objTaskInfo["PagesAt"].isNull())
		return true;
	taskInfo.PagesAt = QString::number(objTaskInfo["PagesAt"].toInt());

	if (!objTaskInfo["Status"].isNull())
		taskInfo.Status = objTaskInfo["Status"].toString();
	
	return false;
}

void Model_TasksTable::LoadTasksFromDisk()
{
	const QDir directory(pConfig->TaskFolderName);
	auto tasks = directory.entryList(QDir::Files);
	foreach(QString filename, tasks)
	{
		QFile file(pConfig->TaskFolderName + filename);
		if (!file.open(QIODevice::ReadOnly))
		{
			continue;
		}

		QTextStream in(&file);
		auto qTaskJsonStr = in.readAll();
		auto jsonDoc = QJsonDocument::fromJson(qTaskJsonStr.toUtf8());
		auto objTaskInfo = jsonDoc.object();

		TaskRowData taskRowData;

		if (BuildTaskRowDataFromJson(objTaskInfo, taskRowData)) continue;

		taskRowData.Status = "Saved";

		taskInfos[taskRowData.TaskName] = taskRowData;
	}
	emit layoutChanged();
}

Model_TasksTable::Model_TasksTable(QObject* parent)
{
	
}




int Model_TasksTable::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return taskInfos.count();
}

int Model_TasksTable::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return COL_COUNT_TASKS;
}

QVariant Model_TasksTable::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole) {
		return QVariant();
	}

	if (taskInfos.count() < index.row())
	{
		QString ErrMsg = "Error: out of bounds in Model_TasksTable::data:";
		ErrMsg += QString::number(index.row());
		AddLog(const_cast<char*>(ErrMsg.toStdString().c_str()));
		return QVariant();
	}

	auto taskKeys = taskInfos.keys();

	const auto keyToBeAccessed = taskKeys[index.row()];

	switch (index.column())
	{
	case tableTasks_TaskName:
		return taskInfos[keyToBeAccessed].TaskName;
	case tableTasks_PagesAt:
		return taskInfos[keyToBeAccessed].PagesAt;
	case tableTasks_JobCount:
		return taskInfos[keyToBeAccessed].JobCount;
	case tableTasks_Status:
		return taskInfos[keyToBeAccessed].Status;
	}

	return QVariant();
}

QVariant Model_TasksTable::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) 
	{
		switch (section)
		{
		case tableTasks_TaskName:
			return QString("Task Name");
		case tableTasks_PagesAt:
			return QString("Pages At");
		case tableTasks_JobCount:
			return QString("Job Count");
		case tableTasks_Status:
			return QString("Status");
		}
	}
	return QVariant();
}

void Model_TasksTable::UpdateRunningTasksInfo(QString runningTasksJson)
{
	auto taskInfoJsonDoc = QJsonDocument::fromJson(runningTasksJson.toUtf8());
	auto listTaskInfos = taskInfoJsonDoc.array();

	for (auto taskInfo : listTaskInfos)
	{
		auto objTaskInfo = taskInfo.toObject();

		TaskRowData taskRowData;

		if (BuildTaskRowDataFromJson(objTaskInfo, taskRowData)) continue;

		taskInfos[taskRowData.TaskName] = taskRowData;
	}

	
	emit layoutChanged();
}

TaskRowData Model_TasksTable::GetTaskRowDataByIndex(int rowIndex)
{
	auto keys = taskInfos.keys();
	return taskInfos[keys[rowIndex]];
}
