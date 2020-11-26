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
    void Addlog(QString message);
    void SyncTasksToUI();

signals:
    void AddLogSignal(QString msg);

public slots:
    void _AddLog(QString message);
    void NewTaskClicked();
    
private:
    Ui::IndeedGUIClass ui;
};
