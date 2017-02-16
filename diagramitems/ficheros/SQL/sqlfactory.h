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

#ifndef SQLFACTORY_H
#define SQLFACTORY_H
#include <QHash>
#include <QString>
#include <ficheros/SQL/sqlcore.h>
#include "utilidades/singelton.h"
#include "diagramitems_global.h"

class  DIAGRAMITEMSSHARED_EXPORT  SQLFactory : public Singelton<SQLFactory>
{
public:
	SQLFactory();
	class CreatorBase
	{
	public:
		virtual SQLCore *create() = 0;
	};

	template <typename ItemType>
	class Creator : public CreatorBase
	{
	public:
		SQLCore *create()
		{ return new ItemType(); }
	};

public:

	static SQLCore *createByKey(const QString &key)
	{
		SQLFactory *inst = SQLFactory::instance();
		const QString id = inst->m_mapId[key];
		return createById(id);
	}
	static SQLCore *createById(const QString &id)
	{
		SQLFactory *inst = SQLFactory::instance();
		if (!inst->m_map.contains(id))
			return NULL;
		return inst->m_map[id]->create();
	}
	static QStringList keys()
	{
		SQLFactory *inst = SQLFactory::instance();
		return inst->m_mapId.keys();
	}

	static QString getId(const QString key){
		SQLFactory *inst = SQLFactory::instance();
		return inst->m_mapId[key];
	}

protected:

	template <typename Type> void add()
	{
		Creator<Type>* cd = new Creator<Type>();
		m_map[Type::staticId()] = cd ;
		m_mapId[Type::staticNombreDBMS()] = Type::staticId();
	}

	QHash<QString, CreatorBase*> m_map;
	QHash<QString, QString> m_mapId;
	friend class Singelton<SQLFactory>;
};
#define SQLFACTORY_ADD_TYPE(ST) add<ST>();
#endif // SQLFACTORY_H
