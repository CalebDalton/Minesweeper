//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/System.hpp>
//#include <iostream>
////#include "WelcomeScreen.h"
//
//using namespace std;
//using namespace sf;
//
//void SetText(Text& text, float x, float y){
//    FloatRect textRect = text.getLocalBounds();
//    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
//    text.setPosition(Vector2f(x,y));
//}
//
//void TextParameters(Text& text, String tString, Font font, int characterSize, Color color, Uint32 tStyle){
//    text.setString(tString);
//    text.setFont(font);
//    text.setCharacterSize(characterSize);
//    text.setFillColor(color);
//    text.setStyle(tStyle);
//}
//
//int WelcomeScreen()
//{
//    int rowCount;
//    int rowHeight;
//    float width = 800;
//    float height = 600;
//    sf::RenderWindow window(sf::VideoMode((int)width, (int)height), "Minesweeper");
//    Font font;
//    if(!font.loadFromFile("files/font.ttf")){
//        cerr << "Font not loaded!" << endl;
//        return 1;
//    }
//
//    Text welcomeText;
//
//    //TextParameters(welcomeText, "WELCOME TO MINESWEEPER!\n\n", font, 24, Color::White, Text::Bold | Text::Underlined);
//    welcomeText.setString("WELCOME TO MINESWEEPER!\n\n");
//    welcomeText.setFont(font);
//    welcomeText.setCharacterSize(24);
//    welcomeText.setFillColor(Color::White);
//    welcomeText.setStyle(Text::Bold | Text::Underlined);
//    SetText(welcomeText ,width/2, (height/2)-150);
//
//    Text nameText;
//    nameText.setString("Enter your name:");
//    nameText.setFont(font);
//    nameText.setCharacterSize(20);
//    nameText.setFillColor(Color::White);
//    nameText.setStyle(Text::Bold);
//    SetText(nameText ,width/2, (height/2)-75);
//    //TextParameters(nameText, "Enter your name:", font, 20, Color::White, Text::Bold);
//
//    String playerInput = "";
//    Text playerText;
//    playerText.setFont(font);
//    playerText.setCharacterSize(18);
//    playerText.setFillColor(Color::Yellow);
//    //playerText.setStyle(Text::Bold | Text::Underlined);
//    //SetText(playerText, width/2, (height/2)-45);
//
//    int countText = 0;
//    RectangleShape line(Vector2f(10,playerText.getCharacterSize()));
//    line.setFillColor(Color::Yellow);
//
//
//
//    bool windowFocus = window.hasFocus(); //changes screen focus to leaderboard if clicked
//
//    while (window.isOpen())
//    {
//        SetText(playerText, width/2, (height/2)-45);
//
//        line.setOrigin(Vector2f(playerText.getLocalBounds().width + playerText.getPosition().x, playerText.getLocalBounds().height));
//        line.setPosition(playerText.getLocalBounds().width + playerText.getPosition().x, playerText.getPosition().y);
//
//        Event event{};
//        while (window.pollEvent(event))
//        {
//
//            if (event.type == Event::Closed)
//                window.close();
//
//            if(event.type == Event::TextEntered){
//                cout << playerText.getPosition().x << ", " << playerText.getPosition().x << ", " << playerText.getPosition().y << endl;
//                cout << line.getPosition().x << ", " << line.getPosition().x << ", " << line.getPosition().y << endl;
//                if(isalpha(event.text.unicode) && countText < 10){
//
//                    if(countText <= 0)
//                        playerInput += (char)toupper(event.text.unicode);
//                    else
//                        playerInput += (char)tolower(event.text.unicode);
//
//                    countText++;
//                }
//                if(event.text.unicode == '\b' && countText > 0) {
//                    playerInput.erase(playerInput.getSize() - 1);
//                    countText--;
//                }
//            }
//        }
//
//        window.clear(Color::Blue);
//
//        window.draw(welcomeText);
//        window.draw(nameText);
//        playerText.setString(playerInput);
//        window.draw(playerText);
//        window.draw(line);
//
//        window.display();
//    }
//
//    return 0;
//}