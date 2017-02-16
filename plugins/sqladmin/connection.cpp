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

#include <sqlmanager.h>
#include <QTextStream>
#include <QDomDocument>
#include <QSqlDatabase>
#include <QFile>
#include <QDir>
#include "propiedades.h"
#include "connection.h"

class Connection::ConnectionPrivate{
 public:
	ConnectionPrivate(Connection *base){
		m_Connection = base;
		m_manager = NULL;
		m_port =0;
	}
	void connectManager(){
		if(m_manager!=NULL){
			m_manager->disconnect();
			delete m_manager;
		}
		m_manager = SQLManager::fromDriverName(m_dataBaseDriver);
		m_manager->setDataBase(m_dataBaseDriver,m_name,m_serverName,m_userName,m_password,m_dataBaseName,m_port);
	}
	SQLManager * m_manager;
	Connection * m_Connection;
	const static QString fileExtension;
	QSqlDatabase m_database;

	QString m_name;
	QString m_dataBaseName;
	QString m_serverName;
	QString m_userName;
	QString m_password;
	QString m_dataBaseDriver;
	int m_port;

};
const QString Connection::ConnectionPrivate::fileExtension = ".skc";

Connection::Connection(QObject *parent) :
	QObject(parent),d(new ConnectionPrivate(this))
{
}

Connection::~Connection(){
	delete d;
}

void Connection::setName(const QString &name)
{
	d->m_name = name;
}

QString Connection::name() const
{
	return d->m_name;
}

void Connection::setDataBaseDriver(const QString &driver)
{
	d->m_dataBaseDriver = driver;
}

QString Connection::dataBaseDriver() const
{
	return d->m_dataBaseDriver;
}

void Connection::setServer(const QString &server)
{
	d->m_serverName = server;
}

QString Connection::server() const
{
	return d->m_serverName;
}

void Connection::setUser(const QString &usuario)
{
	d->m_userName = usuario;
}

QString Connection::user() const
{
	return d->m_userName;
}

void Connection::setPassword(const QString &password)
{
	d->m_password = password;
}

QString Connection::password() const
{
	return d->m_password;
}

void Connection::setDataBaseName(const QString &name)
{
	d->m_dataBaseName = name;
}

QString Connection::dataBaseName() const
{
	return d->m_dataBaseName;
}

void Connection::setPort(const int &port)
{
	d->m_port = port;
}

int Connection::port() const
{
	return d->m_port;
}

bool Connection::load(const QString &name)
{
	if(exists(name)){
		QString fileName =Propiedades::DataDirectory + name+ Connection::ConnectionPrivate::fileExtension;
		QDomDocument doc("mydocument");
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly))
			return false;
		if (!doc.setContent(&file)) {
			file.close();
			return false;
		}
		file.close();
		QDomElement el = doc.firstChildElement();
		if(el.tagName()=="ConnectionFile"){
			QDomElement es = el.firstChildElement();
			while(!es.isNull()){
				QString tx = es.text();
				QString tag=es.tagName();
				if(tag =="Driver")
					d->m_dataBaseDriver= tx;
				else if (tag=="User")
					d->m_userName = tx;
				else if(tag == "Password")
					d->m_password = tx;
				else if(tag == "Server")
					d->m_serverName = tx;
				else if(tag== "DataBaseName")
					d->m_dataBaseName = tx;
				es = es.nextSiblingElement();
			}
		}
		d->connectManager();

		return true;
	}
	return false;
}

bool Connection::exists(const QString &name) const
{
	return QFile::exists(Propiedades::DataDirectory+ name+ Connection::ConnectionPrivate::fileExtension);
}

void Connection::save()
{

	QDomDocument doc ;
	QDomElement root = doc.createElement("ConnectionFile");
	doc.appendChild(root);

	QDomElement t1 = doc.createElement("Driver");
	t1.appendChild(doc.createTextNode(d->m_dataBaseDriver));
	root.appendChild(t1);

	QDomElement t2 = doc.createElement("User");
	t2.appendChild(doc.createTextNode(d->m_userName));
	root.appendChild(t2);

	QDomElement t3 = doc.createElement("Password");
	t3.appendChild(doc.createTextNode(d->m_password));
	root.appendChild(t3);

	QDomElement t4 = doc.createElement("Server");
	t4.appendChild(doc.createTextNode(d->m_serverName));
	root.appendChild(t4);

	QDomElement t5 =doc.createElement("DataBaseName");
	t5.appendChild(doc.createTextNode(d->m_dataBaseName));
	root.appendChild(t5);

	QFile file(Propiedades::DataDirectory + d->m_name+ Connection::ConnectionPrivate::fileExtension);
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream stream(&file);
		doc.save(stream, 2);
		file.close();
	}
}

QList<Connection *> Connection::getSavedConnections()
{
	QList<Connection *> connections;
	QDir d(Propiedades::DataDirectory);
	QStringList l =d.entryList(QStringList()<<Connection::ConnectionPrivate::fileExtension,QDir::Files);
	foreach(QString file, l){
		Connection * c =loadFromFile(file);
		connections.append(c);
	}
	return connections;
}

Connection * Connection::loadFromFile(const QString &filename)
{
	Connection * c = new Connection();
	QString name = filename;
	name = name.replace(Connection::ConnectionPrivate::fileExtension,"");
	c->load(name);
	return c;
}

SQLManager * Connection::manager()
{
	if(d->m_manager==NULL)
		d->connectManager();
	return d->m_manager;
}

bool Connection::connect()
{
	if(d->m_manager==NULL)
		d->connectManager();
	return d->m_manager->open();
}

bool Connection::isConnected() const
{
	return d->m_manager->isOpen();
}
