#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdlib>
#include <iostream>

const int G = 900;

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
	float jump_force = 900;
	int max_y_speed = 500;

public:
	int direction = NONE;

	Doodle() {
		t.loadFromFile("res/doodle.png");
		s.setTexture(t);
		x = 400.f / 2; y = 533.f / 2;
	}

	inline void reset() {
		x = 400.f / 2; y = 533.f / 2;
		y_speed = 0;
		s.setPosition(x, y);
	}

	inline bool is_falling() {
		return y_speed > 0;
	}

	inline sf::Vector2f position() {
		return {x, y};
	}

	inline sf::FloatRect bounds() {
		return s.getGlobalBounds();
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

	inline void clamp(int x1, int x2) {
		if (x + s.getGlobalBounds().width < x1) {
			x = x2;
		}

		if (x > x2) {
			x = x1 - s.getGlobalBounds().width;
		}
	}

	inline void jump() {
		y_speed = -jump_force;
	}
};

class Platform {
private:
	float x, y;
	
	static sf::Texture t;
	static bool textureHasBeenLoaded;
	sf::Sprite s;

public:
	inline Platform(float x, float y) {
		this->x = x;
		this->y = y;

		s.setPosition(x, y);

		if (!Platform::textureHasBeenLoaded) {
			Platform::t.loadFromFile("res/platform.png");
			textureHasBeenLoaded = true;
		}
		s.setTexture(t);
	}

	inline sf::Vector2f position() {
		return {x, y};
	}

	inline bool handle_collision(Doodle& doodle) {
		if (s.getGlobalBounds().intersects(doodle.bounds())) {
			if (y > doodle.position().y + doodle.bounds().height / 2 && doodle.is_falling()) {
				doodle.jump();
				return true;
			}
		}

		return false;
	}

	inline bool is_in_view(sf::View view) {
		if (std::abs(s.getPosition().y - view.getCenter().y) <= view.getSize().y / 2) {
			return true;
		}

		return false;
	}

	inline void draw(sf::RenderWindow& window) {
		window.draw(s);
	}
};

bool Platform::textureHasBeenLoaded = false;
sf::Texture Platform::t;

int main() {
	srand(time(NULL));

	bool game_over = false;

	sf::RenderWindow window(sf::VideoMode(400.f, 533.f), "Doodle jump", sf::Style::Titlebar);
	window.setFramerateLimit(60);

	sf::View view(sf::FloatRect(0.0f, 0.0f, 400.f, 533.f));
	window.setView(view);

	Doodle doodle;
	doodle.jump();

	std::vector<Platform> platforms;
	auto generate_platforms = [&]() {
		if (platforms.size() == 0) {
			Platform platform(rand() % 400, window.getSize().y - 100);
			platforms.push_back(platform);
		}

		int min_offset = 100;
		int max_offset = 250;
		for (int i = 0; i < 10; i++) {
			Platform platform(
				rand() % 400,
				platforms[platforms.size() - 1].position().y - (min_offset + (rand() % (max_offset + 1)))
			);
			platforms.push_back(platform);
		}
	};
	generate_platforms();

	sf::Texture background_t;
	background_t.loadFromFile("res/background.png");

	sf::Sprite background_s;
	background_s.setTexture(background_t);

	auto reset_game = [&]() {
		doodle.reset();
		platforms.clear();
		view.setCenter((float)window.getSize().x / 2, (float)window.getSize().y / 2);
		game_over = false;
	};

	sf::Clock clock;
	while (true) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
				return 0;
			}

			if (e.type == sf::Event::KeyPressed) {
				if (e.key.scancode == sf::Keyboard::Scancode::R) {
					reset_game();
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) doodle.move(RIGHT);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) doodle.move(LEFT);
		else doodle.move(NONE);

		float dt = game_over ? 0 : clock.restart().asSeconds();

		doodle.update(dt);
		doodle.clamp(0, view.getSize().x);

		for (Platform platform : platforms) {
			platform.handle_collision(doodle);
		}

		if (platforms[0].position().y > view.getCenter().y + view.getSize().y) {
			platforms.erase(platforms.begin());
		}

		if (platforms[platforms.size() - 1].is_in_view(view)) {
			generate_platforms();
		}

		if (doodle.position().y < view.getCenter().y) {
			view.setCenter(view.getCenter().x, doodle.position().y);
		}

		int tolerance = 50;
		if (doodle.position().y > view.getCenter().y + view.getSize().y / 2 + tolerance) {
			game_over = true;
		}
		
		window.clear(sf::Color().White);
		
		window.setView(window.getDefaultView());
		window.draw(background_s);
		
		window.setView(view);
		for (Platform platform : platforms) {
			platform.draw(window);
		}
		doodle.draw(window);

		if (game_over) {
			window.setView(window.getDefaultView());
			
			sf::RectangleShape overlay((sf::Vector2f)window.getSize());
			overlay.setFillColor(sf::Color(0, 0, 0, 120));

			sf::Font font;
			font.loadFromFile("res/game_over.ttf");

			sf::Text game_over_text;
			game_over_text.setString("GAME OVER");
			game_over_text.setFillColor(sf::Color::Red);
			game_over_text.setFont(font);
			game_over_text.setCharacterSize(128);
			game_over_text.setPosition(
				(float)window.getSize().x / 2 - game_over_text.getGlobalBounds().width / 2,
				(float)window.getSize().y / 2 - game_over_text.getGlobalBounds().height / 2 - 200
			);

			sf::Text restart_game_text;
			restart_game_text.setString("Press R to restart");
			restart_game_text.setFillColor(sf::Color::Red);
			restart_game_text.setFont(font);
			restart_game_text.setCharacterSize(32);
			restart_game_text.setPosition(
				(float)window.getSize().x / 2 - restart_game_text.getGlobalBounds().width / 2,
				(float)window.getSize().y / 2 - restart_game_text.getGlobalBounds().height / 2
			);

			window.draw(overlay);
			window.draw(game_over_text);
			window.draw(restart_game_text);
		}

		window.display();
	}

	return 1;
}

