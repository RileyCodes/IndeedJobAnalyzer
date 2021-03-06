#include "global.h"
#include "RequestMgr.h"

void RequestMgr::Enqueue(Msg msg)
{
	QMutexLocker mutexLocker(&qMutex_queue);
	msgQueue.enqueue(msg);
}

Msg RequestMgr::Dequeue()
{
	QMutexLocker mutexLocker(&qMutex_queue);
	if (msgQueue.isEmpty())
		return Msg();
	
	return msgQueue.dequeue();
}

void OnResponse(Msg* msg, char* response)
{
	std::string testStr;
	testStr = std::to_string(msg->reqNum) + '\n';
	printf(testStr.c_str());

	QString qResponse = response;
	msg->DeallocBuffer();

	msg->reqMgr->AddCompletedRequest(msg->reqNum, qResponse);
}

class ResponseMgr
{
	
};

void RequestMgr::DoTest()
{
	QJsonObject msgJson;
	msgJson["type"] = MsgTest;
	msgJson["count"] = "0";
	
	DoRequest(msgJson);
}

QString RequestMgr::DoRequest(QJsonObject& msgJsonObj)
{
	QJsonDocument doc(msgJsonObj);
	QString const msgJson(doc.toJson(QJsonDocument::Compact));
	
	Msg msg;

	//eliminate potential risks in race condition for reqCount
	//or, use UUID instead
	qMutex_request.lock();
	msg.PackRequest(this, msgJson, OnResponse, reqCount);
	Enqueue(msg);
	const auto currentReqCount = reqCount;
	reqCount++;
	qMutex_request.unlock();

	return GetReponseSync(currentReqCount);
}


QString RequestMgr::GetReponseSync(int req)		
{
	while(completedRequests.count(req) == 0)
	{
		Sleep(1);
		QApplication::processEvents();
	}
	
	//There is no guarantee GetReponseSync and AddCompletedRequest will never executed
	//in same time or run in same thread.
	//especially in event this class is ported to another project in future.
	//therefore, we eliminate all potential risks in race condition.
	
	qMutex_completedRequests.lock();
	QString responseJson = completedRequests[req];
	qMutex_completedRequests.unlock();

	completedRequests.remove(req);
	return responseJson;
}

void RequestMgr::AddCompletedRequest(int ReqNum,QString response)
{
	QMutexLocker mutexLocker(&qMutex_completedRequests);
	completedRequests[ReqNum] = response;
}


void RequestMgr::StartTask(QString taskName, QString url)
{
	QJsonObject msgJson;
	msgJson["type"] = MsgStartTask;
	msgJson["taskName"] = taskName;
	msgJson["url"] = url;

	auto response = DoRequest(msgJson);
}

void RequestMgr::StopTask(QString taskName)
{
	QJsonObject msgJson;
	msgJson["type"] = MsgStopTask;
	msgJson["taskName"] = taskName;

	auto response = DoRequest(msgJson);
}


void RequestMgr::RestartTask(QString taskName,QString url)
{
	QJsonObject msgJson;
	msgJson["type"] = MsgRestartTask;
	msgJson["taskName"] = taskName;
	msgJson["url"] = url;
	
	auto response = DoRequest(msgJson);
}


