#include "Game.h"
#include "GameObjects.h"
#include <iostream>

Game::Game()
	: m_gameOver(false)
	, m_playerupgrade(false)
	, m_mainmenu(true)
	, m_dropPU(false)
	, m_timeUntilPlayerSpawn(2.0f)
	, m_livesRemaining(3)
	, m_score(0)
	, m_multiplier(1.0)
	, m_level(0)
	, m_upgradeROF(0.50f)
	, m_upgradeAOB(3)
{
	m_playerTexture.loadFromFile("Sprites/PNG/playerShip2_red.png");
	m_mainFont.loadFromFile("Fonts/kenpixel_high_square.ttf");

	m_music.openFromFile("Audio/music.ogg");
	m_music.setLoop(true);
	m_music.play();
	m_sounds[SoundType::EXPLOSION].loadFromFile("Audio/Explosion.wav");
	m_sounds[SoundType::POWERUP].loadFromFile("Audio/PowerUp.wav");
	m_sounds[SoundType::SHOOT].loadFromFile("Audio/Laser_Shoot.wav");
	m_sounds[SoundType::DEATH].loadFromFile("Audio/Death.wav");
	m_sounds[SoundType::SPAWN].loadFromFile("Audio/Spawn.wav");
	m_sounds[SoundType::BOMB].loadFromFile("Audio/Bomb.wav");
	m_sounds[SoundType::INVULNERABLE_RUNNING_OUT].loadFromFile("Audio/Spawn.wav");

	for (int i = 0; i < SoundType::COUNT; i++)
	{
		m_soundInstances[i].setBuffer(m_sounds[i]);
	}

	SpawnAsteroids();
}
void Game::Draw(sf::RenderWindow * window)
{	
	if (m_mainmenu)
	{


		sf::Text mainMenu;
		mainMenu.setFont(m_mainFont);
		mainMenu.setString("ASTEROIDS");
		mainMenu.setCharacterSize(110);
		mainMenu.setPosition(450, 50);
		window->draw(mainMenu);

		sf::Text mainMenuText;
		mainMenuText.setFont(m_mainFont);
		mainMenuText.setString(" PRESS 1 TO START\nPRESS ESC TO QUIT\n\n			");
		mainMenuText.setCharacterSize(50);
		mainMenuText.setColor(sf::Color(rand() % 250,  150,  150));
		mainMenuText.setPosition(500, 250);
		window->draw(mainMenuText);

		sf::Text tutorialText;
		tutorialText.setFont(m_mainFont);
		tutorialText.setString(" PRESS Z AND X TO SHOOT - ARROW KEYS FOR MOVEMENT - TRY AND GET A HIGH SCORE - GREY ASTROIDSD DROP POWERUPS");
		tutorialText.setCharacterSize(25);
		tutorialText.setColor(sf::Color(250, 250, 150));
		tutorialText.setPosition(250, 400);
		window->draw(tutorialText);



	}
	if (m_mainmenu == false)
	{
		for (int i = 0; i < m_gameObjects.size(); i++)
		{
			GameObject* current = m_gameObjects[i];
			current->Draw(window);
		}
	}

	

	for (int i = 0; i < m_livesRemaining; i++)
	{
		sf::Sprite playerSprite;
		playerSprite.setTexture(m_playerTexture);
		playerSprite.setScale(0.5f, 0.5f);
		playerSprite.setPosition(sf::Vector2f(50 + 50 * i, 50));
		sf::Color color = sf::Color::White;
		color.a = 120;
		playerSprite.setColor(color);
		window->draw(playerSprite);
	}

	sf::Text scoreText;
	scoreText.setFont(m_mainFont);
	scoreText.setString("SCORE: "+std::to_string(m_score));
	scoreText.setCharacterSize(50);
	scoreText.setPosition(50, 120);
	window->draw(scoreText);

	sf::Text multiplierText;
	multiplierText.setFont(m_mainFont);
	multiplierText.setString("multiplier: " + std::to_string(m_multiplier));
	multiplierText.setCharacterSize(25);
	multiplierText.setPosition(50, 170);
	window->draw(multiplierText);
	
	if (m_gameOver)
	{
		sf::Text gameOverText;
		gameOverText.setFont(m_mainFont);
		gameOverText.setString("GAME OVER!");
		gameOverText.setCharacterSize(110);
		gameOverText.setPosition(450, 150);
		window->draw(gameOverText);
	}

	if (m_dropPU)
	{
		

	}

	if (m_playerupgrade)
	{
		sf::Text playerUpgradeText;
		playerUpgradeText.setFont(m_mainFont);
		playerUpgradeText.setString("PICK AN UPGRADE");
		playerUpgradeText.setCharacterSize(110);
		playerUpgradeText.setPosition(350, 50);
		window->draw(playerUpgradeText);

		sf::Text playerUpgradeText2;
		playerUpgradeText2.setFont(m_mainFont);
		playerUpgradeText2.setString("			PRESS 1 TO UPGRADE SINGLE SHOT RATE OF FIRE\n\n			PRESS 2 TO INCREASE BURST SHOT AMOUNT\n\n			PRESS 3 TO GAIN A LIFE\n");
		playerUpgradeText2.setCharacterSize(25);
		playerUpgradeText2.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		playerUpgradeText2.setPosition(300, 160);
		window->draw(playerUpgradeText2);

		sf::RectangleShape ROFupgrade(sf::Vector2f(50, 50));
		ROFupgrade.setFillColor(sf::Color(250, 250, 0));
		ROFupgrade.setPosition(550, 300);
		//window->draw(ROFupgrade);

		sf::RectangleShape AOBupgrade(sf::Vector2f(50, 50));
		AOBupgrade.setFillColor(sf::Color(250, 0, 250));
		AOBupgrade.setPosition(650, 300);
		//window->draw(AOBupgrade);

	}

}



void Game::Update(sf::RenderWindow * window, float dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		
		
		m_mainmenu = false;

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{

		

	}
	if (m_mainmenu == false)
	{
	
	if (m_timeUntilPlayerSpawn > 0)
	{
		m_timeUntilPlayerSpawn -= dt;
		if (m_timeUntilPlayerSpawn <= 0)
		{
			SpawnPlayer();
		}
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		current->Update(window, dt);
	}


	// See if any collisions occurred
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject* current = m_gameObjects[i];
		for (int j = 0; j < m_gameObjects.size(); j++)
		{
			GameObject* other = m_gameObjects[j];
			if (current->IsColliding(other))
			{
				current->CollidedWith(other);
			}
		}
	}

	bool allAsteroidsDead = true;
	for (int i = m_gameObjects.size() - 1; i >= 0; i--)
	{
		GameObject* current = m_gameObjects[i];
		if (current->IsDestroyed())
		{
			delete current;
			m_gameObjects.erase(m_gameObjects.begin() + i);
		}
		else if (dynamic_cast<Asteroid*>(current) != NULL)
		{
			allAsteroidsDead = false;
		}
	}


	if (allAsteroidsDead)
	{
		m_playerupgrade = true;

		if (m_playerupgrade == true)
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
			{
				//Upgrade rate of fire
				m_upgradeROF -= .1f;
				m_playerupgrade = false;

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
			{
				//Upgrade Amount of bullets
				m_upgradeAOB += 1;
				m_playerupgrade = false;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
			{
				//Upgrade Amount of Lives
				m_livesRemaining += 1;
				m_playerupgrade = false;
			}


			if (m_playerupgrade == false)
			{
				m_level++;
				m_multiplier += 0.2;
				SpawnAsteroids();

				for (int i = m_gameObjects.size() - 1; i >= 0; i--)
				{
					GameObject* current = m_gameObjects[i];
					Player* player = dynamic_cast<Player*>(current);
					if (player != NULL)
					{
						player->MakeInvulnerable();
					}
				}
			}
		}
	}
}
}

void Game::AddObject(GameObject * object)
{
	object->SetOwner(this);
	m_gameObjects.push_back(object);
}

void Game::Died()
{
	m_livesRemaining--; 
	m_multiplier = 1.0;
	if (m_livesRemaining < 0)
	{
		GameOver();
	}
	else
	{
		m_timeUntilPlayerSpawn = 2.0f;
	}
}

void Game::RemoveAll()
{
	for (int i = m_gameObjects.size() - 1; i >= 0; i--)
	{
		GameObject* current = m_gameObjects[i];
		current->Destroy();
	}
}

void Game::SpawnAsteroids()
{
	for (int i = 0; i < (m_level + 1); i++)
	{
		Asteroid* newAsteroid = newAsteroid = new LargeAsteroid(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
		newAsteroid->SetAngle(rand() % 360);
		newAsteroid->SetVelocity(150);
		AddObject(newAsteroid);
	}

	for (int i = 0; i < (1); i++)
	{
		PUAsteroid* newPUAsteroid = newPUAsteroid = new PUAsteroid(sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
		newPUAsteroid->SetAngle(rand() % -360);
		newPUAsteroid->SetVelocity(150);
		AddObject(newPUAsteroid);
	}


}

void Game::SpawnPowerUp()
{
	srand(static_cast<unsigned int>(time(0)));
	int die = (rand() % 4) + 1;
	


	if (die == 1)
	{


		RedPowerUpStar* redstarPU = redstarPU = new RedPowerUpStar("Sprites/PNG/Power-ups/powerupRed_star.png", sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
		redstarPU->SetVelocity(150);
		redstarPU->SetAngle(rand() % -360);
		AddObject(redstarPU);
	}
	else if (die == 2)
	{
		PowerUpStar* starPU = starPU = new PowerUpStar("Sprites/PNG/Power-ups/star_gold.png", sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
		starPU->SetVelocity(150);
		starPU->SetAngle(rand() % -360);
		AddObject(starPU);

	}
	else if (die == 3)
	{
		RedSheild* redshieldPU = redshieldPU = new RedSheild("Sprites/PNG/Power-ups/powerupRed_shield.png", sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
		redshieldPU->SetVelocity(150);
		redshieldPU->SetAngle(rand() % -360);
		AddObject(redshieldPU);

	}
	else if (die == 4)
	{
		GreenPowerUp* powerupGreen = powerupGreen = new GreenPowerUp("Sprites/PNG/Power-ups/powerupGreen.png", sf::Vector2f(rand() % 1000 + 100, rand() % 600 + 50));
		powerupGreen->SetVelocity(150);
		powerupGreen->SetAngle(rand() % -360);
		AddObject(powerupGreen);

	}
	
}

void Game::SpawnPlayer()
{
	Player* player = new Player("Sprites/PNG/playerShip2_red.png", sf::Vector2f(600, 300));
	AddObject(player);
	PlaySound(SoundType::SPAWN);
}

void Game::PlaySound(SoundType type)
{
	m_soundInstances[type].play();
}

void Game::PlayerUpgrade()
{


}

ParticleSystem::ParticleSystem(float count, sf::Vector2f pos, sf::Color color, float size, int spread, float angle, float speed, int speedSpread)
	: GameObject("", pos)
{
	for (int i = 0; i < count; i++)
	{
		Particle newParticle;
		newParticle.m_pos = pos;
		newParticle.m_color = color;
		newParticle.m_size = size + (rand() % 10 - 5);
		newParticle.m_lifetime = 1 + (rand() % 5 / 10.0f);
		newParticle.m_initialLifetime = newParticle.m_lifetime;		
		float randAngle = angle +  (spread > 0 ? (rand() % spread) : 0);	
		randAngle *= DEG_TO_RAD;
		float particleSpeed = speed  + (speedSpread > 0 ? (rand() % speedSpread ): 0);
		newParticle.m_vel = sf::Vector2f(sin(randAngle) * particleSpeed, cos(randAngle) * particleSpeed);
		newParticle.m_accel = sf::Vector2f(0.5f * -sin(randAngle) * particleSpeed, -0.5f * cos(randAngle) * particleSpeed);

		m_particles.push_back(newParticle);
	}
}

void ParticleSystem::Draw(sf::RenderWindow * window)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		Particle& particle = m_particles[i];
		
		m_rectShape.setPosition(particle.m_pos);
		m_rectShape.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		m_rectShape.setRadius(particle.m_size);
		m_rectShape.setPointCount(4);
		window->draw(m_rectShape);
	}
}

void ParticleSystem::Update(sf::RenderWindow * window, float dt)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		Particle& particle = m_particles[i];
		particle.m_vel += particle.m_accel * dt;
		particle.m_pos += particle.m_vel * dt;
		particle.m_lifetime -= dt;
		particle.m_color.a = 255 * particle.m_lifetime / particle.m_initialLifetime;

		if (particle.m_lifetime <= 0.0f)
		{
			m_particles.erase(m_particles.begin() + i);
			i--;
		}
	}

	if (m_particles.size() <= 0)
	{
		Destroy();
	}
}
