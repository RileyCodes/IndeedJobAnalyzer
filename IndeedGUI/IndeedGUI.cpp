#include "global.h"
#include "IndeedGUI.h"

IndeedGUI::IndeedGUI(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);	
    connect(ui.actionNewTaskButton, SIGNAL(triggered()), this, SLOT(NewTaskClicked()));

    connect(this, SIGNAL(AddLogSignal(QString)), this, SLOT(_AddLog(QString)));
    SyncTasksToUI();
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