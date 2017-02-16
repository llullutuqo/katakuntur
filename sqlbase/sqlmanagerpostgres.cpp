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

#include "sqlmanagerpostgres.h"
#include <QSqlQuery>
#include <QVariant>

SQLManagerPostgres::SQLManagerPostgres()
{
}
QStringList SQLManagerPostgres::getDatabases(){
	return executeQueryToList("SELECT datname FROM pg_database;");
}

QStringList SQLManagerPostgres::getTables(QString ){
	return executeQueryToList("select tablename from pg_tables where schemaname LIKE 'public'");// "+dataBase+";");
}

QStringList SQLManagerPostgres::getColumns(QString table){
	QStringList ls;
	QSqlQuery q;
	q= getQuery("SELECT column_name,udt_name,character_maximum_length FROM information_schema.columns WHERE table_name ='"+table+"'");
	while(q.next()){
		QString sald;
		QString a;// = q.value(3).toString();
		if(a.contains("PRI"))
			sald = "PK ";
		else if(a.contains("MU"))
			sald = "FK ";
		sald += "["+q.value(0).toString()+"]";
		sald += " " + q.value(1).toString();
		QString aa=q.value(2).toString();
		if(!aa.isEmpty() && aa!="0")
			sald += "(" +aa+")";
		ls.append(sald);
	}
	QSqlQuery pk = getQuery("select constraint_name from information_schema.table_constraints where table_name ='"+table+"' and constraint_type LIKE 'PRI%'");
	while(pk.next()){
		ls.append("PK <" + pk.value(0).toString() + ">");
	}
	QSqlQuery fk = getQuery("select constraint_name from information_schema.table_constraints where table_name ='"+table+"' and constraint_type LIKE 'FOR%'");
	while(fk.next()){
		ls.append("FK <" + fk.value(0).toString() + ">");
	}
	q.finish();
	pk.finish();
	fk.finish();
	return ls;
}
