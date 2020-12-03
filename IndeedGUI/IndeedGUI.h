#pragma once

#include "global.h"
#include "ui_IndeedGUI.h"


class IndeedGUI : public QMainWindow
{
    Q_OBJECT

    void _Test(void (IndeedGUI::* contextMenuAction)(const TaskRowData&));
public:
    IndeedGUI(QWidget *parent = Q_NULLPTR);

    void AddLogWithMeta(QString message);
    void AddLog(QString message);
    void AddPendingLog();
    void UpdateTaskInfo(QString message);

signals:
    void AddLogSignal(QString msg);
    void UpdateTaskUiSignal(QString msg);

private slots:
    void _AddLog(QString message);
    void NewTaskClicked();
    void _UpdateTaskUi(QString taskInfoJson);
    void customMenuRequested(QPoint pos);
    void rerunTask();

    QList<TaskRowData> GetSelectedTasks();
    void stopTask();
    void restartTask();
    void generateReport();
    
private:
    Ui::IndeedGUIClass ui;
};
