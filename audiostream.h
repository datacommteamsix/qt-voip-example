#pragma once

#include <QDebug>

#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QFile>
#include <QHostAddress>
#include <QPushButton>
#include <QTcpSocket>
#include <QTcpServer>

#include <QtWidgets/QMainWindow>
#include "ui_audiostream.h"

class audiostream : public QMainWindow
{
	Q_OBJECT

public:
	audiostream(QWidget *parent = Q_NULLPTR);
	~audiostream();

private:
	Ui::audiostreamClass ui;

	QAudioFormat mFormat;

	QAudioInput * mAudioIn;
	QAudioOutput * mAudioOut;

	QTcpServer * mServer;
	QTcpSocket * mSocket;

private slots:
	void streamHandler();
	void listenHandler();

	void newConnectionHandler();

	void incomingDataHandler();
	void disconnectionHander();
};
