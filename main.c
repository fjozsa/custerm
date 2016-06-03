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
    
    
int main(){
  int line = -1;
  initscr();
  cbreak();
  noecho();
  start_color();

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
  


  short int r,g,b;
  for(int i = 0; i < 16; i++){

    char dnum[2], snum[2];
    char prefix[13] = "Urxvt.color";
    
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
    init_color(i,hex2cursescolor(col+1),hex2cursescolor(col+3),hex2cursescolor(col+5));
    color_content(i,&r,&g,&b);
    init_pair(i,0,i); 
    printw(" *.color%d: #%02x%02x%02x\n",i,(int)(r*.255),(int)(g*.255),(int)(b*.255));
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
        if(r > 1024)
          r-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(int)(r*.255),(int)(g*.255),(int)(b*.255));
        break;

      case 'z':

        color_content(line,&r,&g,&b);
        r-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(int)(r*.255),(int)(g*.255),(int)(b*.255));
        break;
      
      case 's':

        color_content(line,&r,&g,&b);
        g+=4;
        if(g > 1000)
          g-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(int)(r*.255),(int)(g*.255),(int)(b*.255));
        break;
      
      case 'x':

        color_content(line,&r,&g,&b);
        g-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(int)(r*.255),(int)(g*.255),(int)(b*.255));
        break;
      
      case 'd':

        color_content(line,&r,&g,&b);
        b+=4;
        if(b > 1000)
          b-=4;
        init_color(line,r,g,b);

        printw(" *.color%d: #%02x%02x%02x\n",line,(int)(r*.255),(int)(g*.255),(int)(b*.255));
        break;
      
      case 'c':

        color_content(line,&r,&g,&b);
        b-=4;
        init_color(line,r,g,b);
        printw(" *.color%d: #%02x%02x%02x\n",line,(int)(r*.255),(int)(g*.255),(int)(b*.255));
        break;

      case 'q':
        clear();
        endwin();
        return 0;
    }
    mvchgat(line, 0, 11, COLOR_PAIR(line), line, NULL);
  }

  endwin();

  return 0;
}

