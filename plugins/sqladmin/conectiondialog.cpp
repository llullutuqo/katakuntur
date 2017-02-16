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

#include "conectiondialog.h"
#include "ui_conectiondialog.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QMessageBox>
#include <QSqlError>
#include "connectionwidget.h"
#include "connection.h"

ConectionDialog::ConectionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConectionDialog)
{
	ui->setupUi(this);
	m_connection =NULL;
	ui->lineEdit->setFocus();
}

ConectionDialog::~ConectionDialog()
{
	delete ui;
}

void ConectionDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void ConectionDialog::on_buttonBox_accepted()
{
	if(ui->lineEdit->text().isEmpty()){
		QMessageBox::warning(this,trUtf8("Error"),trUtf8("Escriba un nombre para la conexión"));
		return;
	}
	accept();
}

Connection *ConectionDialog::connection(){
	if(m_connection==NULL)
		m_connection= new Connection();
	m_connection->setDataBaseDriver(ui->cnWidget->driver());
	m_connection->setDataBaseName(ui->cnWidget->dataBase());
	m_connection->setName(ui->lineEdit->text());
	m_connection->setPassword(ui->cnWidget->password());
	m_connection->setServer(ui->cnWidget->server());
	m_connection->setUser(ui->cnWidget->user());
	m_connection->setPort(ui->cnWidget->port());
	m_connection->connect();
	return m_connection;
}

void ConectionDialog::setConnection(Connection * connection)
{
	m_connection = connection;
	ui->cnWidget->setDriver(m_connection->dataBaseDriver());
	ui->lineEdit->setText(m_connection->name());
	ui->cnWidget->setDataBase(m_connection->dataBaseName());
	ui->cnWidget->setPassword(m_connection->password());
	ui->cnWidget->setServer(m_connection->server());
	ui->cnWidget->setUser(m_connection->user());
	ui->cnWidget->setPort(m_connection->port());
}
