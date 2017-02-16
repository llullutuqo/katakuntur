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

#include "attributes.h"
#include "attribute.h"
#include "baseitem.h"

Attributes::Attributes(BaseItem * parent)
	: QObject(parent){
}

BaseItem*
Attributes::itemParent(){
	return qobject_cast<BaseItem *>(parent());
}

int
Attributes::indexOf(Attribute *column) const {
	return m_columns.indexOf(column);
}

Attribute*
Attributes::column(int index) {
	return m_columns[index];
}

int
Attributes::columnCount() const {
	return m_columns.size();
}

int
Attributes::insertColumna(int index, Attribute *column){
	Q_ASSERT(m_columns.contains(column) == false);
	column->setParent(this);
	connect(column, SIGNAL(propertyChanged(const QString &, const QVariant &)), this, SLOT(updateColumn(const QString &, const QVariant &)));
	emit columnaAboutToBeInserted(index);
	m_columns.insert(index, column);
	emit columnaInserted(index);
	return index;
}

bool
Attributes::contains(Attribute *column){
	return m_columns.contains(column);
}

int
Attributes::appendColumna(Attribute *column){
	return insertColumna(columnCount(), column);
}

int
Attributes::removeColumna(Attribute *column){
	int index = m_columns.indexOf(column);
	Q_ASSERT(index >=0);
	removeColumna(index);
	return index;
}

Attribute*
Attributes::removeColumna(int index){
	emit columnaAboutToBeRemoved(index);
	Attribute *column = m_columns.takeAt(index);
	emit columnaRemoved(index);
	return column;
}

void
Attributes::swapColumnas(int oldIndex, int newIndex){
	m_columns.move(oldIndex, newIndex);
	emit columnaChanged(oldIndex);
	emit columnaChanged(newIndex);
}

void
Attributes::updateColumn(const QString &, const QVariant &){
	Attribute *column = qobject_cast<Attribute *>(sender());
	int index = m_columns.indexOf(column);
	Q_ASSERT(index != -1);
	emit columnaChanged(index);
}

const
QMetaObject * Attributes::metaColumnObject(){
	return new QMetaObject();
}

int
Attributes::countIneditableColumns(EditableColumn editable){
	int count=0;
	switch(editable){
	case  Firsts:
		for(int i =0;i<m_columns.count(); i++){
			if(m_columns.at(i)->isEditable())
				break;
			count++;
		}
		break;
	case Lasts:
		for(int i =m_columns.count()-1;i>=0; i--){
			if(m_columns.at(i)->isEditable())
				break;
			count++;
		}
		break;
	default:
		break;
	}
	return count;
}
