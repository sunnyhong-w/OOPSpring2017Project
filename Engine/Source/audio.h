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

		FMOD::System *m_pSystem;

		AudioPlayer();
		~AudioPlayer();

		void ParseSoundJSON(json j);
		void ParseMusicJSON(json j);
		void ReleaseBuffer();


	private:
		void initSound(string name, string filename);
		void initStream(string name, string filename);
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

		void Play();
		void PlayOneShot();
		void Pause();
		void Stop();
		void SetVolume(float volume);
		void SetPitch(float pitch);
	};
}