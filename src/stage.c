#include "stage.h"

static struct Entity *player;
static SDL_Texture *playerTexture;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *enemyBulletTexture;

int enemySpawnTimer;
int stageResetTimer;

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
            return 1;
        }
    }

    return 0;
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

    initPlayer();

    enemySpawnTimer = 0;

    // to tell the counter to start at two seconds.
    stageResetTimer = FPS * 2;
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

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyTexture;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->dx = -(2 + (rand() % 4));

        enemy->side = SIDE_ENEMY;
        enemy->health = 1;

        enemy->reload = FPS * (1 + (rand() % 3));

        enemySpawnTimer = 30 + (rand() % 60);
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
    drawBullets();

    drawFighters();
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
    doPlayer();

    doFighters();

    doEnemies();

    doBullets();

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

    resetStage();
}