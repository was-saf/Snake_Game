#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <fstream>
#define side 1
#define size 64

NODE Game::insertHead(NODE head, sf::Sprite* sprite) {
	NODE temp;
	temp = (NODE)malloc(sizeof(struct node));
	switch (head->direction) {
	case direction(up):
		temp->posy = head->posy - side * 2;
		temp->posx = head->posx;
		break;
	case direction(down):
		temp->posy = head->posy + side * 2;
		temp->posx = head->posx;
		break;
	case direction(left):
		temp->posx = head->posx - side * 2;
		temp->posy = head->posy;
		break;
	case direction(right):
		temp->posx = head->posx + side * 2;
		temp->posy = head->posy;
		break;
	}
	temp->direction = head->direction;
	temp->s = sprite;
	temp->previous = NULL;
	temp->next = head;
	head->previous = temp;
	return temp;

}

NODE Game::deleteTail(NODE tail) {
	NODE temp;
	temp = tail->previous;
	temp->next = NULL;
	free(tail);
	tail = NULL;
	return temp;
}

void Game::MainMenu() {
	LoadSprites();
	window.create(sf::VideoMode(800, 600), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
	GameTitle.setFont(Font);
	GameTitle.setString("Snake Game");
	GameTitle.setFillColor(sf::Color::Black);

	Prompt.setFont(Font);
	Prompt.setString("Press 'Enter' Key to Write your name and start playing\nHint : Use W, A, S, D to manuver the Snake\n\nPress 'L' for Leaderboard\nPress 'Esc' to quit the Game");
	Prompt.setFillColor(sf::Color::Black);

	float titleX = (window.getSize().x - GameTitle.getGlobalBounds().width) / 2;
	float promptX = (window.getSize().x - Prompt.getGlobalBounds().width) / 2;
	GameTitle.setPosition(titleX, 100);
	Prompt.setPosition(promptX, 300);

	while (window.isOpen()) {
		processMainMenuEvents();
		window.clear();
		window.draw(Background);
		window.draw(GameTitle);
		window.draw(Prompt);
		window.display();
	}
}

void Game::EnterPlayerName() {
	sf::Text NamePrompt;
	NamePrompt.setFont(Font);
	NamePrompt.setString("Enter your name:");
	NamePrompt.setFillColor(sf::Color::Black);
	NamePrompt.setPosition((window.getSize().x - NamePrompt.getGlobalBounds().width) / 2, 450);

	sf::Text PlayerName;
	PlayerName.setFont(Font);
	PlayerName.setFillColor(sf::Color::Black);
	PlayerName.setPosition((window.getSize().x - PlayerName.getGlobalBounds().width) / 2, 500);

	std::string playerNameInput;
	bool nameEntered = false;

	while (window.isOpen() && !nameEntered) {
		window.clear();
		window.draw(Background);
		window.draw(NamePrompt);
		window.draw(PlayerName);
		window.display();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode == '\b' && !playerNameInput.empty()) {
					// Handle backspace
					playerNameInput.pop_back();
				}
				else if (event.text.unicode < 128 && std::isalnum(event.text.unicode)) {
					// Handle alphanumeric characters
					playerNameInput += static_cast<char>(event.text.unicode);
				}
				else if (event.text.unicode == 13 && !playerNameInput.empty()) {
					// Handle Enter key
					playerName = playerNameInput;
					nameEntered = true;
				}
			}
		}

		PlayerName.setString(playerNameInput);
	}
}


void Game::processMainMenuEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Enter: {
				EnterPlayerName();
				Run();
				break;
			}
			case sf::Keyboard::L: {
				DisplayLeaderboardWindow();
				break;
			}
			case sf::Keyboard::Escape: {
				window.close();
				break;
			}
			}
		}
	}
}

void Game::DisplayLeaderboardWindow() {
	window.close();

	sf::RenderWindow leaderboardWindow(sf::VideoMode(800, 600), "Leaderboard", sf::Style::Titlebar | sf::Style::Close);

	sf::Sprite leaderboardBackground(Background);
	sf::Font leaderboardFont(Font);

	sf::Text leaderboardTitle;
	leaderboardTitle.setFont(leaderboardFont);
	leaderboardTitle.setString("Leaderboard");
	leaderboardTitle.setFillColor(sf::Color::Black);
	leaderboardTitle.setCharacterSize(36); // Larger font size
	leaderboardTitle.setPosition((leaderboardWindow.getSize().x - leaderboardTitle.getGlobalBounds().width) / 2, 20);

	const int maxScores = 10;
	std::string playerNames[maxScores];
	int scores[maxScores];
	sf::Text nameTexts[maxScores];
	sf::Text scoreTexts[maxScores];

	std::ifstream leaderboard("leaderboard.txt");
	if (leaderboard.is_open()) {

		sf::Text header;
		header.setFont(leaderboardFont);
		header.setFillColor(sf::Color::Black);
		header.setCharacterSize(18);
		header.setString("   Player Name\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tScore");
		header.setPosition(20, 80);
		leaderboardWindow.draw(header);

		for (int i = 0; i < maxScores && leaderboard >> playerNames[i] >> scores[i]; i++) {
			nameTexts[i].setFont(leaderboardFont);
			nameTexts[i].setFillColor(sf::Color::Black);
			nameTexts[i].setCharacterSize(18);
			nameTexts[i].setString("   " + playerNames[i]);
			nameTexts[i].setPosition(20, 110 + i * 30);
			leaderboardWindow.draw(nameTexts[i]);

			scoreTexts[i].setFont(leaderboardFont);
			scoreTexts[i].setFillColor(sf::Color::Black);
			scoreTexts[i].setCharacterSize(18);
			scoreTexts[i].setString("\t\t\t\t\t\t\t\t" + std::to_string(scores[i]));
			scoreTexts[i].setPosition(400, 110 + i * 30);
			leaderboardWindow.draw(scoreTexts[i]);
		}

		leaderboard.close();

		sf::Text returnPrompt;
		returnPrompt.setFont(leaderboardFont);
		returnPrompt.setString("Press Escape to return to the main menu");
		returnPrompt.setFillColor(sf::Color::Black);
		returnPrompt.setCharacterSize(16);
		returnPrompt.setPosition((leaderboardWindow.getSize().x - returnPrompt.getGlobalBounds().width) / 2, 500);

		while (leaderboardWindow.isOpen()) {
			sf::Event event;
			while (leaderboardWindow.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					leaderboardWindow.close();
				}
				else if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Escape) {
						leaderboardWindow.close();
						MainMenu();
					}
				}
			}

			leaderboardWindow.clear();
			leaderboardWindow.draw(leaderboardBackground);
			leaderboardWindow.draw(leaderboardTitle);

			leaderboardWindow.draw(header);

			for (int i = 0; i < maxScores; i++) {
				leaderboardWindow.draw(nameTexts[i]);
				leaderboardWindow.draw(scoreTexts[i]);
			}

			leaderboardWindow.draw(returnPrompt);
			leaderboardWindow.display();
		}
	}
	else {
		std::cerr << "Error: Unable to open leaderboard file for reading." << std::endl;
	}
}


void Game::SavePlayerScore() {
	const std::string leaderboardFileName = "leaderboard.txt";
	const int maxScores = 11;

	std::string playerNames[maxScores];
	int scores[maxScores];

	std::ifstream leaderboard(leaderboardFileName);
	if (leaderboard.is_open()) {
		int i = 0;
		while (i < maxScores && leaderboard >> playerNames[i] >> scores[i]) {
			i++;
		}
		leaderboard.close();
	}
	else {
		std::cerr << "Error: Unable to open leaderboard file for reading." << std::endl;
		return;
	}

	playerNames[maxScores - 1] = playerName;
	scores[maxScores - 1] = score;

	for (int i = 0; i < maxScores - 1; i++) {
		for (int j = 0; j < maxScores - i - 1; j++) {
			if (scores[j] < scores[j + 1]) {
				std::swap(scores[j], scores[j + 1]);
				std::swap(playerNames[j], playerNames[j + 1]);
			}
		}
	}

	std::ofstream leaderboardOut(leaderboardFileName);
	if (leaderboardOut.is_open()) {
		for (int i = 0; i < maxScores - 1; i++) {
			leaderboardOut << playerNames[i] << " " << scores[i] << "\n";
		}
		leaderboardOut.close();
	}
	else {
		std::cerr << "Error: Unable to open leaderboard file for writing." << std::endl;
	}
}

void Game::ShowPlayerScore() {
	window.close();

	sf::RenderWindow ShowPlayerScoreWindow(sf::VideoMode(800, 600), "Game Over", sf::Style::Titlebar | sf::Style::Close);

	sf::Sprite ShowPlayerScoreBackground(Background);
	sf::Font ShowPlayerScoreFont(Font);

	sf::Text ShowPlayerScoreText;
	ShowPlayerScoreText.setFont(ShowPlayerScoreFont);
	ShowPlayerScoreText.setString("Congratulations " + playerName + " , Your Score is : " + std::to_string(score));
	ShowPlayerScoreText.setFillColor(sf::Color::Black);
	ShowPlayerScoreText.setCharacterSize(24);
	ShowPlayerScoreText.setPosition((ShowPlayerScoreWindow.getSize().x - ShowPlayerScoreText.getGlobalBounds().width) / 2, 300);
	sf::Text returnPrompt;
	returnPrompt.setFont(ShowPlayerScoreFont);
	returnPrompt.setString("Press Escape to return to the main menu");
	returnPrompt.setFillColor(sf::Color::Black);
	returnPrompt.setCharacterSize(16);
	returnPrompt.setPosition((ShowPlayerScoreWindow.getSize().x - returnPrompt.getGlobalBounds().width) / 2, 500);
	while (ShowPlayerScoreWindow.isOpen()) {
		sf::Event event;
		while (ShowPlayerScoreWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				ShowPlayerScoreWindow.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					ShowPlayerScoreWindow.close();
					MainMenu();
				}
			}
		}

		ShowPlayerScoreWindow.clear();
		ShowPlayerScoreWindow.draw(ShowPlayerScoreBackground);
		ShowPlayerScoreWindow.draw(ShowPlayerScoreText);
		ShowPlayerScoreWindow.draw(returnPrompt);
		setHeadSprite(head, &SnakeHeadSprite);
		cur = head;
		while (cur != NULL) {
			if (cur != head->next) {
				setTrailingSprite(cur, &SnakeBodySprite);
			}
			(*cur->s).setPosition(cur->posx * 16, cur->posy * 16);
			ShowPlayerScoreWindow.draw(*cur->s);
			cur = cur->next;
		}
		ShowPlayerScoreWindow.display();

	}

}


void Game::Update() {
	handleFoodEaten();
	head = insertHead(head, &SnakeHeadSprite);
	tail = deleteTail(tail);
	checkIntersection(head, &window);
	checkBoundary(head, &window);
	count = 0;
}

void Game::Start() {
	LoadSprites();
	head = (NODE)malloc(sizeof(struct node));
	head->posx = 0;
	head->posy = 0;
	head->direction = right;
	head->s = &SnakeHeadSprite;
	head->previous = head->next = NULL;
	tail = head;
	FoodSprite.setPosition(head->posx, head->posy);
	Score.setPosition(700, 10);
	Score.setString(sf::String(std::to_string(score + 1)));

	window.create(sf::VideoMode(800, 600), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
}


void Game::Run() {
	score = 0;
	Start();
	while (window.isOpen()) {
		Render();
		processEvents();
		if (count == speed) {
			Update();
		}
		count++;
	}
	if (!window.isOpen()) {
		SavePlayerScore();
		ShowPlayerScore();
	}
}

void Game::Render() {
	window.clear();
	window.draw(Background);
	window.draw(Score);
	setHeadSprite(head, &SnakeHeadSprite);
	cur = head;
	while (cur != NULL) {
		if (cur != head) {
			setTrailingSprite(cur, &SnakeBodySprite);
		}
		(*cur->s).setPosition(cur->posx * 16, cur->posy * 16);
		window.draw(*cur->s);
		cur = cur->next;
	}
	window.draw(FoodSprite);
	window.display();
}

void Game::LoadSprites() {
	this->HeadTexture.loadFromFile("snake_green_head_64.png");
	SnakeHeadSprite.setTexture(HeadTexture);
	SnakeHeadSprite.setScale(sf::Vector2f(0.5, 0.5));
	SnakeHeadSprite.setOrigin(0, 0);

	this->BodyTexture.loadFromFile("snake_green_blob_64.png");
	SnakeBodySprite.setTexture(BodyTexture);
	SnakeBodySprite.setScale(sf::Vector2f(0.5, 0.5));
	SnakeBodySprite.setOrigin(0, 0);

	this->FoodTexture.loadFromFile("apple_alt_64.png");
	FoodSprite.setTexture(FoodTexture);
	FoodSprite.setScale(sf::Vector2f(0.5, 0.5));
	FoodSprite.setOrigin(0, 0);

	this->BackgroundTexture.loadFromFile("sand.jpg");
	Background.setTexture(BackgroundTexture);
	Background.setScale(2, 2);

	Font.loadFromFile("Roboto-Bold.ttf");
	Score.setFont(Font);
	Score.setFillColor(sf::Color::Black);
}

void Game::setHeadSprite(NODE node, sf::Sprite* sprite) {
	sprite->setTextureRect(sf::IntRect(size * 0, size * 0, size, size));
	head->s = sprite;
}

void Game::setTrailingSprite(NODE node, sf::Sprite* sprite) {
	sprite->setTextureRect(sf::IntRect(size * 0, size * 0, size, size));
	node->s = sprite;
}

void Game::processEvents() {
	if (window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::W && head->direction != down) {
				head->direction = up;
			}
			else if (event.key.code == sf::Keyboard::S && head->direction != up) {
				head->direction = down;
			}
			else if (event.key.code == sf::Keyboard::A && head->direction != right) {
				head->direction = left;
			}
			else if (event.key.code == sf::Keyboard::D && head->direction != left) {
				head->direction = right;
			}
		}
	}
}

sf::Vector2f Game::getFoodCoords() {
	return sf::Vector2f((rand() % 300 + 1) * side * 2, (rand() % 250 + 1) * side * 2);
}

void Game::handleFoodEaten() {
	if (isFoodEaten(head, &FoodSprite)) {
		head = insertHead(head, &SnakeHeadSprite);
		speed++;
		while (1) {
			foodPos = getFoodCoords();
			cur = head;
			while (cur != NULL) {
				if (cur->posx == foodPos.x && cur->posy == foodPos.y) {
					break;
				}
				cur = cur->next;
			}
			if (cur == NULL) {
				break;
			}
		}
		FoodSprite.setPosition(foodPos);
		score++;
		Score.setString(sf::String(std::to_string(score)));
	}
}

bool Game::isFoodEaten(NODE head, sf::Sprite* FoodSprite) {
	sf::FloatRect headBounds = head->s->getGlobalBounds();
	sf::FloatRect foodBounds = FoodSprite->getGlobalBounds();
	return headBounds.intersects(foodBounds);
}

void Game::checkIntersection(NODE head, sf::RenderWindow* window) {
	NODE cur = head->next;
	while (cur != NULL) {
		if (head->posx == cur->posx && head->posy == cur->posy) {
			window->close();
		}
		cur = cur->next;
	}
}

void Game::checkBoundary(NODE head, sf::RenderWindow* window) {

	int minX = 0;
	int maxX = window->getSize().x / 16;
	int minY = 0;
	int maxY = window->getSize().y / 16;

	if (head->posx < minX || head->posx >= maxX || head->posy < minY || head->posy >= maxY) {
		window->close();
	}
}