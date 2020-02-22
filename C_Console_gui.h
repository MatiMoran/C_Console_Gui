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

#define CCG_MAX_SCREEN_HORIZONTAL_SIZE 200
#define CCG_MAX_SCREEN_VERTICAL_SIZE 60

#define CCG_MAX_MENU_NAME_LEN 40
#define CCG_MAX_ESCAPE_GRAPH_LEN 13
#define CCG_MAX_MENU_OPTIONS (CCG_MAX_SCREEN_VERTICAL_SIZE - 2 - 2) //vertical - walls - menu title
#define CCG_MAX_MENU_OPTION_LEN (CCG_MAX_SCREEN_HORIZONTAL_SIZE - 1 - 2 + CCG_MAX_ESCAPE_GRAPH_LEN ) // horizontal - '\0' - menu walls + bbs graph characters

#define CCG_MAX_CCG_CHAR_LEN 15

#define CCG_UNKNOW_OPTION (CCG_MAX_MENU_OPTIONS + 1)
#define CCG_UNKNOW_MENU 255

#define CCG_UNDERSCORE_BLINK_MS 500

#define CCG_KEY_ENTER '\r'
#define CCG_KEY_ESCAPE '\033'
#define CCG_KEY_UP 'w'
#define CCG_KEY_UP2 'W'
#define CCG_KEY_DOWN 's'
#define CCG_KEY_DOWN2 'S'
#define CCG_KEY_BACKSPACE 'm'



typedef enum{
  STATE_LOW,
  STATE_SAME,
  STATE_UP
}ccg_menu_state;

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
}ccg_colour_t;

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
}ccg_style_t;

typedef struct
{
  ccg_colour_t background;
  ccg_colour_t foreground;
  ccg_colour_t edges_background;
  ccg_colour_t edges_foreground;
  ccg_style_t style;
}ccg_menu_colour_t;

typedef struct
{
  int x_margin;
  int y_margin;
  char roof[CCG_MAX_CCG_CHAR_LEN];
  char walls[CCG_MAX_CCG_CHAR_LEN];
  char floor[CCG_MAX_CCG_CHAR_LEN];
}ccg_menu_edges_t;

typedef struct{

  char *name;
  void (*callback)(char*);
  int associated_menu;
  bool is_prompt;
  char *value_string;
}ccg_menu_option_t;

typedef struct{
  ccg_menu_colour_t colors;
  ccg_menu_edges_t edges;
}ccg_menu_format;

typedef struct
{
  struct bbs_menu_t *child, *next, *prev, *father;

  ccg_menu_option_t option[CCG_MAX_MENU_OPTIONS];
  int n_options;
  int option_max_len;
  ccg_menu_format format;
}ccg_menu_t;


bool ccg_itoa(char *buf, int in);
bool ccg_is_char(const char *str);
bool ccg_is_string(const char *str);
int ccg_strlen(const char *str);
bool ccg_str_colour(char *buf, ccg_style_t sty, ccg_colour_t for_col, ccg_colour_t back_col);

void ccg_screen_get_size(int *x, int *y);
void ccg_screen_clear();

void ccg_menu_show(const ccg_menu_t *menu);
void ccg_menu_show_at(const ccg_menu_t *menu, int x_pos, int y_pos);

void *ccg_menucpy(ccg_menu_t *to, const ccg_menu_t *from);

bool ccg_option_menu_new(ccg_menu_option_t ret[], char *str_array[], void (*callbacks_array[])(char*),bool *is_prompt_array, int options_qty);
void ccg_menu_delete(ccg_menu_t *menu);
ccg_menu_t* ccg_menu_new(ccg_menu_option_t *option_array ,int options_qty, ccg_menu_colour_t colors, ccg_menu_edges_t edges);
bool ccg_add_menu_to_menu(ccg_menu_t *father,ccg_menu_t *child, int assosiated_option);

void ccg_start();


#endif /* C_CONSOLE_GUI_H_ */
