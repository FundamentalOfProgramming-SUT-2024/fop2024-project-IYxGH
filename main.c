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


//functions
void board();
void menu_1();
void new_user_page();
void clear_line(int y, int x, int length);
int check_email(char e[]);
int check_pass(char p[]);
int name_exist(char n[]);
void add_name(char n[]);
void login_page();
// void menu_2();
void add_user(user_info u);
int pass_authenticator(user_info u);

int main(){
    initscr();
    curs_set(FALSE);
    board();
    keypad(stdscr , TRUE);
    menu_1();
    // menu_2();
    


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
        login_page(&u);
        break;

    case 2:
        strcpy(u.name , "GUEST");
        break;
    }
}

void new_user_page(user_info *u)
{
    clear();
    echo();
    board();
    curs_set(TRUE);
    mvprintw( LINES/2 - 7 , COLS/2 - 8 , "Username: ");
    mvprintw( LINES/2 - 5 , COLS/2 - 8 , "Email: ");
    mvprintw( LINES/2 - 3 , COLS/2 - 8 , "Password: ");

    while (1)
    {
        clear_line(LINES/2 - 7 , COLS/2 +2 , 50);
        move(LINES/2 - 7 , COLS/2 +2);
        getnstr(u->name ,  50);

        if(name_exist( u->name ) )
        {
            mvprintw(LINES/2   , COLS/2 - 4 , "This username already exists!");
        }else{
            add_name(u->name);
            break;
        }  
    }
    clear_line(LINES/2   , COLS/2 - 5 , 40);
    while(1){
        clear_line(LINES/2 - 5 , COLS/2 +2 , 50);
        move(LINES/2 - 5 , COLS/2 +2);
        getnstr(u->email ,  50);
        if( check_email(u->email) == 1){
            break;
        }else{
            mvprintw( LINES/2   , COLS/2 - 4 , "Not valid email!");
        }


    }
    clear_line(LINES/2   , COLS/2 - 5 , 40);
    while(1){
        clear_line(LINES/2 - 3 , COLS/2 +2 , 50);
        move(LINES/2 - 3 , COLS/2 +2);
        getnstr(u->pass ,  50);
        if(check_pass( u->pass ) == 0){
            mvprintw( LINES/2   , COLS/2 - 4 , "Not valid password!");
        }else{
            break;
        }
    }
    curs_set(FALSE);
    add_user( *u);

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

int name_exist(char n[]){

    FILE *file = fopen("usernames.txt", "r");


    char line[100];
    while (fgets(line, sizeof(line), file)) {
        if(line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }
        if (strcmp(line, n) == 0) {
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0; 
}

void add_name(char n[]){
    FILE *file = fopen("usernames.txt" , "a");
    fprintf(file , "\n%s" , n);
    fclose(file);
}

void clear_line(int y, int x, int length) {
    move(y, x);
    for (int i = 0; i < length; i++) {
        addch(' ');
    }
    move(y, x);
}

void login_page(){
    clear();
    echo();
    board();
    curs_set(TRUE);
    mvprintw( LINES/2 - 7 , COLS/2 - 8 , "username: ");
    mvprintw( LINES/2 - 5 , COLS/2 - 8 , "Password: ");

    while (1)
    {
        clear_line(LINES/2 - 7 , COLS/2 +2 , 50);
        move(LINES/2 - 7 , COLS/2 +2);
        getnstr(u.name ,  50);

        if(name_exist( u.name ) )
        {
            break;
        }else{
            mvprintw(LINES/2   , COLS/2 - 4 , "Not valid usrename!");
        }  
    }

    clear_line(LINES/2   , COLS/2 - 5 , 40);
    while(1){
        clear_line(LINES/2 - 5 , COLS/2 +2 , 50);
        move(LINES/2 - 5 , COLS/2 +2);
        getnstr(u.pass ,  50);
        if(pass_authenticator( u ) == 0){
            mvprintw( LINES/2  , COLS/2 - 4 , "Incorrect passwoord!");
        }else{
            break;
        }
    }
}

void add_user(user_info u){
    char temp[100];
    strcpy(temp , u.name);
    strcat(temp , ".txt");
    FILE *file = fopen( temp , "w");
    fprintf(file , "%s\n%s\n%s" , u.name , u.email , u.pass);
    fclose(file);
}

int pass_authenticator(user_info u){
    char temp[100];
    strcpy(temp , u.name);
    strcat(temp , ".txt");
    FILE *file = fopen( temp , "r");
    int line_number = 0;
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_number++;
        if (line_number == 3) {
            if(strcmp(buffer , u.pass) == 0){
                fclose(file);
                return 1;
            }else{
                fclose(file);
                return 0;
            }
        }
    }
}



