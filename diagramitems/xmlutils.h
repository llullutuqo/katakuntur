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

#ifndef XMLUTILS_H
#define XMLUTILS_H

#include <QDomDocument>
#include <QDomElement>
#include <QMetaEnum>
#include <QMetaObject>
#include <QPointF>

namespace XmlUtils{
inline void
appendStringElement(QDomDocument &doc, QDomElement &parent, const QString &name, const QString &value)
{
	if (!value.isEmpty()) {
		QDomElement element = doc.createElement(name);
		element.appendChild(doc.createTextNode(value));
		parent.appendChild(element);
	}
}

inline QString
readStringElement(QDomElement &parent, const QString &name, const QString &defaultValue = QString())
{
	QDomElement element = parent.firstChildElement(name);
	if (!element.isNull()) {
		return element.text();
	}
	return defaultValue;
}

inline const char *
enumNameFromValue(const QObject *obj, const char *enumerator, int value)
{
	const QMetaObject *metaObj = obj->metaObject();
	int enumeratorIndex = metaObj->indexOfEnumerator(enumerator);
	return metaObj->enumerator(enumeratorIndex).valueToKey(value);
}

inline int
enumValueFromName(const QObject *obj, const char *enumerator, const char *key)
{
	const QMetaObject *metaObj = obj->metaObject();
	int enumeratorIndex = metaObj->indexOfEnumerator(enumerator);
	return metaObj->enumerator(enumeratorIndex).keyToValue(key);
}

template <typename T> inline void
appendEnumElement(QDomDocument doc, QDomElement element, const QString &name, T value, const QObject *obj, const char *enumName)
{
	appendStringElement(doc, element, name, enumNameFromValue(obj, enumName, value));
}

template <typename T> inline T
readEnumElement(QDomElement &parent, const QString &name, T defaultValue, const QObject *obj, const char *enumName)
{
	QDomElement element = parent.firstChildElement(name);
	if (!element.isNull()) {
		return T(enumValueFromName(obj, enumName, element.text().toLatin1().constData()));
	}
	return defaultValue;
}

inline void
appendBoolElement(QDomDocument doc, QDomElement element, const QString &name, bool value, const QString &trueName = "True", const QString &falseName = "False")
{
	appendStringElement(doc, element, name, value ? trueName : falseName);
}

inline bool
readBoolElement(QDomElement &parent, const QString &name, bool defaultValue = false, const QString &trueName = "True", const QString &falseName = "False")
{
	QDomElement element = parent.firstChildElement(name);
	if (!element.isNull()) {
		QString value = element.text();
		if (value == trueName) {
			return true;
		}
		else if (value == falseName) {
			return false;
		}
		else {
//			qWarning() << "Invalid value for" << name <<  ":" << value << "(must be either" << trueName << "or" << falseName << ")";
		}
	}
	return defaultValue;
}

inline void
appendFloatElement(QDomDocument &doc, QDomElement &parent, const QString &name, qreal value)
{
	QDomElement element = doc.createElement(name);
	element.appendChild(doc.createTextNode(QString::number(double(value))));
	parent.appendChild(element);
}

inline qreal
readFloatElement(QDomElement &parent, const QString &name, qreal defaultValue = 0)
{
	QDomElement element = parent.firstChildElement(name);
	if (!element.isNull()) {
		return element.text().toDouble();
	}
	return defaultValue;
}

inline void
appendIntElement(QDomDocument &doc, QDomElement &parent, const QString &name, int value)
{
	QDomElement element = doc.createElement(name);
	element.appendChild(doc.createTextNode(QString::number(value)));
	parent.appendChild(element);
}

inline int
readIntElement(QDomElement &parent, const QString &name, int defaultValue = 0)
{
	QDomElement element = parent.firstChildElement(name);
	if (!element.isNull()) {
		return element.text().toInt();
	}
	return defaultValue;
}

inline void
appendPointElement(QDomDocument &doc, QDomElement &parent, const QString &name, QPointF value)
{
	if (!value.isNull()) {
		QDomElement element = doc.createElement(name);
		appendFloatElement(doc, element, "x", value.x());
		appendFloatElement(doc, element, "y", value.y());
		parent.appendChild(element);
	}
}

inline QPointF
readPointElement(QDomElement &parent, const QString &name)
{
	QDomElement element = parent.firstChildElement(name);
	if (!element.isNull()) {
		qreal x = readFloatElement(element, "x");
		qreal y = readFloatElement(element, "y");
		return QPointF(x, y);
	}
	return QPointF();
}

}

#endif // XMLUTILS_H
