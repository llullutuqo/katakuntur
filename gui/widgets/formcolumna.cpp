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

#include "formcolumna.h"
#include "ui_formcolumna.h"
#include "comandos.h"
#include "model/entity.h"

FormColumna::FormColumna(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FormColumna)
{
	ui->setupUi(this);
	m_col=NULL;
	setEnabled(false);
	ui->cb_llaveUnica->addItems(QStringList()<<QString()<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12");
	m_pant = new FormPantallaDiagrama(this);
}

FormColumna::~FormColumna()
{
	delete ui;
}

void FormColumna::changeEvent(QEvent *e)
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
void FormColumna::setItemActual(EntityAttibute *columna){
	if(m_col==columna)
		return;
	m_col = NULL;
	if(!columna){
		//En caso de que la columna sea NULA
		//se limpia la interfaz
		this->ui->lneNombre->setText("");
		ui->cbTipoDato->lineEdit()->setText("");
		this->ui->chbLlaveprimaria->setChecked(false);
		ui->cb_llaveUnica->lineEdit()->setText("");
		this->ui->chbNulo->setChecked(false);
		this->ui->ptNotas->setPlainText("");
		this->setEnabled(false);
		return;
	}
	ui->groupBox->setEnabled(false);
	if(columna->isForeignKey()){
		m_pant->setPantallaDiagrama(columna->foreignTable()->diagrama(),
													 trUtf8("Las llaves foráneas pueden ser editadas modificando la fórmula del diagrama."));
		m_pant->exec();
	}
	else{
		ui->groupBox->setEnabled(true);
		this->ui->lneNombre->setText(columna->name());
		ui->cbTipoDato->lineEdit()->setText(columna->dataType());
		this->ui->chbLlaveprimaria->setChecked(columna->isPrimaryKey());
		ui->cb_llaveUnica->lineEdit()->setText(columna->llaveUnica());
		this->ui->chbNulo->setChecked(columna->isNull());
		this->ui->ptNotas->setPlainText(columna->notes());
		this->setEnabled(true);
		m_col=columna;
	}
}

void
FormColumna::setStack(QUndoStack *stack){
	m_stack = stack;
}

void FormColumna::setTipoDatos(QStringList lista){
	//Se evita que se realicen cambios cuando se cambia la lista
	//de items
	EntityAttibute * temp = m_col;
	setItemActual(NULL);
	this->ui->cbTipoDato->clear();
	this->ui->cbTipoDato->addItems(lista);
	setItemActual(temp);
}

void FormColumna::on_lneNombre_textChanged(QString text)
{
	if(m_col)
		m_stack->push(new ComandoCambiarPropiedad(m_col,"nombre",text));
}

void FormColumna::on_chbLlaveprimaria_clicked(bool checked)
{
	if(m_col)
		m_stack->push(new ComandoCambiarPropiedad(m_col,"llavePrimaria",checked));
}

void FormColumna::on_cb_llaveUnica_currentTextChanged(const QString &arg1)
{
	if(m_col)
		m_stack->push(new ComandoCambiarPropiedad(m_col,"llaveUnica",arg1));
}

void FormColumna::on_chbNulo_clicked(bool checked)
{
	if(m_col)
		m_stack->push(new ComandoCambiarPropiedad(m_col,"nulo",checked));
}

void FormColumna::on_ptNotas_textChanged()
{
	if(m_col)
		m_stack->push(new ComandoCambiarPropiedad(m_col,"notas",ui->ptNotas->toPlainText()));
}

void FormColumna::on_cbTipoDato_editTextChanged(QString texto)
{
	if(m_col)
		m_stack->push(new ComandoCambiarPropiedad(m_col,"tipoDato",texto));
}


