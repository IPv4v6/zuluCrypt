/*
 * copyright: 2013
 * name : mhogo mchungu
 * email: mhogomchungu@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef LXQT_SECRET_SERVICE_H
#define LXQT_SECRET_SERVICE_H

#include "lxqt_wallet.h"

#include <QString>
#include <QByteArray>
#include <QDebug>

namespace lxqt{

namespace Wallet{

class secretService : public lxqt::Wallet::Wallet
{
	Q_OBJECT
public:
	secretService() ;
	~secretService() ;
	bool addKey( const QString& key,const QByteArray& value ) ;
	bool open( const QString& walletName,const QString& applicationName,const QString& password = QString() ) ;
	QByteArray readValue( const QString& key ) ;
	QVector<lxqt::Wallet::walletKeyValues> readAllKeyValues( void ) ;
	QStringList readAllKeys( void ) ;
	void deleteKey( const QString& key ) ;
	int walletSize( void )  ;
	void closeWallet( bool ) ;
	lxqt::Wallet::walletBackEnd backEnd( void ) ;
	bool walletIsOpened( void ) ;
	void setInterfaceObject( QObject * parent ) ;
	QObject * qObject( void ) ;
	QString storagePath( void ) ;
	void changeWalletPassWord( const QString& walletName,const QString& applicationName = QString() ) ;
	QStringList managedWalletList( void ) ;
signals:
	void walletIsOpen( bool ) ;
private slots:
	void walletOpened( bool ) ;
private:
	QByteArray m_byteArrayWalletName ;
	QByteArray m_byteArrayApplicationName ;
	QByteArray m_byteArraySchemaName ;

	const char * m_walletName ;
	const char * m_applicationName ;

	QString m_password ;
	QObject * m_interfaceObject ;

	void * m_schema   ;
	void * m_schema_1 ;
};

}

}

#endif // LXQT_KWALLET_H