#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#define DEG_TO_RAD (0.0174532925f)

class Game;

class GameObject
{
public:
	GameObject(std::string texturePath, const sf::Vector2f& pos);
	virtual void Draw(sf::RenderWindow* window);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void LimitVelocity(float dt);
	virtual void ApplyDrag(float dt);
	bool DropPU;
	void SetAngle(float angle);
	float GetAngle();
	
	void SetPos(const sf::Vector2f& pos);
	void SetAccel(float amount);
	void SetVelocity(float amount);
	void SetOwner(Game* owner);
	virtual void Destroy();
	bool IsDestroyed();

	void SetCollisionRadius(float val) { m_collisionRadius = val;  }
	float GetCollisionRadius() { return m_collisionRadius; }

	bool IsColliding(GameObject* other);
	virtual void CollidedWith(GameObject* other);

protected:
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_accel;

	sf::Sprite m_sprite;
	sf::Texture m_texture;

	Game* m_owner;
	float m_angle;
	float m_collisionRadius;
	bool m_destroyed;
};

class Bullet : public GameObject
{
public:
	Bullet(const sf::Vector2f& pos);	
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void CollidedWith(GameObject* other);
	float m_timeAlive;
};

class Player : public GameObject
{
public:
	Player(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void CollidedWith(GameObject* other);
	void MakeInvulnerable();
	float upgradeROF = 0.2f;
	int upgradeAOB = 3;

private:
	bool m_firing;
	float m_fireCooldown;
	float m_invulnerableTimeLeft;
};


class Asteroid : public GameObject
{
public:
	Asteroid(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void ApplyDrag(float dt) {};
	virtual void Destroy();
	bool DropPU;
private:
	float m_rotationRate;
};


class SmallAsteroid : public Asteroid
{
public:
	SmallAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

class MediumAsteroid : public Asteroid
{
public:
	MediumAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

class LargeAsteroid : public Asteroid
{
public:
	LargeAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};
class PUAsteroid : public Asteroid
{
public:
	PUAsteroid(const sf::Vector2f& pos);
	virtual void Destroy();
};

class PowerUp : public GameObject
{
public:
	PowerUp(std::string texturePath, const sf::Vector2f& pos);
	virtual void Update(sf::RenderWindow * window, float dt);
	virtual void Destroy();
	float m_timeAlive;
	bool DropPU;
private:
	float m_rotationRate;
	
};

class PowerUpStar : public PowerUp
{
public:
	PowerUpStar(std::string texturePath, const sf::Vector2f& pos);

	virtual void Destroy();
};
class RedPowerUpStar : public PowerUp
{
public:
	RedPowerUpStar(std::string texturePath, const sf::Vector2f& pos);

	virtual void Destroy();
};
class RedSheild : public PowerUp
{
public:
	RedSheild(std::string texturePath, const sf::Vector2f& pos);

	virtual void Destroy();
};
class GreenPowerUp : public PowerUp
{
public:
	GreenPowerUp(std::string texturePath, const sf::Vector2f& pos);

	virtual void Destroy();
};