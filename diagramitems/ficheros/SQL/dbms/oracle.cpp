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

#include "oracle.h"
#include "model/selfrelacion.h"
#include "model/relacion.h"
#include "model/entity.h"
#include "model/attribute.h"
#include "pantalladiagrama.h"

const QString COMMENT_SYNTAX = "COMMENT ON %1 %2 IS '%3';";

Oracle::Oracle()
{
	m_listatipos.append("NUMBER");
	m_listatipos.append("NUMBER(n)");
	m_listatipos.append("NUMBER(n,n)");
	m_listatipos.append("CLOB");
	m_listatipos.append("BLOB");
	m_listatipos.append("CHAR(n)");
	m_listatipos.append("VARCHAR2(n)");
	m_listatipos.append("DATE");
	m_listatipos.append("BFILE");
	m_listatipos.append("TIMESTAMP");
	m_listatipos.append("BINARY_DOUBLE");
	m_listatipos.append("FLOAT");
	m_listatipos.append("INT");
	m_listatipos.append("INTEGER");
	m_listatipos.append("INTERVAL DAY");
	m_listatipos.append("INTERVAL YEAR");
	m_listatipos.append("LONG");
	m_listatipos.append("NCHAR(n)");
	m_listatipos.append("NVARCHAR2(n)");
	m_listatipos.append("SMALLINT");
	m_listatipos.append("REAL");
	m_listatipos.append("ROWID");
	m_listatipos.append("RAW");
	m_listatipos.append("DOUBLE PRECISION");
	m_listatipos.append("DECIMAL");
	m_listatipos.append("DEC");

	ejecutar_por_comas = true;
	m_limiteCaracteres = 30;
}
QString Oracle::tipoDatoLLaveFoanea(QString cadena){
	cadena = cadena.toUpper();
	if(cadena=="SERIAL")
		return "INTEGER";
	if(cadena=="MEDSERIAL")
		return "MEDIUMINT";
	if(cadena=="SMALLSERIAL")
		return "SMALLINT";
	return cadena;
}
QString Oracle::toMysqlAuto(QString value){
	return tipoDatoLLaveFoanea(value) + " AUTO_INCREMENT";
}
bool Oracle::isAutoincrementDataType(QString value){
	if(value.toUpper().contains("SERIAL"))
		return true;
	return false;
}

QString Oracle::toStandarQtType(QString sqlType){
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

void Oracle::generarTablas(QStringList &lista, PantallaDiagrama *np, const QString &prefijoTabla, const CaseTable &caseTable){
	SQLCore::generarTablas(lista,np,prefijoTabla,caseTable);
	m_alterTable.clear();
	foreach(Entity *item, np->itemsByType<Entity>()){

		const QString l_tableName = generateTableName(item->nombre(),prefijoTabla,caseTable);

		QString llave;
		QHash<QString,QStringList> llavesUnicas;
		QStringList comentarios;

		lista.append("CREATE TABLE "+l_tableName+" (");
		if(!item->descripcion().isEmpty())
			comentarios.append(COMMENT_SYNTAX.arg("TABLE").arg(l_tableName).arg(item->descripcion()) );
		foreach(EntityAttibute *col, item->columnas()){
			const QString l_columnName = makeObjectName(col->name());

			QString null=" NOT NULL,";
			if(col->isNull())
			null=" NULL,";
			QString tp = col->dataType();
			if(isAutoincrementDataType(tp))
				tp = toMysqlAuto(tp);
			lista.append("\t"+makeObjectName(col->name())+" "+(col->isForeignKey()?tipoDatoLLaveFoanea(col->dataType()) : tp) +null);
			if(col->isForeignKey()){
				m_alterTable.append("ALTER TABLE "+ l_tableName +" ADD CONSTRAINT "+ generateAlterName(col) + " FOREIGN KEY ("+l_columnName+") REFERENCES "+generateTableName(col->foreignTable()->nombre(),prefijoTabla,caseTable)+"("+makeObjectName(col->reference())+") ;");
			}
			if(col->isPrimaryKey())
				llave += makeObjectName(col->name())+",";
			else if(col->isLlaveUnica()){
				if(!llavesUnicas.contains(col->llaveUnica())){
					llavesUnicas.insert(col->llaveUnica(),QStringList());
				}
				llavesUnicas[col->llaveUnica()].append(makeObjectName(col->name()));
			}
			if(!col->notes().isEmpty())
				comentarios.append(COMMENT_SYNTAX.arg("COLUMN").arg(l_tableName +"." +l_columnName ).arg(col->notes()));

		}
		if(!llave.isEmpty()){
			lista.append("\tPRIMARY KEY("+delLastChar(",",llave)+"),");
		}
		if(llavesUnicas.count() > 0){
			foreach(QStringList columnas, llavesUnicas ){
				lista.append("\tUNIQUE ("+columnas.join(",")+"),");
			}
		}

		QString s =lista.takeAt(lista.count() -1);
		s=delLastChar(",",s);
		lista.append(s);
		lista.append(");");
		if(!comentarios.isEmpty()){
			lista.append(comentarioSQL() + trUtf8("COMENTARIOS DE TABLA %1").arg(item->nombre()));
			lista.append(comentarios);
			lista.append(QString());
		}
	}
}

bool Oracle::checkCountAutoIncrement(){
	return true;
}

bool Oracle::checkAutoIncrementKey(){
	return true;
}

bool Oracle::createDataBase()
{
	return false;
}
