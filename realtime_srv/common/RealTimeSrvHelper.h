
// �Ƿ�Ҫ��ʾ���Դ�ӡ��Ϣ
#define REAL_TIME_SRV_SHOW_DEBUG_MESSAGE					true

namespace RealtimeSrvHelper
{
	void SaveCommandLineArg( const int argc, const char** argv );
	string GetCommandLineArg( int inIndex );

	string Sprintf( const char* inFormat, ... );

	void	Log( const char* inFormat );
	void	Log( const char* inFormat, ... );

	bool	SequenceGreaterThanOrEqual( PacketSN s1, PacketSN s2 );
	bool	SequenceGreaterThan( PacketSN s1, PacketSN s2 );

	bool ChunkPacketIDGreaterThanOrEqual( ChunkPacketID s1, ChunkPacketID s2 );
	bool ChunkPacketIDGreaterThan( ChunkPacketID s1, ChunkPacketID s2 );

	bool BecomeDaemon();
}

#define LOG( ... ) RealtimeSrvHelper::Log( __VA_ARGS__ );