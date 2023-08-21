#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

const int G = 100;

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
	float jump_force = 500;
	int max_y_speed = 500;

public:
	bool jumping = false;
	bool moving = false;
	int direction = NONE;

	Doodle() {
		t.loadFromFile("res/doodle.png");
		s.setTexture(t);
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
		jumping = true;
		y_speed = -jump_force;
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(640.f, 480.f), "Doodle jump", sf::Style::Resize);
	window.setFramerateLimit(60);

	sf::View view(sf::FloatRect(0.0f, 0.0f, 640.f, 480.f));
	window.setView(view);

	Doodle doodle;

	sf::Clock clock;
	while (true) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
				return 0;
			} else if (e.type == sf::Event::Resized) {
				view.setSize(e.size.width, e.size.height);
			} else if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::Key::Up && !doodle.jumping) {
					doodle.jump();
				}
			} else if (e.type == sf::Event::KeyReleased) {
				if (e.key.code == sf::Keyboard::Key::Up) {
					doodle.jumping = false;
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) doodle.move(RIGHT);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) doodle.move(LEFT);
		else doodle.move(NONE);

		float dt = clock.restart().asSeconds();

		doodle.update(dt);

/*		if (doodle.moving) {
			doodle.move(8.f*doodle.direction);
		}*/

		window.setView(view);
		window.clear(sf::Color().White);
		doodle.draw(window);
		window.display();
	}

	return 1;
}

