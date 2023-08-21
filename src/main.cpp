#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>

const int G = 600;

const int RIGHT = 1;
const int LEFT = -1;
const int NONE = 0;

class Doodle {
private:
	sf::Texture t;
	sf::Sprite s; 
	float x, y = 0;
	float x_speed = 300;
	float y_speed = 0;
	float jump_force = 700;
	int max_y_speed = 500;

public:
	bool moving = false;
	int direction = NONE;

	Doodle() {
		t.loadFromFile("res/doodle.png");
		s.setTexture(t);
		x = 640.f / 2; y = 480.f / 2;
	}

	inline void update(float dt) {
		y_speed += G * dt;
		if (y_speed > max_y_speed) {
			y_speed = max_y_speed;
		}
		x += x_speed * direction * dt;
		y += y_speed * dt;
		s.setPosition(x, y);
	}

	inline void draw(sf::RenderWindow& window) {
		window.draw(s);
	}

	inline void move(int direction) {
		this->direction = direction;
	}

	inline void stop() {
		direction = NONE;
	}

	inline void jump() {
		y_speed = -jump_force;
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(400.f, 533.f), "Doodle jump", sf::Style::Titlebar);
	window.setFramerateLimit(60);

	sf::View view(sf::FloatRect(0.0f, 0.0f, 400.f, 533.f));
	window.setView(view);

	Doodle doodle;

	sf::Texture background_t;
	background_t.loadFromFile("res/background.png");

	sf::Sprite background_s;
	background_s.setTexture(background_t);
	
	sf::Clock clock;
	while (true) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
				return 0;
			} else if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::Key::Up) {
					doodle.jump();
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) doodle.move(RIGHT);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) doodle.move(LEFT);
		else doodle.move(NONE);

		float dt = clock.restart().asSeconds();

		doodle.update(dt);

		window.setView(view);
		window.clear(sf::Color().White);
		doodle.draw(window);
		window.draw(background_s);
		window.display();
	}

	return 1;
}

