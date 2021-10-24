#include "clickermanager.h"



ClickerManager::ClickerManager() :
	QThread(),
	clicker(new QSerialPort(this)),
	randomDelay(false),
	minDelay(0),
	maxDelay(1000)
{
	qRegisterMetaType<responsePacket>("responsePacket");
	connect(this->clicker, &QSerialPort::errorOccurred, this, &ClickerManager::serialError);
	connect(this->clicker, &QSerialPort::readyRead, this, &ClickerManager::readIncoming);
	connect(this,&ClickerManager::packetReady,this,&ClickerManager::sendResponsePacket);
}

ClickerManager::~ClickerManager(){
	this->clicker->close();
}

bool ClickerManager::connectSerial(QSerialPortInfo &info, int baud) {
	this->clicker->setBaudRate(baud);
	this->clicker->setPort(info);
	return this->clicker->open(QIODevice::ReadWrite);
}

void ClickerManager::disconnectSerial() {
	this->clicker->close();
}

void ClickerManager::serialError(QSerialPort::SerialPortError error){
	switch (error) {
		case QSerialPort::SerialPortError::ReadError:
		qDebug()<<"Serial read error";
			this->clicker->close();
			emit this->disconnected();
		break;
		default:
			qDebug()<<error;
		break;
	}
}

void ClickerManager::newResponsePacket(int adress[3], response resp) {
	responsePacket response;
	for (int i = 0; i<3; i++){
		response.adress[i] = adress[i];
	}
	response.resp = resp;
	this->packetLock.lock();
	this->toSend.append(response);
	this->packetLock.unlock();

	if (!this->isRunning()){
		this->start();
	}
}

void ClickerManager::run(){
	while (!this->toSend.isEmpty()) {
		this->packetLock.lock();
		emit this->packetReady(this->toSend.dequeue());
		this->packetLock.unlock();
		if (this->randomDelay){
			this->msleep(QRandomGenerator::global()->bounded(this->minDelay,this->maxDelay));
		}
	}
}


void ClickerManager::sendResponsePacket(const responsePacket &response){
	this->clicker->write(
				QString("adress %1 %2 %3\n").arg(response.adress[0])
											.arg(response.adress[1])
											.arg(response.adress[2])
											.toStdString().c_str()
			);

	this->msleep(100);

	this->clicker->write(
				QString("send %1\n").arg(response.resp)
				.toStdString().c_str()
			);
	this->msleep(100); //wait 50ms to be sure that everithing has bee send
}

void ClickerManager::setChannel(int channel){
	this->clicker->write(
				QString("channel %1").arg(channel)
				.toStdString().c_str()
			);
}

void ClickerManager::readIncoming(){
	qDebug()<<"Incoming info";
	QByteArray data = this->clicker->readAll();
	qDebug()<<QString::fromStdString(data.toStdString());
}
