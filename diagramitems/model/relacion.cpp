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

#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsTextItem>
#include "../pantalladiagrama.h"
#include "entityattibute.h"
#include "columnastabla.h"
#include "entity.h"
#include "itemlinea.h"
#include "relacion.h"
#include "../hub.h"
#include "math.h"
#include "diagramnamespace.h"

class Relacion::PrivateRelacion{
public:
	PrivateRelacion(const Relacion * base) :
		cardinalidadInicio(Uno),
		cardinalidadFinal(Uno),
		cardinalidadLineaInicio(Puede),
		cardinalidadLineaFinal(Debe),
		onUpdateAction(NoAction),
		onDeleteAction(NoAction)
	{
		this->textInicio = new QGraphicsTextItem();
		this->textFinal = new QGraphicsTextItem();
		this->textInicio->setTextInteractionFlags(Qt::TextEditable|Qt::TextSelectableByMouse);
		this->textFinal->setTextInteractionFlags(Qt::TextEditable|Qt::TextSelectableByMouse);
		showcols=true;
		if (!pathsInitialized) {
			pathsInitialized = true;
			//Uno
			//Muchos Final
			paths[0].moveTo(0, 12);
			paths[0].lineTo(7, 0);
			paths[0].moveTo(0, 12);
			paths[0].lineTo(-7, 0);
			paths[0].moveTo(0, 12);
			paths[0].lineTo(0, 0);
			//Identificativa Final
			paths[1].moveTo(0, 12);
			paths[1].lineTo(7, 0);
			paths[1].moveTo(0, 12);
			paths[1].lineTo(-7, 0);
			paths[1].moveTo(7, 12);
			paths[1].lineTo(-7, 12);
			paths[1].moveTo(0, 12);
			paths[1].lineTo(0, 0);
			//Uno
			paths[2].moveTo(0, 12);
			paths[2].lineTo(0, 0);
			//Uno Identificativo
			paths[3].moveTo(-7, 12);
			paths[3].lineTo(7, 12);
			paths[3].moveTo(0, 12);
			paths[3].lineTo(0, 0);
		}
		m_base = base;
	}
	~PrivateRelacion(){
		delete textInicio;
		delete textFinal;
	}

	QPainterPath muchos()
	{
		return paths[0];
	}
	QPainterPath identificativo()
	{
		return paths[1];
	}
	QPainterPath uno()
	{
		return paths[2];
	}
	QPainterPath unoIdentificativo(){
		return paths[3];
	}

	bool isIdentificativo(Cardinalidad value){
		if(value == Identificativo || value==UnoIdentificativo)
			return true;
		return false;
	}

	void deleteInicioCols(){
		foreach(EntityAttibute *col, m_columnasForaneasInicio){
			m_tabInicio->listaColumnas()->removeColumna(col);
			delete col;
		}
		m_columnasForaneasInicio.clear();
	}
	void deleteFinalCols(){
		foreach(EntityAttibute *col, m_columnasForaneasFinal){
			m_tabFinal->listaColumnas()->removeColumna(col);
			delete col;
		}
		m_columnasForaneasFinal.clear();
	}

	EntityAttibute * crearColumna(QString nombreCol,QString tipoDato,QString notas,Entity *tablaPropietaria, Entity *tablaForanea,Cardinalidad carl,CardinalidadLinea car){
		EntityAttibute *c = new EntityAttibute(tablaPropietaria);
		c->setEditable(false);
		QString nombre =m_base->diagrama()->formula();
		nombre.replace("%t",tablaForanea->nombre());
		nombre.replace("%c",nombreCol);
		c->setReference(nombreCol);
		c->setName(nombre);
		c->setNotes(notas);
		if(carl==Identificativo || carl==UnoIdentificativo)
			c->setPrimaryKey(true);
		c->setDataType(tipoDato);
		c->setForeignEntity(tablaForanea);
		c->setForeignKey(true);
		if(car==Puede)
			c->setNull(true);
		return c;
	}
	void updateInicio(){
		deleteInicioCols();
		switch(cardinalidadInicio){
		case Relacion::Uno:
			return;
		default:
			QList<EntityAttibute *> cols=m_tabFinal->obtenerllavesPrimarias();
			foreach(EntityAttibute * col,cols){
				EntityAttibute * coln = crearColumna(col->name(),col->dataType(),col->notes(),m_tabInicio,m_tabFinal,cardinalidadInicio,cardinalidadLineaInicio);
				m_columnasForaneasInicio.append(coln);
				if(coln->isPrimaryKey())
					m_tabInicio->listaColumnas()->insertColumna(0,coln);
				else
					m_tabInicio->listaColumnas()->appendColumna(coln);
			}
			break;
		}
	}
	void updateFinal(){
		deleteFinalCols();
		switch(cardinalidadFinal){
		case Relacion::Uno:
			return;
		default:
			QList<EntityAttibute *> cols=m_tabInicio->obtenerllavesPrimarias();
			foreach(EntityAttibute * col,cols){
				EntityAttibute * coln =crearColumna(col->name(),col->dataType(),col->notes(),m_tabFinal,m_tabInicio,cardinalidadFinal,cardinalidadLineaFinal);
				m_columnasForaneasFinal.append(coln);
				if(coln->isPrimaryKey())
					m_tabFinal->listaColumnas()->insertColumna(0,coln);
				else
					m_tabFinal->listaColumnas()->appendColumna(coln);
			}
			break;
		}
	}
	void updateCardinalidadLinea(CardinalidadLinea cardinalidad, bool inicio){
		switch(cardinalidad){
		case Puede:
			if(inicio){
				foreach(EntityAttibute *col,m_columnasForaneasInicio)
					col->setNull(true);
			}
			else{
				foreach(EntityAttibute *col,m_columnasForaneasFinal)
					col->setNull(true);
			}
			break;
		case Debe:
			if(inicio){
				foreach(EntityAttibute *col,m_columnasForaneasInicio)
					col->setNull(false);
			}
			else{
				foreach(EntityAttibute *col,m_columnasForaneasFinal)
					col->setNull(false);
			}
			break;
		}
	}

	QGraphicsTextItem * labelInicio(){
		return this->textInicio;
	}
	QGraphicsTextItem * labelFinal(){
		return this->textFinal;
	}
	Cardinalidad cardinalidadInicio;
	Cardinalidad cardinalidadFinal;
	CardinalidadLinea cardinalidadLineaInicio;
	CardinalidadLinea cardinalidadLineaFinal;
	Action onUpdateAction;
	Action onDeleteAction;
	QGraphicsTextItem * textInicio;
	QGraphicsTextItem * textFinal;
	static bool pathsInitialized;
	static QPainterPath paths[4];
	Qt::PenStyle m_pInicio;
	Qt::PenStyle m_pFinal;
	QPointF pInicio;
	QPointF pFinal;
	Entity * m_tabInicio;
	Entity * m_tabFinal;
	QList<EntityAttibute *> m_columnasForaneasInicio;
	QList<EntityAttibute *> m_columnasForaneasFinal;
	bool showcols;
	const Relacion *m_base;
};
bool Relacion::PrivateRelacion::pathsInitialized = false;
QPainterPath Relacion::PrivateRelacion::paths[4];

Relacion::Relacion(BaseItem *parent)
	: ItemLinea(parent), d(new PrivateRelacion(this))
{
	setFlag(ItemIsSelectable);
	d->labelInicio()->setParentItem(this);
	d->labelFinal()->setParentItem(this);
	d->m_tabInicio = NULL;
	d->m_tabFinal =NULL;
	setEndArrow(d->uno());
	setStartArrow(d->uno());
}

const char *Relacion::staticTypeName() {
	return "modelo-relacion";
}
const char *Relacion::typeName() {
	return staticTypeName();
}
QString Relacion::translatedTypeName(){
	return trUtf8("Relación").toUtf8();
}
Entity * Relacion::tablaInicio(){
	return d->m_tabInicio;
}

Entity * Relacion::tablaFinal(){
	return d->m_tabFinal;
}
QString Relacion::labelInicio(){
	return d->labelInicio()->toPlainText();
}
void Relacion::setLabelInicio(QString value){
	if(value != labelInicio()){
		d->labelInicio()->setPlainText(value.toLower());
		updateLabels();
		emit propiedadCambiada("labelInicio", value);

		//TODO:Implementar actualizacion de label
	}

}

QString Relacion::labelFinal(){
	return d->labelFinal()->toPlainText();
}
void Relacion::setLabelFinal(QString value){
	if(labelFinal()!=value){
		d->labelFinal()->setPlainText(value.toLower());
		updateLabels();
		emit propiedadCambiada("labelFinal", value);
	}

	//TODO: implementar actualizacion de label
}

Relacion::~Relacion()
{
	delete d;
}

Relacion::Cardinalidad
Relacion::cardinalidadInicio() const
{
	return d->cardinalidadInicio;
}
Relacion::Cardinalidad
Relacion::cardinalidadFinal() const
{
	return d->cardinalidadFinal;
}
Relacion::CardinalidadLinea Relacion::cardinalidadLineaInicio() const{
	return d->cardinalidadLineaInicio;
}
void
Relacion::setCardinalidadLineaInicio(CardinalidadLinea value){
	if(d->cardinalidadLineaInicio!=value){
		d->cardinalidadLineaInicio = value;
		emit propiedadCambiada("cardinalidadLineaInicio", value);
		d->updateCardinalidadLinea(value,true);
		updateLineaInicio();
		update();
	}
}

Relacion::CardinalidadLinea
Relacion::cardinalidadLineaFinal() const{
	return d->cardinalidadLineaFinal;
}
void Relacion::setCardinalidadLineaFinal(CardinalidadLinea value){
	if(d->cardinalidadLineaFinal!=value){
		d->cardinalidadLineaFinal = value;
		emit propiedadCambiada("cardinalidadLineaFinal", value);
		d->updateCardinalidadLinea(value,false);
		updateLineaFinal();
		update();
	}

}
void Relacion::repintado(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
	if (m_dirty){
		updateCache();
	}
	QPen pen(DiagramItems::normalColor);
	if (isSelected()) {
		pen.setColor(DiagramItems::selectedColor);
	}
	d->labelInicio()->setPos(d->pInicio);
	d->labelFinal()->setPos(d->pFinal);
	pen.setStyle(d->m_pInicio);
	painter->setPen(pen);
	painter->drawPolyline(m_primerTrazo);
	pen.setStyle(d->m_pFinal);
	painter->setPen(pen);
	painter->drawPolyline(m_segundoTrazo);
	ItemLinea::repintado(painter,NULL,NULL);
}

void
Relacion::setCardinalidadInicio(Cardinalidad cardinality)
{

	if (d->cardinalidadInicio != cardinality) {
		if(d->isIdentificativo(d->cardinalidadFinal)&& d->isIdentificativo(cardinality))
			return;
		d->cardinalidadInicio = cardinality;
		if(d->m_base->diagrama())
			d->updateInicio();
		emit propiedadCambiada("cardinalidadInicio", cardinality);
		updateInicio();
		update();
	}
}
void
Relacion::setCardinalidadFinal(Cardinalidad cardinality)
{
	if (d->cardinalidadFinal != cardinality) {
		if(d->isIdentificativo(d->cardinalidadInicio) && d->isIdentificativo(cardinality))
			return;
		d->cardinalidadFinal = cardinality;
		if(d->m_base->diagrama())
			d->updateFinal();
		emit propiedadCambiada("cardinalidadFinal", cardinality);
		updateFinal();
		update();
	}
}


void
Relacion::updateLineaInicio(){
	switch(d->cardinalidadLineaInicio){
	case Debe:
		d->m_pInicio=Qt::SolidLine;
		break;
	case Puede:
		d->m_pInicio=Qt::DashLine;
		break;
	}
}

void Relacion::updateForeignKeys()
{
	d->updateInicio();
	d->updateFinal();
}
void
Relacion::updateLineaFinal(){
	switch(d->cardinalidadLineaFinal){
	case Debe:
		d->m_pFinal=Qt::SolidLine;
		break;
	case Puede:
		d->m_pFinal=Qt::DashLine;
		break;
	}
}
void
Relacion::updateInicio()
{
	switch(d->cardinalidadInicio){
	case Uno:
		setFillStartArrow(false);
		setStartArrow(d->uno());
		break;
	case Muchos:
		setFillStartArrow(true);
		setStartArrow(d->muchos());
		break;
	case Identificativo:
		setFillStartArrow(true);
		setStartArrow(d->identificativo());
		break;
	case UnoIdentificativo:
		setFillStartArrow(false);
		setStartArrow(d->unoIdentificativo());
		break;
	}
	setLineStyle(Qt::SolidLine);
}

void
Relacion::updateFinal()
{
	switch(d->cardinalidadFinal){
	case Uno:
		setFillEndArrow(false);
		setEndArrow(d->uno());
		break;
	case Muchos:
		setFillEndArrow(true);
		setEndArrow(d->muchos());
		break;
	case Identificativo:
		setFillEndArrow(true);
		setEndArrow(d->identificativo());
		break;
	case UnoIdentificativo:
		setFillEndArrow(false);
		setEndArrow(d->unoIdentificativo());
		break;
	}
	setLineStyle(Qt::SolidLine);
}

void
Relacion::actualizarBoceto()
{
	//qDebug()<<"Relacion::updateLayout Inicio";
	prepareGeometryChange();

	Conector *connector1 = conector(ItemLinea::StartConector);
	Conector *connector2 = conector(ItemLinea::EndConector);

	//bool haveLine = false;

	//QPolygonF line;
	QPolygonF linea;
	QPolygonF lineb;

	QPointF p1 = connector1->pos();
	QPointF p2 = connector2->pos();
	qreal a1 = connector1->angle();
	qreal a2 = connector2->angle();

	QLineF line1 = QLineF::fromPolar(1, a1).translated(p1);
	QLineF line2 = QLineF::fromPolar(1, a2).translated(p2);
	QPointF intersection;

	linea << p1;
	//HACK: algunas intersecciones generan numeros como :-5.40432e+16
	//Se hace una comparacion para evitar esas distancias
	if (line1.intersect(line2, &intersection) != QLineF::NoIntersection &&!(intersection.x()>10000 ||intersection.y()>10000 ) &&!(intersection.x()<-10000 ||intersection.y()<-10000 )) {
		linea<<intersection;
		lineb<<intersection;
	}
	else {

		if (line1.intersect(line2.normalVector(), &intersection) != QLineF::NoIntersection) {
			// 3-segment line
			qreal len = QLineF(p1, intersection).length() * 0.5;
			QPointF p_1=QLineF::fromPolar(len, a1).translated(p1).p2();
			QPointF p_2=QLineF::fromPolar(len, a2).translated(p2).p2();
			QLineF lineas = QLineF(p_1,p_2);
			linea << p_1;
			linea << lineas.pointAt(0.5);
			lineb << lineas.pointAt(0.5);
			lineb << p_2;
		}
		else {
			qFatal("No line?");
		}
	}
	lineb << p2;
	updateLabels();
	setLinePoints(linea,lineb);
	updateLineaInicio();
	updateLineaFinal();
	//updateInicio();
	//updateFinal();
	//qDebug()<<"Actualizado Boceto";
	m_dirty=true;
}

void Relacion::removedFromScene(){
	d->deleteFinalCols();
	d->deleteInicioCols();
	this->disconnect();
	disconnect(d->m_tabInicio,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTablaInicio()));
	disconnect(d->m_tabFinal,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTablaFinal()));
	disconnect(d->m_tabFinal,SIGNAL(propiedadCambiada(QString,QVariant)),this,SLOT(onPropertyTablaFinal(QString,QVariant)));
	disconnect(d->m_tabInicio,SIGNAL(propiedadCambiada(QString,QVariant)),this,SLOT(onPropertyTablaInicio(QString,QVariant)));
	ItemLinea::removedFromScene();
}

void Relacion::addedToScene(){
	updateForeignKeys();
	connect(d->m_tabInicio,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTablaInicio()));
	connect(d->m_tabFinal,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTablaFinal()));
	connect(diagrama(),SIGNAL(formulaChanged(QString)),this,SLOT(updateForeignKeys()));
	ItemLinea::addedToScene();
}

void Relacion::updateLabels(){
	d->pInicio = conector(ItemLinea::StartConector)->pos() + QPointF(7,7);
	d->pFinal = conector(ItemLinea::EndConector)->pos() + QPointF(7,7);
	if(d->pInicio.y()<d->pFinal.y())
		d->pFinal.setY(d->pFinal.y()-this->d->labelFinal()->boundingRect().height()-14);
	else
		d->pInicio.setY(d->pInicio.y()-this->d->labelFinal()->boundingRect().height()-14);

	if(d->pInicio.x()<d->pFinal.x())
		d->pFinal.setX(d->pFinal.x()-this->d->labelFinal()->boundingRect().width()-14);
	else
		d->pInicio.setX(d->pInicio.x()-this->d->labelInicio()->boundingRect().width()-14);
}
void Relacion::setInicioObject(ItemMovible * obj){
	Q_ASSERT(obj);
	ItemLinea::setInicioObject(obj);
	d->m_tabInicio  =static_cast<Entity*>(obj);
	connect(d->m_tabInicio,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTablaInicio()));
	connect(d->m_tabInicio,SIGNAL(propiedadCambiada(QString,QVariant)),this,SLOT(onPropertyTablaInicio(QString,QVariant)));
}
void Relacion::setFinalObject(ItemMovible * obj){
	Q_ASSERT(obj);
	ItemLinea::setFinalObject(obj);
	d->m_tabFinal  =static_cast<Entity*>(obj);
	connect(d->m_tabFinal,SIGNAL(columnaPrimariaModificada()),this,SLOT(onUpdateTablaFinal()));
	connect(d->m_tabFinal,SIGNAL(propiedadCambiada(QString,QVariant)),this,SLOT(onPropertyTablaFinal(QString,QVariant)));
	d->m_tabFinal->updateHubs();
	d->m_tabInicio->updateHubs();
}
bool Relacion::hasEmptyLabels(){
	return this->labelFinal().isEmpty() ||this->labelInicio().isEmpty();
}
QString Relacion::tipoLinea(){
	return "relacion";
}

void Relacion::onUpdateTablaInicio(){
	d->updateFinal();
}

void Relacion::onUpdateTablaFinal(){
	d->updateInicio();
}

void Relacion::onPropertyTablaInicio(const QString value, const QVariant)
{
	if(value=="nombre"){
		onUpdateTablaInicio();
	}
}

void Relacion::onPropertyTablaFinal(const QString value, const QVariant)
{
	if(value=="nombre"){
		onUpdateTablaFinal();
	}
}
