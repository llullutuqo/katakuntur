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

#include "sqlmanager.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>

#include <sqlmanagermysql.h>
#include <sqlmanagerpostgres.h>
#include <sqlmanagersqlite.h>
#ifdef QT_DEBUG
#include <QDebug>
#endif
class SQLManager::SQLManagerPrivate{
public:
	SQLManagerPrivate(){
		m_emitMessages = false;
	}
	QSqlDatabase m_dataBase;
	bool m_emitMessages;
};

SQLManager::SQLManager(QObject *parent) :
	QObject(parent),d(new SQLManagerPrivate)
{
}
SQLManager::~SQLManager(){
	d->m_dataBase.close();
	delete d;
}

void SQLManager::setDataBase(const QString &driver,const QString &conectionName, const QString &host, const QString &user,const QString &password,const QString &dataBase,const int &port){
	/*QSqlDatabase::removeDatabase("test");
	QSqlDatabase data = QSqlDatabase::addDatabase(s.dataBaseDriver(),"test");
	data.setHostName(s.server());
	data.setUserName(s.user());
	data.setPassword(s.password());
	if(!s.dataBaseName().isEmpty())
		data.setDatabaseName(s.dataBaseName());
	if(s.port()!=0)
		data.setPort(s.port());
	if(data.open()){
		//d->m_conections.append(s);
		return true;
	}*/
	d->m_dataBase = QSqlDatabase::addDatabase(driver,conectionName);
	d->m_dataBase.setHostName(host);
	d->m_dataBase.setUserName(user);
	d->m_dataBase.setPassword(password);
	d->m_dataBase.setDatabaseName(dataBase);
	if(port!=0)
		d->m_dataBase.setPort(port);
}

void SQLManager::setDataBase(QSqlDatabase dataBase){
	d->m_dataBase = dataBase;
}

QSqlDatabase SQLManager::dataBase() const{
	return d->m_dataBase;
}

QStringList
SQLManager::executeQueryToList(const QString &query,const int &i){
	QStringList lst;
	QSqlQuery qe = getQuery(query);
	while (qe.next()){
		lst.append(qe.value(i).toString());
	}
	qe.finish();
	return lst;
}

void
SQLManager::executeUpdateQuery(const QString &query)
{
	executeQuery(query).finish();
}

QSqlQuery
SQLManager::executeQuery(const QString &query)
{
	QSqlQuery qe= getQuery(query);
	return qe;
}

QSqlQuery&
SQLManager::getQuery(const QString &query){
	QSqlQuery *qe = new QSqlQuery(d->m_dataBase);
	qe->prepare(query);
	qe->exec();
	if(d->m_emitMessages){
		if(qe->lastError().isValid() && d->m_emitMessages)
			emit message(tr("Error al ejecutar sentencia SQL(%1) :%2.").arg(query).arg(qe->lastError().text()),Error);
		else
			emit message(tr("Sentencia ejecutada correctamente."),Success);
	}
	return *qe;
}

SQLManager * SQLManager::fromDriverName(const QString driver)
{
	SQLManager * result = NULL;
	const QString driv = driver.toUpper();
	if(driv.contains("MYSQL"))
		result = new SQLManagerMysql();
	else if(driv.contains("PSQL"))
		result =new SQLManagerPostgres();
	else if(driv.contains("SQLITE"))
		result = new SQLManagerSQLite();
	return result;
}

void SQLManager::setEmitMessages(const bool value)
{
	d->m_emitMessages =value;
}

bool SQLManager::emitMessages()
{
	return d->m_emitMessages;
}

bool SQLManager::changeSelectedDataBase(const QString &databasename)
{
	//if(!databse.driverName().contains("SQLITE")){
	d->m_dataBase.setDatabaseName(databasename);
	if(!d->m_dataBase.open()){
		emit message(tr("No se pudo conectar a la base de datos %1(%2)").arg(databasename,d->m_dataBase.lastError().text()),Error);
		d->m_dataBase.setDatabaseName("");
		d->m_dataBase.open();
		return false;
	}
	return true;
}

bool SQLManager::open()
{
	if(d->m_dataBase.isOpen())
		return true;
	if(!d->m_dataBase.open()){
		emit message(d->m_dataBase.lastError().text(),Error);
#ifdef QT_DEBUG
	qDebug()<<"Error:"<<d->m_dataBase.lastError().text();
#endif
		return false;
	}
	return true;
}

bool SQLManager::isOpen() const
{
	return d->m_dataBase.isOpen();
}
