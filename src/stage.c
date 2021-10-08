#include "stage.h"

static struct Entity *player;
static SDL_Texture *playerTexture;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static SDL_Texture *enemyBulletTexture;
static SDL_Texture *explosionTexture;
static SDL_Texture *pointsTexture;

int enemySpawnTimer;
int stageResetTimer;

int highscore = 0;

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

static void addPointsPod(int x, int y)
{
    struct Entity *e;

    e = malloc(sizeof(struct Entity));
    memset(e, 0, sizeof(struct Entity));

    e->x = x;
    e->y = y;
    e->dx = -(rand() % 5);
    e->dy = (rand() % 5) - (rand() % 5);
    e->health = FPS * 10;
    e->texture = pointsTexture;
    SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);
    e->x -= e->w / 2;
    e->y -= e->h / 2;

    stage.pointsTail->next = e;
    stage.pointsTail = e;
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

            if (fighter == player)
            {
                playSound(SND_PLAYER_DIE, CH_PLAYER);
            }
            else
            {
                playSound(SND_ENEMY_DIE, CH_ANY);

                addPointsPod(fighter->x + fighter->w / 2, fighter->y + fighter->h / 2);

                stage.score++;

                highscore = MAX(highscore, stage.score);
            }

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

    while (stage.pointsHead.next)
    {
        e = stage.pointsHead.next;
        stage.pointsHead.next = e->next;
        free(e);
    }

    memset(&stage, 0, sizeof(Stage));

    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;
    stage.pointsTail = &stage.pointsHead;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;
    stage.score = 0;

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

static void drawPointsPods()
{
    struct Entity *e;

    for (e = stage.pointsHead.next; e != NULL; e = e->next)
    {
        blit(e->texture, e->x, e->y);
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

static void drawHud()
{
    drawText(10, 10, 255, 255, 255, TEXT_RIGHT, "SCORE: %03d", stage.score);

    int breakScore = stage.score > 0 && stage.score == highscore;
    if (breakScore)
    {
        drawText(960, 10, 0, 255, 0, TEXT_RIGHT, "HIGH SCORE: %03d", highscore);
    }
    else
    {
        drawText(960, 10, 255, 255, 255, TEXT_RIGHT, "HIGH SCORE: %03d", highscore);
    }
}

static void draw()
{
    drawBackground();

    drawStarfield();

    drawPointsPods();

    drawBullets();

    drawFighters();

    drawDebris();

    drawExplosions();

    drawHud();
}

void doBackground()
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}

void doStarfield()
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
        playSound(SND_PLAYER_FIRE, CH_PLAYER);

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

static void doPointsPods()
{
    struct Entity *e, *prev;

    prev = &stage.pointsHead;

    for (e = stage.pointsHead.next; e != NULL; e = e->next)
    {
        if (e->x < 0)
        {
            e->x = 0;
            e->dx = -e->dx;
        }

        if (e->x + e->w > SCREEN_WIDTH)
        {
            e->x = SCREEN_WIDTH - e->w;
            e->dx = -e->dx;
        }

        if (e->y < 0)
        {
            e->y = 0;
            e->dy = -e->dy;
        }

        if (e->y + e->h > SCREEN_HEIGHT)
        {
            e->y = SCREEN_HEIGHT - e->h;
            e->dy = -e->dy;
        }

        e->x += e->dx;
        e->y += e->dy;

        if (player != NULL && collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h))
        {
            e->health = 0;

            stage.score++;

            highscore = MAX(highscore, stage.score);

            playSound(SND_POINTS, CH_POINTS);
        }

        if (--e->health <= 0)
        {
            if (e == stage.pointsTail)
			{
				stage.pointsTail = prev;
			}

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
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

            playSound(SND_ENEMY_FIRE, CH_ENEMY_FIRE);
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

    doPointsPods();

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
        addHighscore(stage.score);

        initHighscores();
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

    explosionTexture = loadTexture((char*)"gfx/explosion.png");

    pointsTexture = loadTexture((char*)"gfx/star_coin_normal_30x30.png");

    resetStage();
}