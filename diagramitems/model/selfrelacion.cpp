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

#include "selfrelacion.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsTextItem>
#include <QPainterPath>
#include "columnastabla.h"
#include "entity.h"
#include "diagramnamespace.h"
#include "pantalladiagrama.h"

class SelfRelacion::PrivateSelfRelacion
{
public:
	PrivateSelfRelacion(const SelfRelacion * base){
		m_tabla=NULL;
		this->textInicio = new QGraphicsTextItem();
		this->textFinal = new QGraphicsTextItem();
		this->textInicio->setTextInteractionFlags(Qt::TextEditable|Qt::TextSelectableByMouse);
		this->textFinal->setTextInteractionFlags(Qt::TextEditable|Qt::TextSelectableByMouse);
		m_showCols=true;
		//recentAdd=false;
		if (!pathsInitialized) {
			pathsInitialized = true;
			//Uno
			//Muchos Final
			//TODO: DIsminuir los paths
			paths[0].moveTo(0, 12);
			paths[0].lineTo(7, 0);
			paths[0].moveTo(0, 12);
			paths[0].lineTo(-7, 0);
			paths[0].moveTo(0, 12);
			paths[0].lineTo(0, 0);
			//Uno
			paths[1].moveTo(0, 12);
			paths[1].lineTo(0, 0);

		}
		m_base = base;
	}

	~PrivateSelfRelacion(){
		delete textInicio;
		delete textFinal;
	}

	void deleteCols(){
		foreach(EntityAttibute *col, m_columnas){
			m_tabla->listaColumnas()->removeColumna(col);
			delete col;
		}
		m_columnas.clear();
	}

	void update(){
		deleteCols();
		QList<EntityAttibute *> cols=m_tabla->obtenerllavesPrimarias();
		foreach(EntityAttibute * col,cols){
			QString nombre =m_base->diagrama()->formula();
			nombre.replace("%t",m_tabla->nombre());
			nombre.replace("%c",col->name());
			EntityAttibute *c = new EntityAttibute(m_tabla);
			c->setEditable(false);
			if(nombre==col->name())
				nombre +="fk";
			c->setReference(col->name());
			c->setName(nombre);
			c->setNotes(col->notes());
			c->setForeignEntity(m_tabla);
			c->setDataType(col->dataType());
			c->setForeignKey(true);
			c->setNull(true);
			m_columnas.append(c);
			m_tabla->listaColumnas()->appendColumna(c);
		}
	}
	QGraphicsTextItem * labelInicio(){
		return this->textInicio;
	}
	QGraphicsTextItem * labelFinal(){
		return this->textFinal;
	}
	QPainterPath muchos()
	{
		return paths[0];
	}
	QPainterPath uno()
	{
		return paths[1];
	}
	QGraphicsTextItem * textInicio;
	QGraphicsTextItem * textFinal;
	static bool pathsInitialized;
	static QPainterPath paths[2];
	QPointF pInicio;
	QPointF pFinal;
	Entity * m_tabla;
	QList<EntityAttibute *> m_columnas;
	//HACK:
	//bool recentAdd;
	bool m_showCols;
	const SelfRelacion * m_base;
};
bool SelfRelacion::PrivateSelfRelacion::pathsInitialized = false;
QPainterPath SelfRelacion::PrivateSelfRelacion::paths[2];
SelfRelacion::SelfRelacion(BaseItem *parent): ItemLinea(parent), d(new PrivateSelfRelacion(this))
{
	setFlag(ItemIsSelectable);
	this->d->labelInicio()->setParentItem(this);
	this->d->labelFinal()->setParentItem(this);
	this->setStartArrow(d->uno());
	this->setEndArrow(d->muchos());

}
SelfRelacion::~SelfRelacion(){
	delete d;
}
void SelfRelacion::updateLabels(){
	d->pInicio = conector(ItemLinea::StartConector)->pos()+ QPointF(0,40);
	d->pFinal = QPointF(conector(ItemLinea::EndConector)->pos().x()+60,conector(ItemLinea::EndConector)->pos().y());
}
const char *SelfRelacion::staticTypeName() {
	return "modelo-selfrelacion";
}
const char *SelfRelacion::typeName() {
	return staticTypeName();
}
QString SelfRelacion::translatedTypeName(){
	return trUtf8("AutoRelación").toUtf8();
}
Entity * SelfRelacion::ownerTabla(){
	return d->m_tabla;
}
QString SelfRelacion::labelInicio(){
	return d->labelInicio()->toPlainText();
}
void SelfRelacion::setLabelInicio(QString value){
	if(value != labelInicio()){
		d->labelInicio()->setPlainText(value.toLower());
		updateLabels();
		emit propiedadCambiada("labelInicio", value);
		//TODO:Implementar actualizacion de label
	}

}

QString SelfRelacion::labelFinal(){
	return d->labelFinal()->toPlainText();
}
void SelfRelacion::setLabelFinal(QString value){
	if(labelFinal()!=value){
		d->labelFinal()->setPlainText(value.toLower());
		updateLabels();
		emit propiedadCambiada("labelFinal", value);
	}

	//TODO: implementar actualizacion de label
}
void SelfRelacion::setInicioObject(ItemMovible * obj){
	ItemLinea::setInicioObject(obj);
	ItemLinea::setFinalObject(obj);
	d->m_tabla =static_cast<Entity*>(obj);
	connect(d->m_tabla,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTabla()));
	connect(d->m_tabla,SIGNAL(propiedadCambiada(QString,QVariant)),this,SLOT(onPropertyTabla(QString,QVariant)));
	obj->updateHubs();
	//d->update();
	//d->recentAdd=true;
}
void SelfRelacion::setFinalObject(ItemMovible * obj){
	Q_UNUSED(obj);
	//m_tabFinal  =static_cast<Tabla*>(obj);
}
void SelfRelacion::repintado(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
	if (m_dirty){
		updateCache();
	}
	QPen pen(DiagramItems::normalColor);
	if (isSelected()) {
		pen.setColor(DiagramItems::selectedColor);
	}
	this->d->labelInicio()->setPos(d->pInicio);
	this->d->labelFinal()->setPos(d->pFinal);
	pen.setStyle(Qt::DashLine);
	painter->setPen(pen);
	painter->drawPolyline(m_primerTrazo);
	painter->drawPolyline(m_segundoTrazo);
	ItemLinea::repintado(painter,NULL,NULL);
}
void
SelfRelacion::actualizarBoceto()
{
	prepareGeometryChange();
	Conector *connector1 = conector(ItemLinea::StartConector);
	Conector *connector2 = conector(ItemLinea::EndConector);
	QPolygonF linea;
	QPolygonF lineb;
	QPointF p1 = connector1->pos();
	QPointF p2 = connector2->pos();
	//QRectF ds =d->m_tabla->boundingRect();
	linea<<p1<<p1+ QPointF(0,40)<<p1+ QPointF(qAbs(p1.x()-p2.x())+60,40);
	lineb<<p1+ QPointF(qAbs(p1.x()-p2.x())+60,40)<<QPointF(p2.x()+60,p2.y())<<p2;
	updateLabels();
	setLinePoints(linea,lineb);
	m_dirty=true;
}

void SelfRelacion::onUpdateTabla(){
	d->update();
}

void SelfRelacion::onPropertyTabla(const QString nombre, const QVariant)
{
	if(nombre =="nombre"){
		onUpdateTabla();
	}
}

void SelfRelacion::removedFromScene(){
	d->deleteCols();
	disconnect(d->m_tabla,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTabla()));
	disconnect(d->m_tabla,SIGNAL(propiedadCambiada(QString,QVariant)),this,SLOT(onPropertyTabla(QString,QVariant)));
	ItemLinea::removedFromScene();
}

void SelfRelacion::addedToScene(){
	d->update();
	connect(d->m_tabla,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTabla()));
	connect(d->m_tabla,SIGNAL(propiedadCambiada(QString,QVariant)),this,SLOT(onPropertyTabla(QString,QVariant)));
	ItemLinea::addedToScene();
}
