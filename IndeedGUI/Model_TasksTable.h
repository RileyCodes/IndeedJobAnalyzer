#pragma once
#include "global.h"

struct TaskRowData
{
    QString taskName;
    QString pagesAt;
    QString jobCount;
    QString status;
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
	
public:

    Model_TasksTable(QObject* parent = 0);
    void populateData(const  QList<TaskRowData>& tasksData);

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    QList<TaskRowData> tasksTableData;

};