// Audio playback, based on the following code:

/*
 * Simple sound playback using ALSA API and libasound.
 *
 * Compile:
 * $ cc -o play sound_playback.c -lasound
 * 
 * Usage:
 * $ ./play <sample_rate> <channels> <seconds> < <file>
 * 
 * Examples:
 * $ ./play 44100 2 5 < /dev/urandom
 * $ ./play 22050 1 8 < /path/to/file.wav
 *
 * Copyright (C) 2009 Alessandro Ghedini <alessandro@ghedini.me>
 * --------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Alessandro Ghedini wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we
 * meet some day, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * --------------------------------------------------------------
 */

#include <stdio.h>
#include "audio/audioout.h"
#include <alsa/asoundlib.h>

#define PCM_DEVICE "default"

#include <alsa/asoundlib.h>


class AudioOutAlsa : public IAudioOut
{
 public:
  AudioOutAlsa() {
    pcm = tmp = dir = 0;
    rate = 16000;
    channels = 1;
    seconds = 100000;
    pcm_handle = NULL;
    params = NULL;
    frames = 0;
    buff = NULL;
    buff_size = 0;
    loops = 0;
  }

  virtual ~AudioOutAlsa() {
    Stop();
  }

  virtual void PlayWav(const char * fileName);

  virtual void Start();
  virtual void AddBuffer(char * buff);
  virtual void Stop();
  virtual int GetBufferSize();

  virtual void SetSampleRate(int i) {
    rate = i;
  }
  virtual void SetChannels(int i) {
    channels = i;
  }
  

  

 private:

  unsigned int pcm, tmp, dir;
  unsigned int rate;
  int channels, seconds;
  snd_pcm_t *pcm_handle;
  snd_pcm_hw_params_t *params;
  snd_pcm_uframes_t frames;
  char *buff;
  int buff_size, loops;
};

IAudioOut * CreateAudio() {
  return new AudioOutAlsa;
}

void DeleteAudio(IAudioOut * p)
{
  delete p;
}

//============================================================
void AudioOutAlsa::Stop() {
  snd_pcm_drain(pcm_handle);
  snd_pcm_close(pcm_handle);
  if (buff != NULL)
    free(buff);
  buff = NULL;
}

void AudioOutAlsa::Start() {
  /* Open the PCM device in playback mode */
  if (pcm != 0)
    Stop();

  if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
			 SND_PCM_STREAM_PLAYBACK, 0) < 0) 
    printf("ERROR: Can't open \"%s\" PCM device. %s\n",
	   PCM_DEVICE, snd_strerror(pcm));
  
  /* Allocate parameters object and fill it with default values*/
  snd_pcm_hw_params_alloca(&params);
  
  snd_pcm_hw_params_any(pcm_handle, params);
  
  /* Set parameters */
  if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
					 SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
    printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));
  
  if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
					 SND_PCM_FORMAT_S16_LE) < 0) 
    printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));
  
  if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0) 
    printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));
  
  if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0) 
    printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));
  
  /* Write parameters */
  if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0)
    printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));
  
  /* Resume information */
  printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));
  
  printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));
  
  snd_pcm_hw_params_get_channels(params, &tmp);
  printf("channels: %i ", tmp);
  
  if (tmp == 1)
    printf("(mono)\n");
  else if (tmp == 2)
    printf("(stereo)\n");
  
  snd_pcm_hw_params_get_rate(params, &tmp, 0);
  printf("rate: %d bps\n", tmp);
  
  printf("seconds: %d\n", seconds);	

  /* Allocate buffer to hold single period */
  snd_pcm_hw_params_get_period_size(params, &frames, 0);


  buff_size = frames * channels * 2 /* 2 -> sample size */;
  if (buff != NULL)
    free(buff);

  printf("Frames: %d, buffer size: %d\n",  frames,  buff_size);

  buff = (char *) malloc(buff_size);

  snd_pcm_hw_params_get_period_time(params, &tmp, NULL);
}


int AudioOutAlsa::GetBufferSize()
{
  return buff_size;
}


void AudioOutAlsa::PlayWav(const char * fileName)
{
  Start();
  FILE * p = fopen(fileName, "rb");

  
  while (fread(buff, buff_size, 1, p) == 1) {  
    AddBuffer(buff);
  }
  fclose(p);
  Stop();
}

void AudioOutAlsa::AddBuffer(char * buff)
{
  if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
    printf("XRUN.\n");
    snd_pcm_prepare(pcm_handle);
  } else if (pcm < 0) {
    printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
  }

}
