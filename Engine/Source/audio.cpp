#include "StdAfx.h"
#include "audio.h"

namespace game_engine
{
    SoundSystemClass* SoundSystemClass::instance;
    FMOD::System *SoundSystemClass::m_pSystem;
    map<string, SoundSystemClass::SoundClass> SoundSystemClass::soundmap;

    SoundSystemClass::SoundSystemClass()
    {
        if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
        {
            // Report Error
            return;
        }

        int driverCount = 0;
        m_pSystem->getNumDrivers(&driverCount);

        if (driverCount == 0)
        {
            // Report Error
            return;
        }

        // Initialize our Instance with 36 Channels
        m_pSystem->init(36, FMOD_INIT_NORMAL, nullptr);
    }

    void SoundSystemClass::insSound(string name)
    {
        SoundClass sc;
        string path = R"(.\Assest\Sounds\)" + name;
        createSound(&sc, path.c_str());
        soundmap[name] = sc;
    }

    void SoundSystemClass::rmSound(string name)
    {
        releaseSound(soundmap[name]);
        soundmap.erase(name);
    }

    void SoundSystemClass::play(string name, bool bLoop)
    {
        if (soundmap.find(name) != soundmap.end())
            playSound(soundmap[name], bLoop);
    }

    void SoundSystemClass::createSound(SoundClass * pSound, const char * pFile)
    {
        FMOD_RESULT fr = m_pSystem->createSound(pFile, FMOD_OPENMEMORY, nullptr, pSound);

        if (fr != FMOD_RESULT::FMOD_OK)
        {
            TRACE("123");
        }
    }

    void SoundSystemClass::playSound(SoundClass pSound, bool bLoop)
    {
        if (!bLoop)
            pSound->setMode(FMOD_LOOP_OFF);
        else
        {
            pSound->setMode(FMOD_LOOP_NORMAL);
            pSound->setLoopCount(-1);
        }

        FMOD::Channel *channel;
        m_pSystem->getChannel(0, &channel);

        m_pSystem->playSound(pSound, nullptr, false, &channel);
    }

    void SoundSystemClass::releaseSound(SoundClass pSound)
    {
        pSound->release();
    }
}