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

#ifndef BASEITEM_H
#define BASEITEM_H

#include <QGraphicsItem>
#include <QObject>
#include "xmlbase.h"
#include "diagramitems_global.h"

class PantallaDiagrama;
class QUuid;
class DIAGRAMITEMSSHARED_EXPORT BaseItem :public QObject, public QGraphicsItem,public XmlBase
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	explicit BaseItem(BaseItem *parent = 0);
	~BaseItem();

	enum {
		BDEntidad = UserType + 1,
		BDRelacion = UserType + 2,
		BDVista= UserType + 3
	};
	PantallaDiagrama *diagrama() const;

	static const char *staticTypeName() { return ""; }
	virtual const char *typeName() = 0;

	virtual QString translatedTypeName()=0;
	bool isVistaLogica();
	bool isVistaFisica();

signals:
	void propiedadCambiada(const QString &name, const QVariant &value);
};

#endif // BASEITEM_H
