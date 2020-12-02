#include "global.h"


void PendingLogMgr::AddPendingLog(string Log)
{
	const std::lock_guard<std::mutex> lock(mutex);
	pendingLog += Log;
	pendingLog += '\n';
}

string PendingLogMgr::GetAllPendingLogs()
{
	const std::lock_guard<std::mutex> lock(mutex);
	string _pendingLog = pendingLog;
	pendingLog.clear();
	return _pendingLog;
}
