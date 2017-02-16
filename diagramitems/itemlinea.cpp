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

#include "itemlinea.h"
#include "pantalladiagrama.h"
#include "diagramnamespace.h"
#include <QPainter>
#include "itemmovible.h"

ItemLinea::ItemLinea(BaseItem *parent)
{
	Q_UNUSED(parent);
	m_connectors[0] = new Conector(this);
	m_connectors[1] = new Conector(this);
	setZValue(1.0);
	m_fillEndArrow = false;
	m_lineStyle = Qt::SolidLine;
	m_fillStartArrow= false;
	m_fillStartArrow = false;
}

ItemLinea::~ItemLinea(){
	delete m_connectors[0];
	delete m_connectors[1];
}

Conector *
ItemLinea::conector(UbicacionConector ubicacion) const
{
	//Q_ASSERT(index == 0 || index == 1);
	return m_connectors[ubicacion];
}

void
ItemLinea::updateObjects(){
	PantallaDiagrama * pant= qobject_cast<PantallaDiagrama *>(this->parent());
	Q_ASSERT(pant);
	if(!m_ownerInicio.isEmpty()){
		ItemMovible *owner = qobject_cast<ItemMovible *>(pant->itemById(m_ownerInicio));
		setInicioObject(owner);
	}
	if(!m_ownerFinal.isEmpty()){
		ItemMovible *owner = qobject_cast<ItemMovible *>(pant->itemById(m_ownerFinal));
		setFinalObject(owner);
	}
}

void
ItemLinea::setInicioOwner(QString value){
	m_ownerInicio =value;
}

void
ItemLinea::setFinalOwner(QString value){
	m_ownerFinal = value;
}

QVariant
ItemLinea::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSceneHasChanged) {
		if(scene()==NULL)
			removedFromScene();
		else
			addedToScene();
		actualizarBoceto();
	}
	return QGraphicsItem::itemChange(change, value);
}

QPolygonF
ItemLinea::linePoints() const
{
	return m_primerTrazo;
}

void
ItemLinea::updateCache()
{
	m_dirty = false;

	QPainterPath path;
	if (m_primerTrazo.isEmpty()||m_segundoTrazo.isEmpty()) {
		m_shape = path;
		return;
	}

	if (!m_startArrow.isEmpty()) {
		QLineF firstSegment(m_primerTrazo.at(0), m_primerTrazo.at(1));
		QMatrix matrix;
		matrix.translate(firstSegment.p1().x(), firstSegment.p1().y());
		matrix.rotate(360 - firstSegment.angle() + 90 + 180);
		m_transformedStartArrow = matrix.map(m_startArrow);
	}
	else {
		m_transformedStartArrow = QPainterPath();
	}

	if (!m_endArrow.isEmpty()) {
		int size = m_segundoTrazo.size();
		QLineF lastSegment(m_segundoTrazo.at(size-2), m_segundoTrazo.at(size-1));
		QMatrix matrix;
		matrix.translate(lastSegment.p2().x(), lastSegment.p2().y());
		matrix.rotate(-lastSegment.angle() + 90);
		m_transformedEndArrow = matrix.map(m_endArrow);
	}
	else {
		m_transformedEndArrow = QPainterPath();
	}

	path.moveTo(m_primerTrazo.at(0));
	for (int i = 1; i < m_primerTrazo.size(); i++)
		path.lineTo(m_primerTrazo.at(i));
	for (int i = 1; i < m_segundoTrazo.size(); i++)
		path.lineTo(m_segundoTrazo.at(i));

	path.addPath(m_transformedStartArrow);
	path.addPath(m_transformedEndArrow);
	QPainterPathStroker ps;
	ps.setWidth(4.33);
	ps.setJoinStyle(Qt::MiterJoin);
	if(!path.isEmpty())
		m_shape = ps.createStroke(path);
}

void
ItemLinea::repintado(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
	QPen pen(DiagramItems::normalColor);
	if (isSelected()) {
		pen.setColor(DiagramItems::selectedColor);
	}
	pen.setStyle(Qt::SolidLine);
	painter->setPen(pen);
	if (m_fillStartArrow) {
		painter->setBrush(pen.color());
	}
	painter->drawPath(m_transformedStartArrow);
	if (m_fillEndArrow) {
		if (!m_fillStartArrow) {
			painter->setBrush(pen.color());
		}
	}
	else {
		if (m_fillStartArrow) {
			painter->setBrush(QBrush());
		}
	}
	painter->drawPath(m_transformedEndArrow);
}

void
ItemLinea::setLinePoints(const QPolygonF &primerTrazo,const QPolygonF &segundoTrazo)
{
	if (m_primerTrazo != primerTrazo) {
		prepareGeometryChange();
		m_primerTrazo = primerTrazo;
		m_dirty = true;
	}
	if (m_segundoTrazo != segundoTrazo) {
		prepareGeometryChange();
		m_segundoTrazo = segundoTrazo;
	}
}

Qt::PenStyle
ItemLinea::lineStyle() const
{
	return m_lineStyle;
}

void
ItemLinea::setLineStyle(Qt::PenStyle style)
{
	if (m_lineStyle != style) {
		m_lineStyle = style;
	}
}

QPainterPath
ItemLinea::startArrow() const
{
	return m_startArrow;
}

void
ItemLinea::setStartArrow(const QPainterPath& path)
{
	if (m_startArrow != path) {
		prepareGeometryChange();
		m_startArrow = path;
		m_dirty = true;
	}
}

bool
ItemLinea::fillStartArrow() const
{
	return m_fillStartArrow;
}

void
ItemLinea::setFillStartArrow(bool fill)
{
	if (m_fillStartArrow != fill) {
		m_fillStartArrow = fill;
		m_dirty = true;
	}
}

QPainterPath
ItemLinea::endArrow() const
{
	return m_endArrow;
}

void
ItemLinea::setEndArrow(const QPainterPath& path)
{
	if (m_endArrow != path) {
		prepareGeometryChange();
		m_endArrow = path;
		m_dirty = true;
	}
}

bool
ItemLinea::fillEndArrow() const
{
	return m_fillEndArrow;
}

void
ItemLinea::setFillEndArrow(bool fill)
{
	if (m_fillEndArrow != fill) {
		m_fillEndArrow = fill;
		m_dirty = true;
	}
}

QRectF
ItemLinea::boundingRect() const
{
	return shape().controlPointRect();
}

QPainterPath
ItemLinea::shape() const
{
	if (m_dirty){
		const_cast<ItemLinea *>(this)->updateCache();
	}

	return m_shape;
}

void
ItemLinea::paint(QPainter *painter, const QStyleOptionGraphicsItem * op, QWidget * wd)
{
	this->repintado(painter,op,wd);

}

void
ItemLinea::connectToHubs(){
	m_connectors[0]->addToHub();
	m_connectors[1]->addToHub();
}

void
ItemLinea::disconectFromHubs(){
	m_connectors[0]->removeFromHub();
	m_connectors[1]->removeFromHub();
}

//TODO: eliminar despues de terminar Cargar y Guardar
void
ItemLinea::setInicioObject(ItemMovible * obj){
	this->m_connectors[0]->setHub(obj->hub());
}
void
ItemLinea::setFinalObject(ItemMovible * obj){
	this->m_connectors[1]->setHub(obj->hub());
}

void
ItemLinea::removedFromScene(){
	disconectFromHubs();
}

void
ItemLinea::addedToScene(){
	connectToHubs();
}
