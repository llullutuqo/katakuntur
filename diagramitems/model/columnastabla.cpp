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

#include "columnastabla.h"
#include "entityattibute.h"
#include "entity.h"
ColumnasTabla::ColumnasTabla(Entity *parent):Attributes(parent)
{
}

QList<EntityAttibute *> ColumnasTabla::columns() const{
	QList<EntityAttibute *> arr;
	foreach (Attribute *var, m_columns) {
		EntityAttibute *col = qobject_cast<EntityAttibute *>(var);
		if(col)
			arr.append(col);
	}
	return arr;
}
const QMetaObject * ColumnasTabla::metaColumnObject(){
	Entity * ta =qobject_cast<Entity *>(parent());
	EntityAttibute col(ta);
	return col.metaObject();
}
Attribute * ColumnasTabla::createColumn(){
	Entity * ta =qobject_cast<Entity *>(parent());
	return new EntityAttibute(ta);
}
int ColumnasTabla::insertColumna(int value, Attribute *col){
	int d = Attributes::insertColumna(value,col);
	EntityAttibute *cl = qobject_cast<EntityAttibute *>(col);

	if(cl->isPrimaryKey())
		emit columnaPrimariaChanged(value);
	return d;
}

Attribute *ColumnasTabla::removeColumna(int value){
	EntityAttibute *column = qobject_cast<EntityAttibute*>(Attributes::removeColumna(value));
		int index = m_columns.indexOf(column);
	if(column->isPrimaryKey())
		emit columnaPrimariaChanged(index);
	return column;
}
int ColumnasTabla::removeColumna(Attribute *value){
	return Attributes::removeColumna(value);
}

//insertColumna
//TODO: verificar si no afecta la velocidad
//if(column->isLlavePrimaria())
//	emit columnaPrimariaChanged(index);
//return index;

//Columnas::updateColumn(const QString &cadena, const QVariant &)
//	//TODO: verificar si no afecta la velocidad
//	if(cadena=="llavePrimaria"||(column->isLlavePrimaria() && cadena=="tipoDato"))
//		emit columnaPrimariaChanged(index);

//Columnas::removeColumna(int index)
//	if(column->isLlavePrimaria())
//		emit columnaPrimariaChanged(index);
//	disconnect(column, 0, this, 0);
//	column->setParent(0);
void
ColumnasTabla::updateColumn(const QString &cadena, const QVariant &var)
{
	EntityAttibute *column = qobject_cast<EntityAttibute *>(sender());
	int index = m_columns.indexOf(column);
	Attributes::updateColumn(cadena,var);
	//TODO: verificar si no afecta la velocidad
	//Se agrega la verificacion cuando se modifica el tipo de Dato.
	if(cadena=="llavePrimaria"||(column->isPrimaryKey() && (cadena=="nombre"||cadena=="tipoDato")))
		emit columnaPrimariaChanged(index);
}
