#include "global.h"
#include "IndeedGUI.h"

IndeedGUI::IndeedGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);	
    connect(ui.actionNewTaskButton, SIGNAL(triggered()), this, SLOT(NewTaskClicked()));
    connect(this, SIGNAL(AddLogSignal(QString)), this, SLOT(_AddLog(QString)));
    connect(this, SIGNAL(UpdateTaskUiSignal(QString)), this, SLOT(_UpdateTaskUi(QString)));

    connect(ui.tableWidget_tasks, SIGNAL(customContextMenuRequested(QPoint)),
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
    const auto selectedItems = ui.tableWidget_tasks->selectedItems();
	
}

void IndeedGUI::restartTask()
{

}

void IndeedGUI::generateReport()
{

}

void IndeedGUI::customMenuRequested(QPoint pos)
{ 
	const auto selectedItems = ui.tableWidget_tasks->selectedItems();

    if (selectedItems.count() == 0)
        return;
	
    QMenu menu(this);

	auto* action_continueTask = new QAction("Continue Task", this);
    connect(action_continueTask, SIGNAL(triggered(bool)), this, SLOT(continueTask(bool)));
    menu.addAction(action_continueTask);

	auto* action_RestartTask = new QAction("Restart Task", this);
    connect(action_RestartTask, SIGNAL(triggered(bool)), this, SLOT(restartTask(bool)));
    menu.addAction(action_RestartTask);

	auto* action_GenerateReport = new QAction("Generate Report", this);
    connect(action_GenerateReport, SIGNAL(triggered(bool)), this, SLOT(generateTask(bool)));
    menu.addAction(action_GenerateReport);
	
	menu.exec(ui.tableWidget_tasks->viewport()->mapToGlobal(pos));
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
    auto taskInfos = taskMgr.GetTasksInfo();

    auto model = new QStandardItemModel();
    ui.tableView_tasks->setModel(model);
    model->setHorizontalHeaderItem(0, new QStandardItem("String Column"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Int Column"));
	
    struct MyData {
        QString str;
        int i;
    };
    QVector<MyData> data = { { "Hello", 1 }, { "World", 2 } };


    // Add rows to the model
    QList<QStandardItem*> rowData;
    Q_FOREACH(const auto& item, data) {
        rowData.clear();
        rowData << new QStandardItem(item.str);
        rowData << new QStandardItem(QString("%1").arg(item.i));
        model->appendRow(rowData);
    }

    //ui.tableWidget_tasks->clearContents();
    //for (auto taskInfo : taskInfos)
    //{
    //    ui.tableWidget_tasks->insertRow(0);

    //    
    //    ui.tableWidget_tasks->setItem(0, tableTasks_TaskName, new QTableWidgetItem(taskInfo.TaskName));
    //    ui.tableWidget_tasks->setItem(0, tableTasks_PagesAt, new QTableWidgetItem(QString::number(taskInfo.PagesAt)));
    //    ui.tableWidget_tasks->setItem(0, tableTasks_JobCount, new QTableWidgetItem(QString::number(taskInfo.JobCount)));
    //    ui.tableWidget_tasks->setItem(0, tableTasks_Status, new QTableWidgetItem("Saved"));
    //}
}

void IndeedGUI::NewTaskClicked()
{
    DialogNewTask newTask;
    newTask.exec();	
}