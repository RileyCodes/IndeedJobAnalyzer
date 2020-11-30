#pragma once
#include "global.h"
#include <QWidget>
#include "ui_DialogNewTask.h"


enum TaskComboType
{
	Select,
	NewTask,
	ExistedTask
};

class DialogNewTask : public  QDialog
{
	Q_OBJECT

public:
	DialogNewTask(QWidget *parent = Q_NULLPTR);
	~DialogNewTask();
	void AddNewTask(QString taskName);


private:
	Ui::DialogNewTask ui;
	TaskComboType taskComboType;

private slots:
	void CloseClicked();
	//void TaskChanged();
	void StartClicked();
};
