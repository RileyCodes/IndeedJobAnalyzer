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
    void SyncTasksToUI();

public slots:
    static void NewTaskClicked();
    
private:
    Ui::IndeedGUIClass ui;
};
