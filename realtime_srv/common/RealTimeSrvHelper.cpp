#include "realtime_srv/common/RealtimeSrvShared.h"


#if !_WIN32
extern const char** __argv;
extern int __argc;
void OutputDebugString( const char* inString )
{
	printf( "%s", inString );
}
#endif

std::string RealtimeSrvHelper::GetCommandLineArg( int inIndex )
{
	if (inIndex < __argc)
	{
		return std::string( __argv[inIndex] );
	}

	return std::string();
}


std::string RealtimeSrvHelper::Sprintf( const char* inFormat, ... )
{
	//not thread safe...
	static char temp[4096];

	va_list args;
	va_start( args, inFormat );

#if _WIN32
	_vsnprintf_s( temp, 4096, 4096, inFormat, args );
#else
	vsnprintf( temp, 4096, inFormat, args );
#endif
	return std::string( temp );
}

// void RealTimeSrvHelper::Log( const char* inFormat )
// {
// 	OutputDebugString( inFormat );
// 	OutputDebugString( "\n" );
// }

void RealtimeSrvHelper::Log( const char* inFormat, ... )
{
	if ( !REAL_TIME_SRV_SHOW_DEBUG_MESSAGE )
		return;

	//not thread safe...
	static char temp[4096];

	va_list args;
	va_start( args, inFormat );

#if _WIN32
	_vsnprintf_s( temp, 4096, 4096, inFormat, args );
#else
	vsnprintf( temp, 4096, inFormat, args );
#endif
	OutputDebugString( temp );
	OutputDebugString( "\n" );
}




bool RealtimeSrvHelper::SequenceGreaterThanOrEqual( PacketSN s1, PacketSN s2 )
{
	return ( ( s1 >= s2 ) && ( s1 - s2 <= HALF_MAX_PACKET_SEQUENCE_NUMBER ) ) ||
		( ( s1 < s2 ) && ( s2 - s1 > HALF_MAX_PACKET_SEQUENCE_NUMBER ) );
	//return s1 >= s2;
}


bool RealtimeSrvHelper::SequenceGreaterThan( PacketSN s1, PacketSN s2 )
{
	return ( ( s1 > s2 ) && ( s1 - s2 <= HALF_MAX_PACKET_SEQUENCE_NUMBER ) ) ||
		( ( s1 < s2 ) && ( s2 - s1 > HALF_MAX_PACKET_SEQUENCE_NUMBER ) );
	//return s1 > s2;
}

bool RealtimeSrvHelper::ChunkPacketIDGreaterThanOrEqual( ChunkPacketID s1, ChunkPacketID s2 )
{
	return ( ( s1 >= s2 ) && ( s1 - s2 <= HALF_MAX_CHUNK_PACKET_ID ) ) ||
		( ( s1 < s2 ) && ( s2 - s1 > HALF_MAX_CHUNK_PACKET_ID ) );
}


bool RealtimeSrvHelper::ChunkPacketIDGreaterThan( ChunkPacketID s1, ChunkPacketID s2 )
{
	return ( ( s1 > s2 ) && ( s1 - s2 <= HALF_MAX_CHUNK_PACKET_ID ) ) ||
		( ( s1 < s2 ) && ( s2 - s1 > HALF_MAX_CHUNK_PACKET_ID ) );
}

#ifdef IS_LINUX
int RealtimeSrvHelper::BecomeDaemon()
{
	int maxfd, fd;

	switch ( fork() )
	{                   /* Become background process */
	case -1:
		return -1;
	case 0:
		break;                     /* Child falls through... */
	default:
		_exit( EXIT_SUCCESS );       /* while parent terminates */
	}

	if ( setsid() == -1 )                 /* Become leader of new session */
		return -1;

	switch ( fork() )
	{                   /* Ensure we are not session leader */
	case -1:
		return -1;
	case 0:
		break;
	default:
		_exit( EXIT_SUCCESS );
	}

	umask( 0 );                       /* Clear file mode creation mask */

	chdir( "/" );                     /* Change to root directory */

	maxfd = sysconf( _SC_OPEN_MAX );
	if ( maxfd == -1 )                /* Limit is indeterminate... */
		maxfd = 8192;				  /* so take a guess */

	for ( fd = 0; fd < maxfd; fd++ )
		close( fd );

	close( STDIN_FILENO );            /* Reopen standard fd's to /dev/null */

	fd = open( "/dev/null", O_RDWR ); // open ���ص��ļ�������һ������С��δ��ʹ�õ���������

	if ( fd != STDIN_FILENO )         /* 'fd' should be 0 */
		return -1;
	if ( dup2( STDIN_FILENO, STDOUT_FILENO ) != STDOUT_FILENO )
		return -1;
	if ( dup2( STDIN_FILENO, STDERR_FILENO ) != STDERR_FILENO )
		return -1;

	return 0;
}
#endif //IS_LINUX