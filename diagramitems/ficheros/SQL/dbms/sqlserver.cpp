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

#include "sqlserver.h"
#include "model/entity.h"
#include "model/attribute.h"
#include "pantalladiagrama.h"
SQLServer::SQLServer()
{
	//INTEGER
	m_listatipos.append("SERIAL");
	//BIGINT
	m_listatipos.append("BIGSERIAL");

	m_listatipos.append("BIGINT");
	m_listatipos.append("BINARY");
	m_listatipos.append("BIT");
	m_listatipos.append("CHAR");
	m_listatipos.append("CURSOR");
	m_listatipos.append("DATE");
	m_listatipos.append("DATETIME");
	m_listatipos.append("DATETIME2");
	m_listatipos.append("DATETIMEOFFSET");
	m_listatipos.append("DECIMAL");
	m_listatipos.append("FLOAT");
	m_listatipos.append("HIERARCHYID");
	m_listatipos.append("INT");
	m_listatipos.append("MONEY");
	m_listatipos.append("NCHAR");
	m_listatipos.append("NUMERIC");
	m_listatipos.append("NVARCHAR(n)");
	m_listatipos.append("NTEXT");
	m_listatipos.append("REAL");
	m_listatipos.append("SMALLDATETIME");
	m_listatipos.append("SMALLINT");
	m_listatipos.append("SMALLMONEY");
	m_listatipos.append("SQL_VARIANT");
	m_listatipos.append("TABLE");
	m_listatipos.append("TEXT");
	m_listatipos.append("TIME");
	m_listatipos.append("TIMESTAMP");
	m_listatipos.append("TINYINT");
	m_listatipos.append("UNIQUEIDENTIFIER");
	m_listatipos.append("VARBINARY");
	m_listatipos.append("VARCHAR(n)");
	m_listatipos.append("XML");
	ejecutar_por_comas=true;
	m_limiteCaracteres = 126;
}
QString SQLServer::tipoDatoLLaveFoanea(QString cadena){
	cadena = cadena.toUpper();
	if(cadena=="SERIAL")
		return "INT";
	else if(cadena=="BIGSERIAL")
		return "BIGINT";
	return cadena;
}

QString SQLServer::toStandarQtType(QString sqlType){
	Q_UNUSED(sqlType);
	return "Unimplement";
}

bool SQLServer::isAutoincrementDataType(QString value){
	if(value.toUpper().contains("SERIAL"))
		return true;
	return false;
}

void SQLServer::generarTablas(QStringList &lista, PantallaDiagrama *np, const QString &prefijoTabla, const CaseTable &caseTable){
	SQLCore::generarTablas(lista,np,prefijoTabla,caseTable);
	/*Se cambia el caracter separador*/
	QString charSep = ";";
	m_alterTable.clear();
	foreach(Entity *item, np->itemsByType<Entity>()){
		QString llave;
		QHash<QString,QStringList> llavesUnicas;
		lista.append("CREATE TABLE "+generateTableName(item->nombre(),prefijoTabla,caseTable)+" (");
		foreach(EntityAttibute *col, item->columnas()){
			QString null=" NOT NULL,";
			if(col->isNull())
			null=" NULL,";
			QString tp = col->dataType();
			if(isAutoincrementDataType(tp))
				tp = tipoDatoLLaveFoanea(tp) + " IDENTITY";
			lista.append("\t"+makeObjectName(col->name())+" "+(col->isForeignKey()?tipoDatoLLaveFoanea(col->dataType()) : tp) +null);
			if(col->isForeignKey()){
				m_alterTable.append("ALTER TABLE "+generateTableName(item->nombre(),prefijoTabla,caseTable)+" ADD CONSTRAINT "+ generateAlterName(col) + " FOREIGN KEY ("+makeObjectName(col->name())+") REFERENCES "+generateTableName(col->foreignTable()->nombre(),prefijoTabla,caseTable)+"("+makeObjectName(col->reference())+") ON DELETE NO ACTION ON UPDATE CASCADE"+ charSep);
			}
			if(col->isPrimaryKey())
				llave += makeObjectName(col->name())+",";

			if(col->isLlaveUnica()){
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
				lista.append("\tUNIQUE("+columnas.join(",")+"),");
			}
		}

		QString s =lista.takeAt(lista.count() -1);
		s=delLastChar(",",s);
		lista.append(s);

		lista.append(")"+charSep);
	}
}

bool SQLServer::checkCountAutoIncrement(){
	return true;
}

bool SQLServer::checkAutoIncrementKey(){
	return true;
}

bool SQLServer::createDataBase()
{
	return false;
}
