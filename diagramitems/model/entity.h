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

#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QGraphicsItem>
#include <QPainter>
#include "itemmovible.h"
#include "diagramitems_global.h"
#include "entityattibute.h"
class ColumnasTabla;
class Relacion;
class SelfRelacion;
class DIAGRAMITEMSSHARED_EXPORT Entity : public ItemMovible
{
	Q_OBJECT
	Q_PROPERTY(QString nombre READ nombre WRITE setNombre)
	Q_PROPERTY(QString descripcion READ descripcion WRITE setDescripcion)

public:
	Entity(BaseItem *parent = 0);
	~Entity();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	enum TipoValor {
		Nulo=0,
		NoNulo=1
	};
	QString nombre() const;
	void setNombre(const QString &name);

	QString descripcion() const;
	void setDescripcion(const QString &value);

	void setInitialName(int counter);

	QList<EntityAttibute *> columnas() const;
	ColumnasTabla * listaColumnas();
	int agregarColumna(EntityAttibute *);
	enum { Type = BaseItem::BDEntidad};
	virtual int type() const { return Type; }

	static const char *staticTypeName();
	virtual const char *typeName();
	virtual QString translatedTypeName();
	QList<EntityAttibute *> obtenerllavesPrimarias();
	QList<EntityAttibute *> obtenerllavesForaneas();
	EntityAttibute * findColumn(QString columName);
	EntityAttibute * findColumn(QUuid id);
	int cantidadRelacionesIdentificativas();
	int cantidadLlavesPrimarias();
	int cantidadLlavesForaneas(bool identificativas=false);
	int countColumns();
	static QPen foreignCols;
	static QPen normalCols;
signals:
	void columnaPrimariaModificada();
public slots:
	void actualizarBoceto();
private slots:
	void onCambioColumnas(int);

private:
	class PrivateEntidad;
	PrivateEntidad *d;
};

#endif
