#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct node{
	int posx, posy;
	int direction;
	sf::Sprite* s;
	struct node* previous;
	struct node* next;
};

typedef struct node* NODE;

enum direction {
	up, down, left, right
};

class Game {
	private:NODE head, tail, cur;

		   int speed = 1500;
		   int count;
		   int score = -1;
		   bool MainMenuPlayButtonSelected = true;
		   bool MainMenuPlayButtonPressed= false;

		   sf::Vector2f foodPos;
	private:sf::Sprite SnakeHeadSprite;
		   sf::Sprite SnakeBodySprite;
		   sf::Sprite FoodSprite;
		   sf::Sprite Background;

		   sf::RenderWindow window;

		   sf::Texture HeadTexture;
		   sf::Texture BodyTexture;
		   sf::Texture FoodTexture;
		   sf::Texture BackgroundTexture;

		   sf::Event event;

		   sf::Text Score;
		   sf::Text GameTitle;
		   sf::Text Prompt;

		   sf::Font Font;

	private:void Update();
		    void Render();
			void Start();
			void LoadSprites();
			NODE insertHead(NODE head, sf::Sprite* sprite);
			NODE deleteTail(NODE tail);
			void processEvents();
	private:void setTrailingSprite(NODE node, sf::Sprite* sprite);
			void setHeadSprite(NODE node, sf::Sprite* sprite);
			sf::Vector2f getFoodCoords();
			void handleFoodEaten();
			bool isFoodEaten(NODE head, sf::Sprite* FoodSprite);
			void checkIntersection(NODE head, sf::RenderWindow* window);
			void checkBoundary(NODE head, sf::RenderWindow* window);
	public:	void Run();
		    void MainMenu();
			void EnterPlayerName();
			void processMainMenuEvents();
			void DisplayLeaderboardWindow();
			void SavePlayerScore();
			void ShowPlayerScore();
			std::string playerName;
};