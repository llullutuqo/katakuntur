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

#include <QDebug>
#include <QGraphicsItem>
#include <QPair>
#include "conector.h"
#include "itemmovible.h"
#include "itemlinea.h"
#include "boxsidehub.h"

BoxSideHub::BoxSideHub(ItemMovible *owner)
    : Hub(owner)
{
}

enum Side {
    Top,
    Right,
    Bottom,
    Left
};

static int
cmpAngle(qreal a, qreal b)
{
    qreal d = a - b;
    if (d > 180)
        d = d - 360;
    else if (d < -180)
        d = d + 360;
    return d;
}

typedef QPair<Conector *, qreal> ConnectorRealPair;

static bool
itemLessThan(const ConnectorRealPair &a, const ConnectorRealPair &b)
{
    int r = cmpAngle(a.second, b.second);
    if (r == 0) {
        return a.first->owner() < b.first->owner();
    }
    return r < 0;
}

//              |
//    180-cA    |   cA
//    _____________________
//              |
//    180+cA    |   360-cA
//              |

void
BoxSideHub::update()
{
    QHash<Side, QList<ConnectorRealPair> > sides;
    ItemMovible *item = owner();
    QRectF rect = item->boundingRect().translated(item->pos());
    bool hasSelfRelacion=false;
    foreach (Conector *connector, connectors()) {
        ItemLinea *connection = connector->owner();
        if(QString(connection->typeName())=="modelo-selfrelacion"){
            if(hasSelfRelacion)
                continue;
            Conector *connector1 = connection->conector(ItemLinea::StartConector);
            Conector *connector2 = connection->conector(ItemLinea::EndConector);
            sides[Bottom].append(qMakePair(connector1, 270.0));
            sides[Right].append(qMakePair(connector2, 270.0));
            hasSelfRelacion=true;
            continue;
        }
        Conector *connector1 = connection->conector(ItemLinea::StartConector);
        Conector *connector2 = connection->conector(ItemLinea::EndConector);
        ItemMovible *item2;
        if (connector1 == connector) {
            item2 = connector2->connectedObject();
        }
        else {
            item2 = connector1->connectedObject();
        }
        QRectF rect1 = rect;
        QRectF rect2 = item2->boundingRect().translated(item2->pos());
        qreal angle = QLineF(rect1.center(), rect2.center()).angle();
        qreal cornerAngle = QLineF(rect1.center(), rect1.topRight()).angle();
        Side side =Right;
        if (angle > cornerAngle) {
            if (angle <= 180 - cornerAngle) {
                side = Top;
            }
            else if (angle <= 180 + cornerAngle) {
                side = Left;
            }
            else if (angle <= 360 - cornerAngle) {
                side = Bottom;
            }
        }
        sides[side].append(qMakePair(connector, angle));
    }
    foreach (Side side, sides.keys()) {
        QPointF p, dp;
        QList<ConnectorRealPair> c = sides[side];
        qSort(c.begin(), c.end(), itemLessThan);
        qreal angle=0;
        switch (side) {
        case Top:
            p = rect.topRight();
            dp = QPointF(-rect.width() / (c.size() + 1), 0);
            angle = 90;
            break;
        case Right:
            p = rect.bottomRight();
            dp = QPointF(0, -rect.height() / (c.size() + 1));
            angle = 0;
            break;
        case Bottom:
            p = rect.bottomLeft();
            dp = QPointF(rect.width() / (c.size() + 1), 0);
            angle = 270;
            break;
        case Left:
            p = rect.topLeft();
            dp = QPointF(0, rect.height() / (c.size() + 1));
            angle = 180;
            break;
        }
        foreach (ConnectorRealPair conector, c) {
            p += dp;
            conector.first->setAngle(angle);
            conector.first->setPos(p);
        }
    }
}
