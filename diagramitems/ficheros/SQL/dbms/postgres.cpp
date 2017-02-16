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

#include "postgres.h"
#include "model/selfrelacion.h"
#include "model/relacion.h"
#include "model/entity.h"
#include "model/attribute.h"
#include "pantalladiagrama.h"

const QString COMMENT_SYNTAX = "COMMENT ON %1 %2 IS '%3';";

Postgres::Postgres()
{
	m_listatipos.append("BIGINT");
	m_listatipos.append("BIGSERIAL");
	m_listatipos.append("BIT(n)");
	m_listatipos.append("BOOLEAN");
	m_listatipos.append("CHAR(n)");
	m_listatipos.append("DATE");
	m_listatipos.append("DOUBLE PRECISION");
	m_listatipos.append("INTEGER");
	m_listatipos.append("NUMERIC(p,s)");
	m_listatipos.append("REAL");
	m_listatipos.append("SERIAL");
	m_listatipos.append("SMALLINT");
	m_listatipos.append("TEXT");
	m_listatipos.append("TIME");
	m_listatipos.append("TIME WITH TIME ZONE");
	m_listatipos.append("TIMESTAMP");
	m_listatipos.append("TIMESTAMP WITH TIME ZONE");
	m_listatipos.append("VARBIT(n)");
	m_listatipos.append("VARCHAR(n)");
	m_listatipos.append("UUID");
	m_listatipos.append("XML");
	m_limiteCaracteres= 63;
}
QString Postgres::toStandarQtType(QString sqlType){
	sqlType =sqlType.toUpper();
	if(sqlType.contains("CHAR") || sqlType.contains("TEXT"))
		return "QString";
	else if(sqlType=="BOOLEAN")
		return "bool";
	else if(sqlType.contains("INT") || sqlType.contains("SERIAL"))
		return "int";
	else if(sqlType.contains("DOUBLE PRECISION"))
		return "double";
	else if(sqlType=="TIME")
		return "QTime";
	else if(sqlType=="DATETIME" || sqlType=="TIMESTAMP")
		return "QDateTime";
	else if(sqlType=="DATE")
		return "QDate";
	return "UNKNOW";
}
bool Postgres::isAutoincrementDataType(QString value){
	if(value.toUpper().contains("SERIAL"))
		return true;
	return false;
}
void Postgres::generarTablas(QStringList &sald, PantallaDiagrama *pnt, const QString &prefijoTabla, const CaseTable &caseTable){
	SQLCore::generarTablas(sald,pnt,prefijoTabla,caseTable);
	m_alterTable.clear();
	foreach (Entity *item, pnt->itemsByType<Entity>()) {
		const QString l_tableName = generateTableName(item->nombre(),prefijoTabla,caseTable);

		QString llaves;
		QHash<QString,QStringList> llavesUnicas;
		QStringList comentarios;

		sald.append("CREATE TABLE "+l_tableName+" (");
		if(!item->descripcion().isEmpty())
			comentarios.append(COMMENT_SYNTAX.arg("TABLE").arg(l_tableName).arg(item->descripcion()));

		foreach (EntityAttibute *column, item->columnas()) {
			const QString l_columnName = makeObjectName(column->name());
			QString null=" NOT NULL ";
			if(column->isNull())
				null =" NULL ";
			sald.append("\t"+l_columnName+" "+(column->isForeignKey()?tipoDatoLLaveFoanea(column->dataType()) : column->dataType())+ null+",");
			if(column->isPrimaryKey())
				llaves +=l_columnName+",";
			else if(column->isLlaveUnica()){
				if(!llavesUnicas.contains(column->llaveUnica())){
					llavesUnicas.insert(column->llaveUnica(),QStringList());
				}
				llavesUnicas[column->llaveUnica()].append(makeObjectName(column->name()));
			}
			if(column->isForeignKey())
				m_alterTable.append("ALTER TABLE "+l_tableName+" ADD FOREIGN KEY ("+l_columnName+") REFERENCES "+generateTableName(column->foreignTable()->nombre(),prefijoTabla,caseTable)+" ("+makeObjectName(column->reference())+");");
			if(!column->notes().isEmpty())
				comentarios.append(COMMENT_SYNTAX.arg("COLUMN").arg(l_tableName +"."+l_columnName).arg(column->notes()));
		}
		if(!llaves.isEmpty()){
			sald.append("\tPRIMARY KEY("+delLastChar(",",llaves)+"),");
		}
		if(llavesUnicas.count() > 0){
			foreach(QStringList columnas, llavesUnicas ){
				sald.append("\tUNIQUE ("+columnas.join(",")+"),");
			}
		}
		QString s =sald.takeAt(sald.count()-1);
		s=s.left(s.count()-1);
		sald.append(s);
		sald.append(");");
		if(!comentarios.isEmpty()){
			sald.append(comentarioSQL() + trUtf8("COMENTARIOS DE TABLA %1").arg(item->nombre()));
			sald.append(comentarios);
			sald.append(QString());
		}
	}
}
QString Postgres::tipoDatoLLaveFoanea(QString cadena){

	if(cadena.contains("BIGSERIAL"))
		return "BIGINT";
	else if(cadena.contains("SERIAL"))
		return "INT";
	return cadena;
}

bool Postgres::checkCountAutoIncrement(){
	return false;
}

bool Postgres::checkAutoIncrementKey(){
	return false;
}

bool Postgres::createDataBase()
{
	return true;
}
