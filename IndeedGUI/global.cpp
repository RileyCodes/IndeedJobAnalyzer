#include "global.h"
#include "IndeedGUI.h"
#define ptr int32_t//this way we can change to x64 whenever we need

Config config;
TaskMgr taskMgr;
RequestMgr requestMgr;

EXTERN_DLL_EXPORT int StartGUI()
{
	int dummyArgc = 0;
	QApplication app(dummyArgc, nullptr);
	IndeedGUI mainWindow;
	mainWindow.show();
	return app.exec();
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
	// msg is a reusable buffer that does not destroy during whole application life.
	static Msg* msg = new Msg();
	*msg = requestMgr.Dequeue();//copy current message to buffer
	
	if (msg->IsEmpty())
		return nullptr;

	return msg;
}