#include <ncurses.h>
#include <X11/Xresource.h>
#include <string.h>
#include <stdlib.h>

int hex2cursescolor(char * hex){
  int num[2];
  for(int i = 0; i < 2; i++){
    if(hex[i] > 47 && hex[i] < 58){
      num[i] = hex[i] - 48;
    }
    else if(hex[i] > 64 && hex[i] < 71){
      num[i] = hex[i] -55;
    }
    else if((hex[i] > 96 && hex[i] < 102)){
      num[i] = hex[i] -87;
    }
  }
  if(num[0] == 0){
    return (num[1]*4) ;
  }
  else{
    return (num[0]*16*4)+(num[1]*4);
  }
}

void _printline(int line, short r, short g, short b){

  if(line == 0){
    printw(" *.background: #%02x%02x%02x\n",(int)(r*.255),(int)(g*.255),(int)(b*.255));
  }
  else if(line == 1){
    printw(" *.foreground: #%02x%02x%02x\n",(int)(r*.255),(int)(g*.255),(int)(b*.255));
  }
  else{
    if(line > 11){
      printw(" *.color%d:    #%02x%02x%02x\n",line-2,(int)(r*.255),(int)(g*.255),(int)(b*.255));
    }
    else{
      printw(" *.color%d:     #%02x%02x%02x\n",line-2,(int)(r*.255),(int)(g*.255),(int)(b*.255));
    }
  }
}

int _setup(){
  short int r,g,b;
  XrmDatabase  db;   /* Xresources database */
  XrmValue   ret;  /* structure that holds pointer to string */
  Display   *dpy;  /* X connection */
  char    *resource_manager;
  char    *type; /* class of returned variable */

  if (!(dpy = XOpenDisplay(NULL)))
    return -1;

  XrmInitialize();
  resource_manager = XResourceManagerString(dpy);

  if (resource_manager == NULL)
    return -2;

  db = XrmGetStringDatabase(resource_manager);

  if (db == NULL)
    return -3;


  XrmGetResource(db,"*.background","String", &type, &ret);
  init_color(0,hex2cursescolor(ret.addr+1),hex2cursescolor(ret.addr+3),hex2cursescolor(ret.addr+5));
  color_content(0,&r,&g,&b);
  init_pair(0,0,0); 
  _printline(0,r,g,b);
  XrmGetResource(db,"*.foreground","String", &type, &ret);
  init_color(1,hex2cursescolor(ret.addr+1),hex2cursescolor(ret.addr+3),hex2cursescolor(ret.addr+5));
  color_content(1,&r,&g,&b);
  init_pair(1,1,0); 
  _printline(1,r,g,b);
  for(int i = 0; i < 16; i++){

    char dnum[2], snum[1];
    char prefix[13] = "*.color";

    //set up color string and grab terminal values
    if(i < 10){
      sprintf(snum,"%d",i);
      strcat(prefix,snum);
      XrmGetResource(db,prefix,"String", &type, &ret);
    }
    else{
      sprintf(dnum,"%d",i);
      strcat(prefix,dnum);
      XrmGetResource(db,prefix,"String", &type, &ret);
    }

    char * col = ret.addr; 
    init_color(i+2,hex2cursescolor(col+1),hex2cursescolor(col+3),hex2cursescolor(col+5));
    color_content(i+2,&r,&g,&b);
    init_pair(i+2,i+2,0); 
    _printline(i+2,r,g,b);
  }
  return 0;
}

int main(){
  int line, col;
  line = col = 0;
  initscr();
  cbreak();
  noecho();
  start_color();

  if(_setup()){
    puts("Error on loading Xdb");
    return -1;
  }
  short int r,g,b;
  mvchgat(0, 0, 11, A_NORMAL, 0, NULL);
  while(1){
    keypad(stdscr,TRUE);
    refresh();
    int c = getch();
    switch(c){

      case KEY_UP:
        color_content(line,&r,&g,&b);
        if( line > 0 && col == 0){
          mvchgat(line, 0, 15, A_NORMAL, 0, NULL);
          line--;
        }
        else{

          if(col == 1){
            r+=4;
          }
          else if(col == 2){
            g+=4;
          }
          else if(col == 3){
            b+=4;
          }
          init_color(line,r,g,b);
          _printline(line,r,g,b);
          mvchgat(line,(col*2) + 14,2, A_BLINK,line,NULL);
        }
        break;
      case KEY_DOWN:
        color_content(line,&r,&g,&b);
        if(line < 17 && col == 0){
          mvchgat(line, 0, 15, A_NORMAL, 0, NULL);
          line++;
        }
        else{

          if(col == 1){
            r-=4;
          }
          else if(col == 2){
            g-=4;
          }
          else if(col == 3){
            b-=4;
          }
          init_color(line,r,g,b);
          _printline(line,r,g,b);
          mvchgat(line,(col*2) + 14,2, A_BLINK,line,NULL);
        }
        break;

      case KEY_RIGHT:
        if(col < 3){
          col++;
        mvchgat(line, 0, 45, A_NORMAL, 0, NULL);
        mvchgat(line,(col*2) + 14,2, A_BLINK,line,NULL);

        }

        break;

      case KEY_LEFT:
        if(col > 0){

          if(col<2){
            col--;
            mvchgat(line, 0, 45, A_NORMAL, 0, NULL);
            break;
          }
          col--;
        mvchgat(line, 0, 45, A_NORMAL, 0, NULL);
        mvchgat(line,(col*2) + 14,2, A_BLINK,line,NULL);
        }

        break;
    }
    mvchgat(line, 0, 15, COLOR_PAIR(line), line, NULL);
  }

  endwin();

  return 0;
}

