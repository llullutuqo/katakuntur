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

#include "entityattibute.h"
#include "entity.h"
class EntityAttibute::PrivateAttribute{
public:
	PrivateAttribute(Entity * tabla){
		m_tabla= tabla;
		m_foreignTable=NULL;
		m_llaveForanea=false;
		m_llaveUnica = QString();
		m_primaryKey = false;
		m_nulo = false;
	}
	bool m_primaryKey;
	bool m_nulo;
	QString m_llaveUnica;
	bool m_llaveForanea;
	QString m_nombre;
	QString m_notas;
	QString m_tipoDato;
	QString m_reference;
	Entity *m_tabla;
	Entity *m_foreignTable;
};
EntityAttibute::EntityAttibute(Entity * parent):Attribute(parent),d(new PrivateAttribute(parent))
{
}

EntityAttibute::~EntityAttibute(){
	delete d;
}

bool EntityAttibute::isForeignKey(){
	return d->m_llaveForanea;
}
void EntityAttibute::setForeignKey(bool value){
	d->m_llaveForanea=value;
}

QString EntityAttibute::name() const {
	return d->m_nombre;
}
void EntityAttibute::setName(const QString &value){
	if(d->m_nombre != value){
        d->m_nombre = value;
		emit propertyChanged("nombre",value);
	}
}
QString EntityAttibute::notes() const {
	return d->m_notas;
}
void EntityAttibute::setNotes(const QString &value){
	if(d->m_notas != value){
		d->m_notas = value;
		emit propertyChanged("notas",value);
	}
}
bool EntityAttibute::isLlaveUnica(){
	return !d->m_llaveUnica.isEmpty();
}
void EntityAttibute::setLlaveUnica(QString value){
	if(d->m_primaryKey)
		value =QString();
	if(d->m_llaveUnica != value){
		d->m_llaveUnica = value;
		if(!value.isEmpty())
			setNull(false);
		emit propertyChanged("llaveUnica",value);
	}
}

QString EntityAttibute::llaveUnica()
{
	return d->m_llaveUnica;
}
bool EntityAttibute::isPrimaryKey(){
	return d->m_primaryKey;
}
void EntityAttibute::setPrimaryKey(bool value){
	if(d->m_primaryKey!=value){
		if(value){
			if(isLlaveUnica())
				return;
//			if(d->m_tabla->cantidadRelacionesIdentificativas()>2)
//				return;
			setNull(false);
			setLlaveUnica(QString());
		}
		d->m_primaryKey = value;
		emit propertyChanged("llavePrimaria",value);
	}
}
bool EntityAttibute::isNull(){
	return d->m_nulo;
}
void EntityAttibute::setNull(bool value){
	if((d->m_primaryKey&&!d->m_llaveForanea) || isLlaveUnica())
		value =false;
	if(d->m_nulo!=value){
		d->m_nulo = value;
		emit propertyChanged("nulo",value);
	}
}
QString EntityAttibute::dataType() const {
	return d->m_tipoDato;
}
void EntityAttibute::setDataType(const QString &value){
	if(d->m_tipoDato!=value){
		d->m_tipoDato = value;
		emit propertyChanged("tipoDato",value);
	}
}
QString EntityAttibute::toText(){
	QString sald;
	sald.append(" type: " + this->dataType().toLower() + ", required: ");//<<;
	if(d->m_nulo)
		sald.append("true");
	else
		sald.append("false");
	sald.append(", ");
	if(isLlaveUnica())
		sald.append("index: unique");
	if(sald.endsWith(", "))
		sald = sald.left(sald.count()-2);
	return sald;
}
QString EntityAttibute::reference(){
	return d->m_reference;
}

void EntityAttibute::setReference(QString reference){
	d->m_reference=reference;
}

Entity * EntityAttibute::tabla(){
	return d->m_tabla;
}
Entity * EntityAttibute::foreignTable(){
	return d->m_foreignTable;
}

void EntityAttibute::setForeignEntity(Entity * table){
	if(table!=d->m_foreignTable)
		d->m_foreignTable= table;
}
