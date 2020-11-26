#pragma once

using namespace std;
#include <Windows.h>
#include <string>
#include <qdir.h>
#include <qqueue.h>
#include <QMutex>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qset.h>
#include "RequestMgr.h"
#include "Config.h"
extern Config config;
extern RequestMgr requestMgr;

#include "Util.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

#include "TaskMgr.h"
extern TaskMgr taskMgr;
#include "DialogNewTask.h"