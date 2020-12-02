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

//---END OF EXTERNAL HEAD FILE---


#define ptr int32_t//this way we can change to x64 whenever we need
#include "Config.h"
extern Config* pConfig;
#include "Model_TasksTable.h"
#include "RequestMgr.h"
#include "PendingLogMgr.h"
#include "IndeedGUI.h"



#include "TaskMgr.h"
extern RequestMgr* pRequestMgr;
extern PendingLogMgr pendingLogMgr;
extern IndeedGUI* pMainWindow;
extern TaskMgr* pTaskMgr;


EXTERN_DLL_EXPORT void AddLog(char* Log);






#include "Util.h"
#include "DialogNewTask.h"