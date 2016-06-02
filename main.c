#include <ncurses.h>

int main(){
  int line = -1;
  initscr();
  cbreak();
  noecho();
  start_color();
  use_default_colors();

  short int r,g,b,c1,c2;
  for(int i = 0; i < 16; i++){
    init_pair(i,i,-1); 
    color_content(i,&r,&g,&b);
    printw(" *.color%d: #%02x%02x%02x\n",i,(r+20)/4,(g+20)/4,(b+20)/4);
  }
  if(!can_change_color()){
    endwin();
    return 0;
  }

  mvchgat(0, 0, 11, A_NORMAL, 0, NULL);
  while(1){
    keypad(stdscr,TRUE);
    refresh();
    int c = getch();
    switch(c){

      case KEY_UP:
        if(line > 0){
          mvchgat(line, 0, 11, A_NORMAL, 0, NULL);
          line--;
        }
        break;
      case KEY_DOWN:
        if(line < 15){
          mvchgat(line, 0, 11, A_NORMAL, 0, NULL);
          line++;
        }
        break;

      case 'a':

        color_content(line,&r,&g,&b);
        r+=4;
        if(r > 1000)
          r-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(r+20)/4,(g+20)/4,(b+20)/4);
        break;

      case 'z':

        color_content(line,&r,&g,&b);
        r-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(r+20)/4,(g+20)/4,(b+20)/4);
        break;
      
      case 's':

        color_content(line,&r,&g,&b);
        g+=4;
        if(g > 1000)
          g-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(r+20)/4,(g+20)/4,(b+20)/4);
        break;
      
      case 'x':

        color_content(line,&r,&g,&b);
        g-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(r+20)/4,(g+20)/4,(b+20)/4);
        break;
      
      case 'd':

        color_content(line,&r,&g,&b);
        b+=4;
        if(b > 1000)
          b-=4;
        init_color(line,r,g,b);

        printw(" *.color%d: #%02x%02x%02x\n",line,(r+20)/4,(g+20)/4,(b+20)/4);
        break;
      
      case 'c':

        color_content(line,&r,&g,&b);
        b-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(r+20)/4,(g+20)/4,(b+20)/4);
        break;

      case 'q':
        clear();
        endwin();
        return 0;
    }
    mvchgat(line, 0, 11, A_STANDOUT, line, NULL);
  }

  endwin();

  return 0;
}

