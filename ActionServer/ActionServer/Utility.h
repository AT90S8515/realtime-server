
// �Ƿ�Ҫ��ʾ���Դ�ӡ��Ϣ
#define ACTION_SHOW_DEBUG_MESSAGE					true

namespace Utility
{
	string GetCommandLineArg( int inIndex );

	string Sprintf( const char* inFormat, ... );

	void	Log( const char* inFormat );
	void	Log( const char* inFormat, ... );
}

#define LOG( ... ) Utility::Log( __VA_ARGS__ );