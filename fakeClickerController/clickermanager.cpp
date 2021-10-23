#include "clickermanager.h"

ClickerManager::ClickerManager() :
	clicker(QSerialPort(this))
{
	connect(&this->clicker, &QSerialPort::errorOccurred, this, &ClickerManager::serialError);
	connect(&this->clicker, &QSerialPort::readyRead, this, &ClickerManager::readIncoming);
}

ClickerManager::~ClickerManager(){
	this->clicker.close();
}

bool ClickerManager::connectSerial(QSerialPortInfo &info, int baud) {
	this->clicker.setBaudRate(baud);
	this->clicker.setPort(info);
	return this->clicker.open(QIODevice::ReadWrite);
}

void ClickerManager::disconnectSerial() {
	this->clicker.close();
}

void ClickerManager::serialError(QSerialPort::SerialPortError error){
	switch (error) {
		case QSerialPort::SerialPortError::ReadError:
		qDebug()<<"Serial read error";
			this->clicker.close();
			emit this->disconnected();
		break;
		default:
			qDebug()<<error;
		break;
	}
}

void ClickerManager::newResponsePacket(int adress[3], response resp, bool autoStart) {
	responsePacket response;
	for (int i = 0; i<3; i++){
		response.adress[i] = adress[i];
	}
	response.resp = resp;
	this->packetLock.lock();
	this->toSend.append(response);
	this->packetLock.unlock();

	if (!this->isRunning() && autoStart){
		this->run();
	}
}

void ClickerManager::run(){
	while (!this->toSend.isEmpty()) {
		this->packetLock.lock();
		this->sendResponsePacket(this->toSend.dequeue());
		this->packetLock.unlock();
	}
}


void ClickerManager::sendResponsePacket(const responsePacket &response){
	qDebug()<<QString("adress %1 %2 %3").arg(response.adress[0])
			.arg(response.adress[1])
			.arg(response.adress[2])
			.toStdString().c_str();
	this->clicker.write(
				QString("adress %1 %2 %3").arg(response.adress[0])
										  .arg(response.adress[1])
										  .arg(response.adress[2])
										  .toStdString().c_str()
			);

	this->clicker.write(
				QString("send %1").arg(response.resp)
				.toStdString().c_str()
			);
}

void ClickerManager::readIncoming(){
	qDebug()<<"Incoming info";
	QByteArray data = this->clicker.readAll();
	qDebug()<<QString::fromStdString(data.toStdString());
}
