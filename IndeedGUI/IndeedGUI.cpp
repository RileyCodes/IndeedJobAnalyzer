#include "global.h"
#include "IndeedGUI.h"

IndeedGUI::IndeedGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);	
    connect(ui.actionNewTaskButton, SIGNAL(triggered()), this, SLOT(NewTaskClicked()));
    connect(this, SIGNAL(AddLogSignal(QString)), this, SLOT(_AddLog(QString)));
    connect(this, SIGNAL(UpdateTaskUiSignal(QString)), this, SLOT(_UpdateTaskUi(QString)));

    connect(ui.tableView_tasks, SIGNAL(customContextMenuRequested(QPoint)),
        SLOT(customMenuRequested(QPoint)));
	
    SyncTasksToUI();
}

void IndeedGUI::_UpdateTaskUi(QString taskInfoJson)
{
	//should use model based ui and data binding instead
    auto taskInfoJsonDoc = QJsonDocument::fromJson(taskInfoJson.toUtf8());
    auto listTaskInfos = taskInfoJsonDoc.array();

    for (auto list_task_info : listTaskInfos)
    {
        int x = 0;
    }
}

void IndeedGUI::_AddLog(QString message)
{
	auto fullMsg = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
    fullMsg += message + '\n';
    ui.plainTextEditLog->appendPlainText(fullMsg);
}

void IndeedGUI::continueTask()
{
    //const auto selectedItems = ui.tableWidget_tasks->selectedItems();
	
}

void IndeedGUI::restartTask()
{

}

void IndeedGUI::generateReport()
{

}

void IndeedGUI::customMenuRequested(QPoint pos)
{

	//const auto selectedItems = ui.tableWidget_tasks->selectedItems();

 //   if (selectedItems.count() == 0)
 //       return;
	//
 //   QMenu menu(this);

	//auto* action_continueTask = new QAction("Continue Task", this);
 //   connect(action_continueTask, SIGNAL(triggered(bool)), this, SLOT(continueTask(bool)));
 //   menu.addAction(action_continueTask);

	//auto* action_RestartTask = new QAction("Restart Task", this);
 //   connect(action_RestartTask, SIGNAL(triggered(bool)), this, SLOT(restartTask(bool)));
 //   menu.addAction(action_RestartTask);

	//auto* action_GenerateReport = new QAction("Generate Report", this);
 //   connect(action_GenerateReport, SIGNAL(triggered(bool)), this, SLOT(generateTask(bool)));
 //   menu.addAction(action_GenerateReport);
	//
	//menu.exec(ui.tableWidget_tasks->viewport()->mapToGlobal(pos));
}


void IndeedGUI::AddLog(QString message)
{
	//pass to main thread by signal
    emit AddLogSignal(message);
}

void IndeedGUI::UpdateTaskInfo(QString TaskInfoJson)
{
    //pass to main thread by signal
	emit UpdateTaskUiSignal(TaskInfoJson);
}



void IndeedGUI::SyncTasksToUI()
{
    ui.tableView_tasks->horizontalHeader()->setVisible(true);

    auto taskInfos = taskMgr.GetTasksInfo();
    auto* modelTasksTable = new Model_TasksTable();

    QList<TaskRowData> tasksTableData;
    {
        TaskRowData test;
        test.status = "1";
        test.jobCount = "2";
        test.pagesAt = "3";
        test.taskName = "4";
        tasksTableData.push_back(test);
    }
    modelTasksTable->populateData(tasksTableData);
    ui.tableView_tasks->setModel(modelTasksTable);

    ui.tableView_tasks->show();
	
    {
        TaskRowData test;
        test.status = "1";
        test.jobCount = "2";
        test.pagesAt = "3";
        test.taskName = "4";
        tasksTableData.push_back(test);
    }
    modelTasksTable->populateData(tasksTableData);
    ui.tableView_tasks->setModel(modelTasksTable);

}

void IndeedGUI::NewTaskClicked()
{
    DialogNewTask newTask;
    newTask.exec();	
}