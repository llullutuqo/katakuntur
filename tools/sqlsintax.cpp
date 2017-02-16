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

#include "sqlsintax.h"
#include <QHash>
#include <QTextCharFormat>

class SQLSintax::SQLSintaxPrivate{
public:
  SQLSintaxPrivate(){
	reservedKeysFormat.setForeground(QBrush(QColor(31,28,27)));
	reservedKeysFormat.setFontWeight(QFont::Bold);
	extraFormat.setForeground(QBrush(QColor(31,28,27)));
	extraFormat.setFontWeight(QFont::Bold);
	typeFormat.setForeground(QBrush(QColor(0,87,174)));
	commentFormat.setForeground(QBrush(QColor(135,137,138)));
	textFormat.setForeground(QBrush(QColor(191,3,3)));
	numberFormat.setForeground(QBrush(QColor(176,128,0)));

	HighlightingRule rule;
	rule.format = numberFormat;
	rule.pattern = QRegExp("[0-9]+");
	highlightingRules.append(rule);

	simpleTextChar.setPattern("'");
  }
  struct HighlightingRule
  {
	QRegExp pattern;
	QTextCharFormat format;
  };
  QVector<HighlightingRule> highlightingRules;
  QVector<HighlightingRule> typeRules;
  QVector<HighlightingRule> processRules;

  QTextCharFormat reservedKeysFormat;
  QTextCharFormat extraFormat;
  QTextCharFormat typeFormat;
  QTextCharFormat commentFormat;
  QTextCharFormat textFormat;
  QTextCharFormat numberFormat;
  QRegExp simpleTextChar;
};

SQLSintax::SQLSintax(QTextDocument *parent): QSyntaxHighlighter(parent),d(new SQLSintaxPrivate){
  SQLSintaxPrivate::HighlightingRule rule;
  QStringList lista;
  QStringList lista_Procesos;
  lista << "\\bCREATE\\b"<< "\\bTABLE\\b"<< "\\bALTER\\b"<<"\\bFOREIGN\\b"<< "\\bKEY\\b"<<"\\bREFERENCES\\b"
	  << "\\bON\\b"<< "\\bDELETE\\b"<< "\\bNO\\b"<< "\\bACTION\\b"<< "\\bUPDATE\\b"<<"\\bCASCADE\\b"
	  <<"\\bADD\\b"<<"\\bCONSTRAINT\\b"<<"\\bPRIMARY\\b"<<"\\bNOT\\b"<<"\\bNULL\\b"<<"\\bGO\\b"<<"\\bINNER\\b"<<"\\bUNIQUE\\b";
  lista << "\\bSELECT\\b"<< "\\bFROM\\b"<< "\\bUPDATE\\b"<<"\\bVALUES\\b"<< "\\bINSERT\\b"<<"\\bSHOW\\b"<<"\\bUSE\\b"
	  <<"\\bDROP\\b"<<"\\bDATABASE\\b"<<"\\bWHERE\\b"<<"\\bORDER BY\\b"<<"\\bJOIN\\b"<<"\\bCOMMENT\\b"<<"\\bCOLUMN\\b"<<"\\bIS\\b"
	  <<"\\bIDENTITY\\b"<<"\\bAUTO_INCREMENT\\b";
  lista_Procesos <<"";
  foreach (const QString &pattern, lista) {
	rule.pattern = QRegExp(pattern);
	rule.format = d->reservedKeysFormat;
	d->highlightingRules.append(rule);
  }
  QStringList extra;
  extra<<"\\bENGINE=INNODB\\b"<<"\\bPRAGMA\\b"<<"\\bFOREIGN_KEYS\\b";
  foreach (const QString &pattern, extra) {
	rule.pattern = QRegExp(pattern);
	rule.format = d->extraFormat;
	d->highlightingRules.append(rule);
  }
  rule.pattern=QRegExp("^#.*");
  rule.format=d->commentFormat;
  d->highlightingRules.append(rule);
  rule.pattern=QRegExp("^--.*");
  d->highlightingRules.append(rule);

}
SQLSintax::~SQLSintax(){
  delete d;
}

void SQLSintax::highlightBlock(const QString &text)
{
  const QString st =text.toUpper();

  foreach (const SQLSintaxPrivate::HighlightingRule &rule, d->typeRules) {
	QRegExp expression(rule.pattern);
	int index = expression.indexIn(st);
	while (index >= 0) {
	  int length = expression.matchedLength();
	  setFormat(index, length, rule.format);
	  index = expression.indexIn(st, index + length);
	}
  }

  foreach (const SQLSintaxPrivate::HighlightingRule &rule, d->highlightingRules) {
	QRegExp expression(rule.pattern);
	int index = expression.indexIn(st);
	while (index >= 0) {
	  int length = expression.matchedLength();
	  setFormat(index, length, rule.format);
	  index = expression.indexIn(st, index + length);
	}
  }

  setCurrentBlockState(0);

  int startIndex = 0;
  if (previousBlockState() != 1)
	startIndex =d->simpleTextChar.indexIn(text);

  while (startIndex >= 0) {
	int endIndex = d->simpleTextChar.indexIn(text, startIndex +1);
	int commentLength;
	if (endIndex == -1) {
	  setCurrentBlockState(1);
	  commentLength = text.length() - startIndex;
	} else {
	  commentLength = endIndex - startIndex
			  + d->simpleTextChar.matchedLength();
	}
	setFormat(startIndex, commentLength, d->textFormat);
	startIndex = d->simpleTextChar.indexIn(text, startIndex + commentLength);
  }
}
void SQLSintax::addTypeList(QStringList list){
  SQLSintaxPrivate::HighlightingRule rule;
  d->typeRules.clear();
  foreach (QString pattern, list) {
	pattern = "\\b" + pattern;
	pattern.replace("(n)","");
	rule.pattern = QRegExp(pattern);
	rule.format = d->typeFormat;
	d->typeRules.append(rule);
  }
}
