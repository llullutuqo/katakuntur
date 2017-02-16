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

#include "columnlistmodel.h"
#include "pantalladiagrama.h"
#include "comandos.h"
#include "model/attribute.h"
#include "model/attributes.h"
#include <QMap>
#include <QVariant>
#include "baseitem.h"

struct MetaAtributes{
	QString propertyName;
	QVariant::Type type;
	Qt::ItemFlags flags;
};
class ColumnListModel::ColumnListModelPrivate{
public:
	ColumnListModelPrivate(){
		m_columnList = NULL;
		colCount =0;
	}

	~ColumnListModelPrivate(){
		delete m_columnList;
	}

	void __updateModel(const QMetaObject *ob){
		if(classname==ob->className())
			return;
		classname =ob->className();
		cols.clear();
		for (int i =1; i<ob->propertyCount();i++){
			QMetaProperty met =ob->property(i);
			MetaAtributes c;
			c.propertyName = met.name();
			c.type =met.type();
			if(met.type() == QVariant::String || met.type()==QVariant::Int){
				c.flags =Qt::ItemIsSelectable | Qt::ItemIsEnabled;
				if(met.isWritable())
					c.flags =c.flags |Qt::ItemIsEditable;
			}
			else if(met.type() == QVariant::Bool){
				c.flags =Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
			}

			cols.insert(cols.count(),c);
		}
		colCount = cols.count();

	}

	Attributes *m_columnList;
	QMap<int , MetaAtributes> cols;
	QMap<int, QVariant> headers;
	QString classname;
	int colCount;
};

ColumnListModel::ColumnListModel(QObject *parent)
	: QAbstractTableModel(parent), d(new ColumnListModelPrivate())
{
}

ColumnListModel::~ColumnListModel(){
	delete d;
}

Attributes *ColumnListModel::columnList() const {
	return d->m_columnList;
}
void
ColumnListModel::setColumnList(Attributes *columnList)
{
	if (d->m_columnList) {
		disconnect(d->m_columnList, 0, this, 0);
	}
	d->m_columnList = columnList;
	beginResetModel();
	if (d->m_columnList) {
		d->__updateModel(columnList->metaColumnObject());
		connect(d->m_columnList, SIGNAL(columnaAboutToBeInserted(int)), this, SLOT(_beginInsertRows(int)));
		connect(d->m_columnList, SIGNAL(columnaInserted(int)), this, SLOT(__endInsertRows()));
		connect(d->m_columnList, SIGNAL(columnaAboutToBeRemoved(int)), this, SLOT(_beginRemoveRows(int)));
		connect(d->m_columnList, SIGNAL(columnaRemoved(int)), this, SLOT(_endRemoveRows()));
		connect(d->m_columnList, SIGNAL(columnaChanged(int)), this, SLOT(_dataChanged(int)));
		emit endResetModel();
	}
	else
		d->classname ="";
	endResetModel();
}
bool ColumnListModel::removeRows(int row, int count, const QModelIndex &parent){
	Q_UNUSED(count)
	Q_UNUSED(parent)
	int tam =d->m_columnList->columnCount();
	if(tam < row+1)
		return false;
	if(!d->m_columnList->column(row)->isEditable())
		return false;
	d->m_columnList->itemParent()->diagrama()->undoStack()->push(new ComandoEliminarColumna(d->m_columnList, row));
	return true;
}
QModelIndex ColumnListModel::moverArriba(int index){
	int i = index;
	if (i > 0) {
		if(i<d->m_columnList->columnCount()){
			d->m_columnList->itemParent()->diagrama()->undoStack()->push(new ComandoIntercambiarColumnas(columnList(), i, i - 1));
			return indexFromRow(i - 1);
		}
	}
	return QModelIndex();
}
QModelIndex ColumnListModel::moverAbajo(int index){
	int i = index;
	if (i + 1 < columnList()->columnCount()) {
		d->m_columnList->itemParent()->diagrama()->undoStack()->push(new ComandoIntercambiarColumnas(columnList(), i, i + 1));
		return indexFromRow(i + 1);
	}
	return QModelIndex();
}
int
ColumnListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return d->m_columnList ? d->m_columnList->columnCount() : 0;
}

int
ColumnListModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	return d->colCount;
}

QVariant
ColumnListModel::data(const QModelIndex &index, int role) const
{
	if (d->m_columnList) {
		Q_ASSERT(index.isValid());
		int row = index.row() - firstRow();
		if (row < 0 || row >= d->m_columnList->columnCount()) {
			return QVariant();
		}
		Attribute *column = d->m_columnList->column(row);
		MetaAtributes at = d->cols.value(index.column());
		if(at.type==QVariant::String || at.type== QVariant::Int){
			if (role == Qt::DisplayRole || role == Qt::EditRole) {
				return column->property(at.propertyName.toLatin1());
			}
		}
		else if(at.type==QVariant::Bool){
			if(role == Qt::CheckStateRole){
				return column->property(at.propertyName.toLatin1()).toBool() ? Qt::Checked : Qt::Unchecked;
			}
		}
	}
	return QVariant();
}

bool
ColumnListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (d->m_columnList) {
		Q_ASSERT(index.isValid());
		Attribute *column;
		int row = index.row() - firstRow();
		column = d->m_columnList->column(row);
		MetaAtributes at = d->cols.value(index.column());
		if(at.type == QVariant::String){
			if (role == Qt::DisplayRole || role == Qt::EditRole) {
				const QString val = value.toString().trimmed();
				if(val != column->property(at.propertyName.toLatin1()))
					d->m_columnList->itemParent()->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(column, at.propertyName, val));
				else
					return false;
				return true;
			}
		}
		else if(at.type == QVariant::Int){
			if (role == Qt::DisplayRole || role == Qt::EditRole) {
				const int val = value.toInt();
				if(val != column->property(at.propertyName.toLatin1()))
					d->m_columnList->itemParent()->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(column, at.propertyName, val));
				else
					return false;
				return true;
			}
		}
		else if(at.type==QVariant::Bool){
			if(role == Qt::CheckStateRole){
				bool checked = value.toInt() == Qt::Checked ? true : false;
				d->m_columnList->itemParent()->diagrama()->undoStack()->push(new ComandoCambiarPropiedad(column, at.propertyName, checked));
				return true;
			}
		}
	}
	return false;
}

Qt::ItemFlags
ColumnListModel::flags(const QModelIndex &index) const
{
	Q_ASSERT(index.isValid());
	if(d->cols.contains(index.column())){
		// Si la columna no es editable
		// Se evita la edidcion del item
		if(index.row()< d->m_columnList->columnCount()){
			if(d->m_columnList->column(index.row())->isEditable())
				return d->cols.value(index.column()).flags;
		}
	}
	return Qt::NoItemFlags;
}

QVariant
ColumnListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		if(d->headers.contains(section))
			return d->headers.value(section);
		else
			return d->cols.value(section).propertyName;
	}
	return QVariant();
}

bool
ColumnListModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		d->headers.insert(section,value);
		return true;
	}
	return false;
}

QModelIndex
ColumnListModel::indexFromRow(int i) const
{
	return createIndex(firstRow() + i, 0);
}

void
ColumnListModel::_beginInsertRows(int index)
{
	beginInsertRows(QModelIndex(), firstRow() + index, firstRow() + index);
}

void
ColumnListModel::__endInsertRows()
{
	endInsertRows();
}

void
ColumnListModel::_beginRemoveRows(int index)
{
	beginRemoveRows(QModelIndex(), firstRow() + index, firstRow() + index);
}

void
ColumnListModel::_endRemoveRows()
{
	endRemoveRows();
}

void
ColumnListModel::_dataChanged(int index)
{
	emit dataChanged(createIndex(firstRow() + index, 0), createIndex(firstRow() + index, d->colCount));
}

bool
ColumnListModel::insertRows(int row, int count, const QModelIndex &parent){
	Q_UNUSED(count);
	Q_UNUSED(parent);
	Attribute *column = d->m_columnList->createColumn();
	column->setProperty("nombre",QVariant(tr("nuevo_Atributo")));
	d->m_columnList->itemParent()->diagrama()->undoStack()->push(new ComandoAgregarColumna(d->m_columnList, column,row));
	return true;
}
