#pragma once

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

using namespace std;
#include <Windows.h>
#include <string>
#include <QDir>
#include <QStandardItemModel>
#include <QProgressDialog>
#include <QUrlQuery>
#include <QTextStream>
#include <QQueue>
#include <QMutex>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QMessageBox>
#include <QInputDialog>
#include <QSet>
#include "Model_TasksTable.h"
#include "RequestMgr.h"
#include "Config.h"

EXTERN_DLL_EXPORT void AddLog(char* Msg);

extern Config config;
extern RequestMgr requestMgr;
extern QQueue 

#include "Util.h"



#include "TaskMgr.h"
extern TaskMgr taskMgr;
#include "DialogNewTask.h"