#include "StdAfx.h"
#include "audio.h"
#include "enginelib.h"
#include <vector>

namespace game_engine
{
    AudioPlayer* AudioPlayer::instance = new AudioPlayer();

    AudioPlayer::AudioPlayer()
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
        instance = this;

        volumeMusic = 1;
        volumeSound = 1;
    }

    AudioPlayer::~AudioPlayer()
    {
        ReleaseBuffer();
        instance->m_pSystem->release();
    }

    void AudioPlayer::ParseSoundJSON(json j)
    {
        for (json::iterator it = j.begin(); it != j.end(); it++)
            initSound(it.key(), it.value());
    }

    void AudioPlayer::ParseMusicJSON(json j)
    {
        for (json::iterator it = j.begin(); it != j.end(); it++)
            initStream(it.key(), it.value());
    }

    void AudioPlayer::initSound(string name, string filename)
    {
        AudioSource *audioSource = new AudioSource();

        auto rstate = instance->m_pSystem->createSound((R"(.\Assest\Sound\)" + filename).c_str(), FMOD_DEFAULT, 0, &audioSource->soundSource);

        GAME_LOG(rstate == FMOD_RESULT::FMOD_OK, "FMOD : Init Sound ERROR");

        audioSource->sourceMode = AudioMode::Sound;

        fileMap[name] = filename;
        sourceMap[filename] = audioSource;
    }

    void AudioPlayer::initStream(string name, string filename)
    {
        AudioSource *audioSource = new AudioSource();

        auto rstate = instance->m_pSystem->createStream((R"(.\Assest\Music\)" + filename).c_str(), FMOD_DEFAULT, 0, &audioSource->soundSource);

        GAME_LOG(rstate == FMOD_RESULT::FMOD_OK, "FMOD : Init Stream ERROR");

        audioSource->sourceMode = AudioMode::Stream;

        fileMap[name] = filename;
        sourceMap[filename] = audioSource;
    }

    void AudioPlayer::Play(AudioSource* sound, bool repeat)
    {
        if (!repeat)
            sound->soundSource->setMode(FMOD_LOOP_OFF);
        else
        {
            sound->soundSource->setMode(FMOD_LOOP_NORMAL);
            sound->soundSource->setLoopCount(-1);
        }

        FMOD::Channel* channel = nullptr;
        instance->m_pSystem->playSound(sound->soundSource, nullptr, false, &channel);
        sound->channel = channel;
    }

    void AudioPlayer::ReleaseSound(string name)
    {
        string filename = fileMap[name];
        auto rstate = sourceMap[filename]->soundSource->release();

        GAME_LOG(rstate == FMOD_RESULT::FMOD_OK, "FMOD RELEASE ERROR");

        delete sourceMap[filename];
        sourceMap.erase(filename);
        fileMap.erase(name);
    }

    void AudioPlayer::ReleaseBuffer()
    {
        vector<string> namelist;

        for (auto pair : fileMap)
            namelist.push_back(pair.first);

        for (auto name : namelist)
            ReleaseSound(name);
    }

    void AudioPlayer::Crossfade(AudioSource *fadeoutSource, AudioSource *fadeinSource, float time, bool forcePlay, float volume)
    {
        fadeoutSource->FadeOut(time);
        fadeinSource->FadeIn(time, forcePlay, volume == -1 ? AudioPlayer::instance->volumeMusic : volume);
    }

    AudioSource* AudioPlayer::GetSource(string name)
    {
        GAME_LOG(instance->fileMap.find(name) != instance->fileMap.end(), "Sound File Not Found : " + name);
        return instance->sourceMap[instance->fileMap[name]];
    }

    AudioSource::AudioSource()
    {
        pause = false;
        channel = nullptr;
    }

    void AudioSource::Pause()
    {
        pause = true;
        channel->setPaused(true);
    }

    void AudioSource::Stop()
    {
        channel->stop();
        channel = nullptr;
    }

    void AudioSource::Fade(float volumeStart, float volumeEnd, float time)
    {
        int rate;
        AudioPlayer::instance->m_pSystem->getSoftwareFormat(&rate, 0, 0);
        unsigned long long dspclock;
        channel->getDSPClock(0, &dspclock);
        channel->addFadePoint(dspclock, clamp(volumeStart, 0, 1));
        channel->addFadePoint(dspclock + rate * time, clamp(volumeEnd, 0, 1));
    }

    void AudioSource::SetVolume(float vol)
    {
        channel->setVolume(clamp(vol, 0, 1));
    }

    void AudioSource::SetPitch(float pitch)
    {
        channel->setPitch(pitch);
    }

    void AudioSource::FadeIn(float time, bool forcePlay, float volume)
    {
        float iv = volume == -1 ? AudioPlayer::instance->volumeMusic : volume;
        Play(forcePlay, iv);
        Fade(0, 1, time);
    }

    void AudioSource::FadeOut(float time)
    {
        Fade(1, 0, time);
    }

    void AudioSource::Play(bool forcePlay, float volume)
    {
        if (channel == nullptr || forcePlay)
        {
            if (forcePlay && channel != nullptr)
                Stop();

            AudioPlayer::instance->Play(this, true);

            if (volume == -1)
            {
                if (this->sourceMode == AudioMode::Sound)
                    SetVolume(AudioPlayer::instance->volumeSound);
                else if (this->sourceMode == AudioMode::Stream)
                    SetVolume(AudioPlayer::instance->volumeMusic);
            }
            else
                SetVolume(volume);
        }
        else
        {
            pause = false;
            channel->setPaused(false);
        }
    }

    void AudioSource::PlayOneShot(bool forcePlay, float volume)
    {
        AudioPlayer::instance->Play(this, false);

        if (volume == -1)
        {
            if (this->sourceMode == AudioMode::Sound)
                SetVolume(AudioPlayer::instance->volumeSound);
            else if (this->sourceMode == AudioMode::Stream)
                SetVolume(AudioPlayer::instance->volumeMusic);
        }
        else
            SetVolume(volume);

        channel->setPaused(false);
    }
}