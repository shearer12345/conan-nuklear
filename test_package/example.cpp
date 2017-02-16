#include <assert.h>
#include <iostream>
#include <limits.h>
#include <math.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// NOTE: include before SDL.h
#include <GL/glew.h>

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include <nuklear.h>
// blank line to prevent clang-format reordering the #includes
#include "nuklear_sdl_gl3.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define UNUSED(a) (void)a
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a) / sizeof(a)[0])

int main(int argc, char *argv[]) {
  // SDL initialise
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
                    SDL_GetError());
    return 1;
  }
  SDL_Log("SDL initialised OK!\n");

  // Window Creation
  SDL_Window *win = nullptr;
  win = SDL_CreateWindow("ConanNuklearTest", 100, 100, 600, 600,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
                             SDL_WINDOW_ALLOW_HIGHDPI);
  if (win == nullptr) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "SDL_CreateWindow init error: %s\n", SDL_GetError());
    return 1;
  }

  // Set OpenGL context parameters
  int major = 3, minor = 3;
  SDL_Log("Asking for OpenGL %d.%d context\n", major, minor);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_PROFILE_CORE); // use core profile
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);      // default, probably

  // OpenGL Context Creation
  SDL_GLContext glcontext = SDL_GL_CreateContext(win);
  if (glcontext == NULL) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "SDL_GL_CreateContext init error: %s\n", SDL_GetError());
    return 1;
  }
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
  SDL_Log("Got an OpenGL %d.%d context\n", major, minor);

  // initialise GLEW - sets up the OpenGL function pointers for the version of
  // OpenGL we're using
  GLenum rev;
  glewExperimental = GL_TRUE; // GLEW isn't perfect - see
  // https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
  rev = glewInit();
  if (GLEW_OK != rev) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "glewInit error: %s\n",
                    glewGetErrorString(rev));
    return 1;
  }
  SDL_Log("glew initialised OK!\n");

  bool done = false;

  /* GUI */
  nk_context *ctx = nk_sdl_init(win);
  if (ctx == nullptr) {
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                    "Nuklear Context init error: %s\n",
                    glewGetErrorString(rev));
    return 1;
  }
  SDL_Log("Nuklear Context ok!\n");

  /* Load Fonts: if none of these are loaded a default font will be used  */
  /* Load Cursor: if you uncomment cursor loading please hide the cursor */
  {
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas,
     * "../../../extra_font/DroidSans.ttf", 14, 0);*/
    /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas,
     * "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
    /*struct nk_font *future = nk_font_atlas_add_from_file(atlas,
     * "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
    /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas,
     * "../../../extra_font/ProggyClean.ttf", 12, 0);*/
    /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas,
     * "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
    /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas,
     * "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
    nk_sdl_font_stash_end();
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    /*nk_style_set_font(ctx, &roboto->handle)*/;
  }

  /* style.c */
  /*set_style(ctx, THEME_WHITE);*/
  /*set_style(ctx, THEME_RED);*/
  /*set_style(ctx, THEME_BLUE);*/
  /*set_style(ctx, THEME_DARK);*/

  nk_color background;

  int win_width, win_height;
  SDL_GetWindowSize(win, &win_width, &win_height);

  background = nk_rgb(28, 48, 62);

  while (!done) {

    // Input
    SDL_Event event;
    nk_input_begin(ctx);
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        done = true;
      nk_sdl_handle_event(&event);
    }
    nk_input_end(ctx);

    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 200, 200),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_CLOSABLE | NK_WINDOW_MINIMIZABLE |
                     NK_WINDOW_TITLE)) {
      nk_menubar_begin(ctx);
      nk_layout_row_begin(ctx, NK_STATIC, 25, 2);
      nk_layout_row_push(ctx, 45);
      if (nk_menu_begin_label(ctx, "FILE", NK_TEXT_LEFT, nk_vec2(120, 200))) {
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_menu_item_label(ctx, "OPEN", NK_TEXT_LEFT);
        nk_menu_item_label(ctx, "CLOSE", NK_TEXT_LEFT);
        nk_menu_end(ctx);
      }
      nk_layout_row_push(ctx, 45);
      if (nk_menu_begin_label(ctx, "EDIT", NK_TEXT_LEFT, nk_vec2(120, 200))) {
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_menu_item_label(ctx, "COPY", NK_TEXT_LEFT);
        nk_menu_item_label(ctx, "CUT", NK_TEXT_LEFT);
        nk_menu_item_label(ctx, "PASTE", NK_TEXT_LEFT);
        nk_menu_end(ctx);
      }
      nk_layout_row_end(ctx);
      nk_menubar_end(ctx);

      enum { EASY, HARD };
      static int op = EASY;
      static int property = 20;
      nk_layout_row_static(ctx, 30, 80, 1);
      if (nk_button_label(ctx, "button"))
        fprintf(stdout, "button pressed\n");
      nk_layout_row_dynamic(ctx, 30, 2);
      if (nk_option_label(ctx, "easy", op == EASY))
        op = EASY;
      if (nk_option_label(ctx, "hard", op == HARD))
        op = HARD;
      nk_layout_row_dynamic(ctx, 25, 1);
      nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);
    }
    nk_end(ctx);

    // Set Viewport
    SDL_GetWindowSize(win, &win_width, &win_height);
    glViewport(0, 0, win_width, win_height);

    // Clear the back buffer
    float bg[4];
    nk_color_fv(bg, background);
    glClearColor(bg[0], bg[1], bg[2], bg[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render
    nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

    // Present the back buffer
    SDL_GL_SwapWindow(win);
  }
  // Clean up
  SDL_Log("Finished. Cleaning up and closing down\n");

  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
