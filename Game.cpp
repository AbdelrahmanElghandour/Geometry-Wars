#include "Game.h"

#include <iostream>
#include <fstream>
#include <sstream>



Game::Game(const std::string& config) 
{
	init(config);
}
 
void Game::init(const std::string& path) {
	// TODO: read config file here
	// use premade PlayerConfig, EnemyConfig, BulletConfig variables
	// Leave it for the end not necessarily the first thing to work on

	std::ifstream fin(path);


	if (!fin.is_open()) {
		std::cerr << "Error Opening File!\n";
		exit(1);
	}

	std::string line;

	while (std::getline(fin, line))
	{
		std::istringstream iss(line);
		std::string command;
		iss >> command;
		if (command == "Window")
		{
			iss >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS;
		}
		else if (command == "Font")
		{
			iss >> m_fontConfig.path >> m_fontConfig.size >> m_fontConfig.r >> m_fontConfig.g >> m_fontConfig.b;
		}
		else if (command == "Player")
		{
			iss >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB
				>> m_playerConfig.OT >> m_playerConfig.V >> m_playerConfig.SI;
		}
		else if (command == "Enemy")
		{
			iss >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN
				>> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SP >> m_enemyConfig.SI;
			// Read Enemy Config
		}
		else if (command == "Bullet")
		{
			// Read Bullet Config
			iss >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB
				>> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
		}
	}


	
	//m_window.create(sf::VideoMode({ m_windowConfig.W, m_windowConfig.H }), "Geometry Wars");


	// Load once, outside your render loop

	if (!m_font.openFromFile("fonts/arial.ttf")) {
		std::cout << "File not found\n";
		exit(-1);
	}
	m_text.emplace(m_font, "Score: 0", 24);
	m_text->setFillColor(sf::Color::White);
	m_text->setPosition({ 10.f, 10.f });

	m_window.create(sf::VideoMode({ m_windowConfig.W, m_windowConfig.H }), "Geometry wars", sf::State::Fullscreen);

	m_window.setFramerateLimit(m_windowConfig.FL);

	
	spawnPlayer();
}


void Game::run() 
{
	while (m_running && m_window.isOpen())
	{
		sUserInput();      // Poll all events

		m_entities.update();

		if (!m_paused)
		{
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
		}

		sRender();

		m_currentFrame++;
	}
}

void Game::setPaused(bool paused) {
	m_paused = paused;
}

void Game::spawnPlayer() 
{
	// TODO: Finish adding all properties of the player with the correct values from the PlayerConfig

	// We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("player");

	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;

	
	// Give this entity a transform so it spawns at (200, 200) with velocity (1, 1) and angle 0

	// Position, Velocity, Angle
	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0, 0), 0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->cShape->circle.setPosition({ mx, my });

	// Add an input component to the player so we can control it
	entity->cInput = std::make_shared<CInput>();


	// Since we want this Entity to be our player, set our Game's player variable to be this Entity
	// This goes slightly against the entity manager paradigm, but we use the player so much it's worth it
	m_player = entity;
}



// Done
void Game::spawnEnemy() 
{
	// TODO: make sure the enemy is spawned properly with the m_enemyConfig values
	//		the enemy must be spawned within the bounds of the window
	auto entity = m_entities.addEntity("enemy");

	/*
		The range of x positions is [r , (width - r)]
		The range of y positions is [r - (h + r)]
	*/
	float radius = m_enemyConfig.SR;
	int x_diff = 1 + (m_window.getSize().x - radius) - radius;
	int y_diff = 1 + (m_window.getSize().y - radius) - radius;


	// Generating random position for the spawning enemy
	float ex = rand() % x_diff;
	ex += radius;
	float ey = rand() % y_diff;
	ey += radius;

	// Generating the number of vertices
	int diff = m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1;
	int V = rand() % diff;
	V += m_enemyConfig.VMIN; // Number of vertices

	// Generating the velocity of enemy
	int diffSpeed = m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1;
	int S = rand() % diffSpeed;
	S += m_enemyConfig.SMIN; // This is the speed

	int mx = m_window.getSize().x / 2.0f;
	int my = m_window.getSize().y / 2.0f;

	// distance from the origin
	int dx = -(ex - mx);
	int dy = -(ey - my);


	// Give it a random angle
	float angle = static_cast<float>(rand() % 360);


	// Velocity x and y components
	float velocity_x = S * cos(angle);
	float velocity_y = S * sin(angle);

	// record when the most recent enemy was spawned

	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(velocity_x, velocity_y), 0);


	// Generating a Random RGB for Enemy
	int diffRGB = 256;
	float FR = rand() % 256;
	float FG = rand() % 256;
	float FB = rand() % 256;

	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, V, sf::Color(FR, FG, FB), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	entity->cScore = std::make_shared<CScore>(V * 100);
	m_lastEnemySpawnTime = m_currentFrame;
}


void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// TODO: spawn small enemies when an enemy is destroyed at the location of the destroyed enemy (entity->cTransform->pos)
	int pointsCount = entity->cShape->circle.getPointCount();
	for (int i = 0; i < pointsCount;++i)
	{
		auto& transfrom = entity->cTransform;
		auto& circle = entity->cShape->circle;


		auto e = m_entities.addEntity("small enemy");
		float angle = (2.f * (3.14159265358979323846f) / pointsCount) * i;
		float parentVelocityX = transfrom->velocity.x;
		float parentVelocityY = transfrom->velocity.y;
		float speed = sqrt((parentVelocityX * parentVelocityX) + (parentVelocityY * parentVelocityY));

		float smallVelocityX = speed * std::cos(angle);
		float smallVelocityY = speed * std::sin(angle);

		e->cTransform = std::make_shared<CTransform>(Vec2{ transfrom->pos.x, transfrom->pos.y}, Vec2{smallVelocityX, smallVelocityY}, 0);
		e->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
		e->cShape = std::make_shared<CShape>(circle.getRadius() / 2.0f, pointsCount, circle.getFillColor(), circle.getOutlineColor(), circle.getOutlineThickness());
	}
}


void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{

	// TODO: implement the spawning of a bullet which travles toward target
	//		- bullet speed is given as a scalar speed
	//		- you must set the velocity by using formula in notes
	auto bullet = m_entities.addEntity("bullet");
	

	float dx = target.x - entity->cTransform->pos.x;
	float dy = target.y - entity->cTransform->pos.y;
	float angle = atan2f(dy, dx);

	float velocityX = m_bulletConfig.S * std::cos(angle);
	float velocityY = m_bulletConfig.S * std::sin(angle);
	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos , Vec2{ velocityX, velocityY }, 0);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OT, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// Is yet to be implemented
}


void Game::sMovement()
{
	// Player Movement
	auto& transform = *m_player->cTransform;
	auto& input = *m_player->cInput;
	auto& FL = m_windowConfig.FL;

	// Reset velocity
	transform.velocity = { 0.f, 0.f };

	// Determine velocity
	if (input.up)    transform.velocity.y = -1.f;
	if (input.down)  transform.velocity.y = 1.f;
	if (input.left)  transform.velocity.x = -1.f;
	if (input.right) transform.velocity.x = 1.f;

	// Move
	float length = std::sqrt(
		transform.velocity.x * transform.velocity.x +
		transform.velocity.y * transform.velocity.y
	);

	if (length > 0.f)
	{
		transform.velocity.x /= length;
		transform.velocity.y /= length;
	}

	transform.velocity.x *= m_playerConfig.S;
	transform.velocity.y *= m_playerConfig.S;
	transform.pos += (transform.velocity);

	// Keep inside window
	float r = m_playerConfig.SR;

	transform.pos.x = std::clamp(
		transform.pos.x,
		r,
		static_cast<float>(m_window.getSize().x) - r
	);

	transform.pos.y = std::clamp(
		transform.pos.y,
		r,
		static_cast<float>(m_window.getSize().y) - r
	);

	// Enemy Movement
	
	for (auto& e : m_entities.getEntitiesByTag("enemy"))
	{
		auto& transform = *e->cTransform;
		if (transform.pos.x <= e->cShape->circle.getRadius() || transform.pos.x >= (m_windowConfig.W - e->cShape->circle.getRadius()))
		{
			transform.velocity.x = -transform.velocity.x;
		}
		if (transform.pos.y <= e->cShape->circle.getRadius() || transform.pos.y >= (m_windowConfig.H - e->cShape->circle.getRadius())) {
			transform.velocity.y = -transform.velocity.y;
		}
		transform.pos += (transform.velocity);
	}

	// Bullet Movement
	for (auto& e : m_entities.getEntitiesByTag("bullet"))
	{
		auto& transform = *e->cTransform;
		transform.pos += (transform.velocity);
	}

	// small enemy Movement
	for (auto& se : m_entities.getEntitiesByTag("small enemy"))
	{
		auto& transform = *se->cTransform;
		transform.pos += (transform.velocity);
	}
	
}

void Game::sLifespan()
{
	for (auto& e : m_entities.getEntities())
	{
		if (!e->cLifespan)
			continue;

		e->cLifespan->remaining--;

		if (e->cLifespan->remaining <= 0)
		{
			e->destroy();
		}
	}
}

void Game::sCollision()
{
	// bullet-enemy collision
	for (auto& b : m_entities.getEntitiesByTag("bullet")) {
		for (auto& e : m_entities.getEntitiesByTag("enemy")) {
			auto& circle1 = b->cShape->circle;
			auto& circle2 = e->cShape->circle;
			int dx = circle1.getPosition().x - circle2.getPosition().x;
			int dy = circle1.getPosition().y - circle2.getPosition().y;
			int radiusSum = circle1.getRadius() + circle2.getRadius();
			if ((dx * dx + dy * dy) < (radiusSum * radiusSum))
			{
				b->destroy();
				e->destroy();
				spawnSmallEnemies(e);
				m_score += e->cScore->score;
				m_text->setString("Score: " + std::to_string(m_score));
			}
		}
	}
	// Repeat the same logic for player
	// Player - Enemy collision
	for (auto& e : m_entities.getEntitiesByTag("enemy"))
	{
		auto& player = m_player->cShape->circle;
		auto& enemy = e->cShape->circle;

		float dx = player.getPosition().x - enemy.getPosition().x;
		float dy = player.getPosition().y - enemy.getPosition().y;
		float radiusSum = player.getRadius() + enemy.getRadius();

		if ((dx * dx + dy * dy) <= (radiusSum * radiusSum))
		{
			spawnSmallEnemies(e);
			e->destroy();

			m_player->destroy();   // if your assignment requires destroying the old player
			spawnPlayer();         // create a new player

			break; // stop checking since m_player has changed
		}
	}
}


// Done
void Game::sEnemySpawner() 
{
	if (m_currentFrame - m_lastEnemySpawnTime >= m_enemyConfig.SP)
	{
		spawnEnemy();
	}
}

void Game::sRender() {
	m_window.clear();
	m_player->cShape->circle.setPosition({m_player->cTransform->pos.x, m_player->cTransform->pos.y});
	m_player->cTransform->angle += m_playerConfig.SI;
	m_player->cShape->circle.setRotation(sf::radians(m_player->cTransform->angle));
	m_window.draw(m_player->cShape->circle);
		
	for (auto& e : m_entities.getEntitiesByTag("enemy"))
	{
		e->cShape->circle.setPosition({ e->cTransform->pos.x, e->cTransform->pos.y });
		e->cTransform->angle += m_enemyConfig.SI;
		e->cShape->circle.setRotation(sf::radians(e->cTransform->angle));
		m_window.draw(e->cShape->circle);
	}
	for (auto& e : m_entities.getEntitiesByTag("bullet"))
	{
		e->cShape->circle.setPosition({ e->cTransform->pos.x, e->cTransform->pos.y });
		m_window.draw(e->cShape->circle);
	}
	for (auto& e : m_entities.getEntitiesByTag("small enemy"))
	{
		e->cShape->circle.setPosition({ e->cTransform->pos.x, e->cTransform->pos.y });
		m_window.draw(e->cShape->circle);
	}
	if (m_text)
	{
		m_window.draw(*m_text);
	}
	m_window.display();
}

void Game::sUserInput()
{
	// TODO: handle user input here
	// Note that you should only be setting the player's input component values here, not moving the player directly
	// You should not implement the player's movement logic here
	// the movement system will read the variables you set in this function
	std::optional<sf::Event> event;

	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			m_running = false;
		}
			
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				m_window.close();
			if (keyPressed->scancode == sf::Keyboard::Scancode::P)
			{
				if (m_paused == true)
				{
					m_paused = false;
					std::cout << "Paused 0\n";
				}
				else if (m_paused == false)
				{
					m_paused = true;
					std::cout << "Paused 1\n";
				}
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::W)
			{
				m_player->cInput->up = true;
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::A)
				m_player->cInput->left = true;
			if (keyPressed->scancode == sf::Keyboard::Scancode::D)
				m_player->cInput->right = true;
			if (keyPressed->scancode == sf::Keyboard::Scancode::X)
				m_player->cInput->down = true;
		}
		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyReleased->scancode == sf::Keyboard::Scancode::W) {
				m_player->cInput->up = false;
			}
					
			if (keyReleased->scancode == sf::Keyboard::Scancode::A)
				m_player->cInput->left = false;
			if (keyReleased->scancode == sf::Keyboard::Scancode::D)
				m_player->cInput->right = false;
			if (keyReleased->scancode == sf::Keyboard::Scancode::X)
				m_player->cInput->down = false;
		}
		if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseButtonPressed->button == sf::Mouse::Button::Left)
			{
				spawnBullet(m_player, {static_cast<float>(mouseButtonPressed->position.x), static_cast<float>(mouseButtonPressed->position.y)});
			}
		}
			
	}
}
