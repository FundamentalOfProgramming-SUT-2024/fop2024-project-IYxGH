#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef struct 
{
    char name[50];
    char email[100];
    char pass[50];
} user_info;

user_info u;
char spaces[50];


//functions
void board();
void menu_1();
void new_user_page();
int check_email(char e[]);
int check_pass(char p[]);

int main(){
    for(int i = 0 ; i < 50 ; i++ ){
        spaces[i] = ' ';
    }
    spaces[50] = '\0'; 
    initscr();
    curs_set(FALSE);
    board();
    keypad(stdscr , TRUE);
    menu_1();
    


    endwin();
    return 0;
}


void board()
{
    clear();
    for (int x = 0; x < COLS; x++)
    {
        mvprintw(0, x, "*");
        mvprintw(LINES - 1, x, "*");
    }
    for (int y = 0; y < LINES; y++)
    {
        mvprintw(y, 0, "*");
        mvprintw(y, COLS - 1, "*");
    }
}

void menu_1()
{
    board();
    int choice = 0;
    const char *sign_or_log[] = {"New user" , "Login" , "Guest"};


    while(1){
        for(int i = 0 ; i < 3 ; i++)
        {
            if(i == choice){
                attron(A_REVERSE);
            }
            mvprintw(LINES/2 - 1 + 2*i , COLS/2 - 5 , "%s" ,sign_or_log[i]);
            if(i == choice){
                attroff(A_REVERSE);
            }
        }
        int ch = getch();
        if (ch == KEY_UP )
            choice = (choice == 0) ? 2 : choice - 1;
        else if(ch == KEY_DOWN)
            choice = (choice == 2) ? 0 : choice + 1;
        else if( ch == 10)
            break;
    }
    switch (choice)
    {
    case 0:
        new_user_page(&u);
        break;

    case 1:
    
    default:
        break;
    }
}

void new_user_page(user_info *u)
{
    clear();
    echo();
    curs_set(TRUE);
    mvprintw( LINES/2 - 7 , COLS/2 - 8 , "Name: ");
    mvprintw( LINES/2 - 5 , COLS/2 - 8 , "Email: ");
    mvprintw( LINES/2 - 3 , COLS/2 - 8 , "Password: ");

    while (1)
    {
    
    move(LINES/2 - 7 , COLS/2 +2);
    getnstr(u->name ,  50);

    if(1)
    {
        break;
    }
        
    }

    while(1){
        
        move(LINES/2 - 5 , COLS/2 +2);
        getnstr(u->email ,  50);
        if( check_email(u->email) == 1){
            break;
        }else{
            mvprintw( LINES/2   , COLS/2 - 5 , "Not valid email!            ");
        }


    }
    mvprintw(LINES/2 , COLS/2 -5  , "%s" , spaces );
    while(1){

        move(LINES/2 - 3 , COLS/2 +2);
        getnstr(u->pass ,  50);
        if(check_pass( u->pass ) == 0){
            mvprintw( LINES/2   , COLS/2 - 5 , "Not valid password!        ");
        }else{
            break;
        }
    }
    curs_set(FALSE);
}

int check_email(char e[]){
    int i = 0;
    if(e[i] == '@'){return 0;};
    while(e[i] != '@'){
        if(e[i] == '\0' || e[i] == ' '){
            return 0;
        }  
        i++;     
    }
    i++;
    if(e[i] == '.'){return 0;};
    while(e[i] != '.'){
        if(e[i] >= 'a' && e[i] <= 'z'){
            i++;
        }
        else if(e[i] >= 'A' && e[i] <= 'Z'){
            i++;
        }else{
            return 0;
        }
    }
    i++;
    if(e[i] == '\0'){return 0;};
    while(e[i] != '\0'){
        if(e[i] >= 'a' && e[i] <= 'z'){
            i++;
        }
        else if(e[i] >= 'A' && e[i] <= 'Z'){
            i++;
        }else{
            return 0;
        }
    }

    return 1;
}

int check_pass(char p[]){
    int i = 0; 
    int small =0 , cap = 0 , num = 0;
    for(int i = 0 ; i < strlen(p) ; i++){
        if(p[i] >= 'a' && p[i] <= 'z'){
            small = 1;
        }
        if(p[i] >= 'A' && p[i] <= 'Z'){
            cap = 1;
        }
        if(p[i] >= '0' && p[i] <= '9'){
            num = 1;
        }
    }
    if(small == 1 && cap == 1 && num == 1){
        return 1;
    }
    return 0;
}


