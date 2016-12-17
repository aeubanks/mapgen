#include <fstream>

#include "mg_util/Stopwatch.hpp"
#include "mg_util/text_processing.hpp"

#include "mapgen/CellularAutomaton.hpp"
#include "mapgen/DiamondSquare.hpp"
#include "mapgen/DrunkMiners.hpp"
#include "mapgen/MapGenerator.hpp"
#include "mapgen/Randomizer.hpp"
#include "mapgen/RoomMaker.hpp"
#include "mapgen/SectionRemover.hpp"

#include "game/Map2.hpp"

#include "fmt/fmt_println.hpp"

#include "sdl2/SDLInit.hpp"
#include "sdl2/SDLRendererWrapper.hpp"
#include "sdl2/SDLSurfaceWrapper.hpp"
#include "sdl2/SDLTTFFontWrapper.hpp"
#include "sdl2/SDLTextureWrapper.hpp"
#include "sdl2/SDLWindowWrapper.hpp"

#include "lodepng/lodepng.hpp"
#include "lodepng/lodepng_map.hpp"

#include "Box2D/Box2D.h"

mapgen::Map some_map() {
    using namespace mapgen;
    int width = 40;
    int height = 40;
    double ground_chance = 0.45;
    int generations = 3;
    int alive = 4;
    int dead = 4;
    mg_util::Random rand;

    Randomizer randomizer(rand, ground_chance);
    RoomMaker<MakeRoomDeciderAlways> room_maker(rand, 3, width / 10, width /
                                                                         5,
                                                height / 10, height / 5);
    CellularAutomaton<CARuleAliveDeadLimit> ca(rand, generations,
                                               CARuleAliveDeadLimit(alive, dead));
    SectionRemover<SectionRemoverSmall> ground_remover(rand,
                                                       MapTileType::Ground, MapTileType::Wall, SectionRemoverSmall(50));
    SectionRemover<SectionRemoverSmall> wall_remover(rand, MapTileType::Wall, MapTileType::Ground, SectionRemoverSmall(200));

    return create_map(width, height, false, randomizer, room_maker, ca,
                      ground_remover, wall_remover);
}

void sdl_main2() {
    int window_width = 800;
    int window_height = 600;

    sdl2::SDLWindowWrapper window("mapgen window", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN |
                                                                                            SDL_WINDOW_RESIZABLE);

    sdl2::SDLRendererWrapper renderer(window, SDL_RENDERER_PRESENTVSYNC |
                                                  SDL_RENDERER_ACCELERATED);
    renderer.set_draw_color(0xFF / 2, 0xFF / 2, 0xFF / 2, 0xFF);

    // texture1
    sdl2::SDLSurfaceWrapper image("resources/test.bmp");
    sdl2::SDLTextureWrapper texture1(renderer, image);

    // texture2
    auto map = some_map();
    sdl2::SDLSurfaceWrapper surface2(map, 4);
    sdl2::SDLTextureWrapper texture2(renderer, surface2);

    // animated
    sdl2::SDLSurfaceWrapper clips_surface("resources/foo.png");
    clips_surface.set_color_key(0, 0xFF, 0xFF);
    sdl2::SDLTextureWrapper clips_texture(renderer, clips_surface);

    std::vector<SDL_Rect> clips{
        {0, 0, 64, 205},
        {64, 0, 64, 205},
        {128, 0, 64, 205},
        {196, 0, 64, 205},
    };
    int clips_idx = 0;

    sdl2::SDLTTFFontWrapper font("resources/lazy.ttf", 28);
    auto text_texture = font.render_font_solid(renderer, "HELLO!", {0, 0xFF / 3, 0xFF / 2, 0xFF});

    auto joystick_texture = font.render_font_solid(renderer, "JOYSTICK!",
                                                   {0, 0, 0, 0xFF});

    SDL_Event event;

    bool running = true;

    int rect_x = 0, rect_y = 0;
    int move_dist = 5;
    Uint8 r_mod = 0xFF;
    Uint8 g_mod = 0xFF;
    Uint8 b_mod = 0xFF;
    Uint8 a_mod = 0xFF;

    double angle = 0.0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    texture1.set_blend_mode(SDL_BLENDMODE_BLEND);
    texture2.set_blend_mode(SDL_BLENDMODE_BLEND);

    std::string mouse_str("wow"), mouse_pos_str("wow");
    auto mouse_texture = font.render_font_solid(renderer, mouse_str, {0, 0,
                                                                      0, 0xFF});
    auto mouse_pos_texture = font.render_font_solid(renderer, mouse_str, {0,
                                                                          0, 0, 0xFF});

    int k;
    SDL_GetKeyboardState(&k);

    while (running) {
        bool mouse_change = false;
        bool mouse_pos_change = false;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEMOTION:
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    mouse_pos_str = fmt::format("{}, {}", mouse_x,
                                                mouse_y);
                    mouse_pos_change = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouse_str = "Down";
                    mouse_change = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouse_str = "Up";
                    mouse_change = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            r_mod += 0xF;
                            break;
                        case SDLK_a:
                            r_mod -= 0xF;
                            break;
                        case SDLK_w:
                            g_mod += 0xF;
                            break;
                        case SDLK_s:
                            g_mod -= 0xF;
                            break;
                        case SDLK_e:
                            b_mod += 0xF;
                            break;
                        case SDLK_d:
                            b_mod -= 0xF;
                            break;
                        case SDLK_r:
                            a_mod += 0xF;
                            break;
                        case SDLK_f:
                            a_mod -= 0xF;
                            break;
                        case SDLK_t:
                            angle += 10.0;
                            break;
                        case SDLK_g:
                            angle -= 10.0;
                            break;
                        case SDLK_y:
                            flip =
                                static_cast<decltype(flip)>(flip ^ SDL_FLIP_VERTICAL);
                            break;
                        case SDLK_h:
                            flip =
                                static_cast<decltype(flip)>(flip ^ SDL_FLIP_HORIZONTAL);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        auto key_state = SDL_GetKeyboardState(NULL);
        if (key_state[SDL_SCANCODE_LEFT] &&
            !key_state[SDL_SCANCODE_RIGHT]) {
            rect_x -= move_dist;
        } else if (!key_state[SDL_SCANCODE_LEFT] &&
                   key_state[SDL_SCANCODE_RIGHT]) {
            rect_x += move_dist;
        }
        if (key_state[SDL_SCANCODE_UP] && !key_state[SDL_SCANCODE_DOWN]) {
            rect_y -= move_dist;
        } else if (!key_state[SDL_SCANCODE_UP] &&
                   key_state[SDL_SCANCODE_DOWN]) {
            rect_y += move_dist;
        }

        if (mouse_change) {
            mouse_texture = font.render_font_solid(renderer, mouse_str,
                                                   {0, 0, 0, 0xFF});
        }
        if (mouse_pos_change) {
            mouse_pos_texture = font.render_font_solid(renderer,
                                                       mouse_pos_str, {0, 0, 0, 0xFF});
        }

        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                static bool printed_controller = false;
                if (!printed_controller) {
                    printf("Index \'%i\' is a compatible controller, named \'%s\'\n", i, SDL_GameControllerNameForIndex(i));
                    printed_controller = true;
                }

                auto ctrl = SDL_GameControllerOpen(i);
                // auto joy =
                SDL_GameControllerGetJoystick(ctrl);
                auto x =
                    static_cast<int32_t>(SDL_GameControllerGetAxis(ctrl,
                                                                   SDL_CONTROLLER_AXIS_LEFTX));
                auto y =
                    static_cast<int32_t>(SDL_GameControllerGetAxis(ctrl,
                                                                   SDL_CONTROLLER_AXIS_LEFTY));
                auto x_norm = static_cast<double>(x) / (0x10000 >>
                                                        1);
                auto y_norm = static_cast<double>(y) / (0x10000 >>
                                                        1);
                auto scale = 1.0;
                if (SDL_GameControllerGetButton(ctrl,
                                                SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) {
                    scale *= 0.5;
                }
                if (SDL_GameControllerGetButton(ctrl,
                                                SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
                    scale *= 2.0;
                }
                rect_x += move_dist * x_norm * scale;
                rect_y += move_dist * y_norm * scale;

            } else {
                printf("Index \'%i\' is not a compatible controller.\n", i);
            }
        }

        texture1.set_color_mod(r_mod, g_mod, b_mod);
        texture2.set_color_mod(r_mod, g_mod, b_mod);
        texture1.set_alpha_mod(a_mod);
        texture2.set_alpha_mod(a_mod);

        renderer.clear();

        renderer.set_draw_color(0, 0, 0xFF);
        renderer.fill();

        auto clip_rect = clips[clips_idx / 4];
        renderer.copy({0, 0, clip_rect.w, clip_rect.h}, clips_texture,
                      clip_rect, angle, flip);
        ++clips_idx;
        clips_idx %= clips.size() * 4;

        renderer.set_draw_color(0xFF, 0, 0);
        renderer.draw_rect({rect_x, rect_y, 50, 50});
        renderer.set_draw_color(0, 0xFF, 0);

        renderer.draw_line(0, 0, window.width(), window.height());
        renderer.copy({window.width() / 2 - texture1.width(),
                       window.height() / 2, texture1.width(), texture1.height()},
                      texture1);
        renderer.copy({window.width() / 2, window.height() / 2 -
                                               texture2.height(),
                       texture2.width(), texture2.height()},
                      texture2);

        renderer.copy({window.width() / 2, window.height() / 2,
                       text_texture.width(), text_texture.height()},
                      text_texture);

        renderer.copy({window.width() - mouse_texture.width(),
                       window.height() / 2 - mouse_texture.height() / 2, mouse_texture.width(),
                       mouse_texture.height()},
                      mouse_texture);
        renderer.copy({window.width() - mouse_pos_texture.width(),
                       window.height() / 2 + mouse_pos_texture.height() / 2,
                       mouse_pos_texture.width(), mouse_pos_texture.height()},
                      mouse_pos_texture);
        if (SDL_NumJoysticks() > 0) {
            renderer.copy({0, window.height() -
                                  joystick_texture.height(),
                           joystick_texture.width(),
                           joystick_texture.height()},
                          joystick_texture);
        }

        renderer.present();
    }
}

void ca_main() {
    int width = 40;
    int height = 40;
    double ground_chance = 0.45;

    std::string line(static_cast<unsigned long>(width), '-');

    mg_util::Stopwatch time;
    mg_util::Random rand;

    std::vector<std::pair<int, int>> alive_dead_pairs{
        {3, 4},
        {4, 3},
        {4, 4},
        {3, 5},
        {5, 3},
    };

    std::vector<int> generation_vec{3};
    std::vector<bool> wrap_vec{false, true};
    int repeat_count = 1;

    for (auto alive_dead : alive_dead_pairs) {
        int alive = alive_dead.first;
        int dead = alive_dead.second;

        fmt::println(line);
        fmt::println("alive: {}, dead: {}", alive, dead);
        fmt::println(line);
        for (auto generations : generation_vec) {
            fmt::println("generations: {}", generations);
            fmt::println(line);
            for (auto wrap : wrap_vec) {
                fmt::println("wrap: {}", wrap);
                fmt::println(line);
                for (int i = 0; i < repeat_count; ++i) {
                    using namespace mapgen;
                    Randomizer randomizer(rand,
                                          ground_chance);
                    RoomMaker<MakeRoomDeciderAlways>
                        room_maker(rand, 3, width / 10, width / 5, height / 10, height / 5);
                    CellularAutomaton<CARuleAliveDeadLimit> ca(rand,
                                                               generations, CARuleAliveDeadLimit(alive, dead));
                    SectionRemover<SectionRemoverSmall>
                        section_remover(rand, MapTileType::Ground, MapTileType::Wall,
                                        SectionRemoverSmall(50));
                    SectionRemover<SectionRemoverSmall>
                        wall_remover(rand, MapTileType::Wall, MapTileType::Ground,
                                     SectionRemoverSmall(200));

                    auto map = create_map(width, height, false,
                                          randomizer, room_maker, ca, section_remover, wall_remover);
                    fmt::println_obj(map);

                    int num_walls =
                        map.num_tiles_of_type(MapTileType::Wall);
                    int num_ground =
                        map.num_tiles_of_type(MapTileType::Ground);
                    fmt::println("num wall:   {}",
                                 num_walls);
                    fmt::println("num ground: {}",
                                 num_ground);
                    time.tick_and_print_millis();
                    fmt::println(line);
                }
            }
        }
    }

    time.print_total_time_millis();
}

void png_main() {
    auto map = some_map();
    fmt::println_obj(map);

    int tile_size = 4;

    lodepng::save_map_as_image("/Users/aeubanks/Downloads/test.png", map,
                               tile_size);
}

std::ostream & operator<<(std::ostream & os, const b2Vec2 & vec) {
    os << '{';
    os << vec.x;
    os << ',';
    os << ' ';
    os << vec.y;
    os << '}';
    return os;
}

constexpr int Box2DToPixelsRatio = 32;
constexpr float32 PixelsToBox2DRatio =
    1.0f / static_cast<float32>(Box2DToPixelsRatio);

mapgen::Coord2D box2d_to_pixels(float32 x, float32 y) {
    return mapgen::Coord2D(static_cast<int>(x * Box2DToPixelsRatio),
                           static_cast<int>(y * Box2DToPixelsRatio));
}

mapgen::Coord2D box2d_to_pixels(const b2Vec2 & v) {
    return box2d_to_pixels(v.x, v.y);
}

b2Vec2 pixels_to_box2d(int x, int y) {
    return b2Vec2(static_cast<float32>(x) * PixelsToBox2DRatio,
                  static_cast<float32>(y) * PixelsToBox2DRatio);
}

b2Vec2 pixels_to_box2d(const mapgen::Coord2D & c) {
    return pixels_to_box2d(c.x, c.y);
}

void sdl_main() {
    constexpr int tile_size_pixels = 32;
    constexpr int player_size_pixels = 25;

    int window_width = 800;
    int window_height = 600;

    sdl2::SDLWindowWrapper window(
        "mapgen window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    sdl2::SDLRendererWrapper renderer(window, SDL_RENDERER_PRESENTVSYNC |
                                                  SDL_RENDERER_ACCELERATED);
    renderer.set_draw_color(0xFF / 2, 0xFF / 2, 0xFF / 2, 0xFF);

    game::MapTileType2::init();

    game::Map2 map(20, 20);

    sdl2::SDLSurfaceWrapper tiles_surface("resources/ultima5tileset.png");
    tiles_surface.set_color_key(0, 0, 0);
    sdl2::SDLTextureWrapper tiles_texture(renderer, tiles_surface);

    b2Vec2 gravity(0.0, 0.0);
    auto world = std::make_unique<b2World>(gravity);

    b2BodyDef player_body_def;
    player_body_def.fixedRotation = true;
    player_body_def.type = b2_dynamicBody;
    player_body_def.position =
        pixels_to_box2d(-tile_size_pixels, -tile_size_pixels);
    player_body_def.linearDamping = 1.0;

    b2PolygonShape player_shape;
    auto player_box_size =
        pixels_to_box2d(player_size_pixels, player_size_pixels);
    player_shape.SetAsBox(player_box_size.x / 2, player_box_size.y / 2);

    b2FixtureDef player_fixture_def;
    player_fixture_def.shape = &player_shape;
    player_fixture_def.density = 1.0;

    auto player_body = world->CreateBody(&player_body_def);
    player_body->CreateFixture(&player_fixture_def);
    float timestep = 1.0f / 60.0f;

    b2BodyDef wall_body_def;
    wall_body_def.fixedRotation = true;
    wall_body_def.type = b2_staticBody;

    b2PolygonShape wall_shape;
    auto wall_box_size = pixels_to_box2d(tile_size_pixels, tile_size_pixels);
    wall_shape.SetAsBox(wall_box_size.x / 2, wall_box_size.y / 2);

    b2FixtureDef wall_fixture_def;
    wall_fixture_def.shape = &wall_shape;
    wall_fixture_def.density = 1.0;

    for (auto coord : map.coords()) {
        auto & name = map[coord].name();
        if (name == "wall") {
            wall_body_def.position = pixels_to_box2d(coord * tile_size_pixels);
            auto wall = world->CreateBody(&wall_body_def);
            wall->CreateFixture(&wall_fixture_def);
        }
    }

    SDL_Event event;

    bool running = true;

    uint64_t frame_count = 0;
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEMOTION:
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                case SDL_MOUSEBUTTONUP:
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            running = false;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        b2Vec2 velocity(0, 0);
        float32 diff = 5;

        auto key_state = SDL_GetKeyboardState(NULL);
        if (key_state[SDL_SCANCODE_LEFT] && !key_state[SDL_SCANCODE_RIGHT]) {
            velocity.x -= diff;
        } else if (!key_state[SDL_SCANCODE_LEFT] && key_state[SDL_SCANCODE_RIGHT]) {
            velocity.x += diff;
        }
        if (key_state[SDL_SCANCODE_UP] && !key_state[SDL_SCANCODE_DOWN]) {
            velocity.y -= diff;
        } else if (!key_state[SDL_SCANCODE_UP] && key_state[SDL_SCANCODE_DOWN]) {
            velocity.y += diff;
        }

        player_body->SetLinearVelocity(velocity);

        world->Step(timestep, 10, 10);

        renderer.clear();

        // set background
        renderer.set_draw_color(0, 0, 0);
        renderer.fill();

        auto player_pos = box2d_to_pixels(player_body->GetPosition());

        // draw map
        for (auto coord : map.coords()) {
            auto & tile_type = map[coord];
            auto x_tile = tile_type.x_tile();
            auto y_tile = tile_type.y_tile();

            //			renderer.copy({coord.x * tile_size_pixels +
            //window.width() / 2 - player_pos.x, coord.y * tile_size_pixels +
            //window.height() / 2 - player_pos.y, tile_size, tile_size},
            //tiles_texture, {x_tile * tile_size, y_tile * tile_size,
            //tile_size_pixels, tile_size_pixels});
            renderer.copy({coord.x * tile_size_pixels, coord.y * tile_size_pixels,
                           tile_size_pixels, tile_size_pixels},
                          tiles_texture,
                          {x_tile * tile_size_pixels, y_tile * tile_size_pixels,
                           tile_size_pixels, tile_size_pixels});
        }

        // draw player
        renderer.set_draw_color(0xFF, 0, 0);
        //		renderer.draw_rect({window.width() / 2 - player_size_pixels / 2,
        //window.height() / 2 - player_size_pixels / 2, player_size_pixels,
        //player_size_pixels});
        renderer.draw_rect(
            {player_pos.x, player_pos.y, player_size_pixels, player_size_pixels});

        renderer.present();

        ++frame_count;
    }
}

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

int main(int argc, char ** argv) {
    sdl2::SDL2Init sdl2init;
    sdl2::SDL2TTFInit sdl2ttfinit;
    sdl2::SDL2ImageInit sdl2imageinit;

#ifdef __APPLE__
    std::ofstream f("/Users/aeubanks/mapgen.log");
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path,
                                          PATH_MAX)) {
        puts("CFURLGetFileSystemRepresentation fail\n");
    }
    CFRelease(resourcesURL);

    chdir(path);
    f << "cur path: " << path << '\n';
    puts("cur path:");
    puts(path);
#endif

    try {
        std::vector<std::string> args(argv, argv + argc);

        sdl_main();
    } catch (sdl2::sdl2_error & e) {
        fmt::println(stderr, "sdl2_error: {}", e.what());
        return 1;
    } catch (mg_util::mg_error & e) {
        fmt::println(stderr, "mg_error: {}", e.what());
        return 2;
    } catch (std::exception & e) {
        fmt::println(stderr, "exception: {}", e.what());
        return -1;
    }

    return 0;
}
