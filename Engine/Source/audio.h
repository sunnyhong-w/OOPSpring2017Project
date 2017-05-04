#pragma once

#include<fmod.hpp>
#include<map>

using namespace std;
namespace game_engine
{
    class SoundSystemClass
    {
        typedef FMOD::Sound* SoundClass;

    public:
        // Pointer to the FMOD instance
        static SoundSystemClass* instance;
        static FMOD::System *m_pSystem;
        static map<string, SoundClass> soundmap;

        static SoundSystemClass* Instance()
        {
            return instance;
        }

        SoundSystemClass();

        ~SoundSystemClass()
        {
            for (auto pair : soundmap)
                releaseSound(pair.second);

            m_pSystem->close();
        }

        void insSound(string name);
        void rmSound(string name);
        void play(string name, bool bLoop = false);

        void createSound(SoundClass *pSound, const char* pFile);
        void playSound(SoundClass pSound, bool bLoop = false);
        void releaseSound(SoundClass pSound);
    };

}