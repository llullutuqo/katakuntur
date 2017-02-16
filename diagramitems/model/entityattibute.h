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

#ifndef COLUMNATABLA_H
#define COLUMNATABLA_H
#include "attribute.h"
class Entity;
class DIAGRAMITEMSSHARED_EXPORT EntityAttibute : public Attribute
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName)
	Q_PROPERTY(bool llavePrimaria READ isPrimaryKey WRITE setPrimaryKey)
	Q_PROPERTY(QString llaveUnica READ llaveUnica WRITE setLlaveUnica)
	Q_PROPERTY(bool nulo READ isNull WRITE setNull)
	Q_PROPERTY(QString dataType READ dataType WRITE setDataType)
	Q_PROPERTY(QString notes READ notes WRITE setNotes)
public:
	EntityAttibute(Entity *);
	~EntityAttibute();

	bool isPrimaryKey();
	void setPrimaryKey(bool value);

	bool isNull();
	void setNull(bool value);

	bool isLlaveUnica();
	void setLlaveUnica(QString value);
	QString llaveUnica();

	bool isForeignKey();
	void setForeignKey(bool value);

	Entity * tabla();
	Entity * foreignTable();
	void setForeignEntity(Entity * table);

	QString name() const;
	void setName(const QString &name);

	QString notes() const;
	void setNotes(const QString &notes);

	QString dataType() const;
	void setDataType(const QString &value);

	QString reference();
	void setReference(QString reference);

	QString toText();
private:
	class PrivateAttribute;
	PrivateAttribute *d;
};
Q_DECLARE_METATYPE(EntityAttibute*)
#endif // COLUMNATABLA_H
