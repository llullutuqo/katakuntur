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

#include "sqlmanagermysql.h"
#include <QSqlQuery>
#include <QVariant>

SQLManagerMysql::SQLManagerMysql()
{
}
QStringList SQLManagerMysql::getDatabases(){
	return executeQueryToList("show databases;");
}

QStringList SQLManagerMysql::getTables(QString dataBase){
	return executeQueryToList("show tables from "+dataBase+";");
}

QStringList SQLManagerMysql::getColumns(QString table){
	QStringList ls;
	QSqlQuery q;
	q= getQuery("show columns from "+ table);

	while(q.next()){
		QString sald;
		QString a = q.value(3).toString();
		if(a.contains("PRI"))
			sald = "PK ";
		else if(a.contains("MU"))
			sald = "FK ";
		sald += "["+q.value(0).toString()+"]";
		sald += " " + q.value(1).toString();
		ls.append(sald);
	}
	q.finish();
	return ls;
}
