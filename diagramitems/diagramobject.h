#ifndef DIAGRAMOBJECT_H
#define DIAGRAMOBJECT_H

#include "baseitem.h"

class Hub;
class Linea;
class DiagramObject : public BaseItem
{
	Q_OBJECT

public:
	DiagramObject(BaseItem *parent = 0);
	Hub *hub() const;
	QList<Linea *> lineasConectadas() const;
	void updateHubs();

public slots:
	virtual void updateLayout();
protected:
	void setHub(Hub *hub);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	Hub *m_hub;
};

#endif
