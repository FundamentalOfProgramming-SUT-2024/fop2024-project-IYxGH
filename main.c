#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

typedef struct 
{
    char name[50];
    char email[100];
    char pass[50];
    int guest;
    int total_golds;
    int total_points;
    int total_games;
    char date_joined[60];
}user_info;

typedef struct{
    int x;
    int y;
}pos;

typedef struct{
    pos position;
    int points;
    int gold;
    int hit;
    int floor;
}player_info;

typedef struct{
    pos up_left;
    pos bottom_right;
    int L;
    int W;
    int E;
}room_info;


user_info u;
user_info users[1000];
room_info room[100];
room_info s;
player_info player;
int diff_level;
int hero_color;
int music;
char last_pos[3];
int num_of_users;


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
    int pass_authenticator(user_info u); //check if the password is correct
    char *generatePassword(); //generate random valid password
    int randomint(int a , int b); //generate random number between a and b
    void new_game(user_info u); //game page
    void room_generator(pos a , pos b); //generate random room
    void corridor_maker(room_info room_1 , room_info room_2 , int type); //generate corridor between rooms
    void add_pillar(room_info room); //generate pillars in chosen room
    void setting_page(); //page for setting after menu
    void diff_page(); //page for chosing difficulty level
    void char_color(); //page to chane the hero color
    void music_page(); //page to change the music
    void draw_map(); //draw the map randomly
    void put_player(); //put the player in random room
    void put_stairs(); //put the stairs randomly in one room
    void new_floor(); // go up to next level
    void handle_movement(); //get and check the movement
    void room_reset(); //reset the room elements
    void closeall(); // close every thing
    void print_info(); //print info during the game
    int check_room(room_info room); //check if the player is in the room or not 
    void add_gold(room_info room); //to add golds to the room
    void rankings_page(); // ranking page
    void get_users(); //store the users info in users
    void sort_users(); //to sort the users based on points , golds , games
    void setcolors(); //init the colors

int main(){
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    setcolors();
    curs_set(FALSE);
    cbreak();
    int required_lines = 35;
    int required_cols = 130;
    int rows , cols;
    getmaxyx(stdscr, rows, cols);
    if (rows < required_lines || cols < required_cols) { endwin();  
    fprintf(stderr, "Window is too small. Requires at least %d lines and %d columns.\n", required_lines, required_cols);
    return 1;
    }
    board();
    srand(time(0));
    keypad(stdscr , TRUE);

    
    menu_1();
    menu_2();
    // new_game(u);

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
            mvprintw(LINES/2 - 10 + 2*i , COLS/2 - 5 , "%s" ,sign_or_log[i]);
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

void room_reset(){
    for (int i = 0; i < 50; i++)
    {
        room[i].L = 0;
        room[i].E = 0;
        room[i].W = 0;
        room[i].bottom_right.x = 0;
        room[i].up_left.x = 0;
        room[i].bottom_right.y = 0;
        room[i].up_left.y = 0;
    }
}

void closeall(){
    endwin();
    exit(0);
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

void rankings_page(){
    clear();
    board();
    get_users();
    sort_users();
    int page = 1;
    page = 1;
            clear();
            board();
            attron(A_BOLD | COLOR_PAIR(3) | A_UNDERLINE);
            mvprintw(2 , 10 ,"Rank" );
            mvprintw(2 , 20 ,"Name" );
            mvprintw(2 , 35 ,"Points" );
            mvprintw(2 , 50 ,"Golds" );
            mvprintw(2 , 65 ,"Games" );
            mvprintw(2 , 80 ,"Date joined" );
            attroff(A_BOLD | COLOR_PAIR(3) | A_UNDERLINE);
            for (int i = 0; i < 10; i++)
            {
                if (i < num_of_users)
                {
                    if (i == 0)
                    {
                        mvprintw(5 , 100 , "GOAT🐐");
                        attron(A_BOLD | A_BLINK | COLOR_PAIR(233) );
                    }
                    if (i == 1)
                    {
                        mvprintw(7 , 100 , "Legend🥈");
                        attron(A_BOLD | COLOR_PAIR(254) );
                    }
                    if (i == 2)
                    {
                        mvprintw(9 , 100 , "Master🥉");
                        attron(A_BOLD | COLOR_PAIR(185) );
                    }
                    mvprintw(5 + 2*i , 10 , "%d.\t\t\t\t\t\t\t\t\t\t" , i + 1);
                    mvprintw(5 + 2*i , 20 , "%s" , users[i].name);
                    mvprintw(5 + 2*i , 35 , "%d" , users[i].total_points);
                    mvprintw(5 + 2*i , 50 , "%d" , users[i].total_golds);
                    mvprintw(5 + 2*i , 65 , "%d" , users[i].total_games);
                    mvprintw(5 + 2*i , 80 , "%s" , users[i].date_joined);
                    if(strcmp(users[i].name , u.name) == 0){
                    attron(A_BOLD);
                        mvprintw(5 + 2*i , 3 , "you ▶");
                        attroff(A_BOLD);
                        //attron(A_ITALIC);
                    }
                    

                    if (i == 0 || i == 1 || i == 2)
                    {
                        attroff(A_BOLD | A_BLINK | COLOR_PAIR(143) );
                    }

                    mvprintw(LINES - 4 , COLS/2 - 6 , "Page:");
                    for(int i = 0 ; i < 3 ; i++){
                        if(page == 1 + i){
                            attron(A_REVERSE);
                        }
                        mvprintw(LINES - 4 , COLS/2 + 2*i , "%d" , i + 1 );
                        if(page == 1 + i){
                            attroff(A_REVERSE);
                        }
                    }
                }
            }
        

    while (1)
    {
        int ch = getch();
        if(ch == '1'){
            page = 1;
            clear();
            board();
            attron(A_BOLD | COLOR_PAIR(3) | A_UNDERLINE);
            mvprintw(2 , 10 ,"Rank" );
            mvprintw(2 , 20 ,"Name" );
            mvprintw(2 , 35 ,"Points" );
            mvprintw(2 , 50 ,"Golds" );
            mvprintw(2 , 65 ,"Games" );
            mvprintw(2 , 80 ,"Date joined" );
            attroff(A_BOLD | COLOR_PAIR(3) | A_UNDERLINE);
            for (int i = 0; i < 10; i++)
            {
                if (i < num_of_users)
                {
                    if (i == 0)
                    {
                        mvprintw(5 , 100 , "GOAT🐐");
                        attron(A_BOLD | A_BLINK | COLOR_PAIR(233) );
                    }
                    if (i == 1)
                    {
                        mvprintw(7 , 100 , "Legend🥈");
                        attron(A_BOLD | COLOR_PAIR(254) );
                    }
                    if (i == 2)
                    {
                        mvprintw(9 , 100 , "Master🥉");
                        attron(A_BOLD | COLOR_PAIR(185) );
                    }
                    if(strcmp(users[i].name , u.name) == 0){
                    attron(A_BOLD);
                        mvprintw(5 + 2*i , 3 , "you ▶");
                        attroff(A_BOLD);
                        //attron(A_ITALIC);
                    }
                    mvprintw(5 + 2*i , 10 , "%d.\t\t\t\t\t\t\t\t\t\t" , i + 1);
                    mvprintw(5 + 2*i , 20 , "%s" , users[i].name);
                    mvprintw(5 + 2*i , 35 , "%d" , users[i].total_points);
                    mvprintw(5 + 2*i , 50 , "%d" , users[i].total_golds);
                    mvprintw(5 + 2*i , 65 , "%d" , users[i].total_games);
                    mvprintw(5 + 2*i , 80 , "%s" , users[i].date_joined);
                    if (i == 0 || i == 1 || i == 2)
                    {
                        attroff(A_BOLD | A_BLINK | COLOR_PAIR(143) );
                    }

                    mvprintw(LINES - 4 , COLS/2 - 6 , "Page:");
                    for(int i = 0 ; i < 3 ; i++){
                        if(page == 1 + i){
                            attron(A_REVERSE);
                        }
                        mvprintw(LINES - 4 , COLS/2 + 2*i , "%d" , i + 1 );
                        if(page == 1 + i){
                            attroff(A_REVERSE);
                        }
                    }
                }
            }
        }else if (ch == '2' || ch == '3') {
            page = ch - '0';
            clear();
            board();
            attron(A_BOLD | COLOR_PAIR(3) | A_UNDERLINE);
            mvprintw(2 , 10 ,"Rank" );
            mvprintw(2 , 20 ,"Name" );
            mvprintw(2 , 35 ,"Points" );
            mvprintw(2 , 50 ,"Golds" );
            mvprintw(2 , 65 ,"Games" );
            mvprintw(2 , 80 ,"Date joined" );
            attroff(A_BOLD | COLOR_PAIR(3) | A_UNDERLINE);
            for (int i = 0; i < 10; i++)
            {
                if (i + page*10 - 10 < num_of_users)
                {
                    if(strcmp(users[i + page*10 - 10].name , u.name) == 0){
                    attron(A_BOLD);
                        mvprintw(5 + 2*i , 3 , "you ▶");
                        attroff(A_BOLD);
                        //attron(A_ITALIC);
                    }
                    mvprintw(5 + 2*i , 10 , "%d.\t\t\t\t\t\t\t\t\t\t" , i + page*10 - 10 + 1);
                    mvprintw(5 + 2*i , 20 , "%s" , users[i + page*10 - 10].name);
                    mvprintw(5 + 2*i , 35 , "%d" , users[i + page*10 - 10].total_points);
                    mvprintw(5 + 2*i , 50 , "%d" , users[i + page*10 - 10].total_golds);
                    mvprintw(5 + 2*i , 65 , "%d" , users[i + page*10 - 10].total_games);
                    mvprintw(5 + 2*i , 80 , "%s" , users[i + page*10 - 10].date_joined);
                }
            }
            mvprintw(LINES - 4 , COLS/2 - 6 , "Page:");
            for(int i = 0 ; i < 3 ; i++){
                if(page == 1 + i){
                    attron(A_REVERSE);
                }
                mvprintw(LINES - 4 , COLS/2 + 2*i , "%d" , i + 1 );
                if(page == 1 + i){
                    attroff(A_REVERSE);
                }
            }
        }else if (ch == '0')
        {
            break;
        }
    }
    menu_2();
}

void get_users(){
    FILE *file = fopen("usernames.txt" , "r");
    char line[256];
    int count = 0; 
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line); 
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        char temp[100];
        strcpy(temp , line);
        strcat(temp , ".txt");
        char linee[256];
        FILE *filee = fopen(temp , "r");
        fgets(linee, sizeof(linee), filee);
        size_t lene = strlen(linee); 
        if (lene > 0 && linee[lene - 1] == '\n') {
            linee[lene - 1] = '\0';
        }
        strcpy(users[count].name , linee);
        fgets(linee, sizeof(linee), filee);
        lene = strlen(linee); 
        if (lene > 0 && linee[lene - 1] == '\n') {
            linee[lene - 1] = '\0';
        }
        strcpy(users[count].email , linee);
        
        fgets(linee, sizeof(linee), filee);
        lene = strlen(linee); 
        if (lene > 0 && linee[lene - 1] == '\n') {
            linee[lene - 1] = '\0';
        }
        strcpy(users[count].pass , linee);

        int num;
        fscanf(filee , "%d" , &num);
        users[count].total_golds = num;
        fscanf(filee , "%d" , &num);
        users[count].total_points = num;
        fscanf(filee , "%d" , &num);
        users[count].total_games = num;
        fgetc(filee);
        fgets(linee, sizeof(linee), filee);
        lene = strlen(linee); 
        if (lene > 0 && linee[lene - 1] == '\n') {
            linee[lene - 1] = '\0';
        }
        strcpy(users[count].date_joined , linee);
        count++;
        fclose(filee);  
    }
    num_of_users = count;
    fclose(file);
}

void sort_users(){
    for(int i = 0; i < num_of_users ; i++){
        for(int j = i ; j < num_of_users ; j++ ){
            if(users[i].total_points < users[j].total_points){
                user_info temp;
                temp = users[i];
                users[i] = users[j];
                users[j] = temp;
            }else if (users[i].total_points == users[j].total_points && users[i].total_golds < users[j].total_golds)
            {
                user_info temp;
                temp = users[i];
                users[i] = users[j];
                users[j] = temp;
            }
            else if (users[i].total_golds == users[j].total_golds && users[i].total_games < users[j].total_games)
            {
                user_info temp;
                temp = users[i];
                users[i] = users[j];
                users[j] = temp;
            }    
        }
    }
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
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", timeinfo);
    strcpy(u.date_joined , buffer);
    fprintf(file , "%s\n%s\n%s\n0\n0\n0\n%s" , u.name , u.email , u.pass , buffer);
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
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
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
    const char *menu_items[] = {"New Game" , "Load game" , "Profile" , "Rankings" , "Settings"};


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

    case 3:
        rankings_page();
        break;

    case 4:
        setting_page();
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
    room_reset();
    noecho();
    curs_set(FALSE);
    for (int i = 0; i < 50; i++)
    {
        room[i].E = 0;
    }
    for (int i = 0; i < LINES; i++)
    {
        mvaddch(i , COLS-18 , '|');
    }
    player.floor = 1;
    draw_map();
    put_player();
    put_stairs();
    handle_movement();    
}

void room_generator(pos a , pos b ){
    int length , width;
    length = randomint(6 , 15);
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
            e.x = randomint(room_1.up_left.x + 1 , room_2.bottom_right.x);
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
    int num = randomint(0,5);
    for (int i = 0; i < num; i++)
    {
        int xx = randomint(room.up_left.x + 2 , room.bottom_right.x -1);
        int yy = randomint(room.up_left.y + 2 , room.bottom_right.y -1);
        mvaddch(xx , yy , 'O');
    }
}

void setting_page(){
    noecho();
    board();
    int choice = 0;
    const char *sign_or_log[] = {"Difficulty" , "Character Color" ,  "Music"};

    refresh();
    while(1){
        for(int i = 0 ; i < 3 ; i++)
        {
            if(i == choice){
                attron(A_REVERSE);
            }
            mvprintw(LINES/2 - 10 + 2*i , COLS/2 - 5 , "%s" ,sign_or_log[i]);
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
        diff_page();
        break;

    case 1:
        char_color();
        break;

    case 2:
        music_page();
        break;
    }
}

void diff_page(){
    noecho();
    board();
    int choice = 0;
    const char *sign_or_log[] = {"Easy" , "Medium" ,  "Hard"};
    refresh();
    while(1){
        for(int i = 0 ; i < 3 ; i++)
        {
            if(i == choice){
                attron(A_REVERSE);
            }
            mvprintw(LINES/2 - 10 + 2*i , COLS/2 - 5 , "%s" ,sign_or_log[i]);
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
        diff_level = 1;
        menu_2();
        break;

    case 1:
        diff_level = 2;
        menu_2();
        break;

    case 2:
        diff_level = 3;
        menu_2();
        break;
    }
}

void char_color(){
    noecho();
    board();
    int choice = 0;
    const char *sign_or_log[] = {"Blue" , "White" ,  "Red"};

    refresh();
    while(1){
        for(int i = 0 ; i < 3 ; i++)
        {
            if(i == choice){
                attron(A_REVERSE);
            }
            mvprintw(LINES/2 - 10 + 2*i , COLS/2 - 5 , "%s" ,sign_or_log[i]);
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
        hero_color = 1;
        menu_2();
        break;

    case 1:
        hero_color = 2;
        menu_2();
        break;

    case 2:
        hero_color = 3;
        menu_2();
        break;
    }
}

void music_page(){
    noecho();
    board();
    int choice = 0;
    const char *sign_or_log[] = {"Music1" , "Music2" ,  "Music3" , "OFF"};

    refresh();
    while(1){
        for(int i = 0 ; i < 4 ; i++)
        {
            if(i == choice){
                attron(A_REVERSE);
            }
            mvprintw(LINES/2 - 10 + 2*i , COLS/2 - 5 , "%s" ,sign_or_log[i]);
            if(i == choice){
                attroff(A_REVERSE);
            }
        }
        int ch = getch();
        if (ch == KEY_UP )
            choice = (choice == 0) ? 3 : choice - 1;
        else if(ch == KEY_DOWN)
            choice = (choice == 3) ? 0 : choice + 1;
        else if( ch == 10)
            break;
    }
    switch (choice)
    {
    case 0:
        music = 1;
        menu_2();
        break;

    case 1:
        music = 2;
        menu_2();
        break;

    case 2:
        music = 3;
        menu_2();
        break;

    case 3:
        music = 0;
        menu_2();
        break;
    }
}

void draw_map(){
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
            a.x = 1 + 19*j;
            b.x = 16 + 19*j;
            if (i==0){
                a.y = COLS/2 - 60;
                b.y = COLS/2 - 36;
            }
            else if (i==1)
            {
                a.y = COLS/2 - 32;
                b.y = COLS/2 - 7;
            }
            else if(i==2){
                a.y = COLS/2;
                b.y = COLS/2 + 22;
            }
            else if(i==3){
                a.y = COLS/2 + 28;
                b.y = COLS/2 + 50;
            }
            room_generator(a , b);
            room[10*j + i] = s;
            room[10*j + i].E = 1;
            add_pillar(room[10*j + i]);
            add_gold(room[10*j + i]);
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
    
}

void put_player(){
    int ii = randomint(0,4);
    int jj = randomint(0,2);
    if(room[10*jj + ii].E == 1){
        player.position.x = (room[10*jj + ii].up_left.x + room[10*jj + ii].bottom_right.x)/2;
        player.position.y = (room[10*jj + ii].up_left.y + room[10*jj + ii].bottom_right.y)/2;
    }
    strcpy(last_pos , ".");
    mvaddch(player.position.x , player.position.y , '@');
    refresh();
}

void put_stairs(){
    int done = 0;
    while(done == 0){
        int ii = randomint(0,4);
        int jj = randomint(0,2);
        
        if(room[10*jj + ii].E == 1 && check_room(room[10*jj + ii]) == 0 ){
            int xx = randomint(room[10*jj + ii].up_left.x + 1 , room[10*jj + ii].bottom_right.x);
            int yy = randomint(room[10*jj + ii].up_left.y + 1 , room[10*jj + ii].bottom_right.y);
            mvaddch(xx , yy , '%');
            done = 1;
        }
    }
    refresh();
}

void new_floor(){
    clear();
    room_reset();
    noecho();
    curs_set(FALSE);
    char check = mvinch(player.position.x , player.position.y) & A_CHARTEXT;
    while(check != '.'){
        clear();
        room_reset();
        draw_map();
        check = mvinch(player.position.x , player.position.y) & A_CHARTEXT;
    }
    for (int i = 0; i < LINES; i++)
    {
        mvaddch(i , COLS-18 , '|');
    }
    mvaddch(player.position.x , player.position.y , '@');
    put_stairs();
    refresh();
    handle_movement();
}

void handle_movement(){
    while (1)
    {
        print_info();
        int ch = getch();
        char check;
        if(ch == 27){closeall(0);}
        switch (ch)
        {
        case 'w':
            check = mvinch(player.position.x - 1, player.position.y) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.position.x , player.position.y , "%s" , last_pos);
                player.position.x--;
                move(player.position.x, player.position.y);
                innstr(last_pos , 1);
                mvaddch(player.position.x , player.position.y , '@');
            }
            break;
        
        case 's':
            check = mvinch(player.position.x + 1, player.position.y) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.position.x , player.position.y , "%s" , last_pos);
                player.position.x++;
                move(player.position.x, player.position.y);
                innstr(last_pos , 1);
                mvaddch(player.position.x , player.position.y , '@');
            }
            break;

        case 'a':
            check = mvinch(player.position.x , player.position.y - 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.position.x , player.position.y , "%s" , last_pos);
                player.position.y--;
                move(player.position.x, player.position.y);
                innstr(last_pos , 1);
                mvaddch(player.position.x , player.position.y , '@');
            }
            break;

        case 'd':
            check = mvinch(player.position.x , player.position.y + 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.position.x , player.position.y , "%s" , last_pos);
                player.position.y++;
                move(player.position.x, player.position.y);
                innstr(last_pos , 1);
                mvaddch(player.position.x , player.position.y , '@');
            }
            break;

        case 'q':
            check = mvinch(player.position.x - 1, player.position.y - 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.position.x , player.position.y , "%s" , last_pos);
                player.position.x--;
                player.position.y--;
                move(player.position.x, player.position.y);
                innstr(last_pos , 1);
                mvaddch(player.position.x , player.position.y , '@');
            }
            break;
        case 'e': 
            check = mvinch(player.position.x - 1, player.position.y + 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.position.x , player.position.y , "%s" , last_pos);
                player.position.x--;
                player.position.y++;
                move(player.position.x, player.position.y);
                innstr(last_pos , 1);
                mvaddch(player.position.x , player.position.y , '@');
            }
            break;
        case 'z':
            check = mvinch(player.position.x + 1, player.position.y - 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.position.x , player.position.y , "%s" , last_pos);
                player.position.x++;
                player.position.y--;
                move(player.position.x, player.position.y);
                innstr(last_pos , 1);
                mvaddch(player.position.x , player.position.y , '@');
            }
            break;
        case 'c':
            check = mvinch(player.position.x + 1, player.position.y + 1) & A_CHARTEXT;
            if(check == '-' || check == '|' || check == ' ' || check == 'O'){
                break;
            }else{
                mvprintw(player.position.x , player.position.y , "%s" , last_pos);
                player.position.x++;
                player.position.y++;
                move(player.position.x, player.position.y);
                innstr(last_pos , 1);
                mvaddch(player.position.x , player.position.y , '@');
            }
            break;
        // case 'x':
        //     if(strcmp(last_pos , "%c" , % ) == 0){
        //         player.floor++;
        //         last_pos = '.';
        //         new_floor();
        //     }
        //     break;
        
        default:
            break;
        }
    }
}

int check_room(room_info room){
    if(player.position.x > room.up_left.x && player.position.x < room.bottom_right.x){
        if(player.position.y > room.up_left.y && player.position.y < room.bottom_right.y){
            return 1;
        }
    }
    return 0;
}

void print_info(){
    mvprintw(1 , COLS - 14 , "---%s---" , u.name);
    for(int i = 0 ; i < 17 ; i ++){
        mvprintw(2 , COLS - i , "■");  //⇔▬▲▼■
        mvprintw(9 , COLS - i , "■");
    }
    mvprintw(11 , COLS - 16 , "Gold:   %d" , player.gold);
    mvprintw(12 , COLS - 16 , "Hits:   %d" , player.hit);
    mvprintw(13 , COLS - 16 , "Floor:  %d" , player.floor);
}

void add_gold(room_info room){
    int count = randomint(0 , 18);
    if(count > 10){
        int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
        int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
        mvprintw(xx , yy , "©");
    }
    if(count > 16){
        int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
        int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
        mvprintw(xx , yy , "©");
    }
}

void setcolors(){
    init_pair(1, COLOR_RED, COLOR_BLACK); 
    init_pair(2, COLOR_GREEN, COLOR_BLACK); 
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_WHITE , COLOR_GREEN );
    init_pair(5, COLOR_WHITE , COLOR_CYAN);
    init_pair(6, COLOR_WHITE , COLOR_BLACK + 8);
    for (int i = 0; i < COLORS; i++) {
        init_pair(i + 7, COLOR_RED, i);
    }
    for (int i = 0; i < COLORS; i++) {
        init_pair(i + 307, COLOR_WHITE, i);
    }    
}



