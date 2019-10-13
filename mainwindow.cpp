#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QThreadPool>
#include <QTimer>
#include "commandtask.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
		, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	stopped = false;
    connect(ui->runButton, SIGNAL(clicked()), this, SLOT(runCommand()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelCommand()));
    connect(ui->clearButton, SIGNAL(clicked()), ui->plainTextEdit, SLOT(clear()));
}

void MainWindow::cancelCommand() {
  stopped = true;
  ui->cancelButton->setEnabled(false);
  is_done();
}

void MainWindow::runCommand() {
    ui->cancelButton->setEnabled(true);
    ui->runButton->setEnabled(false);
    CommandTask *t = new CommandTask(this, &stopped, ui->lineEdit->text());
    QThreadPool::globalInstance()->start(t);
    /*
    QObject *sender = QObject::sender();
	if(sender->objectName() == "pushButton1") {
		QPushButton *pb = static_cast<QPushButton*>(sender);
		pb->setEnabled(false);
		ui->pushButton3->setEnabled(true);
        CommandTask *t = new CommandTask(this, &stopped, "iostat -xtc 1");
        QThreadPool::globalInstance()->start(t);
	} else if(sender->objectName() == "pushButton2") {
		QPushButton *pb = static_cast<QPushButton*>(sender);
		pb->setEnabled(false);
		ui->pushButton3->setEnabled(true);
        CommandTask *t = new CommandTask(this, &stopped, "top -b -n 1");
        QThreadPool::globalInstance()->start(t);
	} else {
		stopped = true;
		ui->pushButton3->setEnabled(false);
		is_done();
        ui->plainTextEdit->clear();
    }
    */
}

void MainWindow::is_done() {
	if(QThreadPool::globalInstance()->activeThreadCount()) {
		QTimer::singleShot(100, this, SLOT(is_done()));
	} else {
        ui->cancelButton->setEnabled(false);
        ui->runButton->setEnabled(true);
		stopped = false;
	}
}

void MainWindow::updateProgress(const QString &message) {
   ui->plainTextEdit->appendPlainText(message);
}

void MainWindow::closeEvent(QCloseEvent *event) {
	stopped = true;
	if(QThreadPool::globalInstance()->activeThreadCount()) {
		QThreadPool::globalInstance()->waitForDone();
	}

	event->accept();
}

MainWindow::~MainWindow()
{
	delete ui;
}

