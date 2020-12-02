#include "global.h"
#include "TaskMgr.h"

bool TaskMgr::EnsureTaskFolderCreated()
{
	if (QDir(pConfig->TaskFolderName).exists())
	{
		return true;
	}

	return QDir().mkdir(pConfig->TaskFolderName);
}


