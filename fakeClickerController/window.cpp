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

	//setup number to send
	this->numberToSend.append(ui->toSend0);
	this->numberToSend.append(ui->toSend1);
	this->numberToSend.append(ui->toSend2);
	this->numberToSend.append(ui->toSend3);
	this->numberToSend.append(ui->toSend4);
	this->numberToSend.append(ui->toSend5);
	this->numberToSend.append(ui->toSend6);
	this->numberToSend.append(ui->toSend7);
	this->numberToSend.append(ui->toSend8);
	this->numberToSend.append(ui->toSend9);
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


void Window::on_adress1_editingFinished() {
	if (ui->adress1->hasFocus()){
		this->on_oneSend_clicked();
	}
}

void Window::on_adress2_editingFinished() {
	if (ui->adress2->hasFocus()){
		this->on_oneSend_clicked();
	}
}

void Window::on_adress3_editingFinished() {
	if (ui->adress3->hasFocus()){
		this->on_oneSend_clicked();
	}
}

void Window::on_numberToSend_editingFinished() {
	if (ui->numberToSend->hasFocus()){
		this->on_oneSend_clicked();
	}
}

void Window::on_actionQuit_triggered(){
	this->close();
}

void Window::on_multipleSend_clicked() {
	QSpinBox *box;

	int adress [] = {
		ui->baseByte1->value(),
		ui->baseByte2->value(),
		ui->baseByte3->value()
	};

	for (int i = 0; i < this->numberToSend.length(); i++){
		box = this->numberToSend.at(i);
		for (int j = 0; j < box->value(); ++j) {
			this->clicker->newResponsePacket(adress,i);

			//increment adress
			adress[0] += 1;
			if (adress[0] > 255){
				adress[0] = 0;
				adress[1] += 1;
			}

			if (adress[1] > 255){
				adress[1] = 0;
				adress[2] += 1;
			}
		}
	}
}
