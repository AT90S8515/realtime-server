#include <realtime_srv/RealtimeServer.h>

#ifdef IS_LINUX

#include "Character.h"
#include "ExampleRedisCli.h"
#include "CharacterLuaBind.h"


using namespace realtime_srv;


class ExampleSrvForUe4Demo : noncopyable
{
public:
	ExampleSrvForUe4Demo( bool willDaemonizeOnLinux = false )
		: server_( willDaemonizeOnLinux )
	{
		db_.Init( server_.GetEventLoop() );
	}

	void Run()
	{
		//	��Windows��,�����������:   ./example_for_ue4_demo.exe 0 0.3 0.8 1
		//	�����������ģ�� : 
		//	-	ģ���ӳ�Ϊ 0.3 : ��ǰ�ӳ�+300������ӳ�, ����ǰ�ӳ�Ϊ30ms,
		//			��ģ��֮����ӳ�ԼΪ30+300=330����
		//	-	ģ�ⶪ����Ϊ 0.8 : �ٷ�֮��ʮ�Ķ�����
		//	-	ģ�����綶��Ϊ : 1Ϊ�������, 0Ϊ������
		const uint8_t SimulateLatencyCmdIndex = 2;
		const uint8_t SimulateDropPacketChanceCmdIndex = 3;
		const uint8_t SimulateJitterCmdIndex = 4;

		server_.SimulateRealWorldOnWin(
			SimulateLatencyCmdIndex,
			SimulateDropPacketChanceCmdIndex,
			SimulateJitterCmdIndex );

		server_.Run( [&]( ClientProxyPtr cp ) { return SpawnNewCharacterForPlayer( cp ); } );
	}

	GameObjPtr SpawnNewCharacterForPlayer( ClientProxyPtr cliProxy )
	{
		db_.SaveNewPlayer( cliProxy->GetPlayerId(),
			cliProxy->GetPlayerName() );

		CharacterLuaBind clb;
		CharacterPtr newCharacter = clb.DoFile();
		newCharacter->SetPlayerId( cliProxy->GetPlayerId() );
		return  std::static_pointer_cast< GameObj >( newCharacter );
	}

private:
	RealtimeServer server_;
	ExampleRedisCli db_;
};





int main( int argc, const char** argv )
{
	RealtimeSrvHelper::SaveCommandLineArg( argc, argv );

	// ��Linux��, ����һ�������в��� 1 ���ɱ�Ϊ�ػ�����,   
	// ��:	   ./rs_example_for_ue4_demo  1
	// ���Ӽ�Ϊǰ̨����
	const uint8_t DaemonizeCmdIndexOnLinux = 1;
	bool willDaemonizeOnLinux = RealtimeSrvHelper::GetCommandLineArg(
		DaemonizeCmdIndexOnLinux ) == "1";

	ExampleSrvForUe4Demo exmaple_server( willDaemonizeOnLinux );
	exmaple_server.Run();

}

#endif // IS_LINUX