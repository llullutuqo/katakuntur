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

#include "formrelacion.h"
#include "ui_formrelacion.h"
#include "comandos.h"
#include "model/relacion.h"
#include "pantalladiagrama.h"
#include "model/entity.h"

FormRelacion::FormRelacion(QWidget *parent) :
	Propiedades(parent),
	ui(new Ui::FormRelacion)
{
	m_relacionActual=NULL;
	ui->setupUi(this);
	this->actualizarCombos();
}
void FormRelacion::actualizarCombos(){
	ui->cbCardinalidadInicio->clear();
	ui->cbCardinalidadInicio->insertItem(0,tr("Debe"));
	ui->cbCardinalidadInicio->insertItem(1,tr("Puede"));

	ui->cbCardinalidadFinal->clear();
	ui->cbCardinalidadFinal->insertItem(0,tr("Debe"));
	ui->cbCardinalidadFinal->insertItem(1,tr("Puede"));

	ui->cbMultiplicidadInicio->clear();
	ui->cbMultiplicidadInicio->insertItem(0,tr("un y solo un"));
	ui->cbMultiplicidadInicio->insertItem(1,trUtf8("uno o más"));
	ui->cbMultiplicidadInicio->insertItem(2,trUtf8("uno o más(Identificativo)"));
	ui->cbMultiplicidadInicio->insertItem(3,trUtf8("uno y solo un(Identificativo)"));

	ui->cbMultiplicidadFinal->clear();
	ui->cbMultiplicidadFinal->insertItem(0,tr("un y solo un"));
	ui->cbMultiplicidadFinal->insertItem(1,trUtf8("uno o más"));
	ui->cbMultiplicidadFinal->insertItem(2,trUtf8("uno o más(Identificativo)"));
	ui->cbMultiplicidadFinal->insertItem(3,trUtf8("uno y solo un(Identificativo)"));
}
FormRelacion::~FormRelacion()
{
	delete ui;
}

void FormRelacion::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		actualizarCombos();
		break;
	default:
		break;
	}
}

void FormRelacion::cambiarItemActual(BaseItem *oldItem, BaseItem *newItem){
	Q_UNUSED(oldItem)
	//if(oldItem)
		//disconnect(oldItem,0,this,0);
	m_relacionActual = static_cast<Relacion *>(newItem);
	if(m_relacionActual){
		/*lblIniciio->setText(m_relacionActual->tablaInicio()->nombre());
		lblFinal->setText(m_relacionActual->tablaFinal()->nombre());*/
		ui->lblFinalA->setText(m_relacionActual->tablaFinal()->nombre());
		ui->lblFinalB->setText(m_relacionActual->tablaFinal()->nombre());
		ui->lblInicioA->setText(m_relacionActual->tablaInicio()->nombre());
		ui->lblInicioB->setText(m_relacionActual->tablaInicio()->nombre());
	}
	actualizarCardinalidadLinealFinal();
	actualizarCardinalidadLinealInicio();
	actualizarLabelFinal();
	actualizarLabelInicio();
	actualizarMultiplicidadFinal();
	actualizarMultiplicidadInicio();
	//actualizarPropiedades();
}
/*void FormRelacion::actualizarPropiedades(){

}*/
void FormRelacion::actualizarMultiplicidadFinal(){

	if(this->m_relacionActual!=0)
		this->ui->cbMultiplicidadFinal->setCurrentIndex(obtenerCardinalidadInt(this->m_relacionActual->cardinalidadFinal()));
}
void FormRelacion::actualizarMultiplicidadInicio(){

	if(this->m_relacionActual!=0)
		this->ui->cbMultiplicidadInicio->setCurrentIndex(obtenerCardinalidadInt(this->m_relacionActual->cardinalidadInicio()));
}

void FormRelacion::actualizarCardinalidadLinealFinal(){

	if(this->m_relacionActual!=0)
		this->ui->cbCardinalidadFinal->setCurrentIndex(obtenerCardinalidadLinealInt(this->m_relacionActual->cardinalidadLineaFinal()));
}
void FormRelacion::actualizarCardinalidadLinealInicio(){

	if(this->m_relacionActual!=0)
		this->ui->cbCardinalidadInicio->setCurrentIndex(obtenerCardinalidadLinealInt(this->m_relacionActual->cardinalidadLineaInicio()));
}

void FormRelacion::actualizarLabelFinal(){
	if(this->m_relacionActual!=0)
		this->ui->leLabelFinal->setText(m_relacionActual->labelFinal());
}
void FormRelacion::actualizarLabelInicio(){
		if(this->m_relacionActual!=0)
			this->ui->leLabelInicio->setText(m_relacionActual->labelInicio());
}
void FormRelacion::on_leLabelInicio_textEdited(QString ns)
{
	if(this->m_relacionActual!=0)
		this->m_relacionActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_relacionActual, "labelInicio", ns));
}

void FormRelacion::on_leLabelFinal_textEdited(QString ns)
{
	if(this->m_relacionActual!=0)
		this->m_relacionActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_relacionActual, "labelFinal", ns));
}
void FormRelacion::on_cbMultiplicidadInicio_activated(int index)
{
	if(this->m_relacionActual!=0){
		Relacion::Cardinalidad c= obtenerCardinalidad(index);
		Relacion::Cardinalidad p = m_relacionActual->cardinalidadInicio();
		m_relacionActual->setCardinalidadInicio(c);
		if(m_relacionActual->cardinalidadInicio()!=c){
			ui->cbMultiplicidadInicio->setCurrentIndex(obtenerCardinalidadInt(m_relacionActual->cardinalidadInicio()));
		}
		else
			m_relacionActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_relacionActual, "cardinalidadInicio", c,p));
	}
}

void FormRelacion::on_cbMultiplicidadFinal_activated(int index)
{
	if(this->m_relacionActual!=0){
		Relacion::Cardinalidad c= obtenerCardinalidad(index);
		Relacion::Cardinalidad p = m_relacionActual->cardinalidadFinal();
		m_relacionActual->setCardinalidadFinal(c);
		if(m_relacionActual->cardinalidadFinal()!=c){
			ui->cbMultiplicidadFinal->setCurrentIndex(obtenerCardinalidadInt(m_relacionActual->cardinalidadFinal()));
		}
		else
			m_relacionActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_relacionActual, "cardinalidadFinal", c,p));
	}
}

void FormRelacion::on_cbCardinalidadInicio_activated(int index)
{
	if(this->m_relacionActual!=0)
		this->m_relacionActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_relacionActual, "cardinalidadLineaInicio", obtenerCardinalidadLineal(index)));
}

void FormRelacion::on_cbCardinalidadFinal_activated(int index)
{
	if(this->m_relacionActual!=0)
		this->m_relacionActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_relacionActual, "cardinalidadLineaFinal", obtenerCardinalidadLineal(index)));
}

Relacion::Cardinalidad FormRelacion::obtenerCardinalidad(int index){
	switch(index){
	case 0:
		return Relacion::Uno;
	case 1:
		return Relacion::Muchos;
	case 2:
		return Relacion::Identificativo;
	case 3:
		return Relacion::UnoIdentificativo;
	}

	return Relacion::Uno;
}

Relacion::CardinalidadLinea FormRelacion::obtenerCardinalidadLineal(int index){
	/*0 tr("Debe")
	1 tr("Puede")
	if(string.compare(tr("Puede"))==0){
		return Relacion::Puede;
	}
	else if(string.compare(tr("Debe"))==0){

	}*/
	switch(index){
	case 0:
		return Relacion::Debe;
	case 1:
		return Relacion::Puede;
	}
	return Relacion::Puede;
}
int FormRelacion::obtenerCardinalidadInt(Relacion::Cardinalidad cardinalidad){
	switch(cardinalidad){
	case Relacion::Uno:
		return 0;
	case Relacion::Muchos:
		return 1;
	case Relacion::Identificativo:
		return 2;
	case Relacion::UnoIdentificativo:
		return 3;
	}
	return 0;
}

int FormRelacion::obtenerCardinalidadLinealInt(Relacion::CardinalidadLinea cardinalidad){
	switch(cardinalidad){
	case Relacion::Debe:
		return 0;
	case Relacion::Puede:
		return 1;
	}
	return 0;
}
