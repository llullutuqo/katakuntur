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
#include <QStringList>
#include <QMimeData>
#include <QDebug>
#include <QString>
#include <QList>
#include "entity.h"
#include "relacion.h"
#include "selfrelacion.h"
#include "../pantalladiagrama.h"
#include "columnastabla.h"
#include "../boxsidehub.h"
#include "diagramnamespace.h"

class Entity::PrivateEntidad{
public:
	PrivateEntidad(Entity *owner){
		m_columnList = new ColumnasTabla(owner);
		m_color = Qt::white;
	}

	QString m_name;
	QString m_descripcion;
	QColor m_color;
	ColumnasTabla *m_columnList;
	QList<QString> m_columnLabels;
	QRectF m_outerRect, m_nameBgRect;
	QPointF m_namePos, m_firstColPos, m_colPosIncrement;
	qreal m_leftSideWidth;
};
QPen Entity::normalCols = QPen(QColor(0,0,0));
QPen Entity::foreignCols = QPen(QColor(149,0,0));

Entity::Entity(BaseItem *parent)
	: ItemMovible(parent),d(new PrivateEntidad(this))
{
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);
	setHub(new BoxSideHub(this));
	connect(d->m_columnList, SIGNAL(columnaInserted(int)), this, SLOT(actualizarBoceto()));
	connect(d->m_columnList, SIGNAL(columnaRemoved(int)), this, SLOT(actualizarBoceto()));
	connect(d->m_columnList, SIGNAL(columnaChanged(int)), this, SLOT(actualizarBoceto()));
	connect(d->m_columnList, SIGNAL(columnaPrimariaChanged(int)), this, SLOT(onCambioColumnas(int)));
}
Entity::~Entity(){
	delete d;
}
const char *Entity::staticTypeName() {
	return "modelo-tabla";
}
const char *Entity::typeName() {
	return staticTypeName();
}
QString Entity::translatedTypeName(){
	return tr("Entidad");
}

int Entity::agregarColumna(EntityAttibute * col){
	return d->m_columnList->appendColumna(col);
}

QRectF
Entity::boundingRect() const
{
	return d->m_outerRect;
}
int Entity::countColumns(){
	return d->m_columnList->columns().count();
}

void
Entity::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QFont font = scene()->font();
	QFont boldFont = font;
	boldFont.setBold(true);

	QPen borderPen(DiagramItems::normalColor);
	borderPen.setWidthF(1.3);
	if (isSelected()) {
		borderPen.setColor(DiagramItems::selectedColor);
	}

	painter->setPen(QPen(DiagramItems::tableContents));
	painter->setFont(font);

	painter->fillRect(d->m_outerRect, QColor(255, 255, 255));

	painter->fillRect(d->m_nameBgRect, QColor(255, 255, 255));
	painter->drawLine(d->m_nameBgRect.bottomLeft(), d->m_nameBgRect.bottomRight());
	painter->drawText(d->m_namePos, d->m_name);
	QPointF colPos = d->m_firstColPos;
	QPointF leftSizePos = colPos - QPointF(d->m_leftSideWidth, 0);
	int i = 0;
	foreach (EntityAttibute *column, d->m_columnList->columns()) {
		if(column->isForeignKey()){
			if(diagrama()->vista()==PantallaDiagrama::Logica)
				continue;
			painter->setPen(foreignCols);
		}
		else
			painter->setPen(normalCols);
		bool isBold = column->isLlaveUnica();
		painter->setFont(isBold ? boldFont : font);
		painter->drawText(colPos, column->name());
		if (!d->m_columnLabels[i].isEmpty()) {
			painter->setFont(isBold ? boldFont : font);
			painter->drawText(leftSizePos, d->m_columnLabels[i]);
		}
		colPos += d->m_colPosIncrement;
		leftSizePos += d->m_colPosIncrement;
		++i;
	}
	painter->setPen(borderPen);
	painter->drawRect(d->m_outerRect);
}
QString Entity::nombre() const {
	return d->m_name;
}
void
Entity::setNombre(const QString &name)
{
	if(name!=d->m_name){
		d->m_name = name;
		actualizarBoceto();
		emit propiedadCambiada("nombre", name);
	}
}

QString Entity::descripcion() const {
	return d->m_descripcion;
}
void
Entity::setDescripcion(const QString &value)
{
	d->m_descripcion = value;
	emit propiedadCambiada("descripcion", value);
}

void
Entity::setInitialName(int counter)
{
	setNombre(tr("ENTIDAD_%1").arg(counter));
}

QList<EntityAttibute *> Entity::columnas() const {
	return d->m_columnList->columns();
}
ColumnasTabla * Entity::listaColumnas(){
	return d->m_columnList;
}
EntityAttibute * Entity::findColumn(QString columName){
	foreach(EntityAttibute *col, d->m_columnList->columns()){
		if(col->name() == columName)
			return col;
	}
	return NULL;
}
EntityAttibute * Entity::findColumn(QUuid id){
	foreach(EntityAttibute *col, d->m_columnList->columns()){
		if(col->id() == id)
			return col;
	}
	return NULL;
}
void
Entity::actualizarBoceto()
{
	if (!scene())
		return;

	prepareGeometryChange();

	QFont font = scene()->font();
	QFontMetricsF fontMetrics(font);

	QFont boldFont = font;
	boldFont.setBold(true);
	QFontMetricsF boldFontMetrics(boldFont);

	qreal spaceWidth = fontMetrics.width(' ');
	qreal nameWidth = fontMetrics.width(d->m_name);
	qreal height = fontMetrics.height();

	d->m_leftSideWidth = fontMetrics.width("   ");

	qreal width = nameWidth + spaceWidth * 2;

	qreal maxColumnWidth = 0;
	d->m_columnLabels.clear();
	int count =0;
	foreach (EntityAttibute *column, d->m_columnList->columns()) {
		if(diagrama()->vista()==PantallaDiagrama::Logica && column->isForeignKey())
			continue;
		qreal columnWidth = column->isLlaveUnica() ? boldFontMetrics.width(column->name()) : fontMetrics.width(column->name());
		maxColumnWidth = qMax(maxColumnWidth, columnWidth);
		qreal columnLeftSideWidth = 0;
		QStringList label;
		label << " ";
		if (column->isPrimaryKey()) {
			label << "#";
		}
		else{
			if(column->isNull()){
				label << "o";
			}
			else
				label << "*";{
			}
		}
		
		if(diagrama()->vista()==PantallaDiagrama::Fisica)
			label << "[" + (column->isForeignKey()? "AUTO" : column->dataType()) + "]";
		QString labelStr = label.join(" ");
		d->m_columnLabels << labelStr;
		if(column->isLlaveUnica())
			columnLeftSideWidth = boldFontMetrics.width(labelStr);
		else
			columnLeftSideWidth = fontMetrics.width(labelStr);
		d->m_leftSideWidth = qMax(d->m_leftSideWidth, columnLeftSideWidth);
		count++;
	}
	d->m_leftSideWidth += spaceWidth * 2;
	width = qMax(nameWidth + spaceWidth * 4, d->m_leftSideWidth + maxColumnWidth + spaceWidth * 2);

	qreal nameHeight = height + spaceWidth;
	d->m_outerRect = QRectF(0, 0, width, nameHeight + qMax(0.66, qreal(count)) * fontMetrics.lineSpacing()
		- fontMetrics.leading() + fontMetrics.descent()+3);

	d->m_nameBgRect = QRectF(0, 0, width, nameHeight);
	d->m_namePos = QPointF((width - nameWidth) / 2, fontMetrics.ascent() + spaceWidth / 2);

	d->m_firstColPos = QPointF(d->m_leftSideWidth + spaceWidth * 1 - 1, nameHeight + fontMetrics.ascent() + 2);
	d->m_colPosIncrement = QPointF(0, fontMetrics.lineSpacing());
	update();
	ItemMovible::actualizarBoceto();
}

QList<EntityAttibute *> Entity::obtenerllavesPrimarias(){
	QList<EntityAttibute *> cols;
	foreach(EntityAttibute* cola, d->m_columnList->columns() ){
		if(cola->isPrimaryKey()){
			cols.append(cola);
		}
	}
	return cols;
}
QList<EntityAttibute *> Entity::obtenerllavesForaneas(){
	QList<EntityAttibute *> cols;
	foreach(EntityAttibute* cola, d->m_columnList->columns() ){
		if(cola->isForeignKey()){
			cols.append(cola);
		}
	}
	return cols;
}
int Entity::cantidadRelacionesIdentificativas(){
	int sald =0;
	foreach(EntityAttibute *col,d->m_columnList->columns()){
		if(!col->isForeignKey())
			continue;
		if(col->isPrimaryKey())
			sald++;
	}
	return sald;
}
int Entity::cantidadLlavesPrimarias(){
	return obtenerllavesPrimarias().count();
}

int
Entity::cantidadLlavesForaneas(bool identificativas){
	int a=0;
	foreach(EntityAttibute* cola, d->m_columnList->columns() ){
		if(cola->isForeignKey()){
			if(!(cola->isPrimaryKey()^identificativas))
				a++;
		}
	}
	return a;
}

void Entity::onCambioColumnas(int){
	emit columnaPrimariaModificada();
}
