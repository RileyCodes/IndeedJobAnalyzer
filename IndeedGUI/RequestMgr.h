#pragma once
#include "global.h"
#define BYTE char
class RequestMgr;

enum MsgType
{
	MsgStartTask,
	MsgTest
};

class Msg
{
	BYTE* reqBuffer = nullptr;
	size_t reqLenAllocated = 0;
	

public:
	int reqNum = 0;
	RequestMgr* reqMgr = nullptr;
	//called by IndeedJobMarketAnalyzer when response is made
	void (*cb)(Msg* msg,char* response) = nullptr;

	bool IsEmpty()
	{
		return reqLenAllocated == 0;
	}

	void PackRequest(RequestMgr* responseMgr,const QString& jsonStr, void (*resCb)(Msg* msg, char* response),int reqCount)
	{
		PutQStringAsUtf8(jsonStr);
		reqMgr = responseMgr;
		cb = resCb;
		reqNum = reqCount;
	}
		

	void PutQStringAsUtf8(const QString&   str)
	{
		auto utf8 = str.toUtf8();
		utf8.append('\0');
		
		auto const bufferSize = utf8.size();
		AllocBuffer(bufferSize);
		memcpy_s(reqBuffer, bufferSize, utf8.data(), bufferSize);
	}
	void PutStdString(const std::string&  str)
	{
		//TODO: '\0' missing
		auto const bufferSize = str.size();
		AllocBuffer(bufferSize);
		memcpy_s(reqBuffer, bufferSize, str.data(), bufferSize);
	}
	
	void AllocBuffer(size_t newReqLen)
	{
		reqBuffer = new BYTE[newReqLen];
		reqLenAllocated = newReqLen;
	}
	
	void DeallocBuffer()
	{
		delete[] reqBuffer;
	}
};



class RequestMgr
{
	QMutex qMutex_queue;
	QMutex qMutex_request;
	QMutex qMutex_completedRequests;
	
	QQueue<Msg> msgQueue;
	int reqCount = 0;
	QMap<int,QString> completedRequests;

public:
	void AddCompletedRequest(int ReqNum, QString response);
	void Enqueue(Msg msg);
	Msg Dequeue();
	void DoTest();
	void DoRequest(QJsonObject& msgJsonObj);
	QString GetReponseSync(int req);
	void StartTask(QString taskName, QString url);

};

