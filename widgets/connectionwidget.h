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

#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QDialog>
#include "widgets_global.h"

namespace Ui {
	class ConnectionWidget;
}

class WIDGETSSHARED_EXPORT ConnectionWidget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(bool saveDatabase READ isSaveForDatabase WRITE setSaveForDatabase)

public:
	enum TypeDBMS{Host=0, File=1,ODBC=2};

	explicit ConnectionWidget(QWidget *parent = 0);
	~ConnectionWidget();

	void setSaveForDatabase(bool value);
	bool isSaveForDatabase();
	void setTypeDBMS(TypeDBMS value);
	TypeDBMS typeDBMS();
	void setDriver(const QString &driver);
	QString driver();
	void setServer(QString );
	QString server();
	void setPassword(QString );
	QString password();
	void setUser(QString );
	QString user();
	void setDataBase(QString );
	QString dataBase();
	void setPort(int );
	int port();

	void setAvailableDrivers(QStringList values);
private slots:
	void changeView(const TypeDBMS value);
	void on_pbExp_clicked();
	void on_cmbDrivers_currentIndexChanged(QString );

private:
	class ConnectionWidgetPrivate;
	ConnectionWidgetPrivate *d;
	Ui::ConnectionWidget *ui;
};

#endif // CONNECTIONWIDGET_H
