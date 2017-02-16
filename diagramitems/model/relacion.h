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

#ifndef RELACION_H
#define RELACION_H

#include <QPair>
#include <QGraphicsItem>
#include <QPainter>
#include "itemlinea.h"
#include "../conector.h"
#include <QGraphicsTextItem>
#include "diagramitems_global.h"
class Entity;
class DIAGRAMITEMSSHARED_EXPORT Relacion : public ItemLinea
{
	Q_OBJECT
	Q_ENUMS(Cardinalidad CardinalidadLinea Action)
	Q_PROPERTY(Cardinalidad cardinalidadInicio READ cardinalidadInicio WRITE setCardinalidadInicio)
	Q_PROPERTY(Cardinalidad cardinalidadFinal READ cardinalidadFinal WRITE setCardinalidadFinal)
	Q_PROPERTY(QString labelInicio READ labelInicio WRITE setLabelInicio)
	Q_PROPERTY(QString labelFinal READ labelFinal WRITE setLabelFinal)
	Q_PROPERTY(CardinalidadLinea cardinalidadLineaInicio READ cardinalidadLineaInicio WRITE setCardinalidadLineaInicio)
	Q_PROPERTY(CardinalidadLinea cardinalidadLineaFinal READ cardinalidadLineaFinal WRITE setCardinalidadLineaFinal)

public:
	Relacion(BaseItem *parent = 0);
	~Relacion();
	enum {
		Type = BaseItem::BDRelacion
	};
	enum Cardinalidad {
		Uno=0,
		Muchos=1,
		Identificativo=2,
		UnoIdentificativo=3
	};
	enum CardinalidadLinea { Debe=0,Puede=1};
	virtual int type() const { return Type; }

	static const char *staticTypeName();
	virtual const char *typeName();
	virtual QString translatedTypeName();
	void repintado(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	bool isIdentifying() const;

	QString labelInicio();
	void setLabelInicio(QString value);

	QString labelFinal();
	void setLabelFinal(QString value);

	Cardinalidad cardinalidadInicio() const;
	void setCardinalidadInicio(Cardinalidad card);

	Cardinalidad cardinalidadFinal() const;
	void setCardinalidadFinal(Cardinalidad card);

	CardinalidadLinea cardinalidadLineaInicio() const;
	void setCardinalidadLineaInicio(CardinalidadLinea);
	CardinalidadLinea cardinalidadLineaFinal() const;
	void setCardinalidadLineaFinal(CardinalidadLinea);
	Entity * tablaInicio();
	Entity * tablaFinal();

	enum Action {
		NoAction,
		Restrict,
		Cascade,
		SetNull,
		SetDefault
	};

	Action onUpdateAction() const;

	Action onDeleteAction() const;

	virtual void setInicioObject(ItemMovible *);
	virtual void setFinalObject(ItemMovible *);
	bool hasEmptyLabels();
	virtual QString tipoLinea();
	virtual void removedFromScene();
	virtual void addedToScene();
	void actualizarBoceto();
public slots:
	void updateInicio();
	void updateFinal();
	void updateLineaFinal();
	void updateLineaInicio();
	void updateForeignKeys();
private slots:
	void onUpdateTablaInicio();
	void onUpdateTablaFinal();
	void onPropertyTablaInicio(const QString, const QVariant);
	void onPropertyTablaFinal(const QString, const QVariant);
private:
	void updateLabels();
	class PrivateRelacion;
	PrivateRelacion *const d;
};

#endif
