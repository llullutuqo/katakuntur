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

#ifndef COLUMNLISTMODEL_H
#define COLUMNLISTMODEL_H

#include <QAbstractTableModel>
#include "diagramitems_global.h"
class Attribute;
class Attributes;
class DIAGRAMITEMSSHARED_EXPORT ColumnListModel : public QAbstractTableModel
{
	Q_OBJECT

public:

	enum TipoColumna {
		NombreClumna = 0,
		LLavePrimaria,
		ColumnaRequerida,
		NuloColumna,
		TipoColumna,
		NotasColumna
	};

	ColumnListModel(QObject *parent = 0);
	~ColumnListModel();

	Attributes *columnList() const;// { return m_columnList; }
	void setColumnList(Attributes *columnList);

	// QAbstractItemModel methods
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
	QModelIndex indexFromRow(int i) const;
	QModelIndex moverArriba(int index);
	QModelIndex moverAbajo(int index);
	bool insertRows(int row, int count, const QModelIndex &parent);
protected:
	virtual int firstRow() const { return 0; }

private slots:
	void _beginInsertRows(int index);
	void __endInsertRows();
	void _beginRemoveRows(int index);
	void _endRemoveRows();
	void _dataChanged(int index);

private:
	class ColumnListModelPrivate;
	ColumnListModelPrivate * d;
};

#endif
