#include "global.h"
#include "IndeedGUI.h"

IndeedGUI::IndeedGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);	
    connect(ui.actionNewTaskButton, SIGNAL(triggered()), this, SLOT(NewTaskClicked()));

    connect(this, SIGNAL(AddLogSignal(QString)), this, SLOT(_AddLog(QString)));
    connect(this, SIGNAL(UpdateTaskUiSignal(QString)), this, SLOT(_UpdateTaskUi(QString)));
	
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

void IndeedGUI::Addlog(QString message)
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
    auto taskNames = taskMgr.GetTaskNames();
    ui.listWidget_tasks->clear();

    for (auto task_name : taskNames)
    {
        task_name = task_name.replace(".txt", "");
        ui.listWidget_tasks->addItem(task_name);
    }
}

void IndeedGUI::NewTaskClicked()
{
    DialogNewTask newTask;
    newTask.exec();	
}