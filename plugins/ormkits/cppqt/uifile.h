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

#ifndef UIFILE_H
#define UIFILE_H
#include <QString>
class EntityAttibute;
class SQLCore;
class UiFile
{
public:
	enum UiPropertyType{
		String =0,
		Number=1,
		Bool=2,
	};
	UiFile();
	~UiFile();
	void setClassName(QString name);
	QString className();
	void setOwnerClassName(QString value);
	void setWindowTitle(QString value);
	QString fatherClassName();
	void setFatherClassName(QString value);
	void save(QString directory);
	void loadFile(QString file);

	void addWidget(int row,int col,QString classType,QString name,QString primayText,int minLimit);
	void addQLabel(int row,QString name,QString primayText);
	void addWidgetForData(int row,QString dataType,QString name,QString primayText,int minLimit);
	void addColumElement(EntityAttibute *col,SQLCore *dbms);
	void addCppInclude(QString include);
private:
	class UiFilePrivate;
	UiFilePrivate *d;
};

#endif // UIFILE_H
