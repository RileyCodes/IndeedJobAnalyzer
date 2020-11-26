#include "global.h"
#include "TaskMgr.h"

bool TaskMgr::EnsureTaskFolderCreated()
{
	if (QDir(config.TaskFolderName).exists())
	{
		return true;
	}

	return QDir().mkdir(config.TaskFolderName);
}


