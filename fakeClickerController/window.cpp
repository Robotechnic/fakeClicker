#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Window)
	, clicker(new ClickerManager())
{
	// setup ui
	ui->setupUi(this);
	for (int &baud : QSerialPortInfo::standardBaudRates()){
		ui->baudRate->addItem(QString::number(baud),baud);
	}

	this->on_updatePort_clicked();

	//setup clicker manager
	connect(clicker,&ClickerManager::disconnected,this,&Window::serialDisconnected);
}

Window::~Window() {
	delete ui;
}

void Window::serialDisconnected(){
	setConnectedState(false);
	QMessageBox::critical(this,"Serial error","Serial port has been unfortunately forcefully disconnected");
}

void Window::setConnectedState(bool connected) {
	ui->updatePort->setEnabled(!connected);
	ui->multipleReponseGroup->setEnabled(connected);
	ui->randomDelayGroup->setEnabled(connected);
	ui->oneReponseGroup->setEnabled(connected);
	ui->channelGroup->setEnabled(connected);

	if (connected){
		ui->connect->setText(tr("Disconnect"));
	} else {
		ui->connect->setText(tr("Connect"));
	}
}

void Window::on_updatePort_clicked() {
	ui->port->clear();
	infoList = QSerialPortInfo::availablePorts();

	for (QSerialPortInfo &info : infoList) {
		ui->port->addItem(info.portName());
	}
}

void Window::on_connect_clicked() {
	if (ui->port->currentText() != "" && !this->clicker->isConnected()) {
		if (this->clicker->connectSerial(infoList[ui->port->currentIndex()], ui->baudRate->currentData().toInt())){
			setConnectedState(true);
			QMessageBox::information(this,"Serial connection","Connection succesfull");
		} else {
			setConnectedState(false);
			QMessageBox::critical(this,"Serial connection","Connection failled to serial");
		}
	} else if (this->clicker->isConnected()) {
		setConnectedState(false);
		this->clicker->disconnectSerial();
		QMessageBox::information(this,"Serial connection","Serial port disconnected succesfully");
	} else {
		setConnectedState(false);
		QMessageBox::warning(this,"Serial connection","Please select a valid port");
	}
}

void Window::on_oneSend_clicked(){
	int adress [] = {
		ui->adress1->value(),
		ui->adress2->value(),
		ui->adress3->value()
	};

	this->clicker->newResponsePacket(
				adress,
				ui->numberToSend->value()
			);
}

void Window::on_randomDelayGroup_toggled(bool arg1) {
	this->clicker->useRandomDelay(arg1);
}

