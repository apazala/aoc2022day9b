#include <cstdio>
#include <unordered_set>
#include <unordered_map>

int loadInputFile();
int nextLine(char *dst);

using std::unordered_set;
using std::unordered_map;

struct position
{
    int x;
    int y;

    position():x(0),y(0){}
    position(int x, int y):x(x),y(y){}

    void move(const position& d)
    {
        x+=d.x;
        y+=d.y;
    }

    bool operator==(const position& p) const
    {
        return (x == p.x) && (y == p.y);
    }

    position operator-(const position& p)const
    {
        return {x-p.x, y-p.y};
    }
};

struct positionHash { 
    size_t operator()(const position& p) const
    {
        return p.x*31+p.y;
    }
};

int main()
{
    char line[16];
    position rope[10];
    if(loadInputFile() < 0){
        fprintf(stderr, "Can't open: input.txt\n");
        return 1;
    }


    unordered_map<char, position> directionMovesMap;
    directionMovesMap['R'] = {1, 0};
    directionMovesMap['L'] = {-1, 0};
    directionMovesMap['U'] = {0, 1};
    directionMovesMap['D'] = {0, -1};
    


    unordered_set<position, positionHash> visitedTailPositions;
    visitedTailPositions.insert(rope[9]);

    unordered_map<position, position, positionHash> knotCorrectionMovesMap;
    for(int x = -1; x <= 1; x++)
        for(int y = -1; y <= 1; y++)
            knotCorrectionMovesMap[position(x, y)] = {0, 0};
    
    for(int y = -1; y <= 1; y++)
        knotCorrectionMovesMap[position(2, y)] = {1, y};
    for(int y = -1; y <= 1; y++)
        knotCorrectionMovesMap[position(-2, y)] = {-1, y};
    for(int x = -1; x <= 1; x++)
        knotCorrectionMovesMap[position(x, 2)] = {x, 1};
    for(int x = -1; x <= 1; x++)
        knotCorrectionMovesMap[position(x, -2)] = {x, -1};
    
    //new posible moves!
    knotCorrectionMovesMap[position(2, 2)] = {1, 1};
    knotCorrectionMovesMap[position(2, -2)] = {1, -1};
    knotCorrectionMovesMap[position(-2, 2)] = {-1, 1};
    knotCorrectionMovesMap[position(-2, -2)] = {-1, -1};

    position headDir, knotDir;
    int n;
    char dirChar;
    while(nextLine(line)){
        if(sscanf(line, " %c %d", &dirChar, &n) != 2) break;

        headDir = directionMovesMap[dirChar];
        while(n--){
            rope[0].move(headDir);
            for(int i = 1; i < 10; i++)
            {
                knotDir = knotCorrectionMovesMap[rope[i-1]-rope[i]];
                rope[i].move(knotDir);
            }
            visitedTailPositions.insert(rope[9]);
        }
    }

    printf("%zu\n", visitedTailPositions.size());

}

#define BUFSIZE 1 << 14

char fileBuf[BUFSIZE];
char *pbuf = fileBuf;
char *pbufend = fileBuf;

int loadInputFile()
{
    FILE *pf = fopen("input.txt", "r");
    if (!pf)
        return -1;
    pbuf = fileBuf;
    pbufend = fileBuf + fread(fileBuf, 1, BUFSIZE, pf);
    fclose(pf);

    return 0;
}

int nextLine(char *dst)
{
    char c;
    char *orig = dst;

    // read line:
    while (pbuf < pbufend && (c = *(pbuf++)) != '\n')
    {
        *(dst++) = c;
    }
    *dst = '\0';
    // return line length
    return dst - orig;
}