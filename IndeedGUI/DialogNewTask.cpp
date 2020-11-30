#include "global.h"
#include "DialogNewTask.h"

DialogNewTask::DialogNewTask(QWidget* parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	ui.setupUi(this);

	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(CloseClicked()));
	connect(ui.pushButton_start, SIGNAL(clicked()), this, SLOT(StartClicked()));
}


DialogNewTask::~DialogNewTask()
{
}

void DialogNewTask::StartClicked()
{
	auto url = ui.lineEdit_url->text();
	if (url.length() == 0)
	{
		QMessageBox::critical(nullptr, "Error",
			"url is required");
		return;
	}

	if (!url.contains("http") ||
		!url.contains("indeed") ||
		!url.contains("jobs") ||
		!url.contains("?q="))
	{
		QMessageBox::critical(nullptr, "Error",
			"incorrect url");
		return;
	}

	QUrl qUrl(url);
	QUrlQuery query(qUrl.query());

	auto queryValue = query.queryItemValue("q");
	if(queryValue.count() == 0)
	{
		QMessageBox::critical(nullptr, "Error",
			"incorrect query, please check url is correct");
		return;
	}
	url = "https://www.indeed.com/jobs?q=" + queryValue;
	
	

	auto taskName = ui.lineEdit_newtaskname->text();
	
	QProgressDialog progressBarDialog("Starting in progress.", "", 0, 0);
	progressBarDialog.setCancelButton(0);
	progressBarDialog.setWindowModality(Qt::WindowModal);
	progressBarDialog.setModal(true);
	progressBarDialog.show();
	requestMgr.StartTask(taskName, url);
	progressBarDialog.close();

}

void DialogNewTask::CloseClicked()
{
	this->close();
}