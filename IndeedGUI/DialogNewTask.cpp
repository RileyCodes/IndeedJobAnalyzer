#include "global.h"
#include "DialogNewTask.h"

DialogNewTask::DialogNewTask(QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	ui.setupUi(this);

	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(CloseClicked()));
	connect(ui.comboBox_taskname, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(TaskChanged()));

	connect(ui.pushButton_start, SIGNAL(clicked()), this, SLOT(StartClicked()));
	
	SyncCurrentTasks();
}


void DialogNewTask::SyncCurrentTasks()
{
	auto taskNames = taskMgr.GetTaskNames();
	ui.comboBox_taskname->clear();

	ui.comboBox_taskname->addItem("(Select)", Select);
	for (auto task_name : taskNames)
	{
		task_name = task_name.replace(".txt", "");
		ui.comboBox_taskname->addItem(task_name,ExistedTask);
	}
	ui.comboBox_taskname->addItem("Add a new  task", NewTask);
}

DialogNewTask::~DialogNewTask()
{
}

void DialogNewTask::AddNewTask(QString taskName)
{
	ui.comboBox_taskname->insertItem(1, taskName);
	ui.comboBox_taskname->setCurrentIndex(1);
}

void DialogNewTask::TaskChanged()
{
	taskComboType = static_cast<TaskComboType>(ui.comboBox_taskname->currentData().toInt());

	switch(taskComboType)
	{
	case Select:
		break;
		case NewTask:
			{
			
				ui.comboBox_taskname->setCurrentIndex(0);
				QInputDialog inputNewTaskName;
				inputNewTaskName.setLabelText("Enter a task name: ");
				if(inputNewTaskName.exec())
				{	
					auto taskName = inputNewTaskName.textValue();
					if (taskName.length() == 0)
						return;

					AddNewTask(taskName);
				}
			}
		break;
	case ExistedTask:
		break;
	}

	return;
}

void DialogNewTask::StartClicked()
{
	auto url = ui.lineEdit_url->text();
	if(url.length() == 0)
	{
		QMessageBox::critical(nullptr, "Error", 
		                      "url is required");
		return;
	}
	
	auto currentIndex = ui.comboBox_taskname->currentIndex();

	if(currentIndex == 0 || currentIndex == ui.comboBox_taskname->count() - 1)
	{
		//if index is first or last, it means no tasks had been selected
		QMessageBox::critical(NULL, "Error",
			"must select a task");
		return;
	}

	auto taskName = ui.comboBox_taskname->currentText();

	requestMgr.StartTask(taskName, url);
}

void DialogNewTask::CloseClicked()
{
	this->close();
}