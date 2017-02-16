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

#include "formexportdbms.h"
#include "ui_formexportdbms.h"
#include "pantalladiagrama.h"
#include "ficheros/SQL/sqlcore.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtConcurrentRun>
#include <QStandardPaths>
#include "exportreportsintax.h"
#include "connectionwidget.h"

FormExportDBMS::FormExportDBMS(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormExportDBMS)
{
	ui->setupUi(this);
	setWindowFlags(Qt::Dialog);
	sql = new SQLSintax(ui->textEdit->document());
	new ExportReportSintax(ui->plainTextEdit->document());
	conWidget = new ConnectionWidget(ui->tabWidget);
	conWidget->setSaveForDatabase(true);
	ui->tabWidget->insertTab(0,conWidget,trUtf8("Conexion"));
	ui->tabWidget->setCurrentIndex(0);
	ui->comboBox->addItem(trUtf8("Normal"));
	ui->comboBox->addItem(trUtf8("Mayusculas"));
	ui->comboBox->addItem(trUtf8("Minusculas"));
	running=false;
	ui->progressBar->setVisible(false);
	ui->lblExport->setVisible(false);
	connect(this,SIGNAL(finished()),this,SLOT(onfinished()));
}

FormExportDBMS::~FormExportDBMS()
{
	delete ui;
}

void FormExportDBMS::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void FormExportDBMS::setPantallaDiagrama(PantallaDiagrama *pantalla){
	if(!pantalla)
		return;
	m_pantalla=pantalla;
	sql->addTypeList(m_pantalla->dbms()->getListaTipos());
	ui->checkBox->setEnabled(m_pantalla->dbms()->suportsCreateDataBase());
	ui->checkBox->setChecked(m_pantalla->dbms()->createDataBase());
	ui->checkBox->setChecked(m_pantalla->dbms()->useQuotes());
	this->setWindowTitle(tr("Exportar a SGBD : ") + m_pantalla->dbms()->nombreDBMS());
	conWidget->setAvailableDrivers(QStringList()<<m_pantalla->dbms()->qtDriver());
	ui->textEdit->setText(m_pantalla->dbms()->generarSQL(m_pantalla,ui->lineEdit->text(),(SQLCore::CaseTable)ui->comboBox->currentIndex()));
	ui->label_7->setText(trUtf8("Script SQL para %1:").arg(m_pantalla->dbms()->nombreDBMS()));
}
void FormExportDBMS::closeEvent( QCloseEvent * ){
	if(m_pantalla)
		disconnect(m_pantalla->dbms(),0,this,0);
	qDebug("Eliminado ");
}
void FormExportDBMS::escribirMensaje(QString mensaje ){
	if(!mensaje.isEmpty())
		ui->plainTextEdit->appendPlainText(mensaje+"\n");
}

void
FormExportDBMS::on_pushButton_clicked(){
	ui->plainTextEdit->clear();
	if(ui->textEdit->toPlainText().isEmpty()){
		QMessageBox::warning(this,trUtf8("Error!"),trUtf8("Error al intentar exportar el SQL, debe generar primero el script."));
		return;
	}
	if(running){
		QMessageBox::warning(this,trUtf8("Error!"),trUtf8("Error al intentar exportar el SQL, el script está siendo exportado."));
		return;
	}
	connect(m_pantalla->dbms(),SIGNAL(onMessage(QString)),this,SLOT(escribirMensaje(QString)));
	ui->progressBar->setVisible(true);
	ui->lblExport->setVisible(true);
	QtConcurrent::run(this,&FormExportDBMS::exportar);
}

void
FormExportDBMS::exportar(){
	running = true;
	/*Se evita alterar la interfaz, produce crash cuando se ejecuta
	el dialogo seleccionar DB con SQLite y KDE 4*/
	//ui->pushButton->setEnabled(false);
	m_pantalla->dbms()->exportarDBMS(ui->textEdit->toPlainText(),
										 conWidget->server(),
										 conWidget->user(),
										 conWidget->password(),
										 conWidget->dataBase(),
										 conWidget->port(),
										 ui->checkBox->isChecked()
										 );
	disconnect(m_pantalla->dbms(),SIGNAL(onMessage(QString)),this,SLOT(escribirMensaje(QString)));
	running = false;
	emit finished();
	/*Se evita alterar la interfaz, produce crash cuando se ejecuta
	el dialogo seleccionar DB con SQLite*/
	//ui->pushButton->setEnabled(true);
}

void FormExportDBMS::on_pushButton_2_clicked()
{
	ui->textEdit->setText(m_pantalla->dbms()->generarSQL(m_pantalla,ui->lineEdit->text(),(SQLCore::CaseTable)ui->comboBox->currentIndex()));
}


void FormExportDBMS::onfinished()
{
	ui->progressBar->setVisible(false);
	ui->lblExport->setVisible(false);
}

void FormExportDBMS::on_checkBox_2_clicked()
{
	m_pantalla->dbms()->setQuotes(ui->checkBox_2->isChecked());
}

void FormExportDBMS::on_pb_saveScript_clicked()
{
	QString pathFile;
	QFileDialog df(this,
				   QString(),
				   QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0),
				   tr("Lenguaje de Consulta Estructurado (%1)").arg("*.sql")
				   );
	df.setFileMode(QFileDialog::AnyFile);
	df.setDefaultSuffix(".sql");
	df.setAcceptMode(QFileDialog::AcceptSave);
	df.setWindowTitle(tr("Guardar como"));
	df.setLabelText(QFileDialog::Accept,tr("Guardar"));
	if (df.exec()){
		pathFile = df.selectedFiles().at(0);
		pathFile = pathFile.trimmed();
	}
	if(pathFile.isEmpty())
		return;
	const QString script =ui->textEdit->toPlainText();
	QFile file(pathFile);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
		QMessageBox::information(this,tr("Error al guardar"),tr("No se pudo abrir el archivo %1 para escritura. Compruebe que tiene los permisos necesarios.").arg(pathFile));
		return;
	}
	QTextStream out(&file);
	out<<script;
	file.close();
	QMessageBox::information(this,tr("Guardado"),trUtf8("Se ha guardado el script a %1 correctamente").arg(pathFile));
}
