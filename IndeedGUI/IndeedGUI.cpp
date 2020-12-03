#include "global.h" 
#include "IndeedGUI.h"

IndeedGUI::IndeedGUI(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.actionNewTaskButton, SIGNAL(triggered()), this, SLOT(NewTaskClicked()));
	connect(this, SIGNAL(AddLogSignal(QString)), this, SLOT(_AddLog(QString)));
	connect(this, SIGNAL(UpdateTaskUiSignal(QString)), this, SLOT(_UpdateTaskUi(QString)));

	connect(ui.tableView_tasks, SIGNAL(customContextMenuRequested(QPoint)),
		SLOT(customMenuRequested(QPoint)));

	pTaskMgr->BindModelToTable(ui.tableView_tasks);
}

void IndeedGUI::_UpdateTaskUi(QString taskInfoJson)
{
	pTaskMgr->UpdateRunningTasksInfo(taskInfoJson);
}

void IndeedGUI::_AddLog(QString message)
{
	ui.plainTextEditLog->appendPlainText(message);
}

void IndeedGUI::rerunTask()
{
	auto selectedTasks = GetSelectedTasks();

	for (auto &selectedTask : selectedTasks)
	{
		if (selectedTask.Status == Util::GetStatusStringByType(Status::Saved) ||
			selectedTask.Status == Util::GetStatusStringByType(Status::Failed) ||
			selectedTask.Status == Util::GetStatusStringByType(Status::Completed))
		{
			pRequestMgr->StartTask(selectedTask.TaskName,selectedTask.Url);
		}
	}
}

void IndeedGUI::_Test(void(IndeedGUI::*contextMenuAction)(const TaskRowData&))
{
	QList<TaskRowData> taskRowDatas;
	auto* const selectionModel = ui.tableView_tasks->selectionModel();
	auto selectedRows = selectionModel->selectedRows();
	for (auto selectedRow : selectedRows)
	{
		const int rowIndex = selectedRow.row();
		auto modelTasksTable = (Model_TasksTable*)selectedRow.model();
		auto taskRowData = modelTasksTable->GetTaskRowDataByIndex(rowIndex);

		//contextMenuAction(taskRowData);
	}
}
QList<TaskRowData> IndeedGUI::GetSelectedTasks()
{
	QList<TaskRowData> taskRowDatas;
	auto* const selectionModel = ui.tableView_tasks->selectionModel();
	auto selectedRows = selectionModel->selectedRows();
	for (auto selectedRow : selectedRows)
	{
		const int rowIndex = selectedRow.row();
		auto modelTasksTable = (Model_TasksTable*)selectedRow.model();
		auto taskRowData = modelTasksTable->GetTaskRowDataByIndex(rowIndex);

		taskRowDatas.push_back(taskRowData);
	}
	return taskRowDatas;
}

void IndeedGUI::stopTask()
{
	auto selectedTasks = GetSelectedTasks();

	for (auto selectedTask : selectedTasks)
	{
		if (selectedTask.Status != Util::GetStatusStringByType(Status::Running) &&
			selectedTask.Status != Util::GetStatusStringByType(Status::Initialized))
			continue;
		
		pRequestMgr->StopTask(selectedTask.TaskName);
	}
}


void IndeedGUI::restartTask()
{
	auto selectedTasks = GetSelectedTasks();

	for (auto& selectedTask : selectedTasks)
	{
		if (selectedTask.Status == Util::GetStatusStringByType(Status::Saved) ||
			selectedTask.Status == Util::GetStatusStringByType(Status::Failed) ||
			selectedTask.Status == Util::GetStatusStringByType(Status::Completed))
		{
			pRequestMgr->RestartTask(selectedTask.TaskName,selectedTask.Url);
		}
	}
}



void IndeedGUI::generateReport()
{

}


void IndeedGUI::customMenuRequested(QPoint pos)
{

	auto* const selectionModel = ui.tableView_tasks->selectionModel();
	auto selectedRows = selectionModel->selectedRows();

	if (selectedRows.count() == 0)
		return;
	
	QMenu menu(this);

	auto* action_continueTask = new QAction("Rerun Task", this);
	connect(action_continueTask, SIGNAL(triggered(bool)), this, SLOT(rerunTask()));
	menu.addAction(action_continueTask);

	auto* action_RestartTask = new QAction("Reset and run Task", this);
	connect(action_RestartTask, SIGNAL(triggered(bool)), this, SLOT(restartTask()));
	menu.addAction(action_RestartTask);

	auto* action_StopTask = new QAction("Stop Task", this);
	connect(action_StopTask, SIGNAL(triggered(bool)), this, SLOT(stopTask()));
	menu.addAction(action_StopTask);

	auto* action_GenerateReport = new QAction("Generate Report", this);
	connect(action_GenerateReport, SIGNAL(triggered(bool)), this, SLOT(generateReport()));
	menu.addAction(action_GenerateReport);

	menu.exec(ui.tableView_tasks->viewport()->mapToGlobal(pos));

}


void IndeedGUI::AddLogWithMeta(QString message)
{
	auto fullMsg = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
	fullMsg += message;

	AddLog(fullMsg);
}

void IndeedGUI::AddLog(QString message)
{
	//pass to main thread by signal
	emit AddLogSignal(message);
}

void IndeedGUI::AddPendingLog()
{
	auto pendingLogsStd = pendingLogMgr.GetAllPendingLogs();
	pendingLogsStd += "---END OF LOGS BEFORE START---\n";

	QString pendingLogs;
	pendingLogs = QString::fromStdString(pendingLogsStd);

	AddLog(pendingLogs);
}

void IndeedGUI::UpdateTaskInfo(QString TaskInfoJson)
{
	//pass to main thread by signal
	emit UpdateTaskUiSignal(TaskInfoJson);
}





void IndeedGUI::NewTaskClicked()
{
	DialogNewTask newTask;
	newTask.exec();
}