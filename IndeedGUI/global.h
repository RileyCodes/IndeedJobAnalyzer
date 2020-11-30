#pragma once

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
#include "RequestMgr.h"
#include "Config.h"
extern Config config;
extern RequestMgr requestMgr;

#include "Util.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

#include "TaskMgr.h"
extern TaskMgr taskMgr;
#include "DialogNewTask.h"