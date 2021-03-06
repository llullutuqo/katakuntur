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

#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>
#include "sqlbase_global.h"

class SQLBASESHARED_EXPORT SQLManager : public QObject
{
	Q_OBJECT
public:
	enum MessageType{Error =0,Warnig =1, Information=2,Success=3 };
	explicit SQLManager(QObject *parent = 0);
	~SQLManager();

	virtual QStringList getDatabases() = 0;
	virtual QStringList getTables(QString dataBase) = 0;
	virtual QStringList getColumns(QString table) = 0;

	virtual bool changeSelectedDataBase(const QString &databasename);

	void setDataBase(const QString &driver,const QString &conectionName, const QString &host, const QString &user,const QString &password,const QString &dataBase,const int &port);
	void setDataBase(QSqlDatabase dataBase);
	QSqlDatabase dataBase() const;

	QStringList executeQueryToList(const QString &query, const int &i=0);
	QSqlQuery executeQuery(const QString &query);
	void executeUpdateQuery(const QString &query);

	static SQLManager * fromDriverName(const QString driver);

	void setEmitMessages(const bool value);
	bool emitMessages();

	bool open();
	bool isOpen() const;
protected:
	class SQLManagerPrivate;
	SQLManagerPrivate *d;
	QSqlQuery &getQuery(const QString &query);
signals:
	void message(const QString message,const MessageType type);
public slots:

};

#endif // SQLMANAGER_H
