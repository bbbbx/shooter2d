#include "stage.h"

static struct Entity *player;
static SDL_Texture *playerTexture;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *enemyBulletTexture;
static SDL_Texture *backgroundTexture;
static SDL_Texture *explosionTexture;

struct Star stars[MAX_STARS];

int backgroundX = 0;

int enemySpawnTimer;
int stageResetTimer;

static void addExplosions(int x, int y, int num)
{
    struct Explosion *e;
    int i;

    for (i = 0; i < num; i++)
    {
        e = malloc(sizeof(struct Explosion));
        memset(e, 0, sizeof(struct Explosion));

        stage.explosionTail->next = e;
        stage.explosionTail = e;

        e->x = (rand() % 32) - (rand() % 32) + x;
        e->y = (rand() % 32) - (rand() % 32) + y;
        e->dx = (rand() % 10) - (rand() % 10);
        e->dy = (rand() % 10) - (rand() % 10);

        e->dx /= 10;
        e->dy /= 10;

        switch (rand() % 4)
        {
        case 0:
            e->r = 255;
            break;
        case 1:
            e->r = 255;
            e->g = 128;
            break;
        case 2:
            e->r = 255;
            e->g = 255;
            break;
        default:
            e->r = 255;
            e->g = 255;
            e->b = 255;
            break;
        }

        e->a = rand() % FPS * 3;
    }
}

static void addDebris(struct Entity *e)
{
    struct Debris *d;
    int x, y, w, h;

    w = e->w / 2;
    h = e->h / 2;

    for (y = 0; y <= h; y += h)
    {
        for (x = 0; x <= w; x += w)
        {
            d = malloc(sizeof(struct Debris));
            memset(d, 0, sizeof(struct Debris));

            stage.debrisTail->next = d;
            stage.debrisTail = d;

            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;
            d->dx = (rand() % 5) - (rand() % 5);
            d->dy = -(5 + (rand() % 12));
            d->life = FPS * 2;
            d->texture = e->texture;

            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}

static int bulletHitFighter(struct Entity *bullet)
{
    struct Entity *fighter;

    for (fighter = stage.fighterHead.next; fighter != NULL; fighter = fighter->next)
    {
        if (
            fighter->side != bullet->side &&
            collision(bullet->x, bullet->y, bullet->w, bullet->h, fighter->x, fighter->y, fighter->w, fighter->h))
        {
            bullet->health = 0;
            fighter->health = 0;

            addDebris(fighter);
            addExplosions(fighter->x, fighter->y, 2);

            return 1;
        }
    }

    return 0;
}

static void initStarfield()
{
    int i;

    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = rand() % 8 + 1;
    }
}

static void initPlayer()
{
    player = malloc(sizeof(struct Entity));
    memset(player, 0, sizeof(struct Entity));

    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->texture = playerTexture;
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    player->side = SIDE_PLAYER;

    player->health = 1;
}

static void resetStage()
{
    struct Entity *e;

    struct Explosion *explosion;
    struct Debris *debris;

    while (stage.debrisHead.next)
    {
        debris = stage.debrisHead.next;
        stage.debrisHead.next = debris->next;
        free(debris);
    }

    while (stage.explosionHead.next)
    {
        explosion = stage.explosionHead.next;
        stage.explosionHead.next = explosion->next;
        free(explosion);
    }

    while (stage.fighterHead.next)
    {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        free(e);
    }

    while (stage.bulletHead.next)
    {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    memset(&stage, 0, sizeof(Stage));

    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    initPlayer();

    initStarfield();

    enemySpawnTimer = 0;

    // to tell the counter to start at two seconds.
    stageResetTimer = FPS * 3;
}

static void fireBullet()
{
    struct Entity *bullet;

    bullet = malloc(sizeof(struct Entity));
    memset(bullet, 0, sizeof(struct Entity));

    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = player->x;
    bullet->y = player->y;
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->y += (player->h / 2) - (bullet->h / 2);

    bullet->side = SIDE_PLAYER;

    player->reload = 8;
}

static void fireEnemyBullet(struct Entity *e)
{
    struct Entity *bullet;

    bullet = malloc(sizeof(struct Entity));
    memset(bullet, 0, sizeof(struct Entity));

    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->side = e->side;
    bullet->texture = enemyBulletTexture;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ENEMY_BULLET_SPEED;
    bullet->dy *= ENEMY_BULLET_SPEED;

    bullet->side = SIDE_ENEMY;

    e->reload = (rand() % FPS * 2);
}

static void spawnEnemies()
{
    struct Entity *enemy;

    if (--enemySpawnTimer <= 0)
    {
        enemy = malloc(sizeof(struct Entity));
        memset(enemy, 0, sizeof(struct Entity));
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
        enemy->x = SCREEN_WIDTH;
        enemy->y = (rand() % SCREEN_HEIGHT);
        enemy->y = MIN(enemy->y, SCREEN_HEIGHT - enemy->h);

        enemy->dx = -(2 + (rand() % 4));

        enemy->side = SIDE_ENEMY;
        enemy->health = 1;

        enemy->reload = FPS * (1 + (rand() % 3));

        enemySpawnTimer = 30 + (rand() % 60);
    }
}

static void drawBackground()
{
    SDL_Rect dest;
    int x;

    for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, backgroundTexture, NULL, &dest);
    }
}

static void drawStarfield()
{
    int i, c;

    for (i = 0; i < MAX_STARS; i++)
    {
        c = stars[i].speed * 32;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
}

static void drawExplosions()
{
    struct Explosion *e;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for (e = stage.explosionHead.next; e != NULL; e = e->next)
    {
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
        SDL_SetTextureAlphaMod(explosionTexture, e->a);

        blit(explosionTexture, e->x, e->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

static void drawDebris()
{
    struct Debris *d;

    for (d = stage.debrisHead.next; d != NULL; d = d->next)
    {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}

static void drawBullets()
{
    struct Entity *b;

    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        blit(b->texture, b->x, b->y);
    }
}

static void drawFighters()
{
    struct Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        blit(e->texture, e->x, e->y);
    }
}

static void draw()
{
    drawBackground();

    drawStarfield();

    drawBullets();

    drawFighters();

    drawDebris();

    drawExplosions();
}

static void doBackground()
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}

static void doStarfield()
{
    int i;

    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        if (stars[i].x < 0)
        {
            stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
}

static void doExplosions()
{
    struct Explosion *e, *prev;

    prev = &stage.explosionHead;

    for (e = stage.explosionHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (--e->a <= 0)
        {
            if (e == stage.explosionTail)
            {
                stage.explosionTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
    }
}

static void doDebris()
{
    struct Debris *d, *prev;

    prev = &stage.debrisHead;

    for (d = stage.debrisHead.next; d != NULL; d = d->next)
    {
        d->x += d->dx;
        d->y += d->dy;

        d->dy += 0.5;

        if (--d->life <= 0)
        {
            if (d == stage.debrisTail)
            {
                stage.debrisTail = prev;
            }

            prev->next = d->next;
            free(d);
            d = prev;
        }

        prev = d;
    }
}

static void doPlayer()
{
    if (player == NULL)
    {
        return;
    }

    player->dx = player->dy = 0;

    if (player->reload > 0)
    {
        player->reload--;
    }

    if (app.keyboard[SDL_SCANCODE_UP])
    {
        player->dy = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_DOWN])
    {
        player->dy = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_LEFT])
    {
        player->dx = -PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_RIGHT])
    {
        player->dx = PLAYER_SPEED;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload == 0)
    {
        fireBullet();
    }
}

static void doBullets()
{
    struct Entity *b, *prev;

    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        b->x += b->dx;
        b->y += b->dy;

        if (b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT || bulletHitFighter(b))
        {
            if (b == stage.bulletTail)
            {
                stage.bulletTail = prev;
            }

            prev->next = b->next;
            free(b);
            b = prev;
        }

        prev = b;
    }
}

static void doFighters()
{
    struct Entity *e, *prev;

    prev = &stage.fighterHead;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        // enemy out of screen
        if (e != player && e->x < -e->w)
        {
            e->health = 0;
        }

        if (e->health == 0)
        {
            if (e == player)
            {
                player = NULL;
            }

            if (e == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
    }
}

static void doEnemies()
{
    struct Entity *e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            fireEnemyBullet(e);
        }
    }
}

static void clipPlayer()
{
    if (player == NULL)
    {
        return;
    }

    player->x = MAX(0, player->x);
    player->y = MAX(0, player->y);
    player->x = MIN(SCREEN_WIDTH - player->w, player->x);
    player->y = MIN(SCREEN_HEIGHT - player->h, player->y);
}

static void logic()
{
    doBackground();

    doStarfield();

    doPlayer();

    doFighters();

    doEnemies();

    doBullets();

    doExplosions();

    doDebris();

    spawnEnemies();

    clipPlayer();

    if (player == NULL && --stageResetTimer <= 0)
    {
        resetStage();
    }
}

void initStage()
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    playerTexture = loadTexture((char*)"gfx/player.png");
    bulletTexture = loadTexture((char*)"gfx/playerBullet.png");
    enemyTexture = loadTexture((char*)"gfx/enemy.png");
    enemyBulletTexture = loadTexture((char*)"gfx/enemyBullet.png");

    backgroundTexture = loadTexture((char*)"gfx/background.jpg");
    explosionTexture = loadTexture((char*)"gfx/explosion.png");

    resetStage();
}