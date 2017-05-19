//
//  level.h
//  Mario
//


#ifndef level_h
#define level_h

#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-system.lib")

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "tinyxml.h"
#include <iostream>


struct Object
{
    
    std::string name;
    std::string type;
    sf::Rect<int> rect;
    std::map<std::string, std::string> properties;
    bool alive;
    bool bonus;
    sf::Sprite sprite;
};


sf::FloatRect getObjRect(Object a)
{
    return (sf::FloatRect)a.rect;
}

struct Layer
{
    int opacity;
    std::vector<sf::Sprite> tiles;
};

class Level
{
public:
    bool LoadFromFile(std::string filename);
    Object GetObject(std::string name);
    std::vector<Object> GetAllObjects();
    std::vector<Object> GetObjects(std::string name);
    void Draw(sf::RenderWindow &window);
    sf::Vector2i GetTileSize();
    int GetWidth();
    int GetHeight();
    void  SetObjects(std::vector<Object> obj);

    
private:
    int width, height, tileWidth, tileHeight, space;
    int firstTileID;
    sf::Rect<float> drawingBounds;
    sf::Texture tilesetImage;
    std::vector<Object> objects;
    std::vector<Layer> layers;
};

void  Level::SetObjects(std::vector<Object> obj)
{
    objects = obj;
}



bool Level::LoadFromFile(std::string filename)
{
    space = 0;
    TiXmlDocument levelFile(filename.c_str());
    
    // Загружаем XML-карту
    if(!levelFile.LoadFile())
    {
        std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
        return false;
    }
    
    TiXmlElement *map;
    map = levelFile.FirstChildElement("map");
    
    width = atoi(map->Attribute("width"));
    height = atoi(map->Attribute("height"));
    tileWidth = atoi(map->Attribute("tilewidth"));
    tileHeight = atoi(map->Attribute("tileheight"));
    
    // Берем описание тайлсета и идентификатор первого тайла
    TiXmlElement *tilesetElement;
    tilesetElement = map->FirstChildElement("tileset");
    firstTileID = atoi(tilesetElement->Attribute("firstgid"));
    space = atoi(tilesetElement->Attribute("spacing"));

    TiXmlElement *image;                                                        //HFSFSAFSAFSAFASFSA
    image = tilesetElement->FirstChildElement("image");
 
    std::string imagepath =  resourcePath() + "SMB-Tiles.gif";
    
    // Пытаемся загрузить тайлсет
    sf::Image img;
    
    if(!img.loadFromFile(imagepath))
    {
        std::cout << "Failed to load tile sheet." << std::endl;
        return false;
    }
    
    tilesetImage.loadFromImage(img);
    tilesetImage.setSmooth(false);
    
    // Получаем количество столбцов и строк тайлсета
    int columns = tilesetImage.getSize().x / tileWidth;
    int rows = tilesetImage.getSize().y / tileHeight;
    
    std::vector<sf::Rect<int>> subRects;
    
    for(int y = 0; y < rows - 1; y++)
        for(int x = 0; x < columns - 1; x++)
        {
            sf::Rect<int> rect;
            rect.top = y * tileHeight + y * space;
            rect.height = tileHeight;
            rect.left = x * tileWidth + x * space;
            rect.width = tileWidth;
            subRects.push_back(rect);
        }
    
    TiXmlElement *layerElement;
    layerElement = map->FirstChildElement("layer");
    while(layerElement)
    {
        Layer layer;
        
        
        if (layerElement->Attribute("opacity") != NULL)
        {
            float opacity = strtod(layerElement->Attribute("opacity"), NULL);
            layer.opacity = 255 * opacity;
        }
        else
        {
            layer.opacity = 255;
        }
        
        TiXmlElement *layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data");
        
        if(layerDataElement == NULL)
        {
            std::cout << "Bad map. No layer information found." << std::endl;
        }
        
        
        TiXmlElement *tileElement;
        tileElement = layerDataElement->FirstChildElement("tile");
        
        if(tileElement == NULL)
        {
            std::cout << "Bad map. No tile information found." << std::endl;
            return false;
        }
        
        int x = 0;
        int y = 0;
        
        while(tileElement)
        {
            int tileGID = atoi(tileElement->Attribute("gid"));
            int subRectToUse = tileGID - firstTileID;
        
            if (subRectToUse >= 0)
            {
                sf::Sprite sprite;
                sprite.setTexture(tilesetImage);
                sprite.setTextureRect(subRects[subRectToUse]);
                sprite.setPosition(x * tileWidth, y * tileHeight);
                sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
                
                layer.tiles.push_back(sprite);
            }
            
            tileElement = tileElement->NextSiblingElement("tile");
            
            x++;
            if (x >= width)
            {
                x = 0;
                y++;
                if(y >= height)
                    y = 0;
            }
        }
        
        layers.push_back(layer);
        layerElement = layerElement->NextSiblingElement("layer");
    }
    
    TiXmlElement *objectGroupElement;

    if (map->FirstChildElement("objectgroup") != NULL)
    {
        objectGroupElement = map->FirstChildElement("objectgroup");
        while (objectGroupElement)
        {
            TiXmlElement *objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");
            
            while(objectElement)
            {
                // Получаем все данны
                std::string objectType;
                if (objectElement->Attribute("type") != NULL)
                {
                    objectType = objectElement->Attribute("type");
                }
                std::string objectName;
                if (objectElement->Attribute("name") != NULL)
                {
                    objectName = objectElement->Attribute("name");
                }
                int x = atoi(objectElement->Attribute("x"));
                int y = atoi(objectElement->Attribute("y"));
                
                int width, height;
                
                sf::Sprite sprite;
                sprite.setTexture(tilesetImage);
                sprite.setTextureRect(sf::Rect<int>(0,0,0,0));
                sprite.setPosition(x, y);
                
                if (objectElement->Attribute("width") != NULL)
                {
                    width = atoi(objectElement->Attribute("width"));
                    height = atoi(objectElement->Attribute("height"));
                }

                Object object;
                object.name = objectName;
                object.type = objectType;
                object.sprite = sprite;
                object.alive = 1;
                if(std::strcmp(object.name.data(), "bonus") == 0)
                    object.bonus = 1;
                else
                    object.bonus = 0;
                sf::Rect <int> objectRect;
                objectRect.top = y;
                objectRect.left = x;
                objectRect.height = height;
                objectRect.width = width;
                object.rect = objectRect;
                
                
                objects.push_back(object);
                objectElement = objectElement->NextSiblingElement("object");
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }
    else
    {
        std::cout << "No object layers found..." << std::endl;
    }
    
    return true;
}


Object Level::GetObject(std::string name)
{
    // Только первый объект с заданным именем
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            return objects[i];
}

std::vector<Object> Level::GetObjects(std::string name)
{
    // Все объекты с заданным именем
    std::vector<Object> vec;
    for(int i = 0; i < objects.size(); i++)
        if(objects[i].name == name)
            vec.push_back(objects[i]);
    
    return vec;
}

std::vector<Object> Level::GetAllObjects()
{
    return objects;
}

sf::Vector2i Level::GetTileSize()
{
    return sf::Vector2i(tileWidth, tileHeight);
}

void Level::Draw(sf::RenderWindow &window)
{
    // Рисуем все тайлы
    for(int layer = 0; layer < layers.size(); layer++)
        for(int tile = 0; tile < layers[layer].tiles.size(); tile++)
            window.draw(layers[layer].tiles[tile]);
    
    sf::Texture tileSet;
    tileSet.loadFromFile(resourcePath() + "Mario_tileset.png");
    sf::Sprite tile(tileSet);
    tile.setTextureRect(sf::IntRect(128 ,113 ,15 ,15));
    
    sf::Texture tileForCoin;
    tileForCoin.loadFromFile(resourcePath() + "18133.png");
    sf::Sprite coin(tileForCoin);
    coin.setTextureRect(sf::IntRect(525, 535, 15, 15));
    

    for(int i = 0; i < objects.size(); i++)
    {
        if(objects[i].name == "bonus")
        {
            tile.setPosition(objects[i].rect.left, objects[i].rect.top );
            window.draw(tile);
        }
        if(objects[i].name == "coin")
        {
            coin.setPosition(objects[i].rect.left, objects[i].rect.top);
            window.draw(coin);

        }
        if(objects[i].name == "destroy")
        {
            sf::RectangleShape rec(sf::Vector2f(15, 15));
            rec.setPosition(objects[i].rect.left, objects[i].rect.top );
            rec.setFillColor(sf::Color(92,148,252));
            
            window.draw(rec);
        }
    }
}

#endif /* level_h */
