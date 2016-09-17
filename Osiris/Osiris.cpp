#include <iostream>
#include <iomanip>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShlObj.h>

#include "Osiris.hpp"

#include "Modules/OsirisModules.hpp"

#include <Utils/Utils.hpp>

#include <Ausar/Ausar.hpp>

#include <vector>
#include <iterator>

Osiris::Osiris()
{
    wchar_t Buffer[MAX_PATH] = { 0 };
    SHGetSpecialFolderPathW(nullptr, Buffer, CSIDL_PROFILE, false);

    std::wstring UserFolder(Buffer);
    UserFolder += L"\\AppData\\Local\\Packages\\Microsoft.Halo5Forge_8wekyb3d8bbwe\\LocalState\\Log.txt";

    Util::Log::Instance()->SetFile(UserFolder);

    LOG << "Osiris" << "---- ";
    LOG << '[' << __DATE__ << " : " << __TIME__ << ']' << std::endl;
    LOG << "\t-https://github.com/Wunkolo/Osiris\n";
    LOG << std::wstring(80, '-') << std::endl;

    LOG << std::hex << std::uppercase << std::setfill(L'0')
        << "Process Base: 0x" << Util::Process::Base() << std::endl;
    LOG << "Osiris Thread ID: 0x" << Util::Thread::GetCurrentThreadId() << std::endl;
    LOG << "Osiris Base: 0x" << Util::Process::GetModuleBase("Osiris.dll") << std::endl;

    const Ausar::ThreadTable *Table;

    Table = Util::Process::GetModuleBase()(0x58CA4B0).Point<Ausar::ThreadTable>();

    //for( size_t i = 0; i < 64; i++ )
    //{
    //    if( Table->Entries[i].Active == 0 )
    //    {
    //        continue;
    //    }
    //    LOG << "Thread Name: " << reinterpret_cast<const char*>(Table->Entries[i].ThreadName) << std::endl;
    //    LOG << "Thread ID: " << Table->Entries[i].ThreadID << std::endl;
    //}

    uint64_t ThreadID = Table->GetThreadIDByName("MAIN");

    LOG << "Main Thread ID:  0x" << ThreadID << std::endl;

    Util::Pointer Tls;

    if( Util::Thread::GetThreadLocalStorage(ThreadID, 0, Tls) )
    {
        LOG << "Tls Globals: " << Tls << std::endl;

        LOG << "Physics Constants: " << Tls[0x2D30] << std::endl;
        LOG << "userGraphicsScalingOptions: " << Tls[0x3050] << std::endl;

        LOG << "random math: " << Tls[0x2C28] << std::endl;
        LOG << "incident globals: " << Tls[0x2C38] << std::endl;

        LOG << "DOF Globals: " << Tls[0x49B0] << std::endl;
        LOG << "DOF Data: " << Tls[0x1310] << std::endl;
        LOG << "Director globals: " << Tls[0x198] << std::endl;
        LOG << "Hue saturation control: " << Tls[0x2FF8] << std::endl;
        LOG << "Game engine globals: " << Tls[0x13A8] << std::endl;
        LOG << "Local Game engine globals: " << Tls[0x13B0] << std::endl;
        LOG << "Game engine render globals: " << Tls[0x13B8] << std::endl;
        LOG << "Game time globals: " << Tls[0x12A8] << std::endl;
        LOG << "game_state_header: " << Tls[0x1280] << std::endl;
        LOG << "Composer globals: " << Tls[0x1C8] << std::endl;
        LOG << "Fp weapons: " << Tls[0x1260] << std::endl;

        LOG << "Player Focus: " << Tls[0x1320] << std::endl;
        LOG << "Player Control Globals: " << Tls[0x1340] << std::endl;
        LOG << "Player Control Globals Deter.: " << Tls[0x1348] << std::endl;
        LOG << "Player Globals: " << Tls[0x1370] << std::endl;
        LOG << "Player Mapping Globals: " << Tls[0x1350] << std::endl;

        LOG << "Player Data: " << Tls[0x1368] << std::endl;

        LOG << "AI Globals: " << Tls[0x2E40] << std::endl;
        LOG << "AI Player state Globals: " << Tls[0x2E18] << std::endl;

        LOG << "Interaction ripples: " << Tls[0x4960] << std::endl;

        LOG << "Rasterizer: " << Tls[0x49A0] << std::endl;
        LOG << "Render game globals: " << Tls[0x49A8] << std::endl;
        LOG << "Render texture globals: " << Tls[0x3058] << std::endl;
        LOG << "atmosphere override settings: " << Tls[0x4998] << std::endl;
        LOG << "fp orientations: " << Tls[0x4A10] << std::endl;

        LOG << "Objects: " << Tls[0x4B18] << std::endl;
        LOG << "Object name list: " << Tls[0x4B20] << std::endl;
        LOG << "Object placement globals: " << Tls[0x4B58] << std::endl;
        LOG << "Object globals: " << Tls[0x4C08] << std::endl;
        LOG << "orientations: " << Tls[0x110] << std::endl;
    }

    // Push Commands
    //PushModule<Research>("research");
    //PushModule<Player>("player");
}

Osiris::~Osiris()
{
}

void Osiris::Tick(const std::chrono::high_resolution_clock::duration &DeltaTime)
{
    for( std::pair<std::string, std::shared_ptr<OsirisModule>> Command : Commands )
    {
        if( Command.second )
        {
            Command.second->Tick(DeltaTime);
        }
    }
}