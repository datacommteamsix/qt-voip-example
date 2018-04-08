#include "audiostream.h"

audiostream::audiostream(QWidget *parent)
	: QMainWindow(parent)
	, mAudioIn(nullptr)
	, mAudioOut(nullptr)
	, mServer(nullptr)
	, mSocket(nullptr)
{
	ui.setupUi(this);

	// Set Audio Format
	mFormat.setSampleRate(44100);
	mFormat.setChannelCount(2);
	mFormat.setSampleSize(16);
	mFormat.setCodec("audio/pcm");
	mFormat.setByteOrder(QAudioFormat::LittleEndian);
	mFormat.setSampleType(QAudioFormat::UnSignedInt);

	// Connect button slots
	connect(ui.buttonStream, &QPushButton::pressed, this, &audiostream::streamHandler);
	connect(ui.buttonListen , &QPushButton::pressed, this, &audiostream::listenHandler);
}

audiostream::~audiostream()
{
	delete mAudioIn;
	delete mAudioOut;
	delete mServer;
	delete mSocket;
}

// Start listening to the microphone and send over tcp
void audiostream::streamHandler()
{
	// Create socket and connect to host
	mSocket = new QTcpSocket(this);
	mSocket->connectToHost(QHostAddress::LocalHost, 5150);

	// Stream
	mAudioIn = new QAudioInput(mFormat, this);
	mAudioIn->setBufferSize(8192);
	mAudioIn->start(mSocket);
}

// Create a new server to listen for a new connection
void audiostream::listenHandler()
{
	mServer = new QTcpServer(this);
	connect(mServer, &QTcpServer::newConnection, this, &audiostream::newConnectionHandler);
	mServer->listen(QHostAddress::Any, 5150);
}

// When a new connection is made, save the new socket and connect its slots
void audiostream::newConnectionHandler()
{
	if (!mSocket)
	{
		mSocket = mServer->nextPendingConnection();
		connect(mSocket, &QTcpSocket::readyRead, this, &audiostream::incomingDataHandler);
		connect(mSocket, &QTcpSocket::disconnected, this, &audiostream::disconnectionHander);
	}
	else
	{
		qDebug() << "Already have a connection";
	}
}

// This program only handles VoIP data so we make a new audio output and play the sound
void audiostream::incomingDataHandler()
{
	if (!mAudioOut)
	{
		mAudioOut = new QAudioOutput(mFormat, this);
		mAudioOut->setBufferSize(8192);
		mAudioOut->start((QTcpSocket *)QObject::sender());
	}
}

// Clean up
void audiostream::disconnectionHander()
{
	delete mAudioOut;
	delete mSocket;
}
