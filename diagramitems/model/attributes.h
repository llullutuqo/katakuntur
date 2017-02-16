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

#ifndef COLUMNAS_H
#define COLUMNAS_H
#include <QObject>
#include "diagramitems_global.h"

class BaseItem;
class Attribute;
class  DIAGRAMITEMSSHARED_EXPORT  Attributes : public QObject
{
	Q_OBJECT

public:
	enum EditableColumn{
		Firsts =0,
		Lasts =1,
		Both = 2,
	};

	Attributes(BaseItem *table);
	BaseItem * itemParent();
	virtual int columnCount() const;
	virtual int indexOf(Attribute *column) const;
	virtual Attribute *column(int index);
	virtual int insertColumna(int, Attribute *);
	virtual int appendColumna(Attribute *);
	virtual void swapColumnas(int, int);
	virtual int removeColumna(Attribute *);
	virtual bool contains(Attribute *);
	virtual Attribute *removeColumna(int);
	//TODO: convertir en funcion virtual pura
	virtual const QMetaObject * metaColumnObject();
	//TODO: COnvertir a virtual
	virtual Attribute * createColumn(){return NULL;}
	int countIneditableColumns(EditableColumn editable);
signals:
	void columnaAboutToBeInserted(int index);
	void columnaInserted(int index);
	void columnaAboutToBeRemoved(int index);
	void columnaRemoved(int index);
	void columnaChanged(int index);
protected slots:
	virtual void updateColumn(const QString &name, const QVariant&);
protected:
	QList<Attribute *> m_columns;
};

#endif // COLUMNAS_H
