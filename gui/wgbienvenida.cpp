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

#include "wgbienvenida.h"
#include "ui_wgbienvenida.h"
#include <QSettings>
#include <QFileInfo>
#include <QStyledItemDelegate>
#include <QDesktopServices>
#include <QUrl>
#include <QtConcurrentRun>
#include <QDomDocument>
#include <QDomElement>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

class ItemDelegate : public QStyledItemDelegate
{
public:
	ItemDelegate(QObject* parent = 0) : QStyledItemDelegate(parent)
	{
		QListWidget* myWidget = qobject_cast<QListWidget *>(parent);
		if(myWidget!=NULL)
			myWidget->viewport()->setAttribute(Qt::WA_Hover);
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		Q_ASSERT(index.isValid());

		QStyleOptionViewItemV4 opt = option;
		initStyleOption(&opt, index);

		if (opt.state & QStyle::State_MouseOver)
		{
			//opt.icon = opt.icon.pixmap(opt.decorationSize, QIcon::Active); // <--
			QListWidget* myWidget = qobject_cast<QListWidget *>(parent());
			QPoint globalCursorPos = QCursor::pos();
			QPoint viewportPos = myWidget->viewport()->mapFromGlobal(globalCursorPos);
			QListWidgetItem * currentIndex = myWidget->itemAt(viewportPos);
			if (currentIndex !=NULL)
				QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
		}
		else
			QApplication::restoreOverrideCursor();
		QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, 0);
	}
};

WgBienvenida::WgBienvenida(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WgBienvenida)
{
	ui->setupUi(this);
	ui->lstAcciones->setItemDelegate(new ItemDelegate(ui->lstAcciones));
	ui->lstNews->setItemDelegate(new ItemDelegate(ui->lstNews));
	QListWidgetItem *mg;
	mg = new QListWidgetItem(QIcon("://icons/images/document-open.png"),trUtf8("Abrir diagrama"),ui->lstAcciones);
	mg->setData(Role,0);
	mg = new QListWidgetItem(QIcon("://icons/images/document-new.png"),trUtf8("Nuevo diagrama"),ui->lstAcciones);
	mg->setData(Role,1);
	mg=new QListWidgetItem(QIcon("://icons/images/facebook_icon.png"),trUtf8("¡Hazte fan en facebook!"),ui->lstAcciones);
	mg->setData(Role,2);
	mg=new QListWidgetItem(QIcon("://icons/images/donate.png"),trUtf8("Apoyar a Kata Kuntur"),ui->lstAcciones);
	mg->setData(Role,3);
	qnam = new QNetworkAccessManager(this);
	loadNews();
	connect(this,SIGNAL(urlOpened()),this,SLOT(onUrlOpened()));
}

WgBienvenida::~WgBienvenida()
{
	delete ui;
}

void WgBienvenida::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui->retranslateUi(this);
	}
	else
		QWidget::changeEvent(event);
}

void WgBienvenida::loadRecents()
{

	QSettings settings;
	QStringList files = settings.value("ficherosRecientes").toStringList();
	ui->lstRecientes->clear();
	foreach(QString file, files){
		QFileInfo d(file);
		QListWidgetItem *wg =new QListWidgetItem(QIcon("://images/application-x-diagram-kkd.png"),d.fileName(), ui->lstRecientes);
		if(!QFile::exists(file)){
			wg->setFlags(Qt::NoItemFlags|Qt::ItemIsSelectable);
			file = trUtf8("(No disponible) %1").arg(file) ;
		}
		wg->setToolTip(file);
	}
}

void WgBienvenida::openURL(const QUrl &url)
{
	QDesktopServices::openUrl(QUrl(url));
	emit urlOpened();
}

void WgBienvenida::loadNews()
{
	connect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finalizado(QNetworkReply*)) );
	qnam->get(QNetworkRequest(QUrl("http://utils.jeanmazuelos.com/rssreader/readerxml.php?feed=http%3A%2F%2Fjeanmazuelos.com%2Fes%2Ftaxonomy%2Fterm%2F126%2Ffeed")));
}

void WgBienvenida::on_lstRecientes_itemDoubleClicked(QListWidgetItem *item)
{
	emit onOpenRequest(item->toolTip());
}

void WgBienvenida::on_lstAcciones_itemClicked(QListWidgetItem *item)
{
	int rol= item->data(Role).toInt();
	switch(rol){
	case 0:
		emit onOpenRequest("com:open");
		break;
	case 1:
		emit onNewRequest();
		break;
	case 2:
		QDesktopServices::openUrl(QUrl("http://www.facebook.com/KataKuntur"));
	case 3:
		QDesktopServices::openUrl(QUrl("http://katakuntur.jeanmazuelos.com/donate"));
		break;
	}
}

void WgBienvenida::onUrlOpened()
{
	setCursor(Qt::ArrowCursor);
}

void WgBienvenida::on_lstNews_itemPressed(QListWidgetItem *item)
{
	QUrl url(item->data(WgBienvenida::Role).toString());
	setCursor(Qt::WaitCursor);
	QtConcurrent::run(this,&WgBienvenida::openURL,url);
}

void WgBienvenida::finalizado(QNetworkReply *reply)
{
	QDomDocument doc;
	if(doc.setContent(reply->readAll())){
		QDomElement base=doc.firstChildElement("news");
		QDomElement element = base.firstChildElement("new");
		for (; !element.isNull(); element = element.nextSiblingElement("new")) {
			QListWidgetItem * item = new QListWidgetItem(QIcon("://icons/images/rss.png"), element.text(),ui->lstNews);
			item->setData(WgBienvenida::Role,element.attribute("href"));
			ui->lstNews->addItem(item);
#ifdef QT_DEBUG
			qDebug()<<element.text()<<":"<<element.attribute("href");
#endif
		}
	}
}
