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

#include "conector.h"
#include "hub.h"
#include "itemlinea.h"

Conector::Conector(ItemLinea *owner)
	: m_owner(owner), m_hub(NULL)
{
	m_angle= 0;
}

ItemLinea *
Conector::owner() const
{
	return m_owner;
}

QPointF
Conector::pos() const
{
	return m_pos;
}

void
Conector::setPos(const QPointF &pos)
{
	if (m_pos != pos) {
		m_pos = pos;
		if (m_owner->scene()){
			owner()->actualizarBoceto();
			//owner()->update();
		}
	}
}

qreal
Conector::angle() const
{
	return m_angle;
}

void
Conector::setAngle(qreal angle)
{
	if (m_angle != angle) {
		m_angle = angle;
		//if (m_owner->scene()){
		//owner()->updateLayout();
		//owner()->update();
		//}
	}
}

Hub *
Conector::hub() const
{
	return m_hub;
}

void
Conector::setHub(Hub *hub)
{
	removeFromHub();
	m_hub = hub;
	addToHub();
}

bool
Conector::isConnected() const
{
	return m_hub;
}

ItemMovible *
Conector::connectedObject() const
{
	if (isConnected())
		return hub()->owner();
	return NULL;
}

Conector *
Conector::otherEnd() const
{
	ItemLinea *conn = owner();
	if (conn->conector(ItemLinea::StartConector) == this) {
		return conn->conector(ItemLinea::EndConector);
	}
	else {
		return conn->conector(ItemLinea::StartConector);
	}
}

void
Conector::addToHub()
{
	if (m_hub)
		m_hub->addConnector(this);
}

void
Conector::removeFromHub()
{
	if (m_hub)
		m_hub->removeConnector(this);
}
