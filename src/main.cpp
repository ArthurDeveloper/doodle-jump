#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <iostream>

#define G 0.2

class Doodle {
private:
	sf::Texture t;
	sf::Sprite s; 
	float x, y = 0;
	float y_acceleration = 0;
	int max_y_speed = 9;

public:
	bool jumping = false;
	bool moving = false;
	int direction = 1;

	Doodle() {
		t.loadFromFile("res/doodle.png");
		s.setTexture(t);
		s.setScale(2.f, 2.f);
		//rect.setSize(sf::Vector2f(32.f, 32.f));
		//rect.setFillColor(sf::Color().Black);
	}

	inline void update() {
		if (y_acceleration < max_y_speed) y_acceleration += G;
		y -= y_acceleration;
		s.setOrigin(x, y);
	}

	inline void draw(sf::RenderWindow& window) {
		window.draw(s);
	}

	inline void move(float distance) {
		x += distance;
	}

	inline void jump() {
		jumping = true;
		y_acceleration = -12;
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(640.f, 480.f), "Doodle jump", sf::Style::Resize);
	window.setFramerateLimit(60);

	sf::View view(sf::FloatRect(0.0f, 0.0f, 640.f, 480.f));
	window.setView(view);

#ifdef SFML_SYSTEM_LINUX
	// Helping my fellow i3 users ;)
	system("i3-msg '[class=\"Doodle jump\"] floating enable' &>/dev/null");
#endif

	Doodle doodle;

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
					doodle.direction = -1;
				} 

				if (e.key.code == sf::Keyboard::Key::Left) {
					doodle.moving = true;
					doodle.direction = 1;
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

		doodle.update();

		if (doodle.moving) {
			doodle.move(8.f*doodle.direction);
		}

		window.setView(view);
		window.clear(sf::Color().White);
		doodle.draw(window);
		window.display();
	}

	return 1;
}

