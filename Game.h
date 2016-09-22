#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "GameObjects.h"

enum SoundType
{
	EXPLOSION,
	SHOOT,
	DEATH,
	SPAWN,
	INVULNERABLE_RUNNING_OUT,
	COUNT
};

class Game
{
public:
	Game();
	void Draw(sf::RenderWindow* window);
	void Update(sf::RenderWindow * window, float dt);
	void AddObject(GameObject* object);
	void GameOver() { m_gameOver = true;  }
	void AddPoints(int amount) { m_score += amount; }
	void Died();
	void RemoveAll();
	void SpawnAsteroids();
	void SpawnPUAstroids();
	void SpawnPowerUp();
	void SpawnPlayer();
	void PlaySound(SoundType type);
	void PlayerUpgrade();
	
	float m_upgradeROF;
	int m_upgradeAOB;


private:
	std::vector<GameObject*> m_gameObjects;
	bool m_playerupgrade;
	bool m_gameOver;
	bool m_dropPU;
	int m_livesRemaining;
	float m_timeUntilPlayerSpawn;
	int m_score;
	int m_level;

	sf::Font m_mainFont;
	sf::Texture m_playerTexture;
	sf::Music m_music;
	sf::SoundBuffer m_sounds[SoundType::COUNT];
	sf::Sound m_soundInstances[SoundType::COUNT];
};

class Particle
{
public:
	Particle()
		: m_pos(0.0f,0.0f)
		, m_vel(0.0f,0.0f)
		, m_accel(0.0f, 0.0f)
	{

	}
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_accel;
	sf::Color m_color;
	float m_size;
	float m_lifetime;
	float m_initialLifetime;
};
class ParticleSystem : public GameObject
{
public:
	ParticleSystem(float count, sf::Vector2f pos, sf::Color color, float size, int spread, float angle, float speed, int speedSpread);
	void Draw(sf::RenderWindow* window);
	void Update(sf::RenderWindow * window, float dt);
	std::vector<Particle> m_particles;

	sf::CircleShape m_rectShape;
};
