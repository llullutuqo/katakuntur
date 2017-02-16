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

#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"
#include "ormkatakowi.h"
#include <QFileDialog>
#include <QMessageBox>
class PropertiesDialog::PropertiesDialogPrivate{
public:
	ORMKataKowi *base;
};

PropertiesDialog::PropertiesDialog(QWidget *parent) :
	QDialog(parent),
	 ui(new Ui::PropertiesDialog),d(new PropertiesDialogPrivate)
{
	ui->setupUi(this);
}

PropertiesDialog::~PropertiesDialog()
{
	delete ui;
}

void PropertiesDialog::setObject(ORMKataKowi *o){
	d->base = o;
	ui->leDirectorio->setText(o->directoryBase());
	ui->leDoc->setText(o->docDirectoryName());
	ui->leIG->setText(o->widgetsDirectoryName());
	ui->leORM->setText(o->modelDirectoryName());
	ui->lePrefijo->setText(o->widgetsPrefix());
	ui->chkDoc->setChecked(o->generateDocumentation());
	ui->chkIG->setChecked(o->generateWidgets());
//	ui->chkORM->setChecked(o->generateORM());
	ui->chkFull->setChecked(o->generateFullProject());
	ui->chkIntegrar->setChecked(o->generateQhcpFile());
	ui->leTarget->setText(o->target());
}

void PropertiesDialog::on_buttonBox_accepted()
{
	if(ui->leDirectorio->text()==""){
		QMessageBox::warning(this,windowTitle(),tr("Debe seleccionar un directorio para exportar."));
		return;
	}
	if(ui->chkDoc->isChecked() && ui->leDoc->text()==""){
		QMessageBox::warning(this,windowTitle(),trUtf8("Debe escribir un nombre para el directorio de documentación."));
		return;
	}
	if(ui->chkIG->isChecked() && ui->leIG->text()==""){
		QMessageBox::warning(this,windowTitle(),trUtf8("Debe escribir un nombre para el directorio de widgets(Interfaces Gráficas)."));
		return;
	}
	if(ui->chkIG->isChecked() && ui->lePrefijo->text()==""){
		QMessageBox::warning(this,windowTitle(),trUtf8("Debe escribir un prefijo para las widgets(Interfaces Gráficas)."));
		return;
	}
	if(ui->chkORM->isChecked() && ui->leORM->text()==""){
		QMessageBox::warning(this,windowTitle(),trUtf8("Debe escribir un nombre para el directorio de clases ORM."));
		return;
	}
	d->base->setDirectoryBase(ui->leDirectorio->text());
	d->base->setDocDirectoryName(ui->leDoc->text());
	d->base->setWidgetsDirectoraName(ui->leIG->text());
	d->base->setModelDirectoryName(ui->leORM->text());
	d->base->setWidgetsPrefix(ui->lePrefijo->text());
	d->base->setGenerateDocumentation(ui->chkDoc->isChecked());
	d->base->setGenerateWidgets(ui->chkIG->isChecked());
//	d->base->setGenerateORM(ui->chkORM->isChecked());
	d->base->setGenerateFullProject(ui->chkFull->isChecked());
	d->base->setGenerateQhcpFile(ui->chkIntegrar->isChecked());
	d->base->setTarget(ui->leTarget->text());
	accept();
}

void PropertiesDialog::on_toolButton_clicked()
{
	QString fileName = QFileDialog::getExistingDirectory(this, QString(),QString());
	if (fileName.isNull())
		return;
	ui->leDirectorio->setText(fileName);
}
