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

#include "formtabla.h"
#include "ui_formtabla.h"
#include "model/entity.h"
#include "comandos.h"
#include "model/columnastabla.h"
#include "ficheros/SQL/sqlcore.h"
#include <QItemDelegate>
#include "pantalladiagrama.h"
#include <delegates/uniquedelegate.h>

FormTabla::FormTabla(QWidget *parent) :
	Propiedades(parent),
	ui(new Ui::FormTabla){
	m_tablaActual = NULL;
	ui->setupUi(this);
	ui->tablita->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tablita->setModel(new ColumnListModel(this));
	ui->tablita->model()->setHeaderData(0,Qt::Horizontal,tr("Nombre"),Qt::DisplayRole);
	ui->tablita->model()->setHeaderData(1,Qt::Horizontal,tr("Clave Primaria"),Qt::DisplayRole);
	ui->tablita->model()->setHeaderData(2,Qt::Horizontal,trUtf8("Clave Única"),Qt::DisplayRole);
	ui->tablita->model()->setHeaderData(3,Qt::Horizontal,tr("Nulo"),Qt::DisplayRole);
	ui->tablita->model()->setHeaderData(4,Qt::Horizontal,tr("Tipo Dato(DBMS)"),Qt::DisplayRole);
	ui->tablita->model()->setHeaderData(5,Qt::Horizontal,trUtf8("Descripción"),Qt::DisplayRole);
	delegate = new ComboBoxDelegate(this);
	delegate->setEditable(true);
	ui->tablita->setItemDelegateForColumn(ColumnListModel::TipoColumna, delegate);
	ui->tablita->setItemDelegateForColumn(ColumnListModel::ColumnaRequerida, new UniqueDelegate(this));
}

FormTabla::~FormTabla(){
	delete ui;
}

void
FormTabla::changeEvent(QEvent *e){
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void
FormTabla::cambiarItemActual(BaseItem *oldItem, BaseItem *newItem){
	Q_UNUSED(oldItem)	
	if(newItem){
		m_tablaActual = static_cast<Entity *>(newItem);
		actualizarNombre();
		actualizarDescripcion();
		ui->tablita->setColumnList(m_tablaActual->listaColumnas());
		if(m_tablaActual->diagrama()->dbms()){
			delegate->setOptions(m_tablaActual->diagrama()->dbms()->getListaTipos());
		}
		else{
			delegate->setOptions(QStringList());
		}
	}
	else
		ui->tablita->setColumnList(NULL);
}

void
FormTabla::actualizarNombre(){
	if(this->m_tablaActual!=0)
		this->ui->leTabla->setText(m_tablaActual->nombre());
}

void
FormTabla::on_leTabla_textEdited(QString ns){
	if(this->m_tablaActual!=0)
		this->m_tablaActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_tablaActual, "nombre", ns));
}

void
FormTabla::actualizarDescripcion(){
	if(this->m_tablaActual!=0)
		this->ui->pteDescripcion->setPlainText(m_tablaActual->descripcion());
}

void
FormTabla::on_pteDescripcion_textChanged(){
	if(this->m_tablaActual!=0){
		QString ns = m_tablaActual->descripcion();
		m_tablaActual->setDescripcion(ui->pteDescripcion->toPlainText());
		if(ns !=ui->pteDescripcion->toPlainText())
			m_tablaActual->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(m_tablaActual, "descripcion",m_tablaActual->descripcion(),ns));
	}
}

void
FormTabla::on_pbEliminar_clicked(){
		ui->tablita->deleteCurrent();
}

void
FormTabla::on_pbMoverArriba_clicked(){
	ui->tablita->moverArriba();
}

void
FormTabla::on_pbMoverAbajo_clicked(){
	ui->tablita->moverAbajo();
}

void
FormTabla::on_pbAgregar_clicked(){
	ui->tablita->addRow();
}
