/* MIT License

Copyright (c) 2020 MAGNIBORO

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 */
 /* Hello World in C, Ansi-style */

#ifndef C_CONSOLE_GUI_H_
#define C_CONSOLE_GUI_H_

#ifndef  WIN32
#def WIN32
#endif

#include <stdbool.h>

#define MAX_SCREEN_HORIZONTAL_SIZE 200
#define MAX_SCREEN_VERTICAL_SIZE 60

#define MAX_MENU_NAME_LEN 40
#define MAX_ESCAPE_GRAPH_LEN 13
#define MAX_MENU_OPTIONS (MAX_SCREEN_VERTICAL_SIZE - 2 - 2) //vertical - walls - menu title
#define MAX_MENU_OPTION_LEN (MAX_SCREEN_HORIZONTAL_SIZE - 1 - 2 + MAX_ESCAPE_GRAPH_LEN ) // horizontal - '\0' - menu walls + bbs graph characters

#define MAX_BBS_CHAR_LEN 15

#define UNKNOW_OPTION (MAX_MENU_OPTIONS + 1)
#define UNKNOW_MENU 255

#define UNDERSCORE_BLINK_MS 500

#define KEY_ENTER '\r'
#define KEY_ESCAPE '\033'
#define KEY_UP 'w'
#define KEY_UP2 'W'
#define KEY_DOWN 's'
#define KEY_DOWN2 'S'
#define KEY_BACKSPACE 'm'



typedef enum{
  STATE_LOW,
  STATE_SAME,
  STATE_UP
}menu_state;

typedef enum
{
  COLOUR_INVALID = -1,
  COLOUR_BLACK,
  COLOUR_RED,
  COLOUR_GREEN,
  COLOUR_YELLOW,
  COLOUR_BLUE,
  COLOUR_PURPLE,
  COLOUR_CYAN,
  COLOUR_WHITE,
  COLOUR_END
}colour_t;

typedef enum
{
  STYLE_INVALID = -1,
  STYLE_DISABLED,
  STYLE_BOLD,
  STYLE_GREY,
  STYLE_ITALIC,
  STYLE_UNDERSCORE,
  STYLE_BLINK,
  STYLE_UNUSED,
  STYLE_REVERSE,
  STYLE_CONCEALED,
  STYLE_END,
}style_t;

typedef struct
{
  colour_t background;
  colour_t foreground;
  colour_t edges_background;
  colour_t edges_foreground;
  style_t style;
}bbs_menu_colour_t;

typedef struct
{
  int x_margin;
  int y_margin;
  char roof[MAX_BBS_CHAR_LEN];
  char walls[MAX_BBS_CHAR_LEN];
  char floor[MAX_BBS_CHAR_LEN];
}bbs_menu_edges_t;

typedef struct{

  char *name;
  void (*callback)(char*);
  int associated_menu;
  bool is_prompt;
  char *value_string;
}bbs_menu_option_t;

typedef struct{
  bbs_menu_colour_t colors;
  bbs_menu_edges_t edges;
}bbs_menu_format;

typedef struct
{
  struct bbs_menu_t *child, *next, *prev, *father;

  bbs_menu_option_t option[MAX_MENU_OPTIONS];
  int n_options;
  int option_max_len;
  bbs_menu_format format;
}bbs_menu_t;

bool bbs_itoa(char *buf, int in);
bool bbs_is_char(const char *str);
bool bbs_is_string(const char *str);
void bbs_screen_get_size(int *x, int *y);

int bbs_strlen(const char *str);
bool bbs_str_colour(char *buf, style_t sty, colour_t for_col, colour_t back_col);
void bbs_screen_get_size(int *x, int *y);

void *bbs_menucpy(bbs_menu_t *to, const bbs_menu_t *from);
bool bbs_option_menu_new(bbs_menu_option_t ret[], char *str_array[], void (*callbacks_array[])(char*),bool *is_prompt_array, int options_qty);
bbs_menu_t* bbs_menu_new(bbs_menu_option_t *option_array ,int options_qty, bbs_menu_colour_t colors, bbs_menu_edges_t edges);
void bbs_menu_delete(bbs_menu_t *menu);
void bbs_menu_show(const bbs_menu_t *menu);
void bbs_menu_show_at(const bbs_menu_t *menu, int x_pos, int y_pos);
void bbs_screen_clear();

bool bbs_add_menu_to_menu(bbs_menu_t *father,bbs_menu_t *child, int assosiated_option);
void bbs_start();


#endif /* C_CONSOLE_GUI_H_ */
