 
 /*
  * 
  *  Copyright (c) 2013
  *  name : mhogo mchungu 
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
 
#include "includes.h"
#include <sys/syscall.h>

/*
 * this header is created at config time
 */
#include "truecrypt_support_1.h"

#if TRUECRYPT_CREATE

static int _create_file_system( const char * device,const char * fs,
				const char * key,size_t key_len,int volume_type )
{	
	string_t m = StringVoid ;
	
	int r ;
	
	const char * device_mapper ;
	const char * mapper ;
	
	size_t len ;
	
	m = String( crypt_get_dir() ) ;
	len = StringLength( m )   ;
	
	StringAppend( m,"/zuluCrypt-" ) ;
	device_mapper = StringAppendInt( m,syscall( SYS_gettid ) ) ;
	mapper = device_mapper + len + 1 ;
	
	/*
	 * zuluCryptOpenTcrypt() is defined in open_tcrypt.c
	 */
	if( zuluCryptOpenTcrypt( device,mapper,key,key_len,TCRYPT_PASSPHRASE,volume_type,NULL,0,0,NULL ) == 0 ){
		/*
		 * zuluCryptCreateFileSystemInAVolume() is defined in create_volume.c
		 */
		if( zuluCryptCreateFileSystemInAVolume( fs,device_mapper ) == 0 ){
			r = 0 ;
		}else{
			r = 3 ;
		}
		/*
		 * zuluCryptCloseMapper() is defined in close_mapper.c
		 */
		zuluCryptCloseMapper( device_mapper );
	}else{
		r = 3 ;
	}
	
	StringDelete( &m ) ;
	return r ;
}

static int _create_file_system_1( const char * device,const char * fs,
				const char * keyfile,int volume_type )
{	
	string_t m = StringVoid ;
	
	int r ;
	
	const char * device_mapper ;
	const char * mapper ;
	
	size_t len ;
	
	m = String( crypt_get_dir() ) ;
	len = StringLength( m )   ;
	
	StringAppend( m,"/zuluCrypt-" ) ;
	device_mapper = StringAppendInt( m,syscall( SYS_gettid ) ) ;
	mapper = device_mapper + len + 1 ;
	
	/*
	 * zuluCryptOpenTcrypt() is defined in open_tcrypt.c
	 */
	if( zuluCryptOpenTcrypt( device,mapper,keyfile,0,TCRYPT_KEYFILE_FILE,volume_type,NULL,0,0,NULL ) == 0 ){
		/*
		 * zuluCryptCreateFileSystemInAVolume() is defined in create_volume.c
		 */
		if( zuluCryptCreateFileSystemInAVolume( fs,device_mapper ) == 0 ){
			r = 0 ;
		}else{
			r = 3 ;
		}
		/*
		 * zuluCryptCloseMapper() is defined in close_mapper.c
		 */
		zuluCryptCloseMapper( device_mapper );
	}else{
		r = 3 ;
	}
	
	StringDelete( &m ) ;
	return r ;
}

static int _create_tcrypt_volume( const char * device,const char * file_system,
				  const char * rng,const char * key,size_t key_len,
				  int key_source,int volume_type )
{
	string_t st = StringVoid ;
	
	tc_api_opts api_opts ;
	
	struct stat statstr ;
	
	int r ;
	int fd ;
	
	const char * keyfiles[ 2 ] ;
	const char * file = NULL ;
	
	keyfiles[ 0 ] = NULL ;
	keyfiles[ 1 ] = NULL ;
		
	if ( zuluCryptPathIsNotValid( device ) ){
		return 1 ;
	}
	
	memset( &api_opts,'\0',sizeof( api_opts ) ) ;
	
	api_opts.tc_device          = device ;
	api_opts.tc_cipher          = "AES-256-XTS";
	api_opts.tc_prf_hash        = "RIPEMD160"  ;
	api_opts.tc_no_secure_erase = 1 ;
	
	if( StringPrefixMatch( rng,"/dev/urandom",12 ) ){
		api_opts.tc_use_weak_keys = 1 ;
	}
	
	if( key_source == TCRYPT_PASSPHRASE ){
		
		api_opts.tc_passphrase  = key ;
		
		if( tc_api_init( 0 ) == TC_OK ){
			r = tc_api_create_volume( &api_opts );
			tc_api_uninit() ;
			if( r == TC_OK ){
				r = _create_file_system( device,file_system,key,key_len,volume_type ) ;
			}else{
				r = 3 ;
			}
		}else{
			r = 3 ;
		}
	}else{
		if( stat( "/run",&statstr ) != 0 ){
			mkdir( "/run",S_IRWXU ) ;
			chown( "/run",0,0 ) ;
		}
		if( stat( "/run/zuluCrypt",&statstr ) != 0 ){
			mkdir( "/run/zuluCrypt",S_IRWXU ) ;
			chown( "/run/zuluCrypt",0,0 ) ;
		}
		
		st = String( "/run/zuluCrypt/create_tcrypt-" ) ;
		file = StringAppendInt( st,syscall( SYS_gettid ) ) ;
		fd = open( file,O_WRONLY|O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP |S_IROTH ) ;
		
		if( fd == -1 ){
			r = 3 ;
		}else{
			write( fd,key,key_len ) ;
			close( fd ) ;
		
			chown( file,0,0 ) ;
			chmod( file,S_IRWXU ) ;
		
			api_opts.tc_keyfiles = keyfiles ;
			keyfiles[ 0 ] = file ;
		
			if( tc_api_init( 0 ) == TC_OK ){
				r = tc_api_create_volume( &api_opts );
				tc_api_uninit() ;
				if( r == TC_OK ){
					r = _create_file_system_1( device,file_system,file,volume_type ) ;
				}else{
					r = 3 ;
				}
			}else{
				r = 3 ;
			}
			
			unlink( file ) ;
		}
			
		StringDelete( &st ) ;
	}
	
	return r ;
}

int zuluCryptCreateTCrypt( const char * device,const char * file_system,const char * rng,
			   const char * key,size_t key_len,int key_source,int volume_type )
{
	int fd ;
	string_t q = StringVoid ;
	int r ;
	if( StringPrefixMatch( device,"/dev/",5 ) ){
		r = _create_tcrypt_volume( device,file_system,rng,key,key_len,key_source,volume_type ) ;
	}else{
		/*
		 * zuluCryptAttachLoopDeviceToFile() is defined in create_loop_device.c
		 */
		if( zuluCryptAttachLoopDeviceToFile( device,O_RDWR,&fd,&q ) ){
			device = StringContent( q ) ;
			r = _create_tcrypt_volume( device,file_system,rng,key,key_len,key_source,volume_type ) ;
			close( fd ) ;
			StringDelete( &q ) ;
		}else{
			r = 3 ;
		}
	}
	
	return r ;
}

#else
int zuluCryptCreateTCrypt( const char * device,const char * file_system,const char * rng,
			   const char * key,size_t key_len,int key_source,int volume_type )
{
	if( 0 && device && file_system && rng && key_source && key && volume_type && key_len ){;}	
	return 3 ;
}
#endif