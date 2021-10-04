emcc \
    src/main.cpp \
    src/input.c \
    src/draw.c \
    src/init.c \
    src/stage.c \
    src/utils.c \
    --preload-file gfx/player.png \
    --preload-file gfx/playerBullet.png \
    --preload-file gfx/enemy.png \
    --preload-file gfx/enemyBullet.png \
    --preload-file gfx/explosion.png \
    --preload-file gfx/background.jpg \
    -s USE_SDL=2 \
    -s WASM=1 \
    -s MIN_WEBGL_VERSION=2 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s USE_SDL_IMAGE=2 \
    -s SDL2_IMAGE_FORMATS=["png","jpg"] \
    -s EXIT_RUNTIME=1 \
    -o web/index.js
