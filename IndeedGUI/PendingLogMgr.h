#pragma once
#include "global.h"

#include <mutex>

class PendingLogMgr
{
	// WARNING: this class is required before QT was initialized.
	// NO QT should be used here
	// see StartQT() for reason
	
	string pendingLog;
	std::mutex mutex;
	QMutex mutex_PendingLog;
public:
	void AddPendingLog(string Log);
	string GetAllPendingLogs();
};

