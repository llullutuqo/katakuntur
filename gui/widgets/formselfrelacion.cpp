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

#include "formselfrelacion.h"
#include "ui_formselfrelacion.h"
#include "comandos.h"
#include "model/entity.h"
#include "pantalladiagrama.h"

FormSelfRelacion::FormSelfRelacion(QWidget *parent) :
	Propiedades(parent),
	ui(new Ui::FormSelfRelacion)
{
	ui->setupUi(this);
	m_relacionActual= NULL;
}

FormSelfRelacion::~FormSelfRelacion()
{
	delete ui;
}

void FormSelfRelacion::changeEvent(QEvent *e)
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
void FormSelfRelacion::cambiarItemActual(BaseItem *oldItem, BaseItem *newItem){
	Q_UNUSED(oldItem)
	//if(oldItem)
		//disconnect(oldItem,0,this,0);
	m_relacionActual = static_cast<SelfRelacion *>(newItem);
	if(m_relacionActual){
		this->ui->leLabelInicio->setText(m_relacionActual->labelInicio());
		this->ui->leLabelFinal->setText(m_relacionActual->labelFinal());
		ui->lblFinalA->setText(m_relacionActual->ownerTabla()->nombre());
		ui->lblFinalB->setText(m_relacionActual->ownerTabla()->nombre());
		ui->lblInicioA->setText(m_relacionActual->ownerTabla()->nombre());
		ui->lblInicioB->setText(m_relacionActual->ownerTabla()->nombre());
	}
}

void FormSelfRelacion::on_leLabelInicio_textEdited(QString string)
{
	if(m_relacionActual!=0)
		m_relacionActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_relacionActual,"labelInicio",string));
}

void FormSelfRelacion::on_leLabelFinal_textEdited(QString string)
{
	if(m_relacionActual!=0)
		m_relacionActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_relacionActual,"labelFinal",string));
}
