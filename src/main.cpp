#include <SFML/Graphics.hpp>
#include <iostream>

const int G = 100;

class Doodle {
private:
	sf::Texture t;
	sf::Sprite s; 
	float x, y = 0;
	float x_speed = 150;
	float y_speed = 0;
	float jump_force = 500;
	int max_y_speed = 500;

public:
	bool jumping = false;
	bool moving = false;
	int direction = 1;

	Doodle() {
		t.loadFromFile("res/doodle.png");
		s.setTexture(t);
	}

	inline void update(float dt) {
		y_speed += G * dt;
		if (y_speed > max_y_speed) {
			y_speed = max_y_speed;
		}
		x += x_speed * dt;
		y += y_speed * dt;
		s.setPosition(x, y);
	}

	inline void draw(sf::RenderWindow& window) {
		window.draw(s);
	}

	inline void move(float distance) {
		x += distance;
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

				if (e.key.code == sf::Keyboard::Key::Right) {
					doodle.moving = true;
					doodle.direction = 1;
				} 

				if (e.key.code == sf::Keyboard::Key::Left) {
					doodle.moving = true;
					doodle.direction = -1;
				}
			} else if (e.type == sf::Event::KeyReleased) {
				if (e.key.code == sf::Keyboard::Key::Up) {
					doodle.jumping = false;
				}
				if (e.key.code == sf::Keyboard::Key::Right && doodle.direction == -1
					|| e.key.code == sf::Keyboard::Key::Left && doodle.direction == 1) {
					doodle.moving = false;
				}
			}
		}

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

