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

#include "sqladminwidget.h"
#include "ui_sqladminwidget.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <conectiondialog.h>
#include <QDebug>
#include <QTextEdit>
#include <QSettings>
#include "sqlquerrywidget.h"
#include <QMenu>
#include "sqlmanager.h"
#include "connection.h"

class SQLAdminWidget::SQLSintaxPrivate{
public: 
	SQLSintaxPrivate(SQLAdminWidget *baseParent):
			ui(new Ui::SQLAdminWidget)
	{
		ui->setupUi(baseParent);
		baseWidget = baseParent;
		addTab();
		selectedDB = NULL;

		//Context menu
		m_contextMenu = new QMenu("Ejemplo",baseParent);
		QAction *act =m_contextMenu->addAction(tr("Eliminar BD"),baseParent,SLOT(borrarDatabase()));
		act->setData(DataBaseItem);

		act =m_contextMenu->addAction(tr("Eliminar tabla"),baseParent,SLOT(borrarTabla()));
		act->setData(TableItem);

		act =m_contextMenu->addAction(trUtf8("Editar conexión"),baseParent,SLOT(editarConexion()));
		act->setData(ConnectionItem);
	}

	void addTab(){
		SQLQuerryWidget * edit = new SQLQuerryWidget(ui->tabWidget);
		int i = ui->tabWidget->addTab(edit,"SQL-Document");
		ui->tabWidget->setCurrentIndex(i);
		connect(edit,SIGNAL(messageEmited(QString,MessageType)),baseWidget,SLOT(receiveMessage(QString,MessageType)));
		m_baseSqlEdit = edit;
	}

	QTreeWidgetItem * createItem(const QString &text, ItemType type, QTreeWidgetItem *parent =0){
		QTreeWidgetItem * ds;
		if(parent!=NULL)
			ds=  new QTreeWidgetItem(parent, QStringList(text));
		else
			ds = new QTreeWidgetItem(ui->treeWidget, QStringList(text));
		QString icon;
		switch(type){
		case ConnectionItem:
			icon=":/icons/connection-connect.png";
			break;
		case DataBaseItem:
			icon =":/icons/server-database.png";
			break;
		case TableItem:
			icon =":/icons/view-form-table.png";
			break;
		case ColumnItem:
			icon =":/icons/column.png";
			break;
		}
		ds->setIcon(0,QIcon(icon));
		ds->setData(0,ItemRole,type);
		if(parent!=NULL)
			parent->addChild(ds);
		return ds;
	}

	int getIndexFromItem(const QTreeWidgetItem *w){
		return w->data(0,ReferencialItem).toInt();
	}

	/*Obtiene el item base*/
	Connection *  getConnectionFromItem(QTreeWidgetItem * item){
		//QTreeWidgetItem *  w= NULL;
		Connection *c = NULL;
		QTreeWidgetItem * temp = item;
		while(temp->parent()!=NULL){
			temp = temp->parent();
		}
		int index = getIndexFromItem(temp);
		c = m_conections.at(index);
		return c;
	}

	void createConnection(Connection* connection){
		m_conections.append(connection);
		QTreeWidgetItem * it = createItem(connection->name(),ConnectionItem);
		it->setData(0,ReferencialItem,m_conections.count()-1);
		itemData.append(it);
		ui->treeWidget->insertTopLevelItem(0, it);
	}

	void loadDataBases(QTreeWidgetItem * item){
		Connection * c = getConnectionFromItem(item);

		QStringList ls =c->manager()->getDatabases();
		foreach (QString string, ls) {
			itemData.append(createItem(string,DataBaseItem,item));
		}
		selectedDB=NULL;
	}

	void loadTables(QString dataBase,QTreeWidgetItem *item){
		Connection * c = getConnectionFromItem(item);

		QFont f;
		if(selectedDB!=item){
			if(selectedDB){
				f =selectedDB->font(0);
				f.setBold(false);
				selectedDB->setFont(0,f);
			}
			if(!c->manager()->changeSelectedDataBase(dataBase)){
				selectedDB=NULL;
				return;
			}
			selectedDB= item;
		}
		f = item->font(0);
		f.setBold(true);
		item->setFont(0,f);
		int i =0;
		QStringList ls =c->manager()->getTables(dataBase);
		foreach (QString string, ls) {
			QTreeWidgetItem *ite = item->child(i);
			if(ite==NULL){
				ite = createItem(string,TableItem,item);
			}
			else
				ite->setText(0,string);
			i++;
		}
		for(int j =item->childCount();j>=i;j--){
			item->removeChild(item->child(j));
		}
	}
	void loadColumns(QTreeWidgetItem *item){
		Connection * c = getConnectionFromItem(item);

		int i =0;
		QStringList ls =c->manager()->getColumns(item->text(0));
		foreach (QString string, ls) {
			QTreeWidgetItem *ite = item->child(i);
			if(ite==NULL){
				ite = createItem(string,ColumnItem,item);
				if(string.startsWith("PK"))
					ite->setIcon(0,QIcon(":/icons/llavePK.png"));
				else if(string.startsWith("FK"))
					ite->setIcon(0,QIcon(":/icons/llaveFK.png"));
			}
			else
				ite->setText(0,string);
			i++;
		}
		for(int j =item->childCount();j>=i;j--){
			item->removeChild(item->child(j));
		}
	}

	void activarMenus(ItemType tp){
		foreach(QAction *ac, m_contextMenu->actions()){
			if(ac->data()==tp)
				ac->setVisible(true);
			else
				ac->setVisible(false);
		}
	}
	~SQLSintaxPrivate(){
		delete ui;
		//TODO: eliminar todas las conexiones
	}

	QTreeWidgetItem * selectedDB;
	QList<QTreeWidgetItem *> itemData;
	Ui::SQLAdminWidget *ui;
	SQLQuerryWidget *m_baseSqlEdit;
	SQLAdminWidget * baseWidget;
	QMenu * m_contextMenu;
	QTreeWidgetItem *m_selectedItem;
	QList<Connection *> m_conections;
};

SQLAdminWidget::SQLAdminWidget(QWidget *parent) :
	QWidget(parent)
{
	d= new SQLSintaxPrivate(this);
	connect(d->ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(changeTab(int)));
	connect(d->ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(removeTab(int)));
	d->ui->treeWidget->setHeaderLabel(trUtf8("Conexiones"));
}

SQLAdminWidget::~SQLAdminWidget()
{
	delete d;
}

void SQLAdminWidget::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		d->ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void SQLAdminWidget::ejecutarSQL()
{
	/*if(d->m_baseSqlEdit){
		d->m_baseSqlEdit->ejecutarSQL(d->manager->dataBase());
	}*/
}

void SQLAdminWidget::conectar(){
	ConectionDialog dl(this);
	if (dl.exec() ==QDialog::Accepted){
		Connection * s= dl.connection();
		if(s!=NULL)
			d->createConnection(s);
	}
}

void SQLAdminWidget::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column){
	Q_UNUSED(column);
	int it= item->data(0,ItemRole).toInt();
	switch(it){
		case ConnectionItem:
		d->loadDataBases(item);
		break;
	case DataBaseItem:
		d->loadTables(item->text(0),item);
		break;
	case TableItem:
		d->loadColumns(item);
		break;
	}

}
void SQLAdminWidget::addTab(){
	d->addTab();
}
void SQLAdminWidget::changeTab(int i){
	if(i<0){
		d->m_baseSqlEdit =NULL;
		return;
	}
	SQLQuerryWidget * edit = qobject_cast<SQLQuerryWidget*>(d->ui->tabWidget->widget(i));
	d->m_baseSqlEdit=edit;
}
void SQLAdminWidget::removeTab(int i){
	if(d->ui->tabWidget->count()==1)
		return;
	QWidget * di= d->ui->tabWidget->widget(i);
	d->ui->tabWidget->removeTab(i);
	delete di;
}
void SQLAdminWidget::saveSize(){
	QSettings da;
	da.setValue("centralState",d->ui->splitter->saveState());
}

void SQLAdminWidget::restoreSize(){
	QSettings da;
	d->ui->splitter->restoreState(da.value("centralState").toByteArray());
}

void SQLAdminWidget::updateDBList(){
	//d->loadDataBases();
}

void
SQLAdminWidget::receiveMessage(QString message,MessageType type){
	QMessageBox::warning(this,"",message);
	emit sendMessage(message,type);
}

void
SQLAdminWidget::on_treeWidget_customContextMenuRequested(const QPoint &pos){
	QTreeWidgetItem *item =d->ui->treeWidget->itemAt(pos);
	if(item!=NULL){
		int data=item->data(0,ItemRole).toInt();
		d->m_selectedItem = item;
		d->activarMenus(static_cast<ItemType>(data));
		d->m_contextMenu->popup(d->ui->treeWidget->mapToGlobal(pos));
	}
}

void SQLAdminWidget::borrarDatabase()
{
	QString sql ="drop databasse " + d->m_selectedItem->text(0);
//	d->manager->executeUpdateQuery(sql);
}

void SQLAdminWidget::borrarTabla()
{
}

void SQLAdminWidget::editarConexion()
{
	ConectionDialog w(this);
	w.setConnection(d->getConnectionFromItem(d->m_selectedItem));
	w.exec();
}

