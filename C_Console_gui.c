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

#include <C_Console_gui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#ifdef WIN32

#include <windows.h>

void get_screen_size(int *x, int *y)
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  if(x)
    *x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  if(y)
    *y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void msleep(int a)
{
  Sleep(a);
}

bool key_is_pressed()
{
  return kbhit();
}

#endif


bool ccg_window_is_resized(int x, int y)
{
  int aux_x, aux_y;

  ccg_screen_get_size(&aux_x, &aux_y);

  if((x != aux_x) || (y != aux_y)){
    return true;
  }

  return false;
}

bool ccg_itoa(char *buf, int in)
{
  if(!buf){
    return false;
  }

  sprintf(buf, "%d", in);
  return true;
}

static bool string_atach_spaces(char *buf, int len)
{
  if(len < 0){
    return false;
  }

  for (int j = 0; j < len; j++){
    strcat(buf, " ");
  }

  return true;
}

static bool colour_parameters_str(char *str, ccg_style_t sty, ccg_colour_t for_col, ccg_colour_t back_col)
{

  if(COLOUR_END <= back_col || back_col <= COLOUR_INVALID){
    return false;
  }
  if(COLOUR_END <= for_col || for_col <= COLOUR_INVALID){
    return false;
  }
  if(STYLE_END <= sty || sty <= STYLE_INVALID){
    return false;
  }

  for (int k = STYLE_INVALID + 1; k < STYLE_END; k++){
    if(sty == k){

      char hold[10];
      ccg_itoa(hold, k);

      strcat(str, hold);
      strcat(str, ";");
      break;
    }
  }

  for (int k = COLOUR_INVALID + 1; k < COLOUR_END; k++){
    if(for_col == k){

      char hold[10];
      ccg_itoa(hold, 30 + k);

      strcat(str, hold);
      strcat(str, ";");
      break;
    }
  }

  for (int k = COLOUR_INVALID + 1; k < COLOUR_END; k++){
    if(back_col == k){

      char hold[10];
      ccg_itoa(hold, 40 + k);

      strcat(str, hold);
      break;
    }
  }

  return true;

}

bool ccg_is_string(const char *str)
{
  return (str[0] == CCG_KEY_ESCAPE && str[1] == '[') ? true : false;
}

int ccg_esclen(const char *str)
{
  int total_len = strlen(str);
  int ret = 0;
  char ch = '\0';

  if(!str){
    return 0;
  }

  if(!ccg_is_string(str)){
    return 0;
  }

  for (int k = 0; ch != 'm'; k++){
    ch = str[k];
    ret++;
  }

  if(str[total_len - 3] != CCG_KEY_ESCAPE){
    // LOG ERROR TO PARSE BBS STRING
    return 0;
  }

  return ret + 3;
}

int ccg_strlen(const char *str)
{
  return strlen(str) - ccg_esclen(str);
}

bool ccg_str_colour(char *buf, ccg_style_t sty, ccg_colour_t for_col, ccg_colour_t back_col)
{
  char aux_buf[CCG_MAX_MENU_OPTION_LEN];
  strcpy(aux_buf, buf);

  if(COLOUR_END <= back_col || back_col <= COLOUR_INVALID){
//    puts("invalid background color");
    return false;
  }
  if(COLOUR_END <= for_col || for_col <= COLOUR_INVALID){
//    puts("invalid foreground color");
    return false;
  }
  if(STYLE_END <= sty || sty <= STYLE_INVALID){
//    puts("invalid style");
    return false;
  }

//TODO dinamic malloc depending of style and color active

  *buf = '\0';
  strcat(buf, "\033[");

  colour_parameters_str(buf, sty, for_col, back_col);

  strcat(buf, "m");
  strcat(buf, aux_buf);
  strcat(buf, "\033[m");

  return true;
}

static void ccg_print_line(const int len, const char *str)
{
  if(!str){
    return;
  }

  if(ccg_strlen(str) != 1){
    return;
  }

  char aux[CCG_MAX_SCREEN_HORIZONTAL_SIZE], ch;
  strcpy(aux, str);

  ch = aux[strlen(aux) - 4];
  aux[strlen(aux) - 3] = '\0';

  for (int k = 1; k < len; k++){
    strncat(aux, &ch, 1);
  }

  strcat(aux, "\033[m");

  puts(aux);

}

static void ccg_char_load(char *dst, const char *src, ccg_style_t sty, ccg_colour_t for_col, ccg_colour_t back_col)
{
  if(ccg_is_char(src)){
    strcpy(dst, src);
  }
  else{
    strcpy(dst, "");
    strncat(dst, src, 1);
    ccg_str_colour(dst, sty, for_col, back_col);
  }

}

bool ccg_option_menu_new(ccg_menu_option_t ret[], char *str_array[], void (*callbacks_array[])(char*), bool *is_prompt_array, int options_qty)
{
  if(!str_array){
    return false;
  }

  if(!callbacks_array){
    return false;
  }

  if(!is_prompt_array){
    return false;
  }

  if(!options_qty){
    return false;
  }

  for (int k = 0; k < options_qty; k++){

    ret[k].name = str_array[k];
    ret[k].callback = callbacks_array[k];
    ret[k].is_prompt = *(is_prompt_array + k * sizeof(bool));
    ret[k].associated_menu = CCG_UNKNOW_MENU;
    ret[k].value_string = NULL;
  }

  return true;
}

static int option_array_max_strlen(ccg_menu_option_t option_array[], int options_qty)
{

  int ret = 0;

  for (int k = 0; k < options_qty; k++){
    if(ccg_strlen(option_array[k].name) > ret){
      ret = ccg_strlen(option_array[k].name);
    }
  }

  return ret;
}

ccg_menu_t* ccg_menu_new(ccg_menu_option_t option_array[], int options_qty, ccg_menu_colour_t colors, ccg_menu_edges_t edges)
{

  if(!option_array){
    return NULL;
  }

  if(options_qty > CCG_MAX_MENU_OPTIONS){
    return NULL;
  }

  ccg_menu_t *ret = malloc(sizeof(ccg_menu_t));

  ret->child = NULL;
  ret->prev = NULL;
  ret->next = NULL;
  ret->father = NULL;

  ret->n_options = options_qty;
  ret->option_max_len = option_array_max_strlen(option_array, options_qty);

  memcpy(&ret->option, option_array, options_qty * sizeof(ccg_menu_option_t));
  free(option_array);

// load default styles and colours
  ret->format.edges.x_margin = edges.x_margin;
  ret->format.edges.y_margin = edges.y_margin;

  ret->format.colors.foreground = colors.foreground;
  ret->format.colors.background = colors.background;
  ret->format.colors.edges_background = colors.edges_background;
  ret->format.colors.edges_foreground = colors.edges_foreground;
  ret->format.colors.style = colors.style;

  ccg_char_load(ret->format.edges.roof, edges.roof, colors.style, colors.edges_foreground, colors.edges_background);
  ccg_char_load(ret->format.edges.walls, edges.walls, colors.style, colors.edges_foreground, colors.edges_background);
  ccg_char_load(ret->format.edges.floor, edges.floor, colors.style, colors.edges_foreground, colors.edges_background);

  return ret;
}

ccg_menu_t* ccg_menu_container()
{
  return malloc(sizeof(ccg_menu_t));
}

void ccg_menu_delete(ccg_menu_t *menu)
{
  ccg_menu_t *next = NULL;

  while (menu != NULL){
    next = menu->next;

    if(menu->child){
      ccg_menu_delete(menu->child);
    }

    free(&menu->format);
    free(&menu->option);
    free(&menu->n_options);
    free(&menu->option_max_len);
    free(&menu->prev);
    free(&menu->next);
    free(&menu->father);
    free(&menu->child);
    free(menu);

    menu = next;

  }

}

bool ccg_add_menu_to_menu(ccg_menu_t *father, ccg_menu_t *child, int assosiated_option)
{

  if(!father){
    return false;
  }

  if(!child){
    return false;
  }

  if(father->option[assosiated_option].is_prompt){
    //the same option cannot be a prompt and acces to a new menu
    //LOG("CANNOT ADD MENU TO PROMPT OPTION");
    return false;
  }

  ccg_menu_t *last_child = father->child;
  int n_menu = 0;

  if(last_child != NULL){

    while (last_child->next != NULL){
      n_menu++;
      last_child = last_child->next;
    }

    n_menu++;
    child->prev = last_child;
    last_child->next = child;
  }
  else{
    child->prev = NULL;
    father->child = child;
  }

  child->next = NULL;
  child->father = father;

  if(assosiated_option != CCG_UNKNOW_OPTION){
    father->option[assosiated_option].associated_menu = n_menu;
  }

  return true;
}

bool ccg_is_char(const char *str)
{
  if(str[1] == '\0'){
    return false;
  }

  return (ccg_strlen(str) == 1) ? true : false;
}

void ccg_print_char(char ch, ccg_style_t sty, ccg_colour_t foreground, ccg_colour_t background)
{

  char buf[CCG_MAX_ESCAPE_GRAPH_LEN + 1] = "";
  strncat(buf, &ch, 1);
  ccg_str_colour(buf, sty, foreground, background);

  printf("%s", buf);
}

static void ccg_menu_print_char_line(const ccg_menu_t *menu, const char *str)
{
  ccg_print_line(1 + menu->format.edges.x_margin + menu->option_max_len + menu->format.edges.x_margin + 1, str);
}

static void ccg_menu_atach_char(char *buf, const ccg_menu_t *menu, const char *str)
{

  if(ccg_is_char(str)){
    strcat(buf, str);
  }
  else{
    char aux[CCG_MAX_ESCAPE_GRAPH_LEN] = "";
    strncat(aux, str, 1);

    ccg_str_colour(aux, menu->format.colors.style, menu->format.colors.edges_foreground, menu->format.colors.edges_background);
    strcat(buf, aux);
  }

}

static void ccg_menu_atach_string_center(char *buf, const ccg_menu_t *menu, const char *str, int len)
{
  if(!menu){
    return;
  }

  if(!buf){
    return;
  }

  if(!str){
    return;
  }

  if(!len){
    return;
  }

  int space_to_fill = len - ccg_strlen(str);
  char aux_buf[CCG_MAX_SCREEN_HORIZONTAL_SIZE - 2] = "";
  char back_str_begin[CCG_MAX_ESCAPE_GRAPH_LEN] = "";

  ccg_str_colour(back_str_begin, menu->format.colors.style, menu->format.colors.foreground, menu->format.colors.background);
  back_str_begin[strlen(back_str_begin) - 3] = '\0';
  strcat(aux_buf, back_str_begin);

  string_atach_spaces(aux_buf, space_to_fill / 2);
  strcat(aux_buf, "\033[m");

  if(ccg_is_string(str)){
    strcat(aux_buf, str);
  }
  else{
    char aux_str[CCG_MAX_SCREEN_HORIZONTAL_SIZE];
    strcpy(aux_str, str);
    ccg_str_colour(aux_str, menu->format.colors.style, menu->format.colors.foreground, menu->format.colors.background);
    strcat(aux_buf, aux_str);
  }

  strcat(aux_buf, back_str_begin);

  if(space_to_fill % 2 == 0){
    string_atach_spaces(aux_buf, space_to_fill / 2);
  }
  else{
    string_atach_spaces(aux_buf, space_to_fill / 2 + 1);
  }

  strcat(aux_buf, "\033[m");

  strcat(buf, aux_buf);
}

void ccg_cursor_move(int x_pos, int y_pos)
{
  printf("\033[%d;%dH", y_pos, x_pos);
}

void ccg_cursor_move_up(int n_lines)
{
  printf("\033[%dA", n_lines);
}

void ccg_cursor_move_down(int n_lines)
{
  printf("\033[%dB", n_lines);
}

void ccg_cursor_move_right(int n_lines)
{
  printf("\033[%dC", n_lines);
}

void ccg_cursor_move_left(int n_lines)
{
  printf("\033[%dD", n_lines);
}

void ccg_screen_clear()
{
  puts("\033[2J");
}

void ccg_screen_line_clear(int row)
{
  ccg_cursor_move(0, row);
  puts("\033[K");
}

void ccg_screen_get_size(int *x, int *y)
{
  get_screen_size(x, y);
}

void ccg_menu_show(const ccg_menu_t *menu)
{
  ccg_menu_show_at(menu, 0, 0);
}

void ccg_menu_show_center(const ccg_menu_t *menu)
{

  if(!menu){
    return;
  }

  int x_scr_len = 0, y_scr_len = 0;

  ccg_screen_get_size(&x_scr_len, &y_scr_len);

  ccg_menu_show_at(menu, (x_scr_len - menu->option_max_len - 2) / 2, (y_scr_len - menu->n_options - 2) / 2);

}

static void ccg_menu_print_row(const ccg_menu_t *menu, char *str, int x_pos, int y_pos)
{

  if(!menu){
    return;
  }

  if(!str){
    return;
  }

  if(!x_pos){
    return;
  }

  if(!y_pos){
    return;
  }

  char buf[CCG_MAX_SCREEN_HORIZONTAL_SIZE] = "";

  ccg_menu_atach_char(buf, menu, menu->format.edges.walls);
  ccg_menu_atach_string_center(buf, menu, str, menu->option_max_len + 2 * menu->format.edges.x_margin);
  ccg_menu_atach_char(buf, menu, menu->format.edges.walls);

  ccg_cursor_move(x_pos, y_pos);
  printf("%s\n", buf);
}

void ccg_menu_show_at(const ccg_menu_t *menu, int x_pos, int y_pos)
{

  if(!menu){
    return;
  }

  if(!x_pos){
    return;
  }

  if(!y_pos){
    return;
  }

  int row = y_pos;

  ccg_cursor_move(x_pos, row);
  ccg_menu_print_char_line(menu, menu->format.edges.roof);

  for (int y = 0; y < menu->format.edges.y_margin; y++){
    row++;
    ccg_menu_print_row(menu, "", x_pos, row);
  }

  for (int y = 0; y < menu->n_options; y++){
    row++;
    ccg_menu_print_row(menu, menu->option[y].name, x_pos, row);
  }

  for (int y = 0; y < menu->format.edges.y_margin; y++){
    row++;
    ccg_menu_print_row(menu, "", x_pos, row);
  }

  row++;
  ccg_cursor_move(x_pos, row);
  ccg_menu_print_char_line(menu, menu->format.edges.floor);
}

static ccg_menu_t* ccg_get_child_menu_from_index(const ccg_menu_t *menu, int index)
{

  if(!menu){
    return NULL;
  }

  if(!menu->child){
    return NULL;
  }

  ccg_menu_t *ret = menu->child;

  for (int k = 0; k < index; k++){
    ret = ret->next;
  }

  return ret;
}

void* ccg_menucpy(ccg_menu_t *to, const ccg_menu_t *from)
{
  return memcpy(to, from, sizeof(ccg_menu_t));
}

bool ccg_timer(time_t *actual_time, time_t *start_time, bool *flag, int timer_ms)
{

  *actual_time = clock();
  double diff_time_ms = (*actual_time - *start_time) * 1000 / CLOCKS_PER_SEC;

  if(diff_time_ms > timer_ms){
    *start_time = clock();
    *flag = !(*flag);
    return true;
  }

  return false;
}

static void ccg_menu_print_string(const ccg_menu_t *menu, const char *buf)
{

  if(!menu){
    return;
  }

  if(!buf){
    return;
  }

  if(ccg_is_string(buf)){
    printf(buf);
  }
  else{

    char aux_buf[CCG_MAX_MENU_OPTION_LEN];
    strcpy(aux_buf, buf);
    ccg_str_colour(aux_buf, menu->format.colors.style, menu->format.colors.foreground, menu->format.colors.background);

    printf(aux_buf);

  }
}

void ccg_menu_row_update(ccg_menu_t *menu, char *buf, char ch)
{
  bool str_is_even = (menu->option_max_len - strlen(buf)) % 2 == 0;

  if(ch != CCG_KEY_BACKSPACE){

    if(str_is_even){

      ccg_cursor_move_left(strlen(buf));
      ccg_menu_print_string(menu, buf);
    }
    else{
      ccg_print_char(ch, menu->format.colors.style, menu->format.colors.foreground, menu->format.colors.background);
    }

  }
  else{
    ccg_cursor_move_left(1);
    ccg_print_char(' ', menu->format.colors.style, menu->format.colors.foreground, menu->format.colors.background);
    ccg_cursor_move_left(1);

    if((menu->option_max_len - strlen(buf)) % 2 == 1){

      ccg_cursor_move_left(strlen(buf));
      ccg_print_char(' ', menu->format.colors.style, menu->format.colors.foreground, menu->format.colors.background);

      ccg_menu_print_string(menu, buf);

    }

  }

}

void ccg_menu_no_prompt_option_highlight(ccg_menu_t *actual_menu, char *actual_line_str, char *buf, int actual_sel_line, int actual_column, int actual_row)
{

  strcpy(buf, actual_line_str);
  ccg_str_colour(buf, actual_menu->format.colors.style, actual_menu->format.colors.background, actual_menu->format.colors.foreground);

  actual_menu->option[actual_sel_line].name = buf;

  ccg_screen_line_clear(actual_row);
  ccg_menu_print_row(actual_menu, buf, actual_column, actual_row);

}

void ccg_menu_prompt_option_highlight(ccg_menu_t *actual_menu, char *buf, int actual_sel_line, int *actual_column, int *actual_row)
{

  if(!buf){
    return;
  }

  if(!actual_menu){
    return;
  }

  if(actual_sel_line >= actual_menu->n_options){
    return;
  }

  strcpy(buf, "");
  actual_menu->option[actual_sel_line].name = buf;
  ccg_screen_line_clear(*actual_row);
  ccg_menu_print_row(actual_menu, buf, *actual_column, *actual_row);

  char ch = '\0';


  *actual_column += actual_menu->format.edges.x_margin + actual_menu->option_max_len / 2 + 1;
  ccg_cursor_move(*actual_column, *actual_row);

  while (ch != CCG_KEY_ENTER){

    if(key_is_pressed()){
      ch = getch();

      switch (ch) {

        case CCG_KEY_ENTER:
          break;

        case CCG_KEY_BACKSPACE:
          if(strlen(buf) > 0){
            buf[strlen(buf) - 1] = '\0';
            ccg_menu_row_update(actual_menu, buf, ch);
          }
          break;

        default:
          if(strlen(buf) < actual_menu->option_max_len){
            strncat(buf, &ch, 1);
            ccg_menu_row_update(actual_menu, buf, ch);
          }
      }
    }

  }

  *actual_column -= actual_menu->format.edges.x_margin + actual_menu->option_max_len / 2 + 1;
}

void ccg_get_coordinates_of_selected_line(ccg_menu_t *menu, int *x_pos, int *y_pos, int actual_sel_line)
{
  int x_scr_len, y_scr_len;
  ccg_screen_get_size(&x_scr_len, &y_scr_len);

  *x_pos = (x_scr_len - menu->option_max_len - 2) / 2;
  *y_pos = (y_scr_len - menu->n_options - 2) / 2 + 1 + menu->format.edges.y_margin + actual_sel_line;
}

void ccg_menu_row_normalize(ccg_menu_t *menu, char *actual_line_str, int actual_sel_line, int actual_column, int actual_row)
{
  if( menu->option[actual_sel_line].is_prompt == false){
    menu->option[actual_sel_line].name = actual_line_str;
    ccg_screen_line_clear(actual_sel_line);
    ccg_menu_print_row(menu, menu->option[actual_sel_line].name, actual_column, actual_row);
  }
}

void ccg_menu_line_exec(ccg_menu_t *menu, char *actual_line_str, int actual_sel_line, char *actual_str_buff, int *actual_column, int *actual_row)
{

  ccg_get_coordinates_of_selected_line(menu, actual_column, actual_row, actual_sel_line);

  if(menu->option[actual_sel_line].is_prompt){
    ccg_menu_prompt_option_highlight(menu, actual_str_buff, actual_sel_line, actual_column, actual_row);
  }
  else{
    ccg_menu_no_prompt_option_highlight(menu, actual_line_str, actual_str_buff, actual_sel_line, *actual_column, *actual_row);
  }

}

void ccg_start(const ccg_menu_t *menu)
{
  ccg_menu_t *actual_menu;
  int actual_sel_line = 0;
  char ch;

  bool flag_harcoded_key_press = false;

  actual_menu = ccg_menu_container();
  ccg_menucpy(actual_menu, menu);

  char actual_str_buff[CCG_MAX_MENU_OPTION_LEN];
  char *actual_line_str = actual_menu->option[actual_sel_line].name;

  int x_scr_len, y_scr_len, actual_row, actual_column;

  ccg_screen_get_size(&x_scr_len, &y_scr_len);
  ccg_get_coordinates_of_selected_line(actual_menu, &actual_column, &actual_row, actual_sel_line);

  ccg_menu_show_center(actual_menu);
  ccg_menu_line_exec(actual_menu, actual_line_str, actual_sel_line, actual_str_buff, &actual_column, &actual_row);

  for (;;){

    if(key_is_pressed() || flag_harcoded_key_press ){


      if (flag_harcoded_key_press){
        flag_harcoded_key_press = false;
      }
      else{
        ch = getch();
      }

      do{

        if(ch == CCG_KEY_UP || ch == CCG_KEY_UP2){

          ccg_menu_row_normalize(actual_menu, actual_line_str, actual_sel_line, actual_column, actual_row);

          actual_sel_line = (actual_sel_line == 0) ? actual_menu->n_options - 1 : actual_sel_line - 1;
          actual_line_str = actual_menu->option[actual_sel_line].name;

          ccg_menu_line_exec(actual_menu, actual_line_str, actual_sel_line, actual_str_buff, &actual_column, &actual_row);

          if(actual_menu->option[actual_sel_line].is_prompt){
            // after executing a prompt option, the selected line go to the above option
            ch = CCG_KEY_DOWN;
            flag_harcoded_key_press = true;
            actual_menu->option[actual_sel_line].callback(actual_str_buff);
            continue;
          }

        }

        if(ch == CCG_KEY_DOWN || ch == CCG_KEY_DOWN2){

          ccg_menu_row_normalize(actual_menu, actual_line_str, actual_sel_line, actual_column, actual_row);

          actual_sel_line = (actual_sel_line == (actual_menu->n_options - 1)) ? 0 : actual_sel_line + 1;

          actual_line_str = actual_menu->option[actual_sel_line].name;
          ccg_menu_line_exec(actual_menu, actual_line_str, actual_sel_line, actual_str_buff, &actual_column, &actual_row);

          if(actual_menu->option[actual_sel_line].is_prompt){

            // after executing a prompt option, the selected line go to the above option
            ch = CCG_KEY_DOWN;
            flag_harcoded_key_press = true;
            actual_menu->option[actual_sel_line].callback(actual_str_buff);
            continue;
          }

        }

        if(ch == CCG_KEY_ENTER){

          if(actual_menu->option[actual_sel_line].callback != NULL){

            if(actual_menu->option[actual_sel_line].is_prompt){
              //
              // this cannot happen because when ending a prompt the line changes to the down option
              //LOG("ERROR KEY PRESS ENTER IN A PROMPT");
            }
            else{
              actual_menu->option[actual_sel_line].callback(NULL);
            }

          }

          if(actual_menu->option[actual_sel_line].associated_menu != CCG_UNKNOW_MENU){
            ccg_menucpy(actual_menu, ccg_get_child_menu_from_index(actual_menu, actual_menu->option[actual_sel_line].associated_menu));

            actual_sel_line = 0;
            actual_line_str = actual_menu->option[actual_sel_line].name;

            ccg_menu_line_exec(actual_menu, actual_line_str, actual_sel_line, actual_str_buff, &actual_column, &actual_row);

            ccg_screen_clear();
            ccg_menu_show_center(actual_menu);
          }

        }

        if(ch == CCG_KEY_ESCAPE){

          if(actual_menu->father != NULL){
            ccg_menucpy(actual_menu, actual_menu->father);

            actual_sel_line = 0;
            actual_line_str = actual_menu->option[actual_sel_line].name;

            ccg_menu_line_exec(actual_menu, actual_line_str, actual_sel_line, actual_str_buff, &actual_column, &actual_row);

            ccg_screen_clear();
            ccg_menu_show_center(actual_menu);
          }

        }

      } while (0);



    }

//    msleep(10);
//    bbs_screen_get_size(&window_x_len,&window_y_len);
//
//    if( bbs_window_is_resized(window_x_len,window_y_len) ){
//      bbs_screen_clear();
//      bbs_menu_show_center(actual_menu);
//    }

  }

  ccg_menu_delete(actual_menu);
}


