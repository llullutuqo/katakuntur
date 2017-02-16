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

#ifndef ITEMLINEA_H
#define ITEMLINEA_H

#include "baseitem.h"
#include "conector.h"

class ItemLinea : public BaseItem
{
	Q_OBJECT
public:
	enum UbicacionConector{StartConector =0,EndConector = 1};
	ItemLinea(BaseItem *parent =0);
	~ItemLinea();

	Conector * conector(UbicacionConector ubicacion) const;
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual void actualizarBoceto()=0;
	QPolygonF linePoints() const;
	void setLinePoints(const QPolygonF &primerTrazo,const QPolygonF &segundoTrazo);
	Qt::PenStyle lineStyle() const;
	void setLineStyle(Qt::PenStyle style);
	QPainterPath startArrow() const;
	void setStartArrow(const QPainterPath &path);
	bool fillStartArrow() const;
	void setFillStartArrow(bool fill);
	QPainterPath endArrow() const;
	void setEndArrow(const QPainterPath &path);
	bool fillEndArrow() const;
	void setFillEndArrow(bool fill);
	virtual void setInicioObject(ItemMovible *);
	virtual void setFinalObject(ItemMovible *);
	virtual void connectToHubs();
	virtual void disconectFromHubs();
	virtual void removedFromScene();
	virtual void addedToScene();
	void updateObjects();
	void setInicioOwner(QString value);
	void setFinalOwner(QString value);
	void updateCache();
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	virtual void repintado(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
	Conector *m_connectors[2];
	QPolygonF m_primerTrazo;
	QPolygonF m_segundoTrazo;
	Qt::PenStyle m_lineStyle;
	QPainterPath m_shape;
	bool m_fillStartArrow;
	QPainterPath m_startArrow;
	QPainterPath m_transformedStartArrow;
	QPainterPath m_endArrow;
	QPainterPath m_transformedEndArrow;
	QString m_ownerInicio;
	QString m_ownerFinal;
	bool m_fillEndArrow;
	bool m_dirty;
};

#endif // ITEMLINEA_H
