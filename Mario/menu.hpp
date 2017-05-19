//
//  menu.hpp
//  Mario
//


#ifndef menu_h
#define menu_h


    
    int start(sf::RenderWindow &window, int kind)
    {
        sf::Texture menuBackground1;
        sf::Sprite menu;
        sf::Font font;
        sf::Text newGame, exit;
        
        menuBackground1.loadFromFile(resourcePath() + "menu.gif");
       
        if ( !font.loadFromFile(resourcePath() + "sansation.ttf") )
        {
            return EXIT_FAILURE ;
        }
        
        newGame.setString("New Game");
        newGame.setFont(font);
        newGame.setCharacterSize(60);
        
        exit.setString("Exit");
        exit.setFont(font);
        exit.setCharacterSize(60);
        
        int sucsess;
        
        if(kind == 1)
        {
            newGame.setPosition(210, 315);
            exit.setPosition(95 * 3, 135 * 3);
            
            menu.setTexture(menuBackground1);
            menu.setPosition(0, 0);
            menu.setScale(3, 3);
           
            
            sf::IntRect newGameRec(60 * 3, 110 * 3, 120 * 3, 20 * 3);
            sf::IntRect exitGameRec(60 * 3, 140 * 3, 120 * 3, 20 * 3);
            bool isMenu = 1;
            while (isMenu)
            {
             
                exit.setFillColor(sf::Color::White);
                newGame.setFillColor(sf::Color::White);
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        window.close();
                        return -1;
                        isMenu = false;
                    }
                }

                window.clear();
                
                if(newGameRec.contains(sf::Mouse::getPosition(window)))
                {
                    newGame.setFillColor(sf::Color::Green);
                    
                }
                
                if(exitGameRec.contains(sf::Mouse::getPosition(window)))
                {
                    exit.setFillColor(sf::Color::Red);
    
                }
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && newGameRec.contains(sf::Mouse::getPosition(window)))
                {
                    newGame.setFillColor(sf::Color::Green);
                    isMenu = false;
                    sucsess = 1;
                    
                }
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && exitGameRec.contains(sf::Mouse::getPosition(window)))
                {
                     exit.setFillColor(sf::Color::Red);
                    isMenu = false;
                    sucsess = -1;
                    
                }

                
                window.draw(menu);
                //window.draw(Game);
                //window.draw(Exit);
                
                window.draw(newGame);
                
                window.draw(exit);
                window.display();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    sucsess = -1;
                    window.close();
                    break;
                }
            }
            
        }
        
        return  sucsess;
}

#endif /* menu_h */
