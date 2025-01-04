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
    int guest;
} user_info;

typedef struct{
    int x;
    int y;
}pos;

typedef struct{
    pos up_left;
    pos bottom_right;
    int L;
    int W;
    int E;
}room_info;

user_info u;
room_info room[100];
room_info s;
pos player;


    // functions
        void board();
        void menu_1();
        void new_user_page();
        void clear_line(int y, int x, int length);
        int check_email(char e[]);
        int check_pass(char p[]);
        int name_exist(char n[]);
        void add_name(char n[]);
        void login_page();
        void menu_2();
        void add_user(user_info u);
        int pass_authenticator(user_info u); //check if the password is valid
        char *generatePassword(); //generate random valid password
        int randomint(int a , int b); //generate random number between a and b
        void new_game(user_info u); //game page
        void room_generator(pos a , pos b); //generate random room
        void corridor_maker(room_info room_1 , room_info room_2 , int type); //generate corridor between rooms
        void add_pillar(room_info room); //generate pillars in chosen room


int main(){
    initscr();
    curs_set(FALSE);
    board();
    srand(time(0));
    keypad(stdscr , TRUE);

    
    // menu_1();
    // menu_2();
    new_game(u);

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
    noecho();
    board();
    int choice = 0;
    const char *sign_or_log[] = {"New user" , "Login" , "Guest"};

    refresh();
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
        u.guest = 1 ;
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
            mvprintw(LINES/2   , COLS/2 - 2 , "This username already exists!");
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
            mvprintw( LINES/2   , COLS/2 - 2 , "Not valid email!");
        }


    }
    clear_line(LINES/2   , COLS/2 - 5 , 40);
    while(1){
        clear_line(LINES/2 - 3 , COLS/2 +2 , 50);
        mvprintw(LINES/2 + 1  , COLS/2 - 4 , "Type \"R\" for random password:");
        move(LINES/2 - 3 , COLS/2 +2);
        getnstr(u->pass ,  50);
        if(strcmp(u->pass , "R") == 0){
            clear_line(LINES/2 + 2  , COLS/2 - 4  , 20);
            char temppass[20];
            strcpy(temppass , generatePassword());
            mvprintw(LINES/2 + 2  , COLS/2 - 4 , "%s" , temppass );
        }
        else if(check_pass( u->pass ) == 0){
            mvprintw( LINES/2   , COLS/2 - 2 , "Not valid password!");
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
    u.guest = 0;
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

void menu_2(){
    curs_set(FALSE);
    noecho();
    board();
    int choice = 0;
    const char *menu_items[] = {"New Game" , "Continue" , "Profile" , "Rankings" , "Settings"};


    while(1){
        for(int i = 0 ; i < 5 ; i++)
        {
            if(i == choice){
                attron(A_REVERSE);
            }
            mvprintw(LINES/2 - 10 + 2*i , COLS/2 - 5 , "%s" ,menu_items[i]);
            if(i == choice){
                attroff(A_REVERSE);
            }
        }
        int ch = getch();
        if (ch == KEY_UP )
            choice = (choice == 0) ? 4 : choice - 1;
        else if(ch == KEY_DOWN)
            choice = (choice == 4) ? 0 : choice + 1;
        else if( ch == 10)
            break;
    }
    switch (choice)
    {
    case 0:
        new_game(u);
        break;

    case 1:
        
        break;

    case 2:

        break;

    default:
    
    break;
    }
}

char* generatePassword() {
    static char password[11];
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int length = 7 + rand() % 4;
    int hasUpper = 0, hasLower = 0, hasDigit = 0;

    for (int i = 0; i < length; i++) {
        char randomChar = charset[rand() % (sizeof(charset) - 1)];
        password[i] = randomChar;

        if (randomChar >= 'A' && randomChar <= 'Z') hasUpper = 1;
        if (randomChar >= 'a' && randomChar <= 'z') hasLower = 1;
        if (randomChar >= '0' && randomChar <= '9') hasDigit = 1;
    }

    if (!hasUpper) password[rand() % length] = 'A' + (rand() % 26);
    if (!hasLower) password[rand() % length] = 'a' + (rand() % 26);
    if (!hasDigit) password[rand() % length] = '0' + (rand() % 10);

    password[length] = '\0';
    return password;
}

int randomint(int a , int b){
    return ((rand() % (b -a)) + a);
}

void new_game(user_info u){
    clear();
    noecho();
    curs_set(FALSE);
    for (int i = 0; i < 50; i++)
    {
        room[i].E = 0;
    }
    for (int i = 0; i < 4; i++)
    {
        for(int j =0 ; j < 2 ; j++){
            if((j == 1) && (i == 0)){
                int v = randomint(0 , 2);
                if(v){
                    break;
                }
            }
            if((j == 0) && (i == 3)){
                int v = randomint(0 , 2);
                if(v){
                    continue;
                }
            }
            pos a;
            pos b;
            a.x = 5 + 15*j;
            b.x = 19 + 15*j;
            if (i==0){
                a.y = COLS/2 - 60;
                b.y = COLS/2 - 33;
            }
            else if (i==1)
            {
                a.y = COLS/2 - 28;
                b.y = COLS/2 - 4;
            }
            else if(i==2){
                a.y = COLS/2 + 4;
                b.y = COLS/2 + 28;
            }
            else if(i==3){
                a.y = COLS/2 + 33;
                b.y = COLS/2 + 60;
            }
            room_generator(a , b);
            room[10*j + i] = s;
            room[10*j + i].E = 1;
            add_pillar(room[10*j + i]);
        }
    }

    corridor_maker(room[0] , room[1] , 1);
    corridor_maker(room[12] , room[13] , 1);
    int uu = randomint(1 , 6);
    if(uu != 1){
        corridor_maker(room[1] , room[2] , 1);
    }
    if(uu != 2){
        corridor_maker(room[11] , room[12] , 1);
    }
    if(uu != 3){
        corridor_maker(room[1] , room[11] , 2);
    }
    if(uu != 4){
        corridor_maker(room[2] , room[12] , 2);
    }
    if(room[10].E == 1){
        if(randomint(0 , 2)){
            corridor_maker(room[10] , room[11] , 1);
        }else{
            corridor_maker(room[0] , room[10] , 2);
        }
    }
    if(room[3].E == 1){
        if(randomint(0 , 2)){
            corridor_maker(room[2] , room[3] , 1);
        }else{
            corridor_maker(room[3] , room[13] , 2);
        }
    }
    
    int ii = randomint(0 , 4);
    int jj = randomint(0,2);
    if(room[10*jj + ii].E == 1){
        player.x = (room[10*jj + ii].up_left.x + room[10*jj + ii].bottom_right.x)/2;
        player.y = (room[10*jj + ii].up_left.y + room[10*jj + ii].bottom_right.y)/2;
    }
    char last_pos = '.';
    mvaddch(player.x , player.y , '@');
    refresh();
    while (1)
    {
        int ch = getch();
        char check;
        if(ch == '0'){break;}
        switch (ch)
        {
        case 'w':
            check = mvinch(player.x - 1, player.y) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.x , player.y , "%c" , last_pos);
                player.x--;
                last_pos = mvinch(player.x, player.y) & A_CHARTEXT;
                mvaddch(player.x , player.y , '@');
            }
            break;
        
        case 's':
            check = mvinch(player.x + 1, player.y) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.x , player.y , "%c" , last_pos);
                player.x++;
                last_pos = mvinch(player.x, player.y) & A_CHARTEXT;
                mvaddch(player.x , player.y , '@');
            }
            break;

        case 'a':
            check = mvinch(player.x , player.y - 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.x , player.y , "%c" , last_pos);
                player.y--;
                last_pos = mvinch(player.x, player.y) & A_CHARTEXT;
                mvaddch(player.x , player.y , '@');
            }
            break;

        case 'd':
            check = mvinch(player.x , player.y + 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.x , player.y , "%c" , last_pos);
                player.y++;
                last_pos = mvinch(player.x, player.y) & A_CHARTEXT;
                mvaddch(player.x , player.y , '@');
            }
            break;

        case 'q':
            check = mvinch(player.x - 1, player.y - 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.x , player.y , "%c" , last_pos);
                player.x--;
                player.y--;
                last_pos = mvinch(player.x, player.y) & A_CHARTEXT;
                mvaddch(player.x , player.y , '@');
            }
            break;
        case 'e': 
            check = mvinch(player.x - 1, player.y + 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.x , player.y , "%c" , last_pos);
                player.x--;
                player.y++;
                last_pos = mvinch(player.x, player.y) & A_CHARTEXT;
                mvaddch(player.x , player.y , '@');
            }
            break;
        case 'z':
            check = mvinch(player.x + 1, player.y - 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.x , player.y , "%c" , last_pos);
                player.x++;
                player.y--;
                last_pos = mvinch(player.x, player.y) & A_CHARTEXT;
                mvaddch(player.x , player.y , '@');
            }
            break;
        case 'c':
            check = mvinch(player.x + 1, player.y + 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.x , player.y , "%c" , last_pos);
                player.x++;
                player.y++;
                last_pos = mvinch(player.x, player.y) & A_CHARTEXT;
                mvaddch(player.x , player.y , '@');
            }
            break;
        
        
        default:
            break;
        }
    }
    
}

void room_generator(pos a , pos b ){
    int length , width;
    length = randomint(7 , 19);
    width = randomint(4 , 8);
    s.W = width;
    s.L = length;
    s.up_left.y = randomint(a.y + 1 , b.y - length - 1);
    s.up_left.x = randomint(a.x + 1 , b.x - width - 1);
    s.bottom_right.y = s.up_left.y + length + 1;
    s.bottom_right.x = s.up_left.x + width + 1;
    for(int i = 0; i < length + 2 ; i++){
        mvaddch( s.up_left.x , s.up_left.y + i , '-');
        mvaddch( s.bottom_right.x , s.bottom_right.y  - i , '-');
    }
    for (int i = 0; i < width; i++)
    {
        mvaddch( s.up_left.x + 1 + i , s.up_left.y , '|');
        mvaddch(s.bottom_right.x - 1 - i , s.bottom_right.y , '|');
    }
    for(int i = 0; i < width ; i++){
        for(int j = 0 ; j< length ; j++){
            mvaddch(s.up_left.x + 1 + i , s.up_left.y + 1 + j , '.');
        }
    }

}

void corridor_maker(room_info room_1 , room_info room_2 , int type){
    if(type == 1){
        pos s , e;
        s.y = room_1.bottom_right.y;
        e.y = room_2.up_left.y;
        s.x = randomint(room_1.up_left.x + 1 , room_1.bottom_right.x);
        e.x = randomint(room_2.up_left.x + 1 , room_2.bottom_right.x);
        while (s.x == e.x)
        {
            e.x = randomint(room_1.up_left.x + 1 , room_1.bottom_right.x);
        }
        int ver = s.x - e.x; 

        mvaddch(s.x , s.y , '+');
        mvaddch(e.x , e.y , '+');

        int break_px = randomint(s.y + 2 , e.y - 1) - s.y;
        pos wheretoput;
        wheretoput.x = s.x;
        wheretoput.y = s.y + 1;
        for (int i = 0; i < break_px; i++)
        {
            mvaddch(wheretoput.x , wheretoput.y , '#');
            wheretoput.y++;
        }
        if(ver < 0){
            for(int i = 0; i > ver ; i--){
                mvaddch(wheretoput.x , wheretoput.y , '#');
                wheretoput.x++;
            }
        }else{
            for(int i = 0; i < ver ; i++){
                mvaddch(wheretoput.x , wheretoput.y , '#');
                wheretoput.x--;
            }
        }
        while(wheretoput.y != e.y){
            mvaddch(wheretoput.x , wheretoput.y , '#');
            wheretoput.y++;
            if(wheretoput.y > 2000){
                break;
            }
        }
        
    }else if(type == 2){
        pos s , e;
        s.x = room_1.bottom_right.x;
        e.x = room_2.up_left.x;
        s.y = randomint(room_1.up_left.y + 1 , room_1.bottom_right.y);
        e.y = randomint(room_2.up_left.y + 1 , room_2.bottom_right.y);
        while (s.y == e.y)
        {
            e.y = randomint(room_1.up_left.y + 1 , room_1.bottom_right.y);
        }
        int ver = s.y - e.y; 

        mvaddch(s.x , s.y , '+');
        mvaddch(e.x , e.y , '+');

        int break_px = randomint(s.x + 2 , e.x - 1) - s.x;
        pos wheretoput;
        wheretoput.x = s.x + 1;
        wheretoput.y = s.y;
        for (int i = 0; i < break_px; i++)
        {
            mvaddch(wheretoput.x , wheretoput.y , '#');
            wheretoput.x++;
        }
        if(ver < 0){
            for(int i = 0; i > ver ; i--){
                mvaddch(wheretoput.x , wheretoput.y , '#');
                wheretoput.y++;
            }
        }else{
            for(int i = 0; i < ver ; i++){
                mvaddch(wheretoput.x , wheretoput.y , '#');
                wheretoput.y--;
            }
        }
        while(wheretoput.x != e.x){
            mvaddch(wheretoput.x , wheretoput.y , '#');
            wheretoput.x++;
            if(wheretoput.x > 2000){
                break;
            }
        }
    }else if(type == 3){

    }
}

void add_pillar(room_info room){
    int num = randomint(0,4);
    for (int i = 0; i < num; i++)
    {
        int xx = randomint(room.up_left.x + 2 , room.bottom_right.x -1);
        int yy = randomint(room.up_left.y + 2 , room.bottom_right.y -1);
        mvaddch(xx , yy , 'O');
    }
}


