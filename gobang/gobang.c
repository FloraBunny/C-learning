#include "lazy.h"
#include "charlib.h"
#define BOUNDRY 19
#define BLACK 1
#define WHITE 2
#define NORMAL 0
#define CLEAR 1
#define TEST              \
    ManualSetUp(1, 2, 1); \
    PrintBoard();

/*
Todo:

timer
board
weight Gen


*/

int board[BOUNDRY][BOUNDRY];
int printboard[BOUNDRY][BOUNDRY];

FILE *log_file;

int PvpMode();
int PveMode();
int SocketMode();     //与其他程序进行无人值守对战用
int SetUpBoard();     //初始化棋盘
int PrintBoard();     //显示棋盘
int ShowBoardArray(); //简单显示棋盘
int JudgeWin();       //判断胜利,若有一方胜利返回对应的color,否则返回0
int JudgeWinPlus();   //判断胜利,若有一方胜利返回对应的color,否则返回0,简化后的未测试代码
int GenWeight();      //产生权重
int MakeMove();       //电脑落子
int Abs();
int ManualSetUpAll();    //手动设置当前棋盘(调试用)
int ManualSetUp();       //手动设置当前棋盘某一位置(调试用)(pvp用)
int ForcedManualSetUp(); //手动设置当前棋盘某一位置(调试用)(无限制修改)
int _Tester();           //判断胜负时使用的子函数
int Save();              //保存当前操作到棋谱
int DisplayLog();        //显示棋谱

FILE *InitializeSaving(); //初始化保存文件

int main(int argc, char *argv[])
{

    //初始化棋盘
    SetUpBoard();

    //mode选择
    int mode_choosed;
    if (argc == 1)
    {
        puts("--------------------------------------");
        LICENSE;
        puts("");
        puts("Run without parameter.");
        puts("Tips: you can also run this program with paramter. ");
        puts("Please choose game mode. 1:pvp 2:pve 3: pevpe 4:test 5:display log 0:exit");
        scanf("%d", &mode_choosed);
        // fflush(stdin);//windows
        setbuf(stdin, NULL); //For all OSs, clear buf to eat \n
    }
    else
    {
        mode_choosed = atoi(argv[1]);
    }

    switch (mode_choosed)
    {
    case 1:
        log_file = InitializeSaving();
        PvpMode();
        break;
    case 2:
        log_file = InitializeSaving();
        PveMode();
        break;
    case 3:
        log_file = InitializeSaving();
        SocketMode();
        break;
    case 4:
        log_file = InitializeSaving();
        TEST
            BP;
        break;
    case 5:
    {
        char log_name[] = "SunOct290114402016.log";
        gets(log_name);
        log_file = fopen(log_name, "r");
        DisplayLog();
        break;
    }
    case 0:
        exit(0);
    default:
        // puts("Invaild input, please input again.");
        // break;
        puts("Use test mode as default.");
        TEST;
        BP;
    }
    fflush(log_file); //write log.
    fclose(log_file); //close log.
    puts("Gobang.c finished successfully.");
}

int PvpMode()
{
    int a;
    int b;
    int movecnt = 1;
    int colornow = BLACK;
    while (!JudgeWin())
    {
        for (int i = 0; i < 30; i++)
            puts("");
        LICENSE;
        puts("------------------------------------------");
        puts("This is pvp mode.");
        puts("The board now is:");
        ShowBoardArray();
        puts("------------------------------------------");
        printf("This is move %d.\n", movecnt);
        printf("Turn for the %s side.\n", (colornow == BLACK ? "black" : "white"));
        puts("Please input your coordinate, the format is like \"a b\"");
        printf("a,b ranges from 0 to %d\n", BOUNDRY - 1);
        scanf("%d%d", &a, &b);
        while (ManualSetUp(a, b, colornow))
            scanf("%d%d", &a, &b);
        ;
        Save(a, b);
        movecnt++;
        if (colornow == BLACK)
        {
            colornow = WHITE;
        }
        else
        {
            colornow = BLACK;
        }
    }
    printf("Game finished in move %d.\n", movecnt);
    puts("--------------------------------------");
    ShowBoardArray();
    puts("--------------------------------------");

    BP;
    return 0;
}

int PveMode()
{
    int a;
    int b;
    int movecnt = 1;
    int colornow = BLACK;

    for (int i = 0; i < 30; i++)
        puts("");
    LICENSE;
    puts("------------------------------------------");
    puts("This is pve mode.");
    puts("Please choose your side: 1 for the black and 2 for the white");
    int player;
    scanf("%d", &player);
    while (player != 1 && player != 2)
    {
        puts("Wrong input!\nPlease choose your side: 1 for the black and 2 for the white");
        scanf("%d", &player);
    }
    setbuf(stdin, NULL); //For all OSs, clear buf to eat \n

    while (!JudgeWin())
    {
        for (int i = 0; i < 30; i++)
            puts("");
        LICENSE;
        puts("------------------------------------------");
        puts("This is pvp mode.");
        puts("The board now is:");
        ShowBoardArray();
        puts("------------------------------------------");
        printf("This is move %d.\n", movecnt);
        printf("Turn for the %s side.\n", (colornow == BLACK ? "black" : "white"));
        puts("Please input your coordinate, the format is like \"a b\"");
        printf("a,b ranges from 0 to %d\n", BOUNDRY - 1);
        scanf("%d%d", &a, &b);
        while (ManualSetUp(a, b, colornow))
            scanf("%d%d", &a, &b);
        ;
        Save(a, b);
        movecnt++;
        if (colornow == BLACK)
        {
            colornow = WHITE;
        }
        else
        {
            colornow = BLACK;
        }
    }
    printf("Game finished in move %d.\n", movecnt);
    puts("--------------------------------------");
    ShowBoardArray();
    puts("--------------------------------------");

    BP;
    return 0;
}

int SocketMode()
{
    BP;
    return 0;
}

int SetUpBoard()
{
    memset(board, 0, sizeof(int) * BOUNDRY * BOUNDRY);
    return 0;
}

int PrintBoard()
{
    // ┠ ┨┯ ┷┏┓┗ ┛┳⊥﹃﹄┌ ┐└ ┘∟
    // http://lulinbest.blog.sohu.com/88118628.html
    int LINE = 103;
    int VLINE = 104;
    int CROSS = 105;

    static int display_array[30][30];
    for (int i = 2; i < 2 + BOUNDRY; i++)
    {
        display_array[0][i] = i - 2;
    }

    for (int i = 2; i < 2 + BOUNDRY; i++)
    {
        display_array[i][0] = i - 2;
    }
    for (int a = 0; a < BOUNDRY; a++)
    {
        for (int b = 0; b < BOUNDRY; b++)
        {
            display_array[2 + a][2 + b] = board[a][b];
        }
    }

    for (int a = 0; a < 30; a++)
    {
        for (int b = 0; b < 30; b++)
        {
            switch (display_array[a][b])
            {

            case BLACK + 100:
                printf("1");
                break;
            case WHITE + 100:
                printf("2");
                break;
            case 0:
                if (a == 0 || b == 0)
                {
                    printf(" 0");
                    break;
                }
                printf(" ");
                break;
            default:
                printf("%d", display_array[a][b]);
            }
        }
        puts("");
    }

    return 0;
}

int ShowBoardArray()
{
    for (int a = 0; a < BOUNDRY; a++)
    {
        for (int b = 0; b < BOUNDRY; b++)
        {
            printf("%d ", board[a][b]);
        }
        printf("\n");
    }
    printf("\n");
    // CP;
    // printf("-----------------------------------------------------\n");
    return 0;
}

int JudgeWin()
{
    //判断胜利,应该是最高效的算法了
    //第一次写的有点丑
    int color = 0;
    int cnt = 0;
    int max = 0;
    for (int a = 0; a < BOUNDRY; a++)
    {
        for (int b = 0; b < BOUNDRY; b++)
        {
            switch (color)
            {
            case 0:
                if (board[a][b] == WHITE)
                {
                    color = WHITE;
                    cnt++;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == BLACK)
                {
                    color = BLACK;
                    cnt++;
                    if (cnt > max)
                        max = cnt;
                }
                break;
            case BLACK:
                if (board[a][b] == WHITE)
                {
                    color = WHITE;
                    cnt = 1;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == BLACK)
                {
                    color = BLACK;
                    cnt++;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == 0)
                {
                    color = 0;
                    cnt = 0;
                    if (cnt > max)
                        max = cnt;
                }
                break;
            case WHITE:
                if (board[a][b] == WHITE)
                {
                    color = WHITE;
                    cnt++;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == BLACK)
                {
                    color = BLACK;
                    cnt = 1;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == 0)
                {
                    color = 0;
                    cnt = 0;
                    if (cnt > max)
                        max = cnt;
                }
                break;
            }
            if (max == 5)
            {
                if (color == WHITE)
                {
                    printf("White wins!\n");
                }
                else if (color == BLACK)
                {
                    printf("Black wins!\n");
                }
                return color;
            }
        }
        max = 0;
        cnt = 0;
        color = 0;
    }
    //row
    color = 0;
    cnt = 0;
    max = 0;
    for (int b = 0; b < BOUNDRY; b++)
    {
        for (int a = 0; a < BOUNDRY; a++)
        {
            switch (color)
            {
            case 0:
                if (board[a][b] == WHITE)
                {
                    color = WHITE;
                    cnt++;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == BLACK)
                {
                    color = BLACK;
                    cnt++;
                    if (cnt > max)
                        max = cnt;
                }
                break;
            case BLACK:
                if (board[a][b] == WHITE)
                {
                    color = WHITE;
                    cnt = 1;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == BLACK)
                {
                    color = BLACK;
                    cnt++;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == 0)
                {
                    color = 0;
                    cnt = 0;
                    if (cnt > max)
                        max = cnt;
                }
                break;
            case WHITE:
                if (board[a][b] == WHITE)
                {
                    color = WHITE;
                    cnt++;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == BLACK)
                {
                    color = BLACK;
                    cnt = 1;
                    if (cnt > max)
                        max = cnt;
                }
                else if (board[a][b] == 0)
                {
                    color = 0;
                    cnt = 0;
                    if (cnt > max)
                        max = cnt;
                }
                break;
            }
            if (max == 5)
            {
                if (color == WHITE)
                {
                    printf("White wins!\n");
                }
                else if (color == BLACK)
                {
                    printf("Black wins!\n");
                }
                return color;
            }
        }
        max = 0;
        cnt = 0;
        color = 0;
    }
    //dig2
    color = 0;
    cnt = 0;
    max = 0;
    for (int aplusb = 0; aplusb < 2 * (BOUNDRY - 1); aplusb++)
    {
        for (int a = 0; a < BOUNDRY; a++)
        {
            int b = aplusb - a;
            if (b >= 0 && b < BOUNDRY)
            {
                switch (color)
                {
                case 0:
                    if (board[a][b] == WHITE)
                    {
                        color = WHITE;
                        cnt++;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == BLACK)
                    {
                        color = BLACK;
                        cnt++;
                        if (cnt > max)
                            max = cnt;
                    }
                    break;
                case BLACK:
                    if (board[a][b] == WHITE)
                    {
                        color = WHITE;
                        cnt = 1;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == BLACK)
                    {
                        color = BLACK;
                        cnt++;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == 0)
                    {
                        color = 0;
                        cnt = 0;
                        if (cnt > max)
                            max = cnt;
                    }
                    break;
                case WHITE:
                    if (board[a][b] == WHITE)
                    {
                        color = WHITE;
                        cnt++;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == BLACK)
                    {
                        color = BLACK;
                        cnt = 1;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == 0)
                    {
                        color = 0;
                        cnt = 0;
                        if (cnt > max)
                            max = cnt;
                    }
                    break;
                }
                if (max == 5)
                {
                    if (color == WHITE)
                    {
                        printf("White wins!\n");
                    }
                    else if (color == BLACK)
                    {
                        printf("Black wins!\n");
                    }
                    return color;
                }
            }
        }
        max = 0;
        cnt = 0;
        color = 0;
    }

    //dig1
    color = 0;
    cnt = 0;
    max = 0;
    for (int aminusb = -(BOUNDRY - 1); aminusb < (BOUNDRY - 1); aminusb++)
    {
        for (int a = 0; a < BOUNDRY; a++)
        {
            int b = a - aminusb;
            if (b >= 0 && b < BOUNDRY)
            {
                switch (color)
                {
                case 0:
                    if (board[a][b] == WHITE)
                    {
                        color = WHITE;
                        cnt++;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == BLACK)
                    {
                        color = BLACK;
                        cnt++;
                        if (cnt > max)
                            max = cnt;
                    }
                    break;
                case BLACK:
                    if (board[a][b] == WHITE)
                    {
                        color = WHITE;
                        cnt = 1;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == BLACK)
                    {
                        color = BLACK;
                        cnt++;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == 0)
                    {
                        color = 0;
                        cnt = 0;
                        if (cnt > max)
                            max = cnt;
                    }
                    break;
                case WHITE:
                    if (board[a][b] == WHITE)
                    {
                        color = WHITE;
                        cnt++;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == BLACK)
                    {
                        color = BLACK;
                        cnt = 1;
                        if (cnt > max)
                            max = cnt;
                    }
                    else if (board[a][b] == 0)
                    {
                        color = 0;
                        cnt = 0;
                        if (cnt > max)
                            max = cnt;
                    }
                    break;
                }
                if (max == 5)
                {
                    if (color == WHITE)
                    {
                        printf("White wins!\n");
                    }
                    else if (color == BLACK)
                    {
                        printf("Black wins!\n");
                    }
                    return color;
                }
            }
        }
        max = 0;
        cnt = 0;
        color = 0;
    }
}

int GenWeight()
{
    //产生权重
}

int MakeMove()
{
    //电脑落子
}

int Abs(int input)
{
    if (input < 0)
    {
        input = -input;
    }
    return input;
}

int ManualSetUpAll() //手动设置当前棋盘(调试用)
{
    for (int a = 0; a < BOUNDRY; a++)
    {
        for (int b = 0; b < BOUNDRY; b++)
        {
            scanf("%d", &board[a][b]);
        }
    }
    setbuf(stdin, NULL); //For all OSs, clear buf to eat \n
    puts("Setup the board manually to:");
    ShowBoardArray();
    return 0;
}

int ManualSetUp(int a, int b, int color) //手动设置当前棋盘某一位置(调试用)
{
    if (a >= 0 && a < BOUNDRY && b >= 0 && b < BOUNDRY && (color == 0 || color == BLACK || color == WHITE))
    {
        if (board[a][b] != 0)
        {
            puts("Wrong input, please input again!");
            setbuf(stdin, NULL);
            return -1;
        }
        board[a][b] = color;
        return 0;
    }
    puts("Wrong input, please input again!");
    setbuf(stdin, NULL);
    return -1;
}

int ForcedManualSetUp(int a, int b, int color) //手动设置当前棋盘某一位置(调试用)(无视规则)
{
    if (a >= 0 && a < BOUNDRY && b >= 0 && b < BOUNDRY && (color == 0 || color == BLACK || color == WHITE))
    {
        board[a][b] = color;
        return 0;
    }
    puts("Wrong input, setup failed!");
    return -1;
}

int _Tester(int a, int b, int mode) //if mode==CLEAR, set cnt,max,color=0
{
    static int cnt;
    static int max;
    static int color;
    if (mode == CLEAR)
    {
        cnt = 0;
        max = 0;
        color = 0;
    }
    switch (color)
    {
    case 0:
        if (board[a][b] == WHITE)
        {
            color = WHITE;
            cnt++;
            if (cnt > max)
                max = cnt;
        }
        else if (board[a][b] == BLACK)
        {
            color = BLACK;
            cnt++;
            if (cnt > max)
                max = cnt;
        }
        break;
    case BLACK:
        if (board[a][b] == WHITE)
        {
            color = WHITE;
            cnt = 1;
            if (cnt > max)
                max = cnt;
        }
        else if (board[a][b] == BLACK)
        {
            color = BLACK;
            cnt++;
            if (cnt > max)
                max = cnt;
        }
        else if (board[a][b] == 0)
        {
            color = 0;
            cnt = 0;
            if (cnt > max)
                max = cnt;
        }
        break;
    case WHITE:
        if (board[a][b] == WHITE)
        {
            color = WHITE;
            cnt++;
            if (cnt > max)
                max = cnt;
        }
        else if (board[a][b] == BLACK)
        {
            color = BLACK;
            cnt = 1;
            if (cnt > max)
                max = cnt;
        }
        else if (board[a][b] == 0)
        {
            color = 0;
            cnt = 0;
            if (cnt > max)
                max = cnt;
        }
        break;
    }
    if (max == 5)
    {
        if (color == WHITE)
        {
            printf("White wins!\n");
        }
        else if (color == BLACK)
        {
            printf("Black wins!\n");
        }
        return color;
    }
}

int JudgeWinPlus()
{
    //判断胜利,应该是最高效的算法了
    //使用static简化了代码

    for (int a = 0; a < BOUNDRY; a++)
    {
        int mark = CLEAR;
        for (int b = 0; b < BOUNDRY; b++)
        {
            _Tester(a, b, mark);
            mark = NORMAL;
        }
    }
    for (int b = 0; b < BOUNDRY; b++)
    {
        int mark = CLEAR;
        for (int a = 0; a < BOUNDRY; a++)
        {
            _Tester(a, b, mark);
            mark = NORMAL;
        }
    }
    for (int aplusb = 0; aplusb < 2 * (BOUNDRY - 1); aplusb++)
    {
        int mark = CLEAR;

        for (int a = 0; a < BOUNDRY; a++)
        {
            int b = aplusb - a;
            if (b >= 0 && b < BOUNDRY)
            {
                _Tester(a, b, mark);
                mark = NORMAL;
            }
        }
    }
    for (int aminusb = -(BOUNDRY - 1); aminusb < (BOUNDRY - 1); aminusb++)
    {
        int mark = CLEAR;

        for (int a = 0; a < BOUNDRY; a++)
        {
            int b = a - aminusb;
            if (b >= 0 && b < BOUNDRY)
            {
                _Tester(a, b, mark);
                mark = NORMAL;
            }
        }
    }
}

int Save(int a, int b) //保存当前操作到棋谱
{
    fprintf(log_file, "%d %d\n", a, b);
}

int DisplayLog()
{

    int a;
    int b;
    int movecnt = 1;
    int colornow = BLACK;
    while (!JudgeWin())
    {
        fscanf(log_file, "%d %d", &a, &b);
        CK(a);
        CK(b);
        if (ManualSetUp(a, b, colornow))
        {
            puts("Wrong log format.");
            exit(-1);
        }
        // Save(a, b);
        for (int i = 0; i < 30; i++)
            puts("");
        LICENSE;
        puts("------------------------------------------");
        puts("This is display mode.");
        puts("The board now is:");
        ShowBoardArray();
        puts("------------------------------------------");
        printf("This is move %d.\n", movecnt);
        printf("Turn for the %s side.\n", (colornow == BLACK ? "black" : "white"));
        // puts("Please input your coordinate, the format is like \"a b\"");
        // printf("a,b ranges from 0 to %d\n", BOUNDRY - 1);

        movecnt++;
        if (colornow == BLACK)
        {
            colornow = WHITE;
        }
        else
        {
            colornow = BLACK;
        }
        puts("Press any key to continue.");
        getchar();
    }
    printf("Game finished in move %d.\n", movecnt - 1);
    puts("--------------------------------------");
    ShowBoardArray();
    puts("--------------------------------------");

    BP;
    return 0;
}

FILE *InitializeSaving() //初始化保存文件
{
    //初始化log文件,保存棋谱.
    time_t time_now;
    time_now = time(NULL);
    char *log_name = DelSpaceAddLog(ctime(&time_now));
    printf("Log file is saved at %s \n", log_name);
    log_file = fopen(log_name, "a+");
    // char log_name_str[] = ctime(&time_now);
    // log_file = fopen(log_name_str, "a");
    return log_file;
}

/*
测试用例 棋盘

0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 2 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 1 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 1 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 2 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
*/

/*
测试用例 棋谱

0 0
9 0
1 % 0
10   0
2 0 
   11 0
3 0
19 19
12 0
4 0
14 0 
5 0

*/