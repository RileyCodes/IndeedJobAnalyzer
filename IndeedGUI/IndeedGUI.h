#pragma once

#include "global.h"
#include <QtWidgets/QMainWindow>
#include "ui_IndeedGUI.h"


class IndeedGUI : public QMainWindow
{
    Q_OBJECT
/// <summary>
/// 
/// </summary>
public:
    IndeedGUI(QWidget *parent = Q_NULLPTR);


	//class TasksTableMgr
	//{
 //       QMap<string, TaskTables> tasksTable;
 //       QTableWidget* tableWidget = nullptr;

 //       enum
 //       {
 //           tableTasks_TaskName,
 //           tableTasks_PagesAt,
 //           tableTasks_JobCount,
 //           tableTasks_Status
 //       };
	//public:
	//	void InsertRow(QString taskName,int pageAt,int jobCount,QString status)
	//	{
 //           TaskTables taskTables;
 //           taskTables.taskName = new QTableWidgetItem(taskName);
 //           taskTables.pagesAt= new QTableWidgetItem(QString::number(pageAt));
 //           taskTables.jobCount = new QTableWidgetItem(QString::number(jobCount));
 //           taskTables.status = new QTableWidgetItem(status);
	//		
 //           tableWidget->insertRow(0);
 //           tableWidget->setItem(0, tableTasks_TaskName, taskTables.taskName);
 //           tableWidget->setItem(0, tableTasks_PagesAt, taskTables.taskName);
 //           tableWidget->setItem(0, tableTasks_JobCount, taskTables.taskName);
 //           tableWidget->setItem(0, tableTasks_Status, taskTables.taskName);
	//	}
	//};

    
	
    //QMap<string
	
    void AddLog(QString message);
    void UpdateTaskInfo(QString message);
    void SyncTasksToUI();

signals:
    void AddLogSignal(QString msg);
    void UpdateTaskUiSignal(QString msg);

public slots:
    void _AddLog(QString message);
    void NewTaskClicked();
    void _UpdateTaskUi(QString taskInfoJson);
    void customMenuRequested(QPoint pos);
    void continueTask();
    void restartTask();
    void generateReport();
    
private:
    Ui::IndeedGUIClass ui;
};
