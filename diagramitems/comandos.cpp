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

#include "comandos.h"
#include "itemmovible.h"
#include "itemlinea.h"
#include "model/attributes.h"
#include "model/attribute.h"
#include <QGraphicsScene>

//Cambiar Propiedades
ComandoCambiarPropiedad::ComandoCambiarPropiedad(QObject *object, QString nombre,QVariant value,QVariant oldValue ,QUndoCommand * parent)
	:QUndoCommand(parent),propiedad(nombre)
{
	m_object = object;
	if(oldValue.isValid())
		m_antVal = oldValue;
	else
		m_antVal = m_object->property(propiedad.toLatin1());
	m_nueVal = value;
}

bool ComandoCambiarPropiedad::mergeWith(const QUndoCommand *command){
	if(m_nueVal.type()!=QVariant::String)
		return false;
	const ComandoCambiarPropiedad * coma = static_cast<const ComandoCambiarPropiedad *>(command);
	QObject *object = coma->m_object;
	QString prop = coma->propiedad;
	QVariant nueVal = coma->m_nueVal;

	if (m_object != object || prop != propiedad)
		return false;
	if(nueVal.toString().contains(m_nueVal.toString())||m_nueVal.toString().contains(nueVal.toString())){
		m_nueVal = nueVal;
		setText(QObject::trUtf8("Se ha cambiado la propiedad %1: de %2  a %3.").arg(propiedad).arg(m_antVal.toString()).arg(m_nueVal.toString()));
		return true;
	}
	return false;
}

int ComandoCambiarPropiedad::id() const{
	return Id;
}
void ComandoCambiarPropiedad::undo(){
	m_object->setProperty(propiedad.toLatin1(),m_antVal);
	setText(QObject::trUtf8("Se ha cambiado la propiedad %1: de %2  a %3.").arg(propiedad).arg(m_nueVal.toString()).arg(m_antVal.toString()));
}

void ComandoCambiarPropiedad::redo(){
	m_object->setProperty(propiedad.toLatin1(),m_nueVal);
	setText(QObject::trUtf8("Se ha cambiado la propiedad %1: de %2  a %3.").arg(propiedad).arg(m_antVal.toString()).arg(m_nueVal.toString()));
}

//Mover Objetos
ComandoMoverObjeto::ComandoMoverObjeto(ItemMovible *object, const QPointF &oldPos,QUndoCommand *parent)
	: QUndoCommand(parent)
{
	myobjeto = object;
	antPos= oldPos;
	nuevaPos = object->pos();
}

void ComandoMoverObjeto::undo(){
	myobjeto->setPos(antPos);
	myobjeto->scene()->update();
	setText(QObject::trUtf8("Se ha movido el ítem %1 a: (%2, %3).").arg(myobjeto->translatedTypeName()).arg(antPos.x()).arg(antPos.y()));
}

void ComandoMoverObjeto::redo(){
	myobjeto->setPos(nuevaPos);
	setText(QObject::trUtf8("Se ha movido el ítem %1 a: (%2, %3).").arg(myobjeto->translatedTypeName()).arg(nuevaPos.x()).arg(nuevaPos.y()));
}

//Agregar Objetos
ComandoAgregarItem::ComandoAgregarItem(QGraphicsScene *scene,BaseItem *d){
	m_scene = scene;
	item = d;
	setText(QObject::trUtf8("Se ha agregado un ítem (%1).").arg(item->translatedTypeName()));
}

ComandoAgregarItem::~ComandoAgregarItem(){
	//Solo se eliminara el objeto en caso de que la ultima accion
	//haya sido undo()
	if(isUndo && !item->scene())
		delete item;
}

void ComandoAgregarItem::undo(){
	m_scene->removeItem(item);
	m_scene->update();
	isUndo = true;
}

void ComandoAgregarItem::redo(){
	m_scene->addItem(item);
	isUndo = false;
}

//Eliminar Objetos:
ComandoEliminarItems::ComandoEliminarItems(QGraphicsScene * scene,QUndoCommand * parent)
	:QUndoCommand(parent)
{
	m_scene =  scene;
	QList<QGraphicsItem*> it =m_scene->selectedItems();
	QList<ItemMovible*> objects;
	QList<ItemLinea*> lineas;
	// Cargamos todos los items.
	foreach(QGraphicsItem * item, it){
		BaseItem * ditem = qgraphicsitem_cast<BaseItem *>(item);
		if(ditem){
			ditem->setSelected(false);
			m_base.append(ditem);
			ItemMovible * obj = qobject_cast<ItemMovible *>(ditem);
			if(obj){
				objects.append(obj);
			}
		}
	}
	foreach(ItemMovible* obj, objects){
		foreach(ItemLinea * linea, obj->lineasConectadas()){
			m_base.removeOne(linea);
			lineas.append(linea);
		}
	}
	m_lineas = lineas;
	setText(QObject::trUtf8("Se ha eliminado: %1 ítem(s).").arg(m_base.count() + m_lineas.count()));
}

ComandoEliminarItems::~ComandoEliminarItems(){
	if(isRedo){
		foreach(ItemLinea * item , m_lineas){
			if(!item->scene())
				delete item;
		}
		foreach(BaseItem * item , m_base){
			if(!item->scene())
				delete item;
		}
	}
}

void ComandoEliminarItems::undo(){
	foreach(BaseItem * item , m_base){
		m_scene->addItem(item);
	}
	foreach(ItemLinea * item , m_lineas){
		m_scene->addItem(item);
	}
	m_scene->update();
	isRedo=false;
}

void ComandoEliminarItems::redo(){
	foreach(ItemLinea * item , m_lineas){
		m_scene->removeItem(item);
	}
	foreach(BaseItem * item , m_base){
		m_scene->removeItem(item);
	}
	isRedo = true;
}

//Agregar Columnas
ComandoAgregarColumna::ComandoAgregarColumna(Attributes * cols,Attribute * attribute,int index){
	m_attributes = cols;
	m_attibute = attribute;
	if(index<0)
		m_fila = m_attributes->columnCount();
	else
		m_fila= index;
}

void ComandoAgregarColumna::undo(){
	m_attributes->removeColumna(m_attibute);
	setText(QObject::trUtf8("Se ha eliminado un atributo."));
}

void ComandoAgregarColumna::redo(){
	m_attributes->insertColumna(m_fila,m_attibute);
	setText(QObject::trUtf8("Se ha agregado un atributo."));
}

//Eliminar Columnas
ComandoEliminarColumna::ComandoEliminarColumna(Attributes * cols,int fila){
	m_columnas = cols;
	m_columna=m_columnas->column(fila);
	m_fila = fila;

}

void ComandoEliminarColumna::undo(){
	m_columnas->insertColumna(m_fila, m_columna);
	setText(QObject::trUtf8("Se ha agregado un atributo."));
}

void ComandoEliminarColumna::redo(){
	m_columnas->removeColumna(m_columna);
	setText(QObject::trUtf8("Se ha eliminado un atributo."));
}

ComandoIntercambiarColumnas::ComandoIntercambiarColumnas(Attributes * columnas,int anterior, int nueva){
	m_columnas = columnas;
	m_anterior = anterior;
	m_nuevo = nueva;
}

void ComandoIntercambiarColumnas::undo(){
	m_columnas->swapColumnas(m_nuevo,m_anterior);
	setText(QObject::trUtf8("Se ha movido un atributo de la posición %1 a la %2.").arg(m_nuevo).arg(m_anterior));
}

void ComandoIntercambiarColumnas::redo(){
	m_columnas->swapColumnas(m_anterior,m_nuevo);
	setText(QObject::trUtf8("Se ha movido un atributo de la posición %1 a la %2.").arg(m_anterior).arg(m_nuevo));
}
