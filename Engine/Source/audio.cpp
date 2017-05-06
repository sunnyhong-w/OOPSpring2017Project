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
	}

	AudioPlayer::~AudioPlayer()
	{
		ReleaseBuffer();
		instance->m_pSystem->release();
	}

	void AudioPlayer::ParseSoundJSON(json j)
	{
		for (json::iterator it = j.begin(); it != j.end(); it++)
			initSound(it.key(),it.value());
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

	void AudioSource::Play()
	{
		if(channel == nullptr)
			AudioPlayer::instance->Play(this, true);
		else
		{
			pause = false;
			channel->setPaused(false);
		}
	}

	void AudioSource::PlayOneShot()
	{
		if (channel == nullptr)
			AudioPlayer::instance->Play(this, false);
		else
		{
			pause = false;
			channel->setPaused(false);
		}

	}
}