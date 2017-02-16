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

#ifndef COMANDOS_H
#define COMANDOS_H
#include <QUndoCommand>
#include <QPointF>
#include <QVariant>
#include "diagramitems_global.h"
class ItemMovible;
class QGraphicsScene;
class BaseItem;
class ItemLinea;
class Attributes;
class Attribute;
class QVariant;
class DIAGRAMITEMSSHARED_EXPORT ComandoCambiarPropiedad:public QUndoCommand{
public:
	enum { Id = 120 };
	ComandoCambiarPropiedad(QObject *object,  QString nombre,QVariant value,QVariant oldValue = QVariant(),QUndoCommand * parent=0);
	bool mergeWith(const QUndoCommand *command);
	int id() const;
	void undo();
	void redo();
private:
	QObject * m_object;
	QVariant m_antVal;
	QVariant m_nueVal;
	QString propiedad;
};

class ComandoMoverObjeto : public QUndoCommand
{
public:
	ComandoMoverObjeto(ItemMovible *object, const QPointF &oldPos,QUndoCommand *parent = 0);
	void undo();
	void redo();

private:
	ItemMovible *myobjeto;
	QPointF antPos;
	QPointF nuevaPos;
};

class ComandoAgregarItem : public QUndoCommand
{
public:
	ComandoAgregarItem(QGraphicsScene *scene, BaseItem *item);
	~ComandoAgregarItem();
	void undo();
	void redo();
private:
	QGraphicsScene * m_scene;
	BaseItem *item;
	bool isUndo;
};

class ComandoEliminarItems : public QUndoCommand
{
public:
	ComandoEliminarItems(QGraphicsScene * scene,QUndoCommand *parent=0);
	~ComandoEliminarItems();
	void undo();
	void redo();
private:
	QList<BaseItem *> m_base;
	QList<ItemLinea *> m_lineas;
	QGraphicsScene * m_scene;
	bool isRedo;
};

class ComandoAgregarColumna : public QUndoCommand
{
public:
	ComandoAgregarColumna(Attributes * cols,Attribute * Attribute,int index=-1);
	void undo();
	void redo();
private:
	Attributes *m_attributes;
	Attribute *m_attibute;
	int m_fila;
};

class ComandoEliminarColumna : public QUndoCommand
{
public:
	ComandoEliminarColumna(Attributes * cols,int fila);
	void undo();
	void redo();
private:
	Attributes *m_columnas;
	Attribute *m_columna;
	int m_fila;
};

class ComandoIntercambiarColumnas : public QUndoCommand
{
public:
	ComandoIntercambiarColumnas(Attributes * columnas,int anterior, int nueva);
	void undo();
	void redo();
private:
	Attributes * m_columnas;
	int m_anterior;
	int m_nuevo;
};

#endif // COMANDOS_H
