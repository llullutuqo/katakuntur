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

#include "mysql.h"
#include "model/selfrelacion.h"
#include "model/relacion.h"
#include "model/entity.h"
#include "model/attribute.h"
#include "pantalladiagrama.h"
Mysql::Mysql()
{
//	m_listatipos.append("INTEGER AUTO_INCREMENT");
//	m_listatipos.append("MEDIUMINT AUTO_INCREMENT");
//	m_listatipos.append("SMALLINT AUTO_INCREMENT");
	m_listatipos.append("SERIAL");
	m_listatipos.append("MEDSERIAL");
	m_listatipos.append("SMALLSERIAL");
	m_listatipos.append("BIGINT");
	m_listatipos.append("BIT");
	m_listatipos.append("BLOB");
	m_listatipos.append("CHAR(n)");
	m_listatipos.append("DATE");
	m_listatipos.append("DATETIME");
	m_listatipos.append("DECIMAL");
	m_listatipos.append("DOUBLE");
	m_listatipos.append("ENUM");
	m_listatipos.append("FLOAT(n)");
	m_listatipos.append("INTEGER");
	m_listatipos.append("LONGBLOB");
	m_listatipos.append("LONGTEXT");
	m_listatipos.append("MEDIUMBLOB");
	m_listatipos.append("MEDIUMTEXT");
	m_listatipos.append("SET");
	m_listatipos.append("SMALLINT");
	m_listatipos.append("TEXT");
	m_listatipos.append("TIME");
	m_listatipos.append("TIMESTAMP");
	m_listatipos.append("TINYBLOB");
	m_listatipos.append("TINYINT");
	m_listatipos.append("TINYTEXT");
	//m_listatipos.append("VARCHAR");
	m_listatipos.append("VARCHAR(n)");
	m_listatipos.append("YEAR");
	ejecutar_por_comas = true;
	m_limiteCaracteres = 63;
}
QString Mysql::tipoDatoLLaveFoanea(QString cadena){
	cadena = cadena.toUpper();
	if(cadena=="SERIAL")
		return "INTEGER";
	if(cadena=="MEDSERIAL")
		return "MEDIUMINT";
	if(cadena=="SMALLSERIAL")
		return "SMALLINT";
	return cadena;
}
QString Mysql::toMysqlAuto(QString value){
	return tipoDatoLLaveFoanea(value) + " AUTO_INCREMENT";
}
bool Mysql::isAutoincrementDataType(QString value){
	if(value.toUpper().contains("SERIAL"))
		return true;
	return false;
}

QString Mysql::toStandarQtType(QString sqlType){
	sqlType =sqlType.toUpper();
	if(sqlType.contains("CHAR") || sqlType.contains("TEXT"))
		return "QString";
	else if(sqlType=="TINYINT")
		return "bool";
	else if(sqlType.contains("INT") || sqlType.contains("SERIAL"))
		return "int";
	else if(sqlType.contains("FLOAT"))
		return "float";
	else if(sqlType.contains("DECIMAL")||sqlType.contains("DOUBLE"))
		return "double";
	else if(sqlType=="TIME")
		return "QTime";
	else if(sqlType=="DATETIME" || sqlType=="TIMESTAMP")
		return "QDateTime";
	else if(sqlType=="DATE")
		return "QDate";
	else if(sqlType.contains("BLOB"))
		return "QByteArray";
	//qDebug()<<"Data Unkn:"<<sqlType;
	return "UNKNOW";
}

void Mysql::generarTablas(QStringList &lista, PantallaDiagrama *np, const QString &prefijoTabla, const CaseTable &caseTable){
	SQLCore::generarTablas(lista,np,prefijoTabla,caseTable);
	m_alterTable.clear();
	foreach(Entity *item, np->itemsByType<Entity>()){
		QString llave;
		QHash<QString,QStringList> llavesUnicas;

		lista.append("CREATE TABLE "+generateTableName(item->nombre(),prefijoTabla,caseTable)+" (");
		foreach(EntityAttibute *col, item->columnas()){
			QString null=" NOT NULL";
			if(col->isNull())
				null=" NULL";
			QString tp = col->dataType();
			if(isAutoincrementDataType(tp))
				tp = toMysqlAuto(tp);
			lista.append("\t"+makeObjectName(col->name())+" "+(col->isForeignKey()?tipoDatoLLaveFoanea(col->dataType()) : tp) + null +(col->notes().isEmpty()?"":" COMMENT '" + col->notes()+"'") +",");
			if(col->isForeignKey()){
				lista.append("\tKEY("+makeObjectName(col->name())+"),");
				m_alterTable.append("ALTER TABLE "+generateTableName(item->nombre(),prefijoTabla,caseTable)+" ADD CONSTRAINT "+ generateAlterName(col) + " FOREIGN KEY ("+makeObjectName(col->name())+") REFERENCES "+generateTableName(col->foreignTable()->nombre(),prefijoTabla,caseTable)+"("+makeObjectName(col->reference())+") ON DELETE NO ACTION ON UPDATE CASCADE;");
			}
			if(col->isPrimaryKey())
				llave += makeObjectName(col->name())+",";
			else if(col->isLlaveUnica()){
				if(!llavesUnicas.contains(col->llaveUnica())){
					llavesUnicas.insert(col->llaveUnica(),QStringList());
				}
				llavesUnicas[col->llaveUnica()].append(makeObjectName(col->name()));
			}
		}
		if(!llave.isEmpty()){
			lista.append("\tPRIMARY KEY("+delLastChar(",",llave)+"),");
		}
		if(llavesUnicas.count() > 0){
			foreach(QStringList columnas, llavesUnicas ){
				lista.append("\tUNIQUE KEY("+columnas.join(",")+"),");
			}
		}

		QString s =lista.takeAt(lista.count() -1);
		s=delLastChar(",",s);
		lista.append(s);
		lista.append(")"+(item->descripcion().isEmpty()?"" : " COMMENT='"+item->descripcion()+"'")+" ENGINE=INNODB;");
	}
}

bool Mysql::checkCountAutoIncrement(){
	return true;
}

bool Mysql::checkAutoIncrementKey(){
	return true;
}

bool Mysql::createDataBase()
{
	return true;
}
