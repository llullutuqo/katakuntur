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

#include "sqladmin.h"
#include "ui_sqladmin.h"
#include "propiedades.h"
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QMessageBox>
#include <QSettings>
#include <QBitArray>

SQLAdmin::SQLAdmin(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SQLAdmin)
{
	ui->setupUi(this);
	widget = new SQLAdminWidget(this);
	setCentralWidget(widget);
	connect(ui->actionConectarse,SIGNAL(triggered()),widget,SLOT(conectar()));
	connect(ui->actionEjecutar_SQL,SIGNAL(triggered()),widget,SLOT(ejecutarSQL()));
	connect(ui->actionNuevo,SIGNAL(triggered()),widget,SLOT(addTab()));
	connect(widget,SIGNAL(sendMessage(QString,MessageType)),this,SLOT(receiveMessage(QString,MessageType)));
	QSettings d;
	this->restoreGeometry(d.value("geometry").toByteArray());
	widget->restoreSize();
}

SQLAdmin::~SQLAdmin()
{
	delete ui;
}

void SQLAdmin::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void SQLAdmin::on_actionActualizar_lista_de_Bases_de_Datos_triggered()
{
	widget->updateDBList();
}
void SQLAdmin::receiveMessage(QString message, MessageType tp){
	if(message.isEmpty())
		return;
	QString style;
	switch(tp){
	case Normal:
		break;
	case Warning:
		style="background-color:yellow;color:black";
		break;
	case Fatal:
		style="background-color:red;color:white";
		break;
	}
	statusBar()->setStyleSheet(style);
	statusBar()->showMessage(message);
}

void SQLAdmin::closeEvent ( QCloseEvent *){
	 QSettings d;
	 d.setValue("geometry",this->saveGeometry());
	 widget->saveSize();
}
