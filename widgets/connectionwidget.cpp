/****************************************************************************
 ** Kata Kuntur is Copyright (C) 2010 - 2017 Jean Mazuelos(LlulluTuqo)
 ** Contact   : http://jeanmazuelos.com/contact
 **
 ** This file is part of Kata Kuntur - Data Modelling Tool.
 ** LGPL V3 License Usage
 **
 ** You should use this file under the terms of the
 ** GNU LESSER GENERAL PUBLIC LICENSE - https://www.gnu.org/licenses/lgpl-3.0.txt
 **
 ** Additionally, any Redistributions of source code must retain the above
 ** copyright notice, this condition and the following disclaimer.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ****************************************************************************/

#include "connectionwidget.h"
#include "ui_connectionwidget.h"
#include <QFileDialog>
#include <QSqlDatabase>
class ConnectionWidget::ConnectionWidgetPrivate{
public:
	ConnectionWidgetPrivate(){
		typedbms = ConnectionWidget::Host;
		saveDatabase = false;
	}

	QString convertNormalName(const QString &value){
		QString val =value;
		return val.replace(QRegExp("^Q"),"");
	}
	TypeDBMS typedbms;
	bool saveDatabase;
};

ConnectionWidget::ConnectionWidget(QWidget *parent) :
	QWidget(parent),
	d(new ConnectionWidgetPrivate),
	ui(new Ui::ConnectionWidget)
{
	ui->setupUi(this);
	changeView(d->typedbms);
	QStringList result=QSqlDatabase::drivers();
	setAvailableDrivers(result);
}

ConnectionWidget::~ConnectionWidget()
{
	delete ui;
	delete d;
}

void ConnectionWidget::setSaveForDatabase(bool value)
{
	d->saveDatabase = value;
}

bool ConnectionWidget::isSaveForDatabase()
{
	return d->saveDatabase;
}

void ConnectionWidget::on_cmbDrivers_currentIndexChanged(QString value)
{
	if(value.contains("SQLITE"))
		setTypeDBMS(File);
	else if(value.contains("ODBC"))
		setTypeDBMS(ODBC);
	else
		setTypeDBMS(Host);
}

void  ConnectionWidget::setTypeDBMS(TypeDBMS value){
	if(d->typedbms!= value){
		d->typedbms = value;
		changeView(d->typedbms);
	}
}

ConnectionWidget::TypeDBMS  ConnectionWidget::typeDBMS(){
	return d->typedbms;
}

void ConnectionWidget::on_pbExp_clicked()
{
	QString fileName;
	if(!d->saveDatabase)
		fileName = QFileDialog::getOpenFileName(this, QString(),QString() , "SQLite Data Base(*.db);;Todos(*.*)");
	else
		fileName = QFileDialog::getSaveFileName(this, QString(),"sqlitedatabase.db" , "SQLite Data Base(*.db);;Todos(*.*)");
	ui->leBaseDatos->setText(fileName);
}

QString ConnectionWidget::driver(){
	return "Q" + ui->cmbDrivers->lineEdit()->text();
}

QString ConnectionWidget::server(){
	return ui->leServidor->text();
}

QString ConnectionWidget::password(){
	return ui->leContrasena->text();
}

QString ConnectionWidget::user(){
	return ui->leUsuario->text();
}

QString ConnectionWidget::dataBase(){
	return ui->leBaseDatos->text();
}

int ConnectionWidget::port(){
	return ui->spnPuerto->value();
}

void ConnectionWidget::setAvailableDrivers(QStringList values)
{
	ui->cmbDrivers->lineEdit()->setText("");
	ui->cmbDrivers->clear();

	QStringList result=QSqlDatabase::drivers();
	foreach(QString val , values){
		if(!result.contains(val))
			values.removeOne(val);
	}


	//Mostrar el driver de SQLITE 3
	values.replaceInStrings(QRegExp("3$"),"");
	values.removeDuplicates();
	int count = values.count();
	if(count<=0){
		setEnabled(false);
		ui->cmbDrivers->lineEdit()->setText(tr("No hay drivers disponibles"));
	}
	else{
		setEnabled(true);
		values.replaceInStrings(QRegExp("^Q"),"");
		ui->cmbDrivers->addItems(values);
	}
}

void ConnectionWidget::changeView(const ConnectionWidget::TypeDBMS value)
{
	switch(value){
		case File:
			ui->pbExp->setHidden(false);
			ui->leUsuario->setHidden(true);
			ui->leContrasena->setHidden(true);
			ui->leServidor->setHidden(true);
			ui->spnPuerto->setHidden(true);
			//ui->checkBox->setHidden(true);
			ui->label_2->setHidden(true);
			ui->label_3->setHidden(true);
			ui->label_4->setHidden(true);
			ui->label_6->setHidden(true);
			ui->label_5->setText(tr("Archivo de base de datos:"));
			ui->leBaseDatos->setFocus(Qt::OtherFocusReason);
		break;
		case Host:
			ui->pbExp->setHidden(true);

			ui->label_5->setText(tr("Base de Datos:"));

			ui->label_2->setHidden(false);
			ui->leServidor->setHidden(false);
			ui->leServidor->setFocus(Qt::OtherFocusReason);

			ui->label_3->setHidden(false);
			ui->leUsuario->setHidden(false);

			ui->label_4->setHidden(false);
			ui->leContrasena->setHidden(false);

			ui->label_6->setHidden(false);
			ui->spnPuerto->setHidden(false);

		break;
	case ODBC:
		ui->pbExp->setHidden(true);
		ui->label_5->setText(tr("Base de Datos:"));

		ui->label_2->setHidden(false);
		ui->label_2->setText(tr("DSN:"));
		ui->leServidor->setHidden(false);
		ui->leServidor->setFocus(Qt::OtherFocusReason);

		ui->label_3->setHidden(false);
		ui->leUsuario->setHidden(false);

		ui->label_4->setHidden(false);
		ui->leContrasena->setHidden(false);

		ui->label_6->setHidden(true);
		ui->spnPuerto->setHidden(true);
		break;
	}
}

void ConnectionWidget::setDriver(const QString &driver){
	int index = ui->cmbDrivers->findText(d->convertNormalName(driver));
	if(index >=0)
		ui->cmbDrivers->setCurrentIndex(index);
}

void ConnectionWidget::setServer(QString value){
	ui->leServidor->setText(value);
}

void ConnectionWidget::setPassword(QString value){
	ui->leContrasena->setText(value);
}

void ConnectionWidget::setUser(QString value){
	ui->leUsuario->setText(value);
}

void ConnectionWidget::setDataBase(QString value){
	ui->leBaseDatos->setText(value);
}

void ConnectionWidget::setPort(int value){
	ui->spnPuerto->setValue(value);
}
