/*
 *
 *  Copyright (c) 2015
 *  name : Francis Banyikwa
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef CHECKFORUPDATES_H
#define CHECKFORUPDATES_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

#include <memory>

class QNetworkReply ;
class QWidget ;
class checkForUpdates : public QObject
{
	Q_OBJECT
public:
	explicit checkForUpdates( QWidget * ) ;
	~checkForUpdates() ;
	void check() ;
	void autoCheck() ;

	static void autoCheckForUpdate( QWidget *,const QString& ) ;
	static void checkForUpdate( QWidget * ) ;
signals:

public slots:
	void networkReply( QNetworkReply * ) ;
private:
	void angalia() ;
	bool m_autocheck ;
	QWidget * m_widget ;
	QNetworkAccessManager m_manager ;
};

#endif // CHECKFORUPDATES_H