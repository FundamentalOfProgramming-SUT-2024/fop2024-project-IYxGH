#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


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
    int Mhit;
    int floor; 
    int fullness;
    int Mfullness;
    int weapon[5]; // 0 for mace, 1 for dagger, 2 for magic wand, 3 for normal arrow, 4 for sword
    int now_weapon;
    int last_shot;
    int spell[3];  // 0 for health, 1 for speed , 2 for damage
}player_info;

typedef struct{
    pos up_left;
    pos bottom_right;
    int L;
    int W;
    int E;
    int type; // 1 for normal , 2 for trasure , 3 for enchant , 4 for nightmare
}room_info;

typedef struct
{
    int E[5]; // 0 for mace, 1 for dagger, 2 for magic wand, 3 for normal arrow, 4 for sword
    pos P[5];
}weapon_info;

typedef struct 
{
    int E[3]; // 0 for health, 1 for speed , 2 for damage 
    pos P[3]; 
}spell_info;

typedef struct 
{
    int num_pillar;
    pos pillar[100];
    int num_gold;
    pos gold[50];
    weapon_info weopons;
    spell_info spells;
    int num_gem;
    pos gem[50];
    int num_trap;
    pos trap[50];
    int num_food;
    pos food[50];
    int num_mkey;
    pos mkey;

}special_items;

typedef struct{
    int vision;
    int what;
    int amount;
}wpos;

typedef struct{
    int exist;
    char type;
    int health;
    int movement_left;
    int ammo_left;
    pos position;

}enemy_info;


wpos w[35][200];
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
int num_of_enemies;
enemy_info enemy[100];


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
    void build_map(); //draw the map randomly
    void put_player(); //put the player in random room
    void put_stairs(); //put the stairs randomly in one room
    void new_floor(); // go up to next level
    void handle_movement(); //get and check the movement
    void room_reset(); //reset the room elements
    void closeall(); // close every thing
    void print_info(); //print info during the game
    int check_room(room_info room); //check if the player is in the room or not 
    void add_gold(room_info room); //to add golds to the room
    void add_trap(room_info room); //to add traps to the room
    void rankings_page(); // ranking page
    void get_users(); //store the users info in users
    void sort_users(); //to sort the users based on points , golds , games
    void setcolors(); //init the colors
    void w_reset(); //empty the wpos
    void w_draw(); // draw whole map based on wposes
    int obstacle_check(int x , int y); //check if move is possible or not
    void pick_item(int x , int y); // pick items from the floor
    void put_spellandweapon();
    void weapon_list(); // after pressing i , this page will be open to show weapons
    void spell_list(); //after j , this page will show list of spells
    void w_redraw(int x , int y); //to avoid crashes
    void treasure_room(room_info room); // make a room treasure room
    void victory_page(); // goes to victory page
    void check_trap(); // check if there is a trap or not
    int check_live(int x , int y , int who); // check if there is enemy or player in that pos ,,, 0 for both ,,, 1 for enemy ,,, 2 for player
    int check_room_pos(int x , int y); // give the room number of pos ,,, give -1 if its in no room


    //functions for enemies
    void put_enemy();
    void reset_enemy();
    void print_enemy();
    void move_enemy();
    int num_D;
    int num_F;
    int num_G;
    int num_S;
    int num_U;

int main(){
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    setcolors();
    curs_set(FALSE);
    cbreak();
    int required_lines = 35;
    int required_cols = 130;
    diff_level = 1;
    int rows , cols;
    getmaxyx(stdscr, rows, cols);
    if (rows < required_lines || cols < required_cols) { endwin();  
    fprintf(stderr, "Window is too small. Requires at least %d lines and %d columns.\n", required_lines, required_cols);
    return 1;
    }
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
                        attron(A_BOLD | A_BLINK | COLOR_PAIR(7) );
                    }
                    if (i == 1)
                    {
                        mvprintw(7 , 100 , "Legend🥈");
                        attron(A_BOLD | COLOR_PAIR(8));
                    }
                    if (i == 2)
                    {
                        mvprintw(9 , 100 , "Master🥉");
                        attron(A_BOLD | COLOR_PAIR(9));
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
                        attroff(A_BOLD | A_BLINK | COLOR_PAIR(9) );
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
                        attron(A_BOLD | A_BLINK | COLOR_PAIR(7) );
                    }
                    if (i == 1)
                    {
                        mvprintw(7 , 100 , "Legend🥈");
                        attron(A_BOLD | COLOR_PAIR(8) );
                    }
                    if (i == 2)
                    {
                        mvprintw(9 , 100 , "Master🥉");
                        attron(A_BOLD | COLOR_PAIR(9) );
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
                        attroff(A_BOLD | A_BLINK | COLOR_PAIR(9) );
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
    w_reset();
    reset_enemy();
    noecho();
    curs_set(FALSE);

    player.floor = 5;
    player.Mfullness = 8 - diff_level; 
    player.fullness = 8 - diff_level; 
    player.Mhit = (8 - diff_level) * 20;
    player.hit = (8 - diff_level) * 20;
    player.weapon[0] = 1;
    player.now_weapon = 0;
    build_map();
    put_player();
    put_enemy();
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
        w[s.up_left.x][s.up_left.y + i].what = 3;
        w[s.bottom_right.x][s.bottom_right.y - i].what = 3;
    }
    for (int i = 0; i < width; i++)
    {
        // mvaddch( s.up_left.x + 1 + i , s.up_left.y , '|');
        // mvaddch(s.bottom_right.x - 1 - i , s.bottom_right.y , '|');
        w[s.up_left.x + 1 + i][s.up_left.y].what = 2;
        w[s.bottom_right.x - 1 - i][s.bottom_right.y].what = 2;
    }
    for(int i = 0; i < width ; i++){
        for(int j = 0 ; j< length ; j++){
            // mvaddch(s.up_left.x + 1 + i , s.up_left.y + 1 + j , '.');
            w[s.up_left.x + 1 + i][s.up_left.y + 1 + j].what = 1;
        }
    }

}

void victory_page(){
    clear();
    board();
    mvprintw(LINES/2 - 5 , COLS/2 - 7 , "You Won!");
    mvprintw(LINES/2 - 5 , COLS/2 - 7 , "Golds: %d" , player.gold);
    getch();
    menu_2();
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

        // mvaddch(s.x , s.y , '+');
        // mvaddch(e.x , e.y , '+');
        w[s.x][s.y].what = 5;
        w[e.x][e.y].what = 5;

        int break_px = randomint(s.y + 2 , e.y - 1) - s.y;
        pos wheretoput;
        wheretoput.x = s.x;
        wheretoput.y = s.y + 1;
        for (int i = 0; i < break_px; i++)
        {
            // mvaddch(wheretoput.x , wheretoput.y , '#');
            w[wheretoput.x][wheretoput.y].what = 6;
            wheretoput.y++;
        }
        if(ver < 0){
            for(int i = 0; i > ver ; i--){
                // mvaddch(wheretoput.x , wheretoput.y , '#');
                w[wheretoput.x][wheretoput.y].what = 6;

                wheretoput.x++;
            }
        }else{
            for(int i = 0; i < ver ; i++){
                // mvaddch(wheretoput.x , wheretoput.y , '#');
                w[wheretoput.x][wheretoput.y].what = 6;
                wheretoput.x--;
            }
        }
        while(wheretoput.y != e.y){
            // mvaddch(wheretoput.x , wheretoput.y , '#');
            w[wheretoput.x][wheretoput.y].what = 6;
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

        // mvaddch(s.x , s.y , '+');
        // mvaddch(e.x , e.y , '+');
        w[s.x][s.y].what = 5;
        w[e.x][e.y].what = 5;
        
        int break_px = randomint(s.x + 2 , e.x - 1) - s.x;
        pos wheretoput;
        wheretoput.x = s.x + 1;
        wheretoput.y = s.y;
        for (int i = 0; i < break_px; i++)
        {
            // mvaddch(wheretoput.x , wheretoput.y , '#');
            w[wheretoput.x][wheretoput.y].what = 6;
            wheretoput.x++;
        }
        if(ver < 0){
            for(int i = 0; i > ver ; i--){
                // mvaddch(wheretoput.x , wheretoput.y , '#');
                w[wheretoput.x][wheretoput.y].what = 6;
                wheretoput.y++;
            }
        }else{
            for(int i = 0; i < ver ; i++){
                // mvaddch(wheretoput.x , wheretoput.y , '#');
                w[wheretoput.x][wheretoput.y].what = 6;
                wheretoput.y--;
            }
        }
        while(wheretoput.x != e.x){
            // mvaddch(wheretoput.x , wheretoput.y , '#');
            w[wheretoput.x][wheretoput.y].what = 6;
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
        // mvaddch(xx , yy , 'O');
        w[xx][yy].what = 4;
    }
}

int obstacle_check(int x , int y){
    if(w[x][y].what == 2 || w[x][y].what == 3 || w[x][y].what == 4 || w[x][y].what == 0
    || w[x][y].what == 1002 || w[x][y].what == 1003 || w[x][y].what == 1004 || check_live(x , y , 0)){
        return 1;
    }else{
        return 0;
    }
}

void build_map(){
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
            b.x = 15 + 19*j;
            if (i==0){
                a.y = COLS - 114;
                b.y = COLS - 91;
            }
            else if (i==1)
            {
                a.y = COLS - 84;
                b.y = COLS - 62;
            }
            else if(i==2){
                a.y = COLS - 55;
                b.y = COLS - 32;
            }
            else if(i==3){
                a.y = COLS - 25;
                b.y = COLS - 2;
            }
            room_generator(a , b);
            room[10*j + i] = s;
            room[10*j + i].E = 1;
            add_pillar(room[10*j + i]);
            add_gold(room[10*j + i]);
            add_trap(room[10*j + i]);
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
    put_spellandweapon();

}

void put_player(){
    int done = 0;
    while(done == 0){
        int ii = randomint(0,4);
        int jj = randomint(0,2);
        if(room[10*jj + ii].E == 1){
            while(done == 0){
                int xx = randomint(room[10*jj + ii].up_left.x + 1 ,room[10*jj + ii].bottom_right.x );
                int yy = randomint(room[10*jj + ii].up_left.y +1 , room[10*jj + ii].bottom_right.y );
                if(w[xx][yy].what == 1){
                    player.position.x = xx;
                    player.position.y = yy;
                    done = 1;
                }
            }
        }
    }
}

void put_stairs(){
    int done = 0;
    int what = 1;
    if(player.floor == 4){
        if(randomint(0 , 2)){
            what = 0;
        }
    }else if (player.floor == 5)
    {
        what = 0;
    }
    
    while(done == 0){
        int ii = randomint(0,4);
        int jj = randomint(0,2);
        
        if(room[10*jj + ii].E == 1 && check_room(room[10*jj + ii]) == 0){
            
            while (done == 0)
            {
                int xx = randomint(room[10*jj + ii].up_left.x + 1 , room[10*jj + ii].bottom_right.x);
                int yy = randomint(room[10*jj + ii].up_left.y + 1 , room[10*jj + ii].bottom_right.y);
                if (w[xx][yy].what == 1 && check_live(xx, yy , 1) == 0)
                {
                    w[xx][yy].what = 7;
                    done = 1;
                }
            }   
            if (what == 0)
            {
                treasure_room(room[10*jj + ii]);
            }
        
        }
    }
    refresh();
}

void new_floor(){
    clear();
    room_reset();
    w_reset();
    reset_enemy();
    noecho();
    curs_set(FALSE);
    while(w[player.position.x][player.position.y].what != 1){
        w_reset();
        room_reset();
        build_map();
    }
    put_enemy();
    put_stairs();
    refresh();
    handle_movement();
}

void handle_movement(){
    while (1)
    {   
        clear();
        print_info();
        w_draw();
        mvaddch(player.position.x , player.position.y , '@');
        print_enemy();
        refresh();
        int ch = getch();
        if(ch == 27){closeall(0);}
        switch (ch)
        {
        case 'w':
            if(obstacle_check(player.position.x - 1 , player.position.y)){
                break;
            }else{
                player.position.x--;
            }
            break;
        
        case 's':
            if(obstacle_check(player.position.x + 1 , player.position.y)){
                break;
            }else{
                player.position.x++;
            }
            break;

        case 'a':
            if(obstacle_check(player.position.x  , player.position.y - 1)){
                break;
            }else{
                player.position.y--;
            }
            break;

        case 'd':
            if(obstacle_check(player.position.x  , player.position.y + 1)){
                break;
            }else{
                player.position.y++;
            }
            break;

        case 'q':
            if(obstacle_check(player.position.x - 1 , player.position.y - 1)){
                break;
            }else{
                player.position.x--;
                player.position.y--;
            }
            break;

        case 'z': 
            if(obstacle_check(player.position.x + 1 , player.position.y - 1)){
                break;
            }else{
                player.position.x++;
                player.position.y--;
            }
            break;

        case 'e':
            if(obstacle_check(player.position.x - 1 , player.position.y + 1)){
                break;
            }else{
                player.position.x--;
                player.position.y++;
            }
            break;

        case 'c':
            if(obstacle_check(player.position.x + 1 , player.position.y + 1)){
                break;
            }else{
                player.position.x++;
                player.position.y++;
            }
            break;

        case 'x':
            if(w[player.position.x][player.position.y].what == 7){
                player.floor++;
                new_floor();
            }
            else if(w[player.position.x][player.position.y].what == 1007){
                victory_page();
            }
            break;

        case 'p':
            pick_item( player.position.x , player.position.y);
            break;
        
        case 'i':
            weapon_list();
            break;

        case 'j':
            spell_list();
            break;

        default:
            break;
        }
        
        if (ch != 'i' && ch!= 'j' && ch != 'o')
        {
            move_enemy();
            check_trap();
            
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

int check_room_pos(int x , int y){
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            int k = 10*j + i;
            if (room[k].E == 1)
            {
                if (room[k].up_left.x < x && room[k].bottom_right.x > x)
                {
                if (room[k].up_left.y < y && room[k].bottom_right.y > y)
                {
                    return k;
                }
                
                }
                
            }
            
        }
        
    }
    return -1;
}

void pick_item(int x , int y){
    if (w[x][y].what == 301)
    {
        w[x][y].what = 1;
        int value = randomint(3 , 10);
        value /= diff_level;
        if(value == 0){value++;}
        player.gold += value;
    }
    else if (w[x][y].what == 302)
    {
        w[x][y].what = 1;
        int value = 30 - 5*diff_level;
        player.gold += value;
    }
    else if (w[x][y].what == 20)
    {
        
    }
    else if (w[x][y].what == 101)
    {
        w[x][y].what = 1;
        player.weapon[0] += 1; 
    }
    else if (w[x][y].what == 102)
    {
        w[x][y].what = 1;
        player.weapon[1] += 1; 
    }
    else if (w[x][y].what == 103)
    {
        w[x][y].what = 1;
        player.weapon[2] += 1; 
    }
    else if (w[x][y].what == 104)
    {
        w[x][y].what = 1;
        player.weapon[3] += 1; 
    }
    else if (w[x][y].what == 105)
    {
        w[x][y].what = 1;
        player.weapon[4] += 1; 
    }
    else if (w[x][y].what == 201)
    {
        w[x][y].what = 1;
        player.spell[0] += 1; 
    }
    else if (w[x][y].what == 202)
    {
        w[x][y].what = 1;
        player.spell[1] += 1; 
    }
    else if (w[x][y].what == 203)
    {
        w[x][y].what = 1;
        player.spell[2] += 1; 
    }
    
      
}

void print_info(){
    mvprintw(1 , 3 , "---%s---" , u.name);
    for (int i = 0; i < LINES; i++)
    {
        mvaddch(i , 20 , '|');
    }
    for(int i = 0 ; i < 20 ; i ++){
        mvprintw(2 ,  i , "■");  //⇔▬▲▼■
        mvprintw(9 ,  i , "■");
    }
    switch (diff_level)
    {
        case 1:
            mvprintw(11 , 0 , "Difficulty: Easy");
            break;
        
        case 2:
            mvprintw(11 , 0 , "Difficulty: Medium");
            break;

        case 3:
            mvprintw(11 , 0 , "Difficulty: Hard");
            break;
        default:
            break;
    }
    mvprintw(12 , 0 , "Gold:   %d" , player.gold);
    mvprintw(13 , 0 , "Hits:   %d" , player.hit);
    mvprintw(14 , 0 , "Floor:  %d" , player.floor);
    mvprintw(15 , 0 , "fullness:");
    mvprintw(15 , 9 , "[");
    for (int i = 0; i < player.Mfullness; i++)
    {
        if (i < player.fullness)
        {
            mvprintw(15 ,  i + 10 , "#");
        }else
        {
            mvprintw(15 ,  i + 10 , ".");
        }
    }
    mvprintw(15 ,  player.fullness + 10 , "]" );
    mvprintw(25 , 0 , "\"i\" -> weapon list");
    mvprintw(27 , 0 , "\"j\" -> spell list");
}

void add_gold(room_info room){
    int count = randomint(0 , 100);
    if(count > 95){
        int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
        int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
        // mvprintw(xx , yy , "©");
        w[xx][yy].what = 302;
    }
    else if(count > 55){
        int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
        int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
        // mvprintw(xx , yy , "©");
        w[xx][yy].what = 301;
        if(count > 80){
            int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
            int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
            // mvprintw(xx , yy , "©");
            w[xx][yy].what = 301;
        }
    }
}

void add_trap(room_info room){
    int count = randomint(0 , 100 * diff_level);
    if(count > 65){
        int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
        int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
        w[xx][yy].what = 8;
        if(count > 90){
            int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
            int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
            w[xx][yy].what = 8;
            if(count > 150){
                int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
                int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
                w[xx][yy].what = 8;
                if(count > 230){
                    int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
                    int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
                    w[xx][yy].what = 8;
                    if(count > 250){
                        int xx = randomint(room.up_left.x + 1 , room.bottom_right.x);
                        int yy = randomint(room.up_left.y + 1 , room.bottom_right.y);
                        w[xx][yy].what = 8;
                    }
                }
            }
        }
    }
}

void setcolors(){
    init_pair(1, COLOR_RED, COLOR_BLACK); 
    init_pair(2, COLOR_GREEN, COLOR_BLACK); 
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_WHITE , COLOR_GREEN );
    init_pair(5, COLOR_WHITE , COLOR_CYAN);
    init_pair(6, COLOR_WHITE , COLOR_BLACK + 8);
    init_pair(7 ,  16 , 226);
    init_pair(8 ,  16 , 51);
    init_pair(9 ,  16 , 179);
    init_pair(10 , 34 , 0 );
    init_pair(11 , 51 , 0 );
    init_pair(12 , 160 , 0 );
    init_pair(13 , 226 , 0 );
    init_pair(14 , 0 , 226);
    init_pair(15 , 204 , 0);
    init_pair(16 , 220 , 0);
    init_pair(17 , 128 , 0);
      
}

void weapon_list(){
    clear();
    board();
    attron(A_BOLD);
    mvprintw(4 , COLS/2 - 10 , "Mace:        %d  ⚒" , player.weapon[0]);
    mvprintw(6 , COLS/2 - 10 , "Dagger:      %d  🗡" , player.weapon[1]);
    mvprintw(8 , COLS/2 - 10 , "Majic Wand:  %d  ⁋" , player.weapon[2]);
    mvprintw(10 , COLS/2 - 10, "Arrow:       %d  ➳" , player.weapon[3]);
    mvprintw(12 , COLS/2 - 10, "Sword:       %d  ⚔" , player.weapon[4]);
    attroff(A_BOLD);
    mvprintw(16 , COLS/2 - 10 , "press any key to continue...");
    getch();
}

void spell_list(){
    clear();
    board();
    attron(A_BOLD);
    mvprintw(4 , COLS/2 - 10 , "Health Spell:  %d  ♨" , player.spell[0]);
    mvprintw(6 , COLS/2 - 10 , "Speed Spell:   %d  ⟫" , player.spell[1]);
    mvprintw(8 , COLS/2 - 10 , "Damage Spell:  %d  ↯" , player.spell[2]);
    attroff(A_BOLD);
    mvprintw(16 , COLS/2 - 10 , "press any key to continue...");
    getch();
}

void w_reset(){
    for (int i = 0; i < 35; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            w[i][j].what = 0;
            w[i][j].vision = 0;
        }
        
    }
    
}

void w_draw(){
    for (int i = 0; i < 35; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            switch (w[i][j].what)
            {
            case 1:
                attron(COLOR_PAIR(11));
                mvprintw(i , j , ".");
                attroff(COLOR_PAIR(11));
                break;

            case 2:
                attron(COLOR_PAIR(10));
                mvprintw(i,j , "|");
                attroff(COLOR_PAIR(10));
                break;

            case 3:
                attron(COLOR_PAIR(10));
                mvprintw(i , j , "-");
                attroff(COLOR_PAIR(10));
                break;
            
            case 4:
                attron(COLOR_PAIR(12));
                mvprintw(i , j , "O");
                attroff(COLOR_PAIR(12));
                break;

            case 5:
                mvprintw(i , j , "+");
                break;

            case 6:
                attron(A_REVERSE);
                mvprintw(i , j , "#");
                attroff(A_REVERSE);
                break;

            case 7:
                mvprintw(i , j , "▲");            
                break;

            case 8:
                if (w[i][j].vision == 2)
                {
                    attron(COLOR_PAIR(17));
                    mvprintw(i , j , "⋀");
                    attroff(COLOR_PAIR(17));
                }else
                {
                    attron(COLOR_PAIR(11));
                    mvprintw(i , j , ".");
                    attroff(COLOR_PAIR(11));      
                }
                
                
                break;
            

            case 20:
                mvprintw(i , j , "◒");
                break;

            case 301:
                attron(COLOR_PAIR(13));
                mvprintw(i , j , "©");
                attroff(COLOR_PAIR(13));
                break;

            case 302:
                attron(COLOR_PAIR(14));
                mvprintw(i , j , "℗");
                attroff(COLOR_PAIR(14));
                break;

            case 101:
                mvprintw(i , j , "⚒");
                break;

            case 102:
                mvprintw(i , j , "🗡");
                break;

            case 103:
                mvprintw(i , j , "⁋");
                break;

            case 104:
                mvprintw(i , j , "➳");
                break;

            case 105:
                mvprintw(i , j , "⚔");
                break;

            case 201:
                mvprintw(i , j , "♨");
                break;

            case 202:
                mvprintw(i , j , "⟫");
                break;

            case 203:
                mvprintw(i , j , "↯");
                break;

            case 1001:
                attron(COLOR_PAIR(15));
                mvprintw(i , j , ".");
                attroff(COLOR_PAIR(15));
                break;

            case 1002:
                attron(COLOR_PAIR(16));
                mvprintw(i,j , "|");
                attroff(COLOR_PAIR(16));
                break;

            case 1003:
                attron(COLOR_PAIR(16));
                mvprintw(i , j , "-");
                attroff(COLOR_PAIR(16));
                break;
            
            case 1004:
                attron(COLOR_PAIR(12));
                mvprintw(i , j , "O");
                attroff(COLOR_PAIR(12));
                break;

            case 1007:
                attron(COLOR_PAIR(11));
                mvprintw(i , j , "⌂");
                attroff(COLOR_PAIR(11));
                break;

            case 1008:
                if (w[i][j].vision == 2)
                {
                    attron(COLOR_PAIR(17));
                    mvprintw(i , j , "⋀");
                    attroff(COLOR_PAIR(17));
                }else
                {
                    attron(COLOR_PAIR(15));
                    mvprintw(i , j , ".");
                    attroff(COLOR_PAIR(15));      
                }
                break; 

            default:
                break;
            }
        }
        
    }
    
}

void put_spellandweapon(){
    int wp = randomint(0 , 8 - diff_level*2);
    while(wp>0){
        int wwp = randomint(1 , 5);    
        if (wwp == 4 && player.weapon[4] > 0)
        {
            continue;
        }
          
        int done = 0;
        while(done == 0){
            int xx = randomint(2 , 34);
            int yy = randomint(20, 132);
            if(w[xx][yy].what == 1){
                w[xx][yy].what = 101 + wwp;
                done = 1;
                wp -= 1;
            }
        }
        
    }

    int sp = randomint(0 , 8 - diff_level*2);
    while(sp){
        int ssp = randomint(0 , 3);
        int done = 0;
        while(done == 0){
            int xx = randomint(2 , 34);
            int yy = randomint(2, 130);
            if(w[xx][yy].what == 1){
                w[xx][yy].what = 201 + ssp;
                done = 1;
                sp = 0;
            }
        }
    
    }
}

void treasure_room(room_info room){
    for (int i = room.up_left.x; i <= room.bottom_right.x ; i++)
    {
        for (int j = room.up_left.y; j <= room.bottom_right.y; j++)
        {
            if (w[i][j].what >= 1 && w[i][j].what <=4)
            {
                w[i][j].what += 1000;
            }else if (w[i][j].what == 7)
            {
                w[i][j].what += 1000;
            }
            else if (w[i][j].what == 8)
            {
                w[i][j].what += 1000;
            }
            
            
            
        }
        
    }
    
    
}

void check_trap(){
    if (w[player.position.x][player.position.y].what == 8 || w[player.position.x][player.position.y].what == 1008)
    {
        player.hit -= 5;
        player.hit -= 3* diff_level;
        if(w[player.position.x][player.position.y].vision <= 2){
            w[player.position.x][player.position.y].vision = 2;
        }
    }
}

int check_live(int x , int y , int who){
    if ((player.position.x == x) && (player.position.y == y) && (who != 1))
    {
        return 1;
    }
    if (who != 2)
    {
        for (int i = 0; i < 50; i++)
        {
            if (enemy[i].exist == 1)
            {
                if((enemy[i].position.x == x) && (enemy[i].position.y == y) ){
                    return 1;
                }
            }
            
        }
        
    }
    

    return 0;
    
    
}

void reset_enemy(){
    num_D = 0;
    num_F = 0;
    num_G = 0;
    num_S = 0;
    num_U = 0;
    num_of_enemies = 0;

    for (int i = 0; i < 100; i++)
    {
        enemy[i].ammo_left = 0;
        enemy[i].exist = 0;
        enemy[i].health = 0;
        enemy[i].movement_left = 0;
        enemy[i].position.x = 0;
        enemy[i].position.y = 0;
        enemy[i].type = ' ';
        
    }
    
}

void put_enemy(){
    for (int ii = 0; ii < 4; ii++)
    {
        for (int jj = 0; jj < 2; jj++)
        {
            if (room[10*jj + ii].E == 1)
            {
                int done1 = 1;

                while(done1){
                    int xx = randomint(room[10*jj + ii].up_left.x + 1 ,room[10*jj + ii].bottom_right.x );
                    int yy = randomint(room[10*jj + ii].up_left.y + 1 , room[10*jj + ii].bottom_right.y );
                    if(w[xx][yy].what == 1 && check_live(xx , yy , 0) == 0 ){
                        enemy[num_of_enemies].exist = 1;
                        while (done1)
                        {
                            int who = randomint(0,5);
                            switch (who)
                            {
                            case 0:
                                if (num_D < 10)
                                {
                                    enemy[num_of_enemies].type = 'D';
                                    enemy[num_of_enemies].health = 5;
                                    enemy[num_of_enemies].movement_left = 20;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_D++;
                                    num_of_enemies++;
                                    done1 = 0;
                                }
                                break;
                            case 1:
                                if (num_F < 5)
                                {
                                    enemy[num_of_enemies].type = 'F';
                                    enemy[num_of_enemies].health = 10;
                                    enemy[num_of_enemies].movement_left = 20;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_F++;
                                    num_of_enemies++;
                                    done1 = 0;
                                }
                                
                                break;

                            case 2:
                                if (num_G < 3)
                                {
                                    enemy[num_of_enemies].type = 'G';
                                    enemy[num_of_enemies].health = 15;
                                    enemy[num_of_enemies].movement_left = 5;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_of_enemies++;
                                    num_G++;
                                    done1 = 0;
                                }
                                break;

                            case 3:
                                if (num_S < 3)
                                {
                                    enemy[num_of_enemies].type = 'S';
                                    enemy[num_of_enemies].health = 20;
                                    enemy[num_of_enemies].movement_left = -1;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_G++;
                                    num_of_enemies++;
                                    done1 = 0;
                                }
                                break;

                            case 4:
                                if (num_U < 2)
                                {
                                    enemy[num_of_enemies].type = 'U';
                                    enemy[num_of_enemies].health = 30;
                                    enemy[num_of_enemies].movement_left = -1;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_S++;
                                    num_of_enemies++;
                                    done1 = 0;
                                }
                                break;

                            default:
                                break;
                            }                        
                        }
                        
                    }
                }

                if (diff_level == 3 && (randomint(0,5)==4))
                {
                       
                while(done1){
                    int xx = randomint(room[10*jj + ii].up_left.x + 1 ,room[10*jj + ii].bottom_right.x );
                    int yy = randomint(room[10*jj + ii].up_left.y +1 , room[10*jj + ii].bottom_right.y );
                    if(w[xx][yy].what == 1 && check_live(xx , yy , 0) == 0 ){
                        enemy[num_of_enemies].exist = 1;
                        while (done1)
                        {
                            int who = randomint(0,5);
                            switch (who)
                            {
                            case 0:
                                if (num_D < 5)
                                {
                                    enemy[num_of_enemies].type = 'D';
                                    enemy[num_of_enemies].health = 5;
                                    enemy[num_of_enemies].movement_left = -1;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_D++;
                                    done1 = 0;
                                }
                                break;
                            case 1:
                                if (num_F < 5)
                                {
                                    enemy[num_of_enemies].type = 'F';
                                    enemy[num_of_enemies].health = 10;
                                    enemy[num_of_enemies].movement_left = -1;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_F++;
                                    done1 = 0;
                                }
                                
                                break;

                            case 2:
                                if (num_G < 3)
                                {
                                    enemy[num_of_enemies].type = 'G';
                                    enemy[num_of_enemies].health = 15;
                                    enemy[num_of_enemies].movement_left = 5;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_G++;
                                    done1 = 0;
                                }
                                break;

                            case 3:
                                if (num_S < 3)
                                {
                                    enemy[num_of_enemies].type = 'S';
                                    enemy[num_of_enemies].health = 20;
                                    enemy[num_of_enemies].movement_left = -1;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_S++;
                                    done1 = 0;
                                }
                                break;

                            case 4:
                                if (num_U < 2)
                                {
                                    enemy[num_of_enemies].type = 'U';
                                    enemy[num_of_enemies].health = 30;
                                    enemy[num_of_enemies].movement_left = 0;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_U++;
                                    done1 = 0;
                                }
                                break;

                            default:
                                break;
                            }                        
                        }
                        
                    }
                }

                }

                if (diff_level == 2 && randomint(0,2))
                {
                       
                while(done1){
                    int xx = randomint(room[10*jj + ii].up_left.x + 1 ,room[10*jj + ii].bottom_right.x );
                    int yy = randomint(room[10*jj + ii].up_left.y +1 , room[10*jj + ii].bottom_right.y );
                    if(w[xx][yy].what == 1 && check_live(xx , yy , 0) == 0 ){
                        enemy[num_of_enemies].exist = 1;
                        while (done1)
                        {
                            int who = randomint(0,5);
                            switch (who)
                            {
                            case 0:
                                if (num_D < 5)
                                {
                                    enemy[num_of_enemies].type = 'D';
                                    enemy[num_of_enemies].health = 5;
                                    enemy[num_of_enemies].movement_left = -1;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_D++;
                                    done1 = 0;
                                }
                                break;
                            case 1:
                                if (num_F < 5)
                                {
                                    enemy[num_of_enemies].type = 'F';
                                    enemy[num_of_enemies].health = 10;
                                    enemy[num_of_enemies].movement_left = -1;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_F++;
                                    done1 = 0;
                                }
                                
                                break;

                            case 2:
                                if (num_G < 3)
                                {
                                    enemy[num_of_enemies].type = 'G';
                                    enemy[num_of_enemies].health = 15;
                                    enemy[num_of_enemies].movement_left = 5;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_G++;
                                    done1 = 0;
                                }
                                break;

                            case 3:
                                if (num_S < 3)
                                {
                                    enemy[num_of_enemies].type = 'S';
                                    enemy[num_of_enemies].health = 20;
                                    enemy[num_of_enemies].movement_left = -1;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_G++;
                                    done1 = 0;
                                }
                                break;

                            case 4:
                                if (num_U < 2)
                                {
                                    enemy[num_of_enemies].type = 'U';
                                    enemy[num_of_enemies].health = 30;
                                    enemy[num_of_enemies].movement_left = 0;
                                    enemy[num_of_enemies].position.x = xx;
                                    enemy[num_of_enemies].position.y = yy;
                                    num_S++;
                                    done1 = 0;
                                }
                                break;

                            default:
                                break;
                            }                        
                        }
                        
                    }
                }

                }
                
                
            
            }
            
        }
        
    }
    
}

void print_enemy(){
    for (int i = 0; i < 50; i++)
    {
        if (enemy[i].exist == 1 && enemy[i].health >= 1 )
        {
            mvaddch(enemy[i].position.x , enemy[i].position.y , enemy[i].type);
        }
        
    }
    
}

void move_enemy(){
    for (int i = 0; i < 50; i++)
    {
    if (enemy[i].exist == 1 && (check_room_pos(player.position.x , player.position.y) == check_room_pos(enemy[i].position.x , enemy[i].position.y)))
    {
        if (enemy[i].type == 'D')
        {
            //damage
            if ((player.position.x >= (enemy[i].position.x - 1)) && (player.position.x <= (enemy[i].position.x + 1)))
            {
            if((player.position.y >= (enemy[i].position.y - 1)) && (player.position.y <= (enemy[i].position.y + 1)))
            {
                player.hit -= 3;
                break;
            }
            }
            
            //move
            pos temp = enemy[i].position;
            if (enemy[i].movement_left > 0)
            {
            if (player.position.x < enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
                
            }
            else if (player.position.x < enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }
            else if (player.position.x > enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
            }
            else if (player.position.x < enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
            }
            else if (player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }

            }

            if (temp.x != enemy[i].position.x || temp.y != enemy[i].position.y)
            {
                enemy[i].movement_left--;
            }
            
        }
        else if (enemy[i].type == 'F')
        {
            //damage
            if ((player.position.x >= (enemy[i].position.x - 2)) && (player.position.x <= (enemy[i].position.x + 2)))
            {
            if((player.position.y >= (enemy[i].position.y - 2)) && (player.position.y <= (enemy[i].position.y + 2)))
            {
                player.hit -= 4;
                break;
            }
            }

            //move
            pos temp = enemy[i].position;
            if (enemy[i].movement_left > 0)
            {
            if (player.position.x < enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.x--;
                    enemy[i].position.y--;
                }
            }
            else if (player.position.x < enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.x--;
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.x++;
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.x++;
                    enemy[i].position.y--;
                }
            }
            else if (player.position.x > enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
            }
            else if (player.position.x < enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
            }
            else if (player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }

            }

            if (temp.x != enemy[i].position.x || temp.y != enemy[i].position.y)
            {
                enemy[i].movement_left--;
            }

        }
        else if (enemy[i].type == 'G')
        {
            //damage
            if ((player.position.x >= (enemy[i].position.x - 1)) && (player.position.x <= (enemy[i].position.x + 1)))
            {
            if((player.position.y >= (enemy[i].position.y - 1)) && (player.position.y <= (enemy[i].position.y + 1)))
            {
                player.hit -= 15;
                break;
            }
            }

            //move
            pos temp = enemy[i].position;
            if (enemy[i].movement_left > 0)
            {
                if (player.position.x < enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
                
            }
            else if (player.position.x < enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }
            else if (player.position.x > enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
            }
            else if (player.position.x < enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
            }
            else if (player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }
            }

            if (temp.x != enemy[i].position.x || temp.y != enemy[i].position.y)
            {
                enemy[i].movement_left--;
            }
            
            
            
        }
        else if (enemy[i].type == 'S')
        {
            //damage
            if ((player.position.x >= (enemy[i].position.x - 1)) && (player.position.x <= (enemy[i].position.x + 1)))
            {
            if((player.position.y >= (enemy[i].position.y - 1)) && (player.position.y <= (enemy[i].position.y + 1)))
            {
                player.hit -= 7;
                break;
            }
            }
            
            //move
            if (player.position.x < enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
                
            }
            else if (player.position.x < enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }
            else if (player.position.x > enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
            }
            else if (player.position.x < enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
            }
            else if (player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }

        }
        else if (enemy[i].type == 'U')
        {
            //damage
            if ((player.position.x >= (enemy[i].position.x - 1)) && (player.position.x <= (enemy[i].position.x + 1)))
            {
            if((player.position.y >= (enemy[i].position.y - 1)) && (player.position.y <= (enemy[i].position.y + 1)))
            {
                player.hit -= 15;
                if (enemy[i].movement_left == -1)
                {
                    enemy[i].movement_left = 5;
                }
                
                break;
            }
            }
            
            //move
            pos temp = enemy[i].position;
            if (enemy[i].movement_left > 0)
            {

            if (player.position.x < enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
                else if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
                
            }
            else if (player.position.x < enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y > enemy[i].position.y)
            {
                
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.x > enemy[i].position.x && player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }
            else if (player.position.x > enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x +1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x++;
                }
            }
            else if (player.position.x < enemy[i].position.x)
            {
                if (obstacle_check(enemy[i].position.x -1 , enemy[i].position.y) == 0)
                {
                    enemy[i].position.x--;
                }
            }
            else if (player.position.y > enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y + 1) == 0)
                {
                    enemy[i].position.y++;
                }
            }
            else if (player.position.y < enemy[i].position.y)
            {
                if (obstacle_check(enemy[i].position.x  , enemy[i].position.y - 1) == 0)
                {
                    enemy[i].position.y--;
                }
            }

            }


            if (temp.x != enemy[i].position.x || temp.y != enemy[i].position.y)
            {
                enemy[i].movement_left--;
            }
        }
        
        
        
        
        
    }
    
    }
    
}





