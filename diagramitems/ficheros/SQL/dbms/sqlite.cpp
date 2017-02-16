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

#include "sqlite.h"
#include "model/entity.h"
#include "model/attribute.h"
#include "pantalladiagrama.h"
SQLite::SQLite()
{
	m_listatipos.append("AUTOINCREMENT");
	m_listatipos.append("INTEGER");
	m_listatipos.append("SMALL INTEGER");
	m_listatipos.append("BOOL");
	m_listatipos.append("NUMERIC(p,e)");
	m_listatipos.append("DECIMAL(p,e)");
	m_listatipos.append("REAL");
	m_listatipos.append("DOUBLE");
	m_listatipos.append("FLOAT");
	m_listatipos.append("CHARACTER(n)");
	m_listatipos.append("CHARACTER VARYING(n)");
	m_listatipos.append("VARCHAR(n)");
	m_listatipos.append("BIT(n)");
	m_listatipos.append("BIT VARYING(n)");
	m_listatipos.append("DATE");
	m_listatipos.append("TIME");
	m_listatipos.append("TIMESTAMP");
	m_listatipos.append("TEXT");
	m_listatipos.append("REAL");
	m_listatipos.append("BLOB");
	ejecutar_por_comas=true;
}
QString SQLite::tipoDatoLLaveFoanea(QString cadena){
	cadena = cadena.toUpper();
	if(cadena.compare("AUTOINCREMENT")==0)
		return "INTEGER";
	return cadena;
}

QString SQLite::toStandarQtType(QString sqlType){
	Q_UNUSED(sqlType);
	sqlType =sqlType.toUpper();
	if(sqlType.contains("CHARACTER") || sqlType.contains("TEXT") ||sqlType.contains("VARCHAR"))
		return "QString";
	else if(sqlType=="BOOL")
		return "bool";
	else if(sqlType.contains("INTEGER") || sqlType.contains("AUTO"))
		return "int";
	else if(sqlType.contains("FLOAT") || sqlType.contains("REAL"))
		return "float";
	else if(sqlType.contains("DECIMAL")||sqlType.contains("NUMERIC") || sqlType.contains("DOUBLE"))
		return "double";
	else if(sqlType=="TIME")
		return "QTime";
	else if(sqlType=="DATETIME" || sqlType=="TIMESTAMP")
		return "QDateTime";
	else if(sqlType=="DATE")
		return "QDate";
	else if(sqlType=="BOOL")
		return "bool";
	else if(sqlType.contains("BLOB"))
		return "QByteArray";
	return "UNKNOW";
}

bool SQLite::isAutoincrementDataType(QString value){
	if(value.toUpper().contains("AUTO"))
		return true;
	return false;
}

void SQLite::generarTablas(QStringList &lista, PantallaDiagrama *np, const QString &prefijoTabla, const CaseTable &caseTable){
	lista.append("PRAGMA foreign_keys = ON;");
	SQLCore::generarTablas(lista,np,prefijoTabla,caseTable);
	foreach(Entity *item, np->itemsByType<Entity>()){
		QString llave;
		QHash<QString,QStringList> llavesUnicas;
		QStringList foreign;
		bool hasAutoIncrement=false;
		lista.append("CREATE TABLE "+generateTableName(item->nombre(),prefijoTabla,caseTable)+" (");
		foreach(EntityAttibute *col, item->columnas()){
			QString null=" NOT NULL ";

			if(col->isNull())
			null=" NULL ";

			if(col->isPrimaryKey()){
				if(col->dataType().contains("AUTOINCREMENT")&& !col->isForeignKey()){
					lista.append("\t" + col->name() + " INTEGER PRIMARY KEY AUTOINCREMENT  " +null+",");
					hasAutoIncrement=true;
				}
				else{
					lista.append("\t"+col->name()+" "+(col->isForeignKey()?tipoDatoLLaveFoanea(col->dataType()) : col->dataType()) +null+",");
					llave += col->name()+",";
				}
			}
			else{
				lista.append("\t"+col->name()+" "+(col->isForeignKey()?tipoDatoLLaveFoanea(col->dataType()) : col->dataType()) +null+",");
				if(col->isLlaveUnica()){
					if(!llavesUnicas.contains(col->llaveUnica())){
						llavesUnicas.insert(col->llaveUnica(),QStringList());
					}
					llavesUnicas[col->llaveUnica()].append(makeObjectName(col->name()));
				}
			}
			if(col->isForeignKey())
				foreign.append("\tFOREIGN KEY (" + col->name() + ") REFERENCES " + generateTableName(col->foreignTable()->nombre(),prefijoTabla,caseTable) + "(" + col->reference() + "),");
		}
		lista.append(foreign);
		if(!llave.isEmpty() && !hasAutoIncrement){
			if(llave.endsWith(","))
			llave = llave.left(llave.count() -1);
			lista.append("\tPRIMARY KEY("+llave+"),");
		}

		if(llavesUnicas.count() > 0){
			foreach(QStringList columnas, llavesUnicas ){
				lista.append("\tUNIQUE("+columnas.join(",")+"),");
			}
		}

		QString s =lista.takeAt(lista.count() -1);
		s=s.left(s.count()-1);
		lista.append(s);
		lista.append(");");
	}

}

bool SQLite::checkCountAutoIncrement(){
	return true;
}

bool SQLite::checkAutoIncrementKey(){
	return true;
}

bool SQLite::createDataBase()
{
	return false;
}

