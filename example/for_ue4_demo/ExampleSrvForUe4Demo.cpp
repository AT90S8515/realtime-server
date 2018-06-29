#include <realtime_srv/RealtimeServer.h>
#include "Character.h"

using namespace realtime_srv;


class ExampleSrvForUe4Demo : noncopyable
{
public:
	ExampleSrvForUe4Demo( bool willDaemonizeOnLinux = false )
		: server_( willDaemonizeOnLinux ) {}

	void Run()
	{
//	��Windows��, �����������:   ./example_for_ue4_demo.exe 0 0.3 0.8 1
//	�����������ģ�� : 
//	-	ģ���ӳ�Ϊ : ��ǰ�ӳ�+300������ӳ�, ����ǰ�ӳ�Ϊ30ms,
//			��ģ��֮����ӳ�ԼΪ30+300=330����
//	-	ģ�ⶪ����Ϊ : �ٷ�֮��ʮ�Ķ�����
//	-	ģ�����綶��Ϊ : 1Ϊ�������, 0Ϊ������
		const uint8_t SimulateLatencyCmdIndexOnWindows = 2;
		const uint8_t SimulateDropPacketChanceCmdIndexOnWindows = 3;
		const uint8_t SimulateJitterCmdIndexOnWindows = 4;

		server_.SimulateRealWorldOnWindows(
			SimulateLatencyCmdIndexOnWindows,
			SimulateDropPacketChanceCmdIndexOnWindows,
			SimulateJitterCmdIndexOnWindows );

		server_.Run( [&]() { return SpawnNewCharacterForPlayer(); } );
	}

	GameObjPtr SpawnNewCharacterForPlayer()
	{
		GameObjPtr newGameObj = Character::StaticCreate();
		CharacterPtr character = std::static_pointer_cast< Character >( newGameObj );

		character->SetLocation( Vector3(
			2500.f + RealtimeSrvMath::GetRandomFloat() * -5000.f,
			2500.f + RealtimeSrvMath::GetRandomFloat() * -5000.f,
			0.f ) );
		character->SetRotation( Vector3(
			0.f,
			RealtimeSrvMath::GetRandomFloat() * 180.f,
			0.f ) );
		return newGameObj;
	}

private:
	RealtimeServer server_;
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