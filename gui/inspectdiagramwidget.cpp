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

#include "inspectdiagramwidget.h"
#include "ui_inspectdiagramwidget.h"
#include "ficheros/SQL/sqlcore.h"
#include "pantalladiagrama.h"
#include "widgets/formcolumna.h"
#include "widgets/formtabla.h"
#include <QList>
#include <QtConcurrentRun>
#include <QMessageBox>
#include "model/entity.h"
#include <QIcon>

const char* STRING_ICON_WARNING = "://icons/images/dialog-warning.png";
const char* STRING_ICON_INFO = "://icons/images/dialog-information.png";

class InspectDiagramWidget::InspectDiagramWidgetPrivate{
public:
	InspectDiagramWidgetPrivate(){
		m_pantalla = NULL;

		itemColumnas = new QTreeWidgetItem(QStringList()<<tr("Errores de columnas"));
		itemTablas = new QTreeWidgetItem(QStringList()<<tr("Errores de entidades"));
		itemRelaciones = new QTreeWidgetItem(QStringList()<<tr("Errores de relaciones"));
		itemBase = new QTreeWidgetItem(QStringList()<<tr("Diagrama"));
		itemBase->addChild(itemTablas);
		itemBase->addChild(itemColumnas);
		itemBase->addChild(itemRelaciones);
	}
	void limpiarTree(QTreeWidgetItem * widget){
		QList<QTreeWidgetItem *> ls = widget->takeChildren ();
		foreach(QTreeWidgetItem * is, ls){
			delete is;
		}
	}
	QTreeWidgetItem *itemColumnas;
	QTreeWidgetItem *itemTablas;
	QTreeWidgetItem *itemRelaciones;
	QTreeWidgetItem *itemBase;
	PantallaDiagrama * m_pantalla;
	FormColumna * m_columna;
	FormTabla * m_tabla;
	QList<EntityAttibute *> m_cols;
	QList<Entity *> m_ent;
};

InspectDiagramWidget::InspectDiagramWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::InspectDiagramWidget),
	d(new InspectDiagramWidgetPrivate)
{
	ui->setupUi(this);
	d->m_columna = new FormColumna(ui->stackedWidget);
	d->m_tabla = new FormTabla(ui->stackedWidget);
	ui->stackedWidget->addWidget(d->m_columna);
	ui->stackedWidget->addWidget(d->m_tabla);

	ui->treeWidget->insertTopLevelItem(0, d->itemBase);
	d->itemBase->setExpanded(true);

	ui->treeWidget->setHeaderLabel("Errores detectados");
	ui->progressBar->setVisible(false);
	connect(this,SIGNAL(finished()),this,SLOT(onFinished()));

	ui->lbl_MensajeError->setVisible(false);
}

InspectDiagramWidget::~InspectDiagramWidget()
{
	delete ui;
	delete d;
}

void InspectDiagramWidget::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event);
	limpiarInterfaz();
}

void InspectDiagramWidget::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui->retranslateUi(this);
	}
	else
		QWidget::changeEvent(event);
}

void
InspectDiagramWidget::on_btnVerificar_clicked()
{
	limpiarInterfaz();

	connect(d->m_pantalla->dbms(),SIGNAL(onColumnError(EntityAttibute*,QStringList)),this,SLOT(onColumnError(EntityAttibute*,QStringList)));
	connect(d->m_pantalla->dbms(),SIGNAL(onEntidadError(Entity*,QStringList)),this,SLOT(onEntidadError(Entity*,QStringList)));
	QtConcurrent::run(this,&InspectDiagramWidget::verificar);
	ui->btnVerificar->setEnabled(false);
	ui->treeWidget->setEnabled(false);
	ui->progressBar->setVisible(true);
}

void
InspectDiagramWidget::verificar(){
	d->m_pantalla->dbms()->inspectDiagram(d->m_pantalla);
	d->m_pantalla->dbms()->disconnect();
	emit finished();
}

void
InspectDiagramWidget::setDiagrama(PantallaDiagrama *diagrama){
	d->m_pantalla = diagrama;
	d->m_columna->setTipoDatos(d->m_pantalla->dbms()->getListaTipos());
	d->m_columna->setStack(d->m_pantalla->undoStack());

	//Se muestra la información del diagrama
	limpiarInterfaz();
	this->setWindowTitle(tr("Validar diagrama - %1").arg(d->m_pantalla->nombreDiagrama()));
	this->ui->lbl_AutorName->setText(d->m_pantalla->nombreAutor());
	this->ui->lbl_DatabaseName->setText(d->m_pantalla->dbms()->nombreDBMS());
	this->ui->lbl_DiagramName->setText(d->m_pantalla->nombreDiagrama());
	this->ui->lbl_Formula->setText(d->m_pantalla->formula());
	this->ui->lbl_TamanoMaximo->setText(QString::number(d->m_pantalla->dbms()->limiteCaracteresNombre()));
}

void
InspectDiagramWidget::onColumnError(EntityAttibute* columna,QStringList messages){
	d->m_cols.append(columna);
	QTreeWidgetItem * it = new QTreeWidgetItem();
	it->setText(0,columna->name());
	it->setData(0,78,"columnas");
	it->setData(0,79,d->m_cols.count()-1);
	it->setData(0,80,messages);
	it->setIcon(0,QIcon(STRING_ICON_WARNING));
	d->itemColumnas->addChild(it);
}

void
InspectDiagramWidget::onEntidadError(Entity *entidad, QStringList messages){
	d->m_ent.append(entidad);
	QTreeWidgetItem * it = new QTreeWidgetItem();
	it->setText(0,entidad->nombre());
	it->setData(0,78,"entidades");
	it->setData(0,79,d->m_ent.count()-1);
	it->setData(0,80,messages);
	it->setIcon(0,QIcon(STRING_ICON_WARNING));
	d->itemTablas->addChild(it);
}
void InspectDiagramWidget::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column){
	Q_UNUSED(column);
	ui->listWidget->clear();
	if(!item->data(0,78).isNull()){
		QString it = item->data(0,78).toString();
		if(it=="columnas"){
			ui->stackedWidget->setCurrentIndex(1);
			d->m_columna->setItemActual(d->m_cols.at(item->data(0,79).toInt()));
			ui->listWidget->addItems(item->data(0,80).toStringList());
		}
		else if(it=="entidades"){
			ui->stackedWidget->setCurrentIndex(2);
			d->m_tabla->setItemActual(d->m_ent.at(item->data(0,79).toInt()));
			ui->listWidget->addItems(item->data(0,80).toStringList());
		}

	}
	else{
		ui->stackedWidget->setCurrentIndex(0);
	}
}

void InspectDiagramWidget::limpiarInterfaz()
{
	d->m_cols.clear();
	d->m_ent.clear();
	d->limpiarTree(d->itemColumnas);
	d->limpiarTree(d->itemTablas);
	d->m_columna->setItemActual(NULL);
	d->m_tabla->setItemActual(NULL);
	this->ui->listWidget->clear();
	this->ui->treeWidget->setCurrentItem(d->itemBase);
	this->ui->stackedWidget->setCurrentIndex(0);
	d->itemTablas->setIcon(0,QIcon());
	d->itemColumnas->setIcon(0,QIcon());
	d->itemRelaciones->setIcon(0,QIcon());
}

void InspectDiagramWidget::onFinished(){
	ui->progressBar->setVisible(false);
	ui->btnVerificar->setEnabled(true);
	ui->treeWidget->setEnabled(true);

	bool diagramError = false;

	//Verificación de errores por columnas
	int cont =d->itemColumnas->childCount();
	if(cont==0){
		d->itemColumnas->setExpanded(false);
	}
	else{
		d->itemColumnas->setIcon(0,QIcon(STRING_ICON_INFO));
		d->itemColumnas->setExpanded(true);
		diagramError= true;
	}
	ui->lbl_Error_Columna->setText(QString::number(cont));

	//Verificación de errores por relaciones
	cont = d->itemRelaciones->childCount();
	if(cont==0){
		d->itemRelaciones->setExpanded(false);
	}
	else{
		d->itemRelaciones->setIcon(0,QIcon(STRING_ICON_INFO));
		d->itemRelaciones->setExpanded(true);
		diagramError = true;
	}
	ui->lbl_Error_relacion->setText(QString::number(cont));

	//Verificación de errores por tablas
	cont = d->itemTablas->childCount();
	if(cont==0){
		d->itemTablas->setExpanded(false);
	}
	else{
		d->itemTablas->setIcon(0,QIcon(STRING_ICON_INFO));
		d->itemTablas->setExpanded(true);
		diagramError = true;
	}
	ui->lbl_Error_tabla->setText(QString::number(cont));

	if(!diagramError){
		ui->lbl_MensajeError->setVisible(false);
		QMessageBox::information(this,tr("Sin errores"),tr("No se han encontrado errores en el diagrama"));
	}
	else
		ui->lbl_MensajeError->setVisible(true);
}
