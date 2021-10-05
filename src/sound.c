#include "sound.h"

void loadMusic(char *filename)
{
    if (music != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }

    music = Mix_LoadMUS(filename);
}

void playMusic(int loop)
{
    Mix_PlayMusic(music, (loop) ? -1 : 0);
}

void playSound(int id, int channel)
{
    Mix_PlayChannel(channel, sounds[id], 0);
}

void loadSounds()
{
    if ((sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sound/334227__jradcoolness__laser.wav")) == NULL)
    {
        printf("Failed to load audio sound/334227__jradcoolness__laser.wav\n");
    }

    if ((sounds[SND_ENEMY_FIRE] = Mix_LoadWAV("sound/196914__dpoggioli__laser-gun.wav")) == NULL)
    {
        printf("Failed to load audio sound/196914__dpoggioli__laser-gun.wav\n");
    }

    if ((sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sound/245372__quaker540__hq-explosion.wav")) == NULL)
    {
        printf("Failed to load audio sound/245372__quaker540__hq-explosion.wav\n");
    }

    if ((sounds[SND_ENEMY_DIE] = Mix_LoadWAV("sound/10 Guage Shotgun-SoundBible.com-74120584.wav")) == NULL)
    {
        puts("Failed to load audio sound/10 Guage Shotgun-SoundBible.com-74120584.wav");
    }
}

void initSounds()
{
    memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

    music = NULL;

    loadSounds();

#ifdef __EMSCRIPTEN__
    loadMusic("sound/476556__magmi-soundtracks__sci-fi-music-loop-01.mp3");
#else
    loadMusic("sound/476556__magmi-soundtracks__sci-fi-music-loop-01.wav");
#endif
    playMusic(1);
}