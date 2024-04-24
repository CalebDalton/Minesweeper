#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace sf;

void SetText(Text& text, float x, float y){
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(Vector2f(x,y));
}

Texture TextureFile(Texture& texture, string loadFile){
    if(!texture.loadFromFile(loadFile)){
        cout << loadFile << " failure" << endl;
    }
    return texture;
}

enum class State{Hidden, Revealed, Flagged};
enum class Content{Empty, Mine, One, Two, Three, Four, Five, Six, Seven, Eight};

struct Tiles{
    State state;
    Content content;
    Tiles& operator=(Tiles& tile){
        if(this != &tile){
            this->state = tile.state;
            this->content = tile.content;
        }
        return *this;
    }
};

void FlagCounter(int flagCount, RenderWindow& gameWindow, int row){
    Texture digitZero;
    digitZero.loadFromFile("files/images/digits.png", IntRect(0,0,21,32));
    Sprite digitZeroS;
    digitZeroS.setTexture(digitZero);
    Texture digitOne;
    digitOne.loadFromFile("files/images/digits.png", IntRect(21,0,21,32));
    Sprite digitOneS;
    digitOneS.setTexture(digitOne);
    Texture digitTwo;
    digitTwo.loadFromFile("files/images/digits.png", IntRect(42,0,21,32));
    Sprite digitTwoS;
    digitTwoS.setTexture(digitTwo);
    Texture digitThree;
    digitThree.loadFromFile("files/images/digits.png", IntRect(63,0,21,32));
    Sprite digitThreeS;
    digitThreeS.setTexture(digitThree);
    Texture digitFour;
    digitFour.loadFromFile("files/images/digits.png", IntRect(84,0,21,32));
    Sprite digitFourS;
    digitFourS.setTexture(digitFour);
    Texture digitFive;
    digitFive.loadFromFile("files/images/digits.png", IntRect(105,0,21,32));
    Sprite digitFiveS;
    digitFiveS.setTexture(digitFive);
    Texture digitSix;
    digitSix.loadFromFile("files/images/digits.png", IntRect(126,0,21,32));
    Sprite digitSixS;
    digitSixS.setTexture(digitSix);
    Texture digitSeven;
    digitSeven.loadFromFile("files/images/digits.png", IntRect(147,0,21,32));
    Sprite digitSevenS;
    digitSevenS.setTexture(digitSeven);
    Texture digitEight;
    digitEight.loadFromFile("files/images/digits.png", IntRect(168,0,21,32));
    Sprite digitEightS;
    digitEightS.setTexture(digitEight);
    Texture digitNine;
    digitNine.loadFromFile("files/images/digits.png", IntRect(189,0,21,32));
    Sprite digitNineS;
    digitNineS.setTexture(digitNine);
    Texture digitMinus;
    digitMinus.loadFromFile("files/images/digits.png", IntRect(210,0,21,32));
    Sprite digitMinusS;
    digitMinusS.setTexture(digitMinus);
    if(flagCount < 0){
        gameWindow.draw(digitMinusS);
        digitMinusS.setPosition(12, 32 * (row + 0.5f) + 16);
    }


}

void RevealTiles(vector<vector<Tiles>>& mineArray, RenderWindow& gameWindow, int i, int j, Texture& revealedT, Sprite& revealedS, Sprite& one, Sprite& two, Sprite& three, Sprite& four, Sprite& five, Sprite& six, Sprite& seven, Sprite& eight){
    int row = mineArray.size();
    int col = mineArray[0].size();
    if(i < 0 || i >= row || j < 0 || j >= col || mineArray[i][j].state == State::Revealed){
        return;
    }
    mineArray[i][j].state = State::Revealed;
    revealedS.setPosition(revealedT.getSize().x * j, revealedT.getSize().y * i);
    gameWindow.draw(revealedS);

    Sprite* contentSprites[] = {&one, &two, &three, &four, &five, &six, &seven, &eight};
    if (mineArray[i][j].content >= Content::One && mineArray[i][j].content <= Content::Eight) {
        int index = (int)(mineArray[i][j].content) - (int)(Content::One);
        Sprite& contentSprite = *contentSprites[index];
        gameWindow.draw(contentSprite);
        contentSprite.setPosition(revealedT.getSize().x * j, revealedT.getSize().y * i);
        return;
    }

    if (mineArray[i][j].content == Content::Empty) {
        static const int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (auto& dir : directions) {
            RevealTiles(mineArray, gameWindow, i + dir[0], j + dir[1], revealedT, revealedS, one, two, three, four, five, six, seven, eight);
        }
    }

}

bool GameOver(vector<vector<Tiles>>& mineArray, RenderWindow& gameWindow, int i, int j, int row, int column, Texture& tileRevealed, Sprite& mine, Sprite& faceLoseS){
    bool gameLost = true;
    for (int x = 0; x < i; x++) {
        for (int y = 0; y <= j; y++) {
            if(mineArray[x][y].content == Content::Mine){
                gameWindow.draw(mine);
                mine.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
            }
        }
    }
    gameWindow.draw(faceLoseS);
    faceLoseS.setPosition(((column / 2) * 32) - 32, 32 * (row + 0.5f));

    return gameLost;
}

void Leaderboard(float width, float height, Font& font){
    RenderWindow leaderWindow(VideoMode((int)width, (int)height-100), "Minesweeper");
    ifstream file;
    file.open("files/leaderboard.txt");
    if(!file.is_open()){
        cerr << "Error opening file." << endl;
    }

    string line;
    map<int, string> myMap;
    int leaderPosition = 1;
    while(getline(file, line)){
        auto comma = line.find(',');
        string combine = line.substr(0, comma) + '\t' + line.substr(comma+2);
        myMap[leaderPosition] = combine;
        leaderPosition++;
    }
    file.close();

    Text leaderText;
    leaderText.setString("LEADERBOARD\n\n");
    leaderText.setFillColor(Color::White);
    leaderText.setFont(font);
    leaderText.setStyle(Text::Bold | Text::Underlined);
    leaderText.setCharacterSize(20);
    SetText(leaderText ,width/2, (height/2)-120);

    Text nameText;
    nameText.setFillColor(Color::White);
    nameText.setFont(font);
    nameText.setStyle(Text::Bold);
    nameText.setCharacterSize(18);

    string names;
    auto iter = myMap.begin();
    for(; iter != myMap.end(); iter++){
        names += (to_string(iter->first) + "." + '\t' + iter->second + '\n' + '\n');
    }
    nameText.setString(names);
    SetText(nameText ,width/2, (height/2)+20);
    while(leaderWindow.isOpen()){
        Event event{};
        while (leaderWindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
                leaderWindow.close();
        }

        leaderWindow.clear(Color::Blue);
        leaderWindow.draw(leaderText);
        leaderWindow.draw(nameText);

        leaderWindow.display();

    }
}

void Minesweeper(string& gameCol, string& gameRow, string& mineNum, Font& font){
    int column = stoi(gameCol);
    int row = stoi(gameRow);
    int mines = stoi(mineNum);
    float width = column * 32;
    float height = (row * 32) + 100;
    RenderWindow gameWindow(sf::VideoMode((int)width, (int)height), "Minesweeper");

    Texture tileHidden;
    TextureFile(tileHidden, "files/images/tile_hidden.png");
    Sprite tileHiddenS;
    tileHiddenS.setTexture(tileHidden);
    Texture tileRevealed;
    TextureFile(tileRevealed, "files/images/tile_revealed.png");
    Sprite tileRevealedS;
    tileRevealedS.setTexture(tileRevealed);
    Texture mineTexture;
    TextureFile(mineTexture, "files/images/mine.png");
    Sprite mineTextureS;
    mineTextureS.setTexture(mineTexture);
    Texture faceHappy;
    TextureFile(faceHappy, "files/images/face_happy.png");
    Sprite faceHappyS;
    faceHappyS.setTexture(faceHappy);
    Texture faceLose;
    TextureFile(faceLose, "files/images/face_lose.png");
    Sprite faceLoseS;
    faceLoseS.setTexture(faceLose);
    Texture faceWin;
    TextureFile(faceWin, "files/images/face_win.png");
    Sprite faceWinS;
    faceWinS.setTexture(faceWin);
    Texture playButton;
    TextureFile(playButton, "files/images/play.png");
    Sprite playButtonS;
    playButtonS.setTexture(playButton);
    Texture pauseButton;
    TextureFile(pauseButton, "files/images/pause.png");
    Sprite pauseButtonS;
    pauseButtonS.setTexture(pauseButton);
    Texture leaderboard;
    TextureFile(leaderboard, "files/images/leaderboard.png");
    Sprite leaderboardS;
    leaderboardS.setTexture(leaderboard);
    Texture flagTexture;
    TextureFile(flagTexture, "files/images/flag.png");
    Sprite flagTextureS;
    flagTextureS.setTexture(flagTexture);
    Texture debugTexture;
    TextureFile(debugTexture, "files/images/debug.png");
    Sprite debugTextureS;
    debugTextureS.setTexture(debugTexture);
    Texture one;
    TextureFile(one, "files/images/number_1.png");
    Sprite oneS;
    oneS.setTexture(one);
    Texture two;
    TextureFile(two, "files/images/number_2.png");
    Sprite twoS;
    twoS.setTexture(two);
    Texture three;
    TextureFile(three, "files/images/number_3.png");
    Sprite threeS;
    threeS.setTexture(three);
    Texture four;
    TextureFile(four, "files/images/number_4.png");
    Sprite fourS;
    fourS.setTexture(four);
    Texture five;
    TextureFile(five, "files/images/number_5.png");
    Sprite fiveS;
    fiveS.setTexture(five);
    Texture six;
    TextureFile(six, "files/images/number_6.png");
    Sprite sixS;
    sixS.setTexture(six);
    Texture seven;
    TextureFile(seven, "files/images/number_7.png");
    Sprite sevenS;
    sevenS.setTexture(seven);
    Texture eight;
    TextureFile(eight, "files/images/number_8.png");
    Sprite eightS;
    eightS.setTexture(eight);


    vector<vector<Tiles>> mineArray(row, vector<Tiles>(column, {State::Hidden, Content::Empty}));

    cout << "Mines: " << mines << endl;
    for (int i = 0; i < mines; ) {
        int randCol = rand() % column;
        int randRow = rand() % row;
        if (mineArray[randRow][randCol].content != Content::Mine) {
            mineArray[randRow][randCol].content = Content::Mine;
            i++;
        }
    }

    for (int x = 0; x < row; x++) {
        for (int y = 0; y < column; y++) {
            if(mineArray[x][y].content == Content::Empty){
                int count = 0;
                for(int i = -1; i <= 1; i++){
                    for(int j = -1; j <= 1; j++){
                        int tempX = i + x;
                        int tempY = j + y;
                        if(tempX >= 0 && tempX < row && tempY >= 0 && tempY < column){
                            if(mineArray[tempX][tempY].content == Content::Mine){
                                count++;
                            }
                        }
                    }
                }
                switch (count) {
                    case 1: mineArray[x][y].content = Content::One; break;
                    case 2: mineArray[x][y].content = Content::Two; break;
                    case 3: mineArray[x][y].content = Content::Three; break;
                    case 4: mineArray[x][y].content = Content::Four; break;
                    case 5: mineArray[x][y].content = Content::Five; break;
                    case 6: mineArray[x][y].content = Content::Six; break;
                    case 7: mineArray[x][y].content = Content::Seven; break;
                    case 8: mineArray[x][y].content = Content::Eight; break;
                }
                cout << count;
            }
            else{
                cout << "X";
            }
        }
        cout << endl;
    }

    pauseButtonS.setPosition((column * 32) - 240, 32 * (row + 0.5f));
    playButtonS.setPosition((column * 32) - 240, 32 * (row + 0.5f));
    bool screenPaused = false;
    bool debugMode = false;
    bool gameLost = false;
    bool gamePaused = false;
    int flagCount = mines;


    while(gameWindow.isOpen()){

        Event event{};
        RectangleShape rectangle;


        gameWindow.clear(Color::White);

        //Draws the tiles
        for (int x = 0; x < row; x++) {
            for (int y = 0; y <= column; y++) {
                tileHiddenS.setPosition(tileHidden.getSize().x * y, tileHidden.getSize().y * x);
                tileRevealedS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                flagTextureS.setPosition(flagTexture.getSize().x * y, flagTexture.getSize().y * x);



                if (mineArray[x][y].state == State::Hidden) {
                    gameWindow.draw(tileHiddenS);
                } else if (mineArray[x][y].state == State::Flagged) {
                    gameWindow.draw(tileHiddenS);
                    gameWindow.draw(flagTextureS);
                } else if (mineArray[x][y].state == State::Revealed) {
                    tileRevealedS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                    gameWindow.draw(tileRevealedS);

                    switch (mineArray[x][y].content) {
                        case Content::Empty:
                            RevealTiles(mineArray, gameWindow, x, y, tileRevealed, tileRevealedS, oneS, twoS, threeS, fourS, fiveS, sixS, sevenS, eightS);
                            break;
                        case Content::One:
                            gameWindow.draw(oneS);
                            oneS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                        case Content::Two:
                            gameWindow.draw(twoS);
                            twoS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                        case Content::Three:
                            gameWindow.draw(threeS);
                            threeS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                        case Content::Four:
                            gameWindow.draw(fourS);
                            fourS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                        case Content::Five:
                            gameWindow.draw(fiveS);
                            fiveS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                        case Content::Six:
                            gameWindow.draw(sixS);
                            sixS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                        case Content::Seven:
                            gameWindow.draw(sevenS);
                            sevenS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                        case Content::Eight:
                            gameWindow.draw(eightS);
                            eightS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                        case Content::Mine:
                            gameWindow.draw(mineTextureS);
                            mineTextureS.setPosition(tileRevealed.getSize().x * y, tileRevealed.getSize().y * x);
                            break;
                    }
                }
                if(debugMode || gameLost){
                    if(mineArray[x][y].content == Content::Mine){
                        mineArray[x][y].state = State::Revealed;
                    }
                }else{
                    if(mineArray[x][y].content == Content::Mine && mineArray[x][y].state != State::Flagged){
                        mineArray[x][y].state = State::Hidden;
                    }
                }
            }
        }


        while (gameWindow.pollEvent(event)) {
            if (event.type == Event::Closed)
                gameWindow.close();

            if(event.type == Event::MouseButtonPressed && !gameLost){
                Vector2f mouse = gameWindow.mapPixelToCoords((Mouse::getPosition(gameWindow)));
                int xValue = (int)mouse.y/32;
                int yValue = (int)mouse.x/32;
                if(event.mouseButton.button == Mouse::Left && mineArray[xValue][yValue].state != State::Flagged && xValue >= 0 && xValue < row && yValue >= 0 && yValue < column) {
                    if(mineArray[xValue][yValue].content == Content::Mine){
                        gameLost = GameOver(mineArray, gameWindow, xValue, yValue, row, column, tileRevealed, mineTextureS, faceLoseS);
                    }else {
                        RevealTiles(mineArray, gameWindow, xValue, yValue, tileRevealed, tileRevealedS, oneS, twoS,
                                    threeS, fourS, fiveS, sixS, sevenS, eightS);
                    }

                }
                if(event.mouseButton.button == Mouse::Right && mineArray[xValue][yValue].state != State::Revealed && xValue >= 0 && xValue < row && yValue >= 0 && yValue < column){
                    if(mineArray[xValue][yValue].state == State::Flagged) {
                        flagCount++;
                        mineArray[xValue][yValue].state = State::Hidden;
                    }
                    else {
                        flagCount--;
                        mineArray[xValue][yValue].state = State::Flagged;
                    }
                }
            }

            if(event.type == Event::MouseButtonReleased){
                if(event.mouseButton.button == Mouse::Left) {
                    //Play/Pause logic
                    Vector2f mouse = gameWindow.mapPixelToCoords(Mouse::getPosition(gameWindow));
                    FloatRect pauseButtonSBounds = pauseButtonS.getGlobalBounds();
                    if (pauseButtonSBounds.contains(mouse)) {
                        screenPaused = !screenPaused;
                    }

                    FloatRect leaderBounds = leaderboardS.getGlobalBounds();
                    if(leaderBounds.contains(mouse)) {
                        Leaderboard(width, height, font);
                    }

                    FloatRect debugBounds = debugTextureS.getGlobalBounds();
                    if(debugBounds.contains(mouse)){
                        debugMode = !debugMode;
                    }

                    FloatRect faceLoseBounds = faceLoseS.getGlobalBounds();
                    FloatRect faceWinBounds = faceHappyS.getGlobalBounds();
                    if(faceLoseBounds.contains(mouse) || faceWinBounds.contains(mouse)){
                        gameWindow.close();
                        Minesweeper(gameCol, gameRow, mineNum, font);
                    }

                }
            }
        }

        gameWindow.draw(tileHiddenS);

        //Need to create different face states
        faceLoseS.setPosition(((column / 2) * 32) - 32, 32 * (row + 0.5f));
        faceHappyS.setPosition(((column / 2) * 32) - 32, 32 * (row + 0.5f));
        if(!gameLost) {
            gameWindow.draw(faceHappyS);
        }else{
            gameWindow.draw(faceLoseS);
            faceLoseS.setPosition(((column / 2) * 32) - 32, 32 * (row + 0.5f));
        }
        gameWindow.draw(debugTextureS);
        debugTextureS.setPosition((column*32)-304, 32*(row+0.5f));

        FlagCounter(flagCount, gameWindow, row);
        //gameWindow.draw(digitZeroS);
        //digitZeroS.setPosition(33, 32 * (row + 0.5f) + 16);




//        if(!tileFlagged){
//            gameWindow.draw(tileHiddenS);
//        }
//        if(tileFlagged){
//            gameWindow.draw(tileHiddenS);
//            gameWindow.draw(flagTextureS);
//        }

        if(!screenPaused){
            gameWindow.draw(pauseButtonS);
        }else{
            gameWindow.draw(playButtonS);
        }

        gameWindow.draw(leaderboardS);
        leaderboardS.setPosition((column*32)-176, 32*(row+0.5f));


        gameWindow.display();
        sleep(seconds(1.0f/30));
    }
}

int main()
{
    //Load config file and count rows and cols
    ifstream file;
    file.open("files/config.cfg");
    string gameRow;
    string gameCol;
    string mineNum;
    if(file.is_open()){
        getline(file, gameCol);
        getline(file, gameRow);
        getline(file, mineNum);
        file.close();
    }

    //Set width and height of game screen
    float width = stof(gameCol) * 32;
    float height = (stof(gameRow) * 32) + 100;
    RenderWindow window(sf::VideoMode((int)width, (int)height), "Minesweeper");
    Font font;
    if(!font.loadFromFile("files/font.ttf")){
        cerr << "Font not loaded!" << endl;
        return 1;
    }

    Text welcomeText;

    //TextParameters(welcomeText, "WELCOME TO MINESWEEPER!\n\n", font, 24, Color::White, Text::Bold | Text::Underlined);
    welcomeText.setString("WELCOME TO MINESWEEPER!\n\n");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(Color::White);
    welcomeText.setStyle(Text::Bold | Text::Underlined);
    SetText(welcomeText ,width/2, (height/2)-150);

    Text nameText;
    nameText.setString("Enter your name:");
    nameText.setFont(font);
    nameText.setCharacterSize(20);
    nameText.setFillColor(Color::White);
    nameText.setStyle(Text::Bold);
    SetText(nameText ,width/2, (height/2)-75);
    //TextParameters(nameText, "Enter your name:", font, 20, Color::White, Text::Bold);

    String playerInput = "";
    Text playerText;
    playerText.setFont(font);
    playerText.setCharacterSize(18);
    playerText.setFillColor(Color::Yellow);
    playerText.setStyle(Text::Bold);
    SetText(playerText, width/2, (height/2)-45);

    int countText = 0;
    RectangleShape line(Vector2f(10,playerText.getCharacterSize()));
    line.setFillColor(Color::Yellow);




    while (window.isOpen())
    {
        SetText(playerText, width/2, (height/2)-45);

        line.setOrigin(0,0);
        line.setPosition(playerText.getPosition().x + playerText.getLocalBounds().width/2+5, playerText.getPosition().y-7);

        Event event{};
        while (window.pollEvent(event))
        {

            if (event.type == Event::Closed)
                window.close();

            if(event.type == Event::TextEntered){
                if(isalpha(event.text.unicode) && countText < 10){

                    if(countText <= 0)
                        playerInput += (char)toupper(event.text.unicode);
                    else
                        playerInput += (char)tolower(event.text.unicode);

                    countText++;
                }
                if(event.text.unicode == '\b' && countText > 0) {
                    playerInput.erase(playerInput.getSize() - 1);
                    countText--;
                }
                if(event.key.code == 13 && countText > 0){
                    window.close();
                    Minesweeper(gameCol, gameRow, mineNum, font);
                }
            }
        }

        window.clear(Color::Blue);

        window.draw(welcomeText);
        window.draw(nameText);
        playerText.setString(playerInput);
        window.draw(playerText);
        window.draw(line);

        window.display();
    }

    return 0;
}