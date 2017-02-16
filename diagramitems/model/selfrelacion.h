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

#ifndef SELFRELACION_H
#define SELFRELACION_H
#include "itemlinea.h"
class Entity;
class DIAGRAMITEMSSHARED_EXPORT  SelfRelacion : public ItemLinea
{
	Q_OBJECT
	Q_PROPERTY(QString labelInicio READ labelInicio WRITE setLabelInicio)
	Q_PROPERTY(QString labelFinal READ labelFinal WRITE setLabelFinal)
public:
	SelfRelacion(BaseItem *parent = 0);
	~SelfRelacion();
	QString labelInicio();
	void setLabelInicio(QString value);
	enum Tipo { Uno=0,Muchos=1};
	QString labelFinal();
	void setLabelFinal(QString value);
	static const char *staticTypeName();
	virtual const char *typeName();
	virtual QString translatedTypeName();
	Entity * ownerTabla();
	virtual void setInicioObject(ItemMovible *);
	virtual void setFinalObject(ItemMovible *);
	void repintado(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual void removedFromScene();
	virtual void addedToScene();
	void actualizarBoceto();
private slots:
	void onUpdateTabla();
	void onPropertyTabla(const QString, const QVariant);
private:
	void updateLabels();
	class PrivateSelfRelacion;
	PrivateSelfRelacion *d;
};

#endif // SELFRELACION_H
