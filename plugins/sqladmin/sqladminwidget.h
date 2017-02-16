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

#ifndef SQLADMINWIDGET_H
#define SQLADMINWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include "base.h"

namespace Ui {
	class SQLAdminWidget;
}
class QTreeWidgetItem;
class SQLManager;
class SQLAdminWidget : public QWidget {
	Q_OBJECT
public:
	enum {ItemRole = Qt::UserRole +1 , ReferencialItem = ItemRole +1};
	enum ItemType{ConnectionItem=0,DataBaseItem =1,TableItem=2,ColumnItem=3};
	SQLAdminWidget(QWidget *parent = 0);
	~SQLAdminWidget();
	void saveSize();
	void restoreSize();
	void updateDBList();
signals:
	void sendMessage(QString message,MessageType = Normal);
protected:
	void changeEvent(QEvent *e);

private:
	class SQLSintaxPrivate;
	SQLSintaxPrivate *d;
	friend class SQLManager;
public slots:
	void ejecutarSQL();
	void conectar();
	void addTab();
	void receiveMessage(QString message,MessageType type);
private slots:
	void on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void changeTab(int i);
	void removeTab(int);
	void on_treeWidget_customContextMenuRequested(const QPoint &pos);
	void borrarDatabase();
	void borrarTabla();
	void editarConexion();
};

#endif // SQLADMINWIDGET_H
