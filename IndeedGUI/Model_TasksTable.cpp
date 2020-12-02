#include "global.h"
#include "Model_TasksTable.h"

#define COL_COUNT_TASKS 4

Model_TasksTable::Model_TasksTable(QObject* parent)
{
}

void Model_TasksTable::populateData(const QList<TaskRowData>& tasksData)
{
	tasksTableData.append(tasksData);
	emit dataChanged(index(0, 0), index(10, 10));
	

}


int Model_TasksTable::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	QString ErrMsg = "Row count:";
	ErrMsg += QString::number(tasksTableData.count());

	AddLog(const_cast<char*>(ErrMsg.toStdString().c_str()));
	
	return tasksTableData.count();
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

	if (tasksTableData.length() < index.row())
	{
		QString ErrMsg = "Error: out of bounds in Model_TasksTable::data:";
		ErrMsg += QString::number(index.row());

		AddLog(const_cast<char*>(ErrMsg.toStdString().c_str()));
		return QVariant();
	}

	auto test = index.row();

	switch (index.column())
	{
	case tableTasks_TaskName:
		return tasksTableData[index.row()].taskName;
	case tableTasks_PagesAt:
		return tasksTableData[index.row()].pagesAt;
	case tableTasks_JobCount:
		return tasksTableData[index.row()].jobCount;
	case tableTasks_Status:
		return tasksTableData[index.row()].status;
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
