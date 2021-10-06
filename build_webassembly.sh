emcc \
    src/main.cpp \
    src/input.c \
    src/draw.c \
    src/init.c \
    src/stage.c \
    src/utils.c \
    src/sound.c \
    src/text.c \
    --preload-file gfx/player.png \
    --preload-file gfx/playerBullet.png \
    --preload-file gfx/enemy.png \
    --preload-file gfx/enemyBullet.png \
    --preload-file gfx/explosion.png \
    --preload-file gfx/background.jpg \
    --preload-file gfx/kromasky_16x16.png \
    --preload-file gfx/star_coin_normal_30x30.png \
    --preload-file sound/196914__dpoggioli__laser-gun.wav \
    --preload-file sound/245372__quaker540__hq-explosion.wav \
    --preload-file sound/334227__jradcoolness__laser.wav \
    --preload-file "sound/10 Guage Shotgun-SoundBible.com-74120584.wav" \
    --preload-file "sound/324252__rhodesmas__rings-of-saturn-music-loop.wav" \
    --preload-file "sound/342749__rhodesmas__notification-01.wav" \
    -s WASM=1 \
    -s MIN_WEBGL_VERSION=2 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s USE_SDL=2 \
    -s USE_SDL_MIXER=2 \
    -s SDL2_MIXER_FORMATS=["mp3","wav"] \
    -s USE_SDL_IMAGE=2 \
    -s SDL2_IMAGE_FORMATS=["png","jpg"] \
    -s EXIT_RUNTIME=1 \
    -o web/index.js
