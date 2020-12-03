#include "global.h"


bool IsQTInit = false;

void InitAllGlobalQObjects()
{
	/*TODO:
	 * even trough following objects are required
	 * during entire application life, should we delete them on exit?
	 */
	pConfig = new Config();
	pRequestMgr = new RequestMgr();
	pTaskMgr = new TaskMgr();
}

EXTERN_DLL_EXPORT int StartQT()
{
	/* WARNING:
	 *it seems QT recognize the thread that initialized first QObject as main thread.
	 * in the event first QObject was not initialized in same thread as QApplication, an assert will occur.
	 * therefore, NEVER INITIALIZE ACTUAL QOBJECT IN GLOBAL.H, USE POINTER ONLY.
	 */

	int dummyArgc = 0;
	QApplication app(dummyArgc, nullptr);
	InitAllGlobalQObjects();

	IndeedGUI mainWindow;
	pMainWindow = &mainWindow;
	pMainWindow->AddPendingLog();
	mainWindow.show();

	IsQTInit = true;
	auto exitCode = app.exec();
	pMainWindow = nullptr;
	return exitCode;
}

struct MsgInfo
{
	ptr pointer = NULL;
	int length = 0;
};

// IndeedJobMarketAnalyzer(IJMA) Polls this function to get requests made from IndeedGUI(IG)
// GetMsg returns a pointer of Msg from queue, allowing IJMA to read request from Msg.reqBuffer,
// make a response in Msg.resBuffer and finally, call Msg.cb to notify IG the response is made.

EXTERN_DLL_EXPORT void* GetMsg()
{
	if (pRequestMgr == nullptr)
		return nullptr;

	// msg is a reusable buffer that does not destroy during whole application life.
	static Msg* msg = new Msg();
	*msg = pRequestMgr->Dequeue();//copy current message to buffer

	if (msg->IsEmpty())
		return nullptr;

	return msg;
}

EXTERN_DLL_EXPORT void AddLog(char* Log)
{
	if (!pMainWindow)
	{
		//Add logs to pending buffer while GUI is not ready.
		pendingLogMgr.AddPendingLog(Log);
		return;
	}

	QString qMsg = Log;
	pMainWindow->AddLogWithMeta(qMsg);
}

EXTERN_DLL_EXPORT void UpdateTaskInfo(char* TaskInfoJson)
{
	if (!pMainWindow)
		return;

	pMainWindow->UpdateTaskInfo(TaskInfoJson);
}