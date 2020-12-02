#pragma once
#include "global.h"

struct TaskRowData
{
    QString TaskName;
    QString PagesAt;
    QString JobCount;
    QString Status;
	QString Url;
};

class Model_TasksTable : public QAbstractTableModel
{
    Q_OBJECT

    enum
    {
        tableTasks_TaskName,
        tableTasks_PagesAt,
        tableTasks_JobCount,
        tableTasks_Status
    };

	QList<TaskRowData> tasksTableData;
	
public:

	void LoadTasksFromDisk()
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

			TaskRowData taskInfo;

			if (!objTaskInfo["TaskName"].isString())
				continue;
			taskInfo.TaskName = objTaskInfo["TaskName"].toString();

			if (!objTaskInfo["Url"].isString())
				continue;
			taskInfo.Url = objTaskInfo["Url"].toString();

			if (objTaskInfo["JobCount"].isNull())
				continue;
			taskInfo.JobCount = QString::number(objTaskInfo["JobCount"].toInt());

			if (objTaskInfo["PagesAt"].isNull())
				continue;
			taskInfo.PagesAt = QString::number(objTaskInfo["PagesAt"].toInt());

			taskInfo.Status = "Saved";
			
			tasksTableData.push_back(taskInfo);
		}
	}

    Model_TasksTable(QObject* parent = 0);
    void populateData(const  QList<TaskRowData>& tasksData);

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};