#include "../include/game.h"
#include "../include/config.h"
#include "../include/menu.h"
#include "../include/hud.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

/* ===========================================================
 * game.c
 *
 * Implementa a inicializacao do Allegro, o loop principal
 * (eventos + atualizacao + desenho) e a maquina de estados
 * descrita em game.h.
 * =========================================================== */

/* ---- helpers internos ---- */

static void start_phase1(Game *g) {
    player_init(&g->player);
    camera_init(&g->camera);
    phase1_init(&g->phase);
    audio_apply_volume(&g->phase.music);
    audio_play_music(&g->phase.music, true);
}

static void advance_to_phase2(Game *g) {
    float keep_health   = (float)g->player.health;
    bool  keep_double   = g->player.has_double_jump;

    phase_destroy(&g->phase);
    phase2_init(&g->phase);

    /* mantem o progresso do jogador (vida atual e pulo duplo) ao
     * trocar de fase, apenas reposiciona no inicio da nova fase */
    g->player.x = PLAYER_START_X;
    g->player.y = GROUND_Y - g->player.height;
    g->player.vx = 0.0f;
    g->player.vy = 0.0f;
    g->player.on_ground = true;
    g->player.jumps_used = 0;
    g->player.health = (int)keep_health;
    g->player.has_double_jump = keep_double;
    g->player.alive = true;

    camera_init(&g->camera);

    audio_apply_volume(&g->phase.music);
    audio_play_music(&g->phase.music, true);
}

static void return_to_main_menu(Game *g) {
    audio_stop_music(&g->phase.music);
    phase_destroy(&g->phase);
    g->state = STATE_MAIN_MENU;
    g->main_menu_selected = 0;
    audio_apply_volume(&g->menu_music);
    audio_play_music(&g->menu_music, true);
}

/* ---- inicializacao ---- */

bool game_init(Game *g) {
    if (!g) return false;

    g->display = NULL;
    g->event_queue = NULL;
    g->timer = NULL;
    g->running = true;
    g->state = STATE_MAIN_MENU;
    g->main_menu_selected = 0;
    g->options_selected = 0;
    g->pause_selected = 0;

    if (!al_init()) {
        fprintf(stderr, "Erro: falha ao inicializar o Allegro core.\n");
        return false;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "Erro: falha ao inicializar o teclado.\n");
        return false;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "Erro: falha ao inicializar o addon de imagem.\n");
        return false;
    }

    al_init_font_addon();

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Erro: falha ao inicializar o addon de primitivas.\n");
        return false;
    }

    /* audio nao e' fatal: se falhar, o jogo continua mudo */
    if (!audio_init()) {
        fprintf(stderr, "Aviso: audio indisponivel, o jogo continuara sem som.\n");
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    g->display = al_create_display(SCREEN_W, SCREEN_H);
    if (!g->display) {
        fprintf(stderr, "Erro: falha ao criar a janela do jogo.\n");
        return false;
    }
    al_set_window_title(g->display, GAME_TITLE);

    g->timer = al_create_timer(1.0 / FPS);
    if (!g->timer) {
        fprintf(stderr, "Erro: falha ao criar o timer principal.\n");
        return false;
    }

    g->event_queue = al_create_event_queue();
    if (!g->event_queue) {
        fprintf(stderr, "Erro: falha ao criar a fila de eventos.\n");
        return false;
    }

    al_register_event_source(g->event_queue, al_get_display_event_source(g->display));
    al_register_event_source(g->event_queue, al_get_keyboard_event_source());
    al_register_event_source(g->event_queue, al_get_timer_event_source(g->timer));

    input_init(&g->input);
    menu_init();
    hud_init();

    player_init(&g->player);
    player_load_sprites(&g->player);
    camera_init(&g->camera);

    g->phase.hazard_count = 0;
    g->phase.collectible_count = 0;
    g->phase.has_vine = false;
    g->phase.music.loaded = false;
    g->phase.music.stream = NULL;

    g->menu_music = audio_load_music(PATH_AUDIO_MUSIC "menu_theme.ogg");
    audio_set_master_volume(0.7f);
    audio_apply_volume(&g->menu_music);
    audio_play_music(&g->menu_music, true);

    al_start_timer(g->timer);

    return true;
}

/* ---- atualizacao por estado ---- */

static void update_main_menu(Game *g) {
    if (input_pressed(&g->input, ALLEGRO_KEY_UP) || input_pressed(&g->input, ALLEGRO_KEY_W)) {
        g->main_menu_selected = (g->main_menu_selected + MAIN_MENU_OPTION_COUNT - 1) % MAIN_MENU_OPTION_COUNT;
    }
    if (input_pressed(&g->input, ALLEGRO_KEY_DOWN) || input_pressed(&g->input, ALLEGRO_KEY_S)) {
        g->main_menu_selected = (g->main_menu_selected + 1) % MAIN_MENU_OPTION_COUNT;
    }
    if (input_pressed(&g->input, ALLEGRO_KEY_ENTER) || input_pressed(&g->input, ALLEGRO_KEY_SPACE)) {
        switch (g->main_menu_selected) {
            case 0: /* Iniciar */
                audio_stop_music(&g->menu_music);
                start_phase1(g);
                g->state = STATE_PLAYING;
                break;
            case 1: /* Opcoes */
                g->options_selected = 0;
                g->state = STATE_OPTIONS;
                break;
            case 2: /* Sair */
                g->running = false;
                break;
        }
    }
}

static void update_options(Game *g) {
    if (input_pressed(&g->input, ALLEGRO_KEY_UP) || input_pressed(&g->input, ALLEGRO_KEY_W)) {
        g->options_selected = (g->options_selected + 1) % 2;
    }
    if (input_pressed(&g->input, ALLEGRO_KEY_DOWN) || input_pressed(&g->input, ALLEGRO_KEY_S)) {
        g->options_selected = (g->options_selected + 1) % 2;
    }

    if (g->options_selected == 0) {
        float vol = audio_get_master_volume();
        if (input_pressed(&g->input, ALLEGRO_KEY_LEFT) || input_pressed(&g->input, ALLEGRO_KEY_A)) {
            vol -= 0.1f;
            if (vol < 0.0f) vol = 0.0f;
            audio_set_master_volume(vol);
            audio_apply_volume(&g->menu_music);
            audio_apply_volume(&g->phase.music);
        }
        if (input_pressed(&g->input, ALLEGRO_KEY_RIGHT) || input_pressed(&g->input, ALLEGRO_KEY_D)) {
            vol += 0.1f;
            if (vol > 1.0f) vol = 1.0f;
            audio_set_master_volume(vol);
            audio_apply_volume(&g->menu_music);
            audio_apply_volume(&g->phase.music);
        }
    }

    if (input_pressed(&g->input, ALLEGRO_KEY_ENTER) || input_pressed(&g->input, ALLEGRO_KEY_ESCAPE)) {
        if (g->options_selected == 1 || input_pressed(&g->input, ALLEGRO_KEY_ESCAPE)) {
            g->state = STATE_MAIN_MENU;
        } else {
            g->state = STATE_MAIN_MENU;
        }
    }
}

static void update_playing(Game *g, double dt) {
    /* [EXTRA 3] botao de pausa */
    if (input_pressed(&g->input, ALLEGRO_KEY_P) || input_pressed(&g->input, ALLEGRO_KEY_ESCAPE)) {
        g->pause_selected = 0;
        g->state = STATE_PAUSED;
        return;
    }

    bool near_vine = phase_player_near_vine(&g->phase, &g->player);
    player_handle_input(&g->player, &g->input, near_vine);
    player_update(&g->player, dt, GROUND_Y);
    phase_update(&g->phase, &g->player, &g->camera, dt);

    if (!g->player.alive) {
        audio_stop_music(&g->phase.music);
        g->state = STATE_GAME_OVER_LOSE;
        return;
    }

    if (phase_is_complete(&g->phase, &g->player)) {
        if (g->phase.number == 1) {
            advance_to_phase2(g);
        } else {
            audio_stop_music(&g->phase.music);
            g->state = STATE_GAME_OVER_WIN;
        }
    }
}

static void update_paused(Game *g) {
    if (input_pressed(&g->input, ALLEGRO_KEY_UP) || input_pressed(&g->input, ALLEGRO_KEY_W)) {
        g->pause_selected = (g->pause_selected + PAUSE_MENU_OPTION_COUNT - 1) % PAUSE_MENU_OPTION_COUNT;
    }
    if (input_pressed(&g->input, ALLEGRO_KEY_DOWN) || input_pressed(&g->input, ALLEGRO_KEY_S)) {
        g->pause_selected = (g->pause_selected + 1) % PAUSE_MENU_OPTION_COUNT;
    }
    if (input_pressed(&g->input, ALLEGRO_KEY_P)) {
        g->state = STATE_PLAYING;
        return;
    }
    if (input_pressed(&g->input, ALLEGRO_KEY_ENTER) || input_pressed(&g->input, ALLEGRO_KEY_SPACE)) {
        switch (g->pause_selected) {
            case 0: /* Continuar */
                g->state = STATE_PLAYING;
                break;
            case 1: /* Menu Principal */
                return_to_main_menu(g);
                break;
            case 2: /* Sair do jogo */
                g->running = false;
                break;
        }
    }
}

static void update_game_over(Game *g) {
    if (input_pressed(&g->input, ALLEGRO_KEY_ENTER) || input_pressed(&g->input, ALLEGRO_KEY_SPACE)) {
        return_to_main_menu(g);
    }
    if (input_pressed(&g->input, ALLEGRO_KEY_ESCAPE)) {
        g->running = false;
    }
}

/* ---- desenho por estado ---- */

static void draw_playing(Game *g) {
    phase_draw(&g->phase, &g->player, &g->camera);
    hud_draw(&g->player, g->phase.number);
}

/* ---- loop principal ---- */

void game_run(Game *g) {
    bool redraw = true;
    ALLEGRO_EVENT ev;

    while (g->running) {
        al_wait_for_event(g->event_queue, &ev);

        switch (ev.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                g->running = false;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                input_set_key(&g->input, ev.keyboard.keycode, true);
                break;

            case ALLEGRO_EVENT_KEY_UP:
                input_set_key(&g->input, ev.keyboard.keycode, false);
                break;

            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            default:
                break;
        }

        if (redraw && al_is_event_queue_empty(g->event_queue)) {
            double dt = 1.0 / FPS;

            switch (g->state) {
                case STATE_MAIN_MENU: update_main_menu(g); break;
                case STATE_OPTIONS:   update_options(g);   break;
                case STATE_PLAYING:   update_playing(g, dt); break;
                case STATE_PAUSED:    update_paused(g);    break;
                case STATE_GAME_OVER_WIN:
                case STATE_GAME_OVER_LOSE:
                    update_game_over(g);
                    break;
            }

            al_clear_to_color(al_map_rgb(20, 20, 30));

            switch (g->state) {
                case STATE_MAIN_MENU:
                    menu_draw_main(g->main_menu_selected);
                    break;
                case STATE_OPTIONS: {
                    int vol_pct = (int)(audio_get_master_volume() * 100.0f + 0.5f);
                    menu_draw_options(vol_pct, g->options_selected);
                    break;
                }
                case STATE_PLAYING:
                    draw_playing(g);
                    break;
                case STATE_PAUSED:
                    draw_playing(g);
                    menu_draw_pause(g->pause_selected);
                    break;
                case STATE_GAME_OVER_WIN:
                    menu_draw_game_over(true);
                    break;
                case STATE_GAME_OVER_LOSE:
                    menu_draw_game_over(false);
                    break;
            }

            al_flip_display();
            input_end_frame(&g->input);
            redraw = false;
        }
    }
}

/* ---- finalizacao ---- */

void game_shutdown(Game *g) {
    if (!g) return;

    if (g->phase.music.stream || g->phase.hazard_count > 0 || g->phase.collectible_count > 0) {
        phase_destroy(&g->phase);
    }
    player_destroy(&g->player);
    audio_destroy_music(&g->menu_music);

    menu_shutdown();
    hud_shutdown();
    audio_shutdown();

    if (g->timer) al_destroy_timer(g->timer);
    if (g->event_queue) al_destroy_event_queue(g->event_queue);
    if (g->display) al_destroy_display(g->display);

    al_uninstall_keyboard();
}