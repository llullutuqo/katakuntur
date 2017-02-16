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

#include "formpantalladiagrama.h"
#include "ui_formpantalladiagrama.h"
#include "pantalladiagrama.h"
#include "ficheros/SQL/sqlcore.h"
#include "ficheros/SQL/sqlfactory.h"
#include <QMessageBox>
#include "comandos.h"

const QString MENSAJE_BASE = "<html><head/><body><p><img src=\":/icons/images/dialog-information.png\"/> %1</p></body></html>";

FormPantallaDiagrama::FormPantallaDiagrama(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FormPantallaDiagrama)
{
	ui->setupUi(this);
	ui->comboBox->addItem("");
	ui->comboBox->addItems(SQLFactory::keys());
	ui->comboBox_2->addItem("");
	//TODO: Arreglar
	//ui->comboBox_2->addItems(ORMFactory::keys());
	ui->cb_llaveForanea->addItems(QStringList()<<"%t_%c"<<"%c"<<"fk_%c");

}

FormPantallaDiagrama::~FormPantallaDiagrama()
{
	delete ui;
}

void FormPantallaDiagrama::changeEvent(QEvent *e)
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
void FormPantallaDiagrama::setPantallaDiagrama(PantallaDiagrama *pantalla, const QString mensaje){
	if(!pantalla)
		return;

	m_pant=pantalla;
	m_pant->deseleccionarTodo();
	if(mensaje.isEmpty())
		ui->lbl_mensaje->setVisible(false);
	else{
		ui->lbl_mensaje->setVisible(true);
		ui->lbl_mensaje->setText(MENSAJE_BASE.arg(mensaje));
	}
	this->ui->comboBox->setCurrentText(m_pant->dataBaseKey());
	//TODO:unncoment
	//this->ui->comboBox_2->setCurrentText(m_pant->ORMKey());
	this->ui->lineEdit->setText(m_pant->nombreDiagrama());
	this->ui->cb_llaveForanea->lineEdit()->setText(m_pant->formula());

	connect(this->ui->comboBox,SIGNAL(currentIndexChanged(QString)),m_pant,SLOT(setDataBaseKey(QString)));
	connect(this->ui->comboBox_2,SIGNAL(currentIndexChanged(QString)),m_pant,SLOT(setORMKey(QString)));
	connect(this->ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(setNombreDiagrama(QString)));
}
void FormPantallaDiagrama::closeEvent( QCloseEvent * ){
	desconectar();
}

void FormPantallaDiagrama::desconectar()
{
	if(m_pant){
		disconnect(this->ui->comboBox,0,m_pant,0);
		disconnect(this->ui->comboBox_2,0,m_pant,0);
		disconnect(this->ui->lineEdit,0,m_pant,0);
		disconnect(this->ui->cb_llaveForanea,0,m_pant,0);
		m_pant = NULL;
	}
}
void FormPantallaDiagrama::setNombreDiagrama(QString nombre){
	m_pant->undoStack()->push(new ComandoCambiarPropiedad(m_pant,"nombre",nombre));
	emit emitCambioNombre(nombre);
}

void FormPantallaDiagrama::on_buttonBox_accepted()
{
	if(ui->lineEdit->text().isEmpty()){
		QMessageBox::warning(this,windowTitle(),trUtf8("Debe escribir un nombre para el diagrama."));
		return;
	}
	const QString l_formula = ui->cb_llaveForanea->lineEdit()->text();
	if(l_formula.isEmpty()){
		QMessageBox::warning(this,windowTitle(),trUtf8("Debe escribir una fórmula para generar las llaves foráneas"));
		return;
	}
	if(m_pant->formula()!=l_formula)
		m_pant->undoStack()->push(new ComandoCambiarPropiedad(m_pant,"formula",l_formula));
	desconectar();
	accept();
}

void FormPantallaDiagrama::on_buttonBox_rejected()
{
	desconectar();
	reject();
}
