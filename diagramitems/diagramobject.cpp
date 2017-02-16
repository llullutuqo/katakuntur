#include "diagramobject.h"
#include "conector.h"
DiagramObject::DiagramObject(BaseItem *parent)
	: BaseItem(parent), m_hub(NULL)
{
	setZValue(100.0);
}

Hub *
DiagramObject::hub() const
{
	return m_hub;
}

void
DiagramObject::setHub(Hub *hub)
{
	m_hub = hub;
}

QList<Linea *> DiagramObject::lineasConectadas() const{
	if(m_hub)
		return m_hub->lineas();
	return QList<Linea *>();
}

void
DiagramObject::updateLayout(){
	updateHubs();
}

QVariant
DiagramObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSceneHasChanged)
		updateLayout();
	if (change == ItemPositionHasChanged)
		updateHubs();
	return QGraphicsItem::itemChange(change, value);
}

void
DiagramObject::updateHubs(){
	if(m_hub==NULL)
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
