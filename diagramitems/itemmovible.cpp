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

#include "itemmovible.h"
#include "hub.h"
#include "conector.h"

class ItemMovible::ItemMoviblePrivate{
public:
	ItemMoviblePrivate(){
		m_hub = NULL;
	}
	~ItemMoviblePrivate(){
		if(m_hub)
			delete m_hub;
	}

	Hub *m_hub;
};

ItemMovible::ItemMovible(BaseItem *parent)
	: BaseItem(parent), d(new ItemMoviblePrivate())
{
	setZValue(100.0);
}

ItemMovible::~ItemMovible()
{
	delete d;
}

Hub *
ItemMovible::hub() const
{
	return d->m_hub;
}

void
ItemMovible::setHub(Hub *hub)
{
	d->m_hub = hub;
}

QList<ItemLinea *> ItemMovible::lineasConectadas() const{
	if(d->m_hub)
		return d->m_hub->lineas();
	return QList<ItemLinea *>();
}

void
ItemMovible::actualizarBoceto(){
	updateHubs();
}

QVariant
ItemMovible::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSceneHasChanged)
		actualizarBoceto();
	if (change == ItemPositionHasChanged)
		updateHubs();
	return QGraphicsItem::itemChange(change, value);
}

void
ItemMovible::updateHubs(){
	if(d->m_hub==NULL)
		return;
	QSet<Hub *> hubsToUpdate;
	hubsToUpdate.insert(hub());
	foreach (Conector *conn, hub()->connectors()) {
		hubsToUpdate.insert(conn->otherEnd()->hub());
	}
	foreach (Hub *hub, hubsToUpdate) {
		if(hub)
			hub->update();
	}
}
