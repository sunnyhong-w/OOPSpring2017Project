#pragma once

#include<fmod.hpp>
#include"json_include.h"
#include<map>

using namespace std;
namespace game_engine
{
	class AudioPlayer
	{
	public:
		friend class AudioSource;

		typedef FMOD::Sound* FMODSound;

		static AudioPlayer* instance;
		static AudioSource* GetSource(string name);
        static void Crossfade(AudioSource *fadeoutSource, AudioSource *fadeinSource, float time, bool forcePlay = false, float volume = -1);

		FMOD::System *m_pSystem;

		AudioPlayer();
		~AudioPlayer();

		void ParseSoundJSON(json j);
		void ParseMusicJSON(json j);
        void ReleaseBuffer();

        static void SetMusicVolume(float v);
        static void SetSoundVolume(float v);

	private:
		void initSound(string name, string filename);
		void initStream(string name, string filename, unsigned int loopstart = -1, unsigned int loopend = -1);
		void Play(AudioSource *sound, bool repeat);
		void ReleaseSound(string name);

		static void GAME_LOG(bool boolexp,string str)
		{
			if (!(boolexp)) {
				int id;
				char s[300] = "";
				sprintf(s, "Game fatal error:\n\n%s\n\nFile: %s\n\nLine: %d"
					"\n\n(Press Retry to debug the application, "
					"if it is executed in debug mode.)"
					"\n(Press Cancel otherwise.)",
					str.c_str(), __FILE__, __LINE__);
				id = AfxMessageBox(s, MB_RETRYCANCEL);
				if (id == IDCANCEL)
					exit(1);
				AfxDebugBreak();
			}
		}

		map<string, string> fileMap;
		map<string, AudioSource*> sourceMap;

        float volumeMusic;
        float volumeSound;
	};

	enum class  AudioMode : int { Sound, Stream };

	class AudioSource
	{
	public:
		AudioSource();

		FMOD::Sound* soundSource;
		AudioMode sourceMode;
		FMOD::Channel* channel;
		bool pause;

		void Play(bool forcePlay = false, float volume = -1);
		void PlayOneShot(bool forcePlay = false, float volume = -1);
		void Pause();
		void Stop();
        void Fade(float volumeStart, float volumeEnd, float time);
		void SetVolume(float volume);
		void SetPitch(float pitch);
        void FadeIn(float time, bool forcePlay = false, float volume = -1);
        void FadeOut(float time);
	};
}