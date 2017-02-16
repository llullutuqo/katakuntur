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

#include "hub.h"
#include "itemmovible.h"
#include "conector.h"
#include "itemlinea.h"
#include <QUuid>

Hub::Hub(ItemMovible *owner)
	: m_owner(owner)
{
}
Hub::~Hub(){

}

ItemMovible *
Hub::owner() const
{
	return m_owner;
}

void
Hub::addConnector(Conector *connector)
{
	if(m_connectors.contains(connector))
		return;
	int b=0;
	for(int i =0;i<m_connectors.count();i++){
		if(connector->owner()->id() < m_connectors[i]->owner()->id()){
			b=1;
			break;
		}
	}
	m_connectors.insert(b,connector);
	if (m_owner->scene()) {
		m_owner->update();
	}
}

void
Hub::removeConnector(Conector *connector)
{
	/*Se evita usar Q_ASSERT en la funcion remove, no funciona en release.*/
	//Q_ASSERT(m_connectors.removeOne(connector));
	Q_ASSERT(m_connectors.contains(connector));
	m_connectors.removeOne(connector);
	if (m_owner->scene()) {
		m_owner->update();
	}
}

QList<Conector *>
Hub::connectors() const
{
	return m_connectors;
}

void
Hub::update()
{
}

QList<ItemLinea *>
Hub::lineas() const
{
	QList<ItemLinea *> sald;
	for(int i =0;i<m_connectors.count();i++){
		ItemLinea *linea = m_connectors.at(i)->owner();
		//La clase SelfRelacion se conecta 2 veces al mismo hub
		//Se detecta primero si la Linea ya ha sido agregada
		if(!sald.contains(linea))
			sald.append(linea);
	}
	return sald;
}
