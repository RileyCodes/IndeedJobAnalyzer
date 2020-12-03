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

	QMap<QString,TaskRowData> taskInfos;
	
public:

    bool BuildTaskRowDataFromJson(QJsonObject objTaskInfo, TaskRowData& taskInfo);
    void LoadTasksFromDisk();

    Model_TasksTable(QObject* parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	void UpdateRunningTasksInfo(QString runningTasksJson);
    TaskRowData GetTaskRowDataByIndex(int rowIndex);
};