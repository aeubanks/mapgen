#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

#include "mg_util/Coord2D.hpp"
#include "mg_util/Direction.hpp"
#include "mg_util/Stopwatch.hpp"
#include "mg_util/text_processing.hpp"
#include "mg_util/GenerativeGrammar.hpp"

#include "mapgen/CellularAutomaton.hpp"
#include "mapgen/DiamondSquare.hpp"
#include "mapgen/DrunkMiners.hpp"
#include "mapgen/MapGenerator.hpp"
#include "mapgen/Randomizer.hpp"
#include "mapgen/RoomMaker.hpp"
#include "mapgen/SectionRemover.hpp"

#include "game/Map2.hpp"

#include "mg_log/Log.hpp"

#include "sdl2/SDLInit.hpp"
#include "sdl2/SDLRendererWrapper.hpp"
#include "sdl2/SDLSurfaceWrapper.hpp"
#include "sdl2/SDLTTFFontWrapper.hpp"
#include "sdl2/SDLTextureWrapper.hpp"
#include "sdl2/SDLWindowWrapper.hpp"

#include "mappng/mappng.hpp"

#include "png++/png.hpp"

#include "Box2D/Box2D.h"

static mapgen::Map some_map() {
    using namespace mapgen;
    int width = 40;
    int height = 40;
    double ground_chance = 0.45;
    int generations = 3;
    int alive = 4;
    int dead = 4;
    mg_util::Random rand;

    Randomizer randomizer(rand, ground_chance);
    RoomMaker<MakeRoomDeciderAlways> room_maker(rand, 3, width / 10, width / 5, height / 10, height / 5);
    CellularAutomaton<CARuleAliveDeadLimit> ca(rand, generations, CARuleAliveDeadLimit(alive, dead));
    SectionRemover<SectionRemoverSmall> ground_remover(rand, MapTileType::Ground, MapTileType::Wall, SectionRemoverSmall(50));
    SectionRemover<SectionRemoverSmall> wall_remover(rand, MapTileType::Wall, MapTileType::Ground, SectionRemoverSmall(200));

    return create_map(width, height, false, randomizer, room_maker, ca, ground_remover, wall_remover);
}

static void sdl_main2() {
    int window_width = 800;
    int window_height = 600;

    sdl2::SDLWindowWrapper window("mapgen window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    sdl2::SDLRendererWrapper renderer(window, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
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

    auto joystick_texture = font.render_font_solid(renderer, "JOYSTICK!", {0, 0, 0, 0xFF});

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
    auto mouse_texture = font.render_font_solid(renderer, mouse_str, {0, 0, 0, 0xFF});
    auto mouse_pos_texture = font.render_font_solid(renderer, mouse_str, {0, 0, 0, 0xFF});

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
                case SDL_MOUSEMOTION: {
                    int mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    std::stringstream ss;
                    ss << mouse_x << ", " << mouse_y;
                    mouse_pos_str = ss.str();
                    mouse_pos_change = true;
                    break;
                }
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
                            flip = static_cast<decltype(flip)>(flip ^ SDL_FLIP_VERTICAL);
                            break;
                        case SDLK_h:
                            flip = static_cast<decltype(flip)>(flip ^ SDL_FLIP_HORIZONTAL);
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
        if (key_state[SDL_SCANCODE_LEFT] && !key_state[SDL_SCANCODE_RIGHT]) {
            rect_x -= move_dist;
        } else if (!key_state[SDL_SCANCODE_LEFT] && key_state[SDL_SCANCODE_RIGHT]) {
            rect_x += move_dist;
        }
        if (key_state[SDL_SCANCODE_UP] && !key_state[SDL_SCANCODE_DOWN]) {
            rect_y -= move_dist;
        } else if (!key_state[SDL_SCANCODE_UP] && key_state[SDL_SCANCODE_DOWN]) {
            rect_y += move_dist;
        }

        if (mouse_change) {
            mouse_texture = font.render_font_solid(renderer, mouse_str, {0, 0, 0, 0xFF});
        }
        if (mouse_pos_change) {
            mouse_pos_texture = font.render_font_solid(renderer, mouse_pos_str, {0, 0, 0, 0xFF});
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
                auto x = static_cast<int32_t>(SDL_GameControllerGetAxis(ctrl, SDL_CONTROLLER_AXIS_LEFTX));
                auto y = static_cast<int32_t>(SDL_GameControllerGetAxis(ctrl, SDL_CONTROLLER_AXIS_LEFTY));
                auto x_norm = static_cast<double>(x) / (0x10000 >> 1);
                auto y_norm = static_cast<double>(y) / (0x10000 >> 1);
                auto scale = 1.0;
                if (SDL_GameControllerGetButton(ctrl, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) {
                    scale *= 0.5;
                }
                if (SDL_GameControllerGetButton(ctrl, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) {
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
        renderer.copy({0, 0, clip_rect.w, clip_rect.h}, clips_texture, clip_rect, angle, flip);
        ++clips_idx;
        clips_idx %= clips.size() * 4;

        renderer.set_draw_color(0xFF, 0, 0);
        renderer.draw_rect({rect_x, rect_y, 50, 50});
        renderer.set_draw_color(0, 0xFF, 0);

        renderer.draw_line(0, 0, window.width(), window.height());
        renderer.copy({window.width() / 2 - texture1.width(), window.height() / 2, texture1.width(), texture1.height()}, texture1);
        renderer.copy({window.width() / 2, window.height() / 2 - texture2.height(), texture2.width(), texture2.height()}, texture2);

        renderer.copy({window.width() / 2, window.height() / 2, text_texture.width(), text_texture.height()}, text_texture);

        renderer.copy({window.width() - mouse_texture.width(), window.height() / 2 - mouse_texture.height() / 2, mouse_texture.width(), mouse_texture.height()}, mouse_texture);
        renderer.copy({window.width() - mouse_pos_texture.width(), window.height() / 2 + mouse_pos_texture.height() / 2, mouse_pos_texture.width(), mouse_pos_texture.height()}, mouse_pos_texture);
        if (SDL_NumJoysticks() > 0) {
            renderer.copy({0, window.height() - joystick_texture.height(), joystick_texture.width(), joystick_texture.height()}, joystick_texture);
        }

        renderer.present();
    }
}

static void ca_main() {
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

        mg_log::info(line);
        mg_log::info("alive: ", alive, ", dead: ", dead);
        mg_log::info(line);
        for (auto generations : generation_vec) {
            mg_log::info("generations: ", generations);
            mg_log::info(line);
            for (auto wrap : wrap_vec) {
                mg_log::info("wrap: ", wrap);
                mg_log::info(line);
                for (int i = 0; i < repeat_count; ++i) {
                    using namespace mapgen;
                    Randomizer randomizer(rand, ground_chance);
                    RoomMaker<MakeRoomDeciderAlways> room_maker(rand, 3, width / 10, width / 5, height / 10, height / 5);
                    CellularAutomaton<CARuleAliveDeadLimit> ca(rand, generations, CARuleAliveDeadLimit(alive, dead));
                    SectionRemover<SectionRemoverSmall> section_remover(rand, MapTileType::Ground, MapTileType::Wall, SectionRemoverSmall(50));
                    SectionRemover<SectionRemoverSmall> wall_remover(rand, MapTileType::Wall, MapTileType::Ground, SectionRemoverSmall(200));

                    auto map = create_map(width, height, false, randomizer, room_maker, ca, section_remover, wall_remover);
                    mg_log::info(map);

                    int num_walls = map.num_tiles_of_type(MapTileType::Wall);
                    int num_ground = map.num_tiles_of_type(MapTileType::Ground);
                    mg_log::info("num wall:   ", num_walls);
                    mg_log::info("num ground: ", num_ground);
                    time.tick_and_print_millis();
                    mg_log::info(line);

                    std::stringstream fileName;
                    fileName << "map-" << alive << "-" << dead << "-" << generations << "-" << wrap << "-" << i << ".png";
                    mappng::map_to_png(fileName.str(), map, 16);
                }
            }
        }
    }

    time.print_total_time_millis();
}

static void png_main() {
    auto map = some_map();
    mg_log::info(map);

    int tile_size = 4;

    mappng::map_to_png("test.png", map, tile_size);
}

static std::ostream & operator<<(std::ostream & os, const b2Vec2 & vec) {
    os << '{';
    os << vec.x;
    os << ',';
    os << ' ';
    os << vec.y;
    os << '}';
    return os;
}

static constexpr int Box2DToPixelsRatio = 32;
static constexpr float32 PixelsToBox2DRatio = 1.0f / static_cast<float32>(Box2DToPixelsRatio);

static mapgen::Coord2D box2d_to_pixels(float32 x, float32 y) {
    return mapgen::Coord2D(static_cast<int>(x * Box2DToPixelsRatio), static_cast<int>(y * Box2DToPixelsRatio));
}

static mapgen::Coord2D box2d_to_pixels(const b2Vec2 & v) {
    return box2d_to_pixels(v.x, v.y);
}

static b2Vec2 pixels_to_box2d(int x, int y) {
    return b2Vec2(static_cast<float32>(x) * PixelsToBox2DRatio, static_cast<float32>(y) * PixelsToBox2DRatio);
}

static b2Vec2 pixels_to_box2d(const mapgen::Coord2D & c) {
    return pixels_to_box2d(c.x, c.y);
}

static void sdl_main() {
    constexpr int tile_size_pixels = 32;
    constexpr int player_size_pixels = 25;

    int window_width = 800;
    int window_height = 600;

    sdl2::SDLWindowWrapper window("mapgen window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    sdl2::SDLRendererWrapper renderer(window, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
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
    player_body_def.position = pixels_to_box2d(-tile_size_pixels, -tile_size_pixels);
    player_body_def.linearDamping = 1.0;

    b2PolygonShape player_shape;
    auto player_box_size = pixels_to_box2d(player_size_pixels, player_size_pixels);
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
            renderer.copy({coord.x * tile_size_pixels, coord.y * tile_size_pixels, tile_size_pixels, tile_size_pixels}, tiles_texture, {x_tile * tile_size_pixels, y_tile * tile_size_pixels, tile_size_pixels, tile_size_pixels});
        }

        // draw player
        renderer.set_draw_color(0xFF, 0, 0);
        //renderer.draw_rect({window.width() / 2 - player_size_pixels / 2,
        //window.height() / 2 - player_size_pixels / 2, player_size_pixels,
        //player_size_pixels});
        renderer.draw_rect({player_pos.x, player_pos.y, player_size_pixels, player_size_pixels});

        renderer.present();

        ++frame_count;
    }
}

template<typename P>
static void draw_line(png::image<P> & image, P pixel, mg_util::Coord2D c0, mg_util::Coord2D c1) {
    auto dx = std::abs(c0.x - c1.x);
    auto dy = std::abs(c0.y - c1.y);
    mg_util::Coord2D::DimType sx = c0.x < c1.x ? 1 : -1;
    mg_util::Coord2D::DimType sy = c0.y < c1.y ? 1 : -1;
    mg_util::Coord2D::DimType err = (dx > dy ? dx : -dy) / 2;
    mg_util::Coord2D::DimType e2;
    for (;;) {
        image.set_pixel(c0.x, c0.y, pixel);
        if (c0 == c1) {
            break;
        }
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            c0.x += sx;
        }
        if (e2 < dy) {
            err += dx;
            c0.y += sy;
        }
    }
}

static void gg_main() {
    mg_util::GenerativeGrammar gg;
    auto F = gg.createSymbol();
    auto L = gg.createTerminal();
    auto R = gg.createTerminal();
    gg.setCurrent({F});
    gg.addRule(F, {F, L, F, R, F, R, F, L, F});

    uint32_t width = 3, height = 1;
    for (int i = 0; i < 7; ++i) {
        gg.step();
        width = width * 3 - 2;
        height *= 3;

        std::stringstream fileNameSS;
        fileNameSS << "koch" << i << ".png";
        png::image<png::gray_pixel_1> image(width, height);

        png::gray_pixel_1 black(1);
        mg_util::Coord2D curCoord(0, 0);
        auto curDir = mg_util::Direction::Right;
        image.set_pixel(curCoord.x, curCoord.y, black);
        for (auto c: gg.getCurrent()) {
            if (c == F) {
                curCoord += curDir;
                image.set_pixel(curCoord.x, curCoord.y, black);
                curCoord += curDir;
                image.set_pixel(curCoord.x, curCoord.y, black);
            } else if (c == L) {
                curDir = curDir.ccw();
            } else if (c == R) {
                curDir = curDir.cw();
            }
        }
        image.write(fileNameSS.str());
        mg_log::info("wrote to ", fileNameSS.str());
    }
}

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

int main(int argc, char ** argv) {
    std::ios_base::sync_with_stdio(false);
    std::string logFileName;
    {
        std::stringstream logFileNameSS;
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        logFileNameSS << std::put_time(std::localtime(&now_c), "%Y-%m-%d-%H-%M-%S");
        logFileName = logFileNameSS.str();
    }
    // mg_log::LogInit logInit(logFileName);
    mg_log::LogInit logInit;
    sdl2::SDL2Init sdl2init;
    sdl2::SDL2TTFInit sdl2ttfinit;
    sdl2::SDL2ImageInit sdl2imageinit;

#ifdef __APPLE__
    std::ofstream f("/Users/aeubanks/mapgen.log");
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)) {
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

        //sdl_main();
        //ca_main();
        //gg_main();
        png::image<png::rgb_pixel> image(20, 30);
        mg_util::Coord2D c0(image.get_width() - 1, image.get_height() - 1);
        mg_util::Coord2D c1(0, 0);
        draw_line(image, png::rgb_pixel{0, 100, 0}, c0, c1);
        c0.y = 0;
        c1.y = image.get_height() - 1;
        draw_line(image, png::rgb_pixel{100, 100, 0}, c0, c1);
        image.write("image.png");
    } catch (sdl2::sdl2_error & e) {
        mg_log::error("sdl2_error: ", e.what());
        return 1;
    } catch (mg_util::mg_error & e) {
        mg_log::error("mg_error: ", e.what());
        return 2;
    } catch (std::exception & e) {
        mg_log::error("exception: ", e.what());
        return -1;
    }

    return 0;
}
