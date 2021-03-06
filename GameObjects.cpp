#include "stdafx.h"
#include "Game.h"
#include "GameObjects.h"

GameObject::GameObject(std::string texturePath, const sf::Vector2f & pos)
	: m_pos(pos)
	, m_destroyed(false)
	, m_collisionRadius(0)
{
	// Load the texture
	// Assign the sprite		
	m_texture.loadFromFile(texturePath);
	m_sprite.setTexture(m_texture);
	m_sprite.setOrigin(m_sprite.getTextureRect().width * 0.5f, m_sprite.getTextureRect().height * 0.5f);
	
}

void GameObject::Destroy()
{
	m_destroyed = true;
}

bool GameObject::IsDestroyed()
{
	return m_destroyed;
}

bool GameObject::IsColliding(GameObject* other)
{
	bool result = false;
	sf::Vector2f vecToOther = other->m_pos - m_pos;
	float dist = sqrt(vecToOther.x * vecToOther.x + vecToOther.y  *vecToOther.y);
	if (this != other &&
		!IsDestroyed() &&
		!other->IsDestroyed() &&
		dist < (m_collisionRadius + other->m_collisionRadius))
	{
		result = true;
	}
	return result;
}

void GameObject::CollidedWith(GameObject * other)
{
}

void GameObject::Draw(sf::RenderWindow * window)
{
	sf::CircleShape collisionRadius;
	collisionRadius.setRadius(m_collisionRadius);
	collisionRadius.setOutlineThickness(2);
	collisionRadius.setFillColor(sf::Color::Transparent);
	collisionRadius.setPosition(m_pos);
	collisionRadius.setOrigin(m_collisionRadius, m_collisionRadius);
	collisionRadius.setOutlineColor(sf::Color::Red);
	window->draw(collisionRadius);
	window->draw(m_sprite);
}

void GameObject::Update(sf::RenderWindow * window, float dt)
{
	float decelScalar = 0.5f;
	m_vel += m_accel * dt;
	
	LimitVelocity(dt);
	ApplyDrag(dt);

	m_pos += m_vel * dt;
	if (m_pos.x < 0)
	{
		m_pos.x = window->getSize().x;
	}
	else if (m_pos.x > window->getSize().x)
	{
		m_pos.x = 0;
	}

	if (m_pos.y < 0)
	{
		m_pos.y = window->getSize().y;
	}
	else if (m_pos.y > window->getSize().y)
	{
		m_pos.y = 0;
	}


	m_sprite.setRotation(m_angle);
	m_sprite.setPosition(m_pos);
}

void GameObject::LimitVelocity(float dt)
{
	float speed = sqrt(m_vel.x * m_vel.x + m_vel.y  *m_vel.y);	

	if (speed <= 0.1)
	{
		return;
	}

	sf::Vector2f normalizedVel = sf::Vector2f(m_vel.x / speed, m_vel.y / speed);

	if (speed > 500)
	{
		speed = 500;
	}

	m_vel.x = normalizedVel.x * speed;
	m_vel.y = normalizedVel.y * speed;
}

void GameObject::SetAngle(float angle)
{
	m_angle = angle;
}

float GameObject::GetAngle()
{
	return m_angle;
}

void GameObject::SetPos(const sf::Vector2f & pos)
{
	m_pos = pos;
}

void GameObject::SetAccel(float amount)
{	
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_accel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_accel = sf::Vector2f(0, 0);
	}
}

void GameObject::SetVelocity(float amount)
{
	if (amount > 0)
	{
		float rotInRadians = DEG_TO_RAD * m_angle;
		m_vel = sf::Vector2f(amount * sin(rotInRadians), -amount * cos(rotInRadians));
	}
	else
	{
		m_vel = sf::Vector2f(0, 0);
	}
}

void GameObject::SetOwner(Game * owner)
{
	m_owner = owner;
}

void GameObject::ApplyDrag(float dt)
{
	if (m_accel.x == 0 && m_accel.y == 0)
	{
		float dragAmount = dt * 5.9f;
		m_vel.x -= dragAmount* m_vel.x;
		m_vel.y -= dragAmount * m_vel.y;
	}
}

Bullet::Bullet(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue08.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(0.5, 0.5);
	SetCollisionRadius(20);
}

void Bullet::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	m_angle += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive > 1.0f)
	{
		Destroy();
	}
	
}

void Bullet::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid)
	{
		Destroy();
		other->Destroy();
	}
}

BulletTwo::BulletTwo(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue08.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(0.5, 0.5);
	SetCollisionRadius(20);
}

void BulletTwo::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	m_angle += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive > 1.0f)
	{
		Destroy();
	}
}

void BulletTwo::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid)
	{
		Destroy();
		other->Destroy();
	}
}

BulletThree::BulletThree(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserBlue05.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(0.5, 1.5);
	SetCollisionRadius(5);
}

void BulletThree::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	
	m_timeAlive += dt;

	if (m_timeAlive > 10.0f)
	{
		Destroy();
	}
}

void BulletThree::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid)
	{
		Destroy();
		other->Destroy();
	}
	Bullet* bullet = dynamic_cast<Bullet*>(other);
	if (bullet)
	{
		other->Destroy();

	}
}
Bomb::Bomb(const sf::Vector2f & pos)
	: GameObject("Sprites/PNG/Lasers/laserRed10.png", pos)
	, m_timeAlive(0.0f)
{
	m_sprite.setScale(.25, .25);
	SetCollisionRadius(10);
	
}

void Bomb::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);
	m_angle += dt * 360;
	m_timeAlive += dt;

	if (m_timeAlive >= 5.0f)
	{
		SetCollisionRadius(200);
		m_sprite.setScale(5.0, 5.0);
		m_owner->PlaySound(SoundType::BOMB);
	}
	if (m_timeAlive > 6.0f)
	{
		Destroy();
	}
}

void Bomb::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid)
	{
		
		other->Destroy();
	}
	if (m_timeAlive <= 0.5f)
	{
		Bullet* bullet = dynamic_cast<Bullet*>(other);
		if (bullet)
		{
			other->Destroy();

		}
	}
}


Player::Player(std::string texturePath, const sf::Vector2f & pos)
	: GameObject(texturePath, pos)
	, m_moveright(false)
	, m_moveleft(false)
	, m_firing(false)
	, m_firingBackwards(false)
	, m_fireCooldown(0.0f)
	, m_backwardsfire(0.0f)
	, m_laser(0.0f)
{
	SetCollisionRadius(30);
	MakeInvulnerable();
}


void Player::Update(sf::RenderWindow * window, float dt)
{
	GameObject::Update(window, dt);

	sf::Color color = m_sprite.getColor();
	if (m_invulnerableTimeLeft > 0.0f)
	{
		color.a = 255 * (sin(m_invulnerableTimeLeft * 30) + 1) * 0.5f;
		m_sprite.setColor(color);
	}	
	else
	{	
		color.a = 255;
		m_sprite.setColor(sf::Color::White);
	}

	m_fireCooldown -= dt;
	m_backwardsfire -= dt;
	m_laser -= dt;
	m_bomb -= dt;
	float prevInvulnerableTime = m_invulnerableTimeLeft;
	m_invulnerableTimeLeft -= dt;
	m_firing = false;
	m_moveright = false;
	m_moveleft = false;

	

	SetAccel(0.0f);
	SetAccel(0.0f);

	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		quick_exit;
	}*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		SetAngle(GetAngle() + 180 * dt);
		m_moveright = true;
	}
	if (m_moveright)
	{

		//SetAngle(90);

		//SetVelocity(200.0f);
		//m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle + 90)) * 50, -cos(DEG_TO_RAD * (m_angle + 90)) * 50), sf::Color::White, 1, 0, -m_angle, 400, 0));
		m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle - 90)) * 50, -cos(DEG_TO_RAD * (m_angle - 90)) * 50), sf::Color::White, 1, 0, -m_angle, 100, 0));

	}
	else
	{
		SetAccel(0.0f);
		

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		SetAngle(GetAngle() - 180 * dt);
		m_moveleft = true;
		
	}
	if (m_moveleft)
	{
		//SetAngle(-90);
		//SetVelocity(200.0f);
		m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle + 90)) * 50, -cos(DEG_TO_RAD * (m_angle + 90)) * 50), sf::Color::White, 1, 0, -m_angle, 100, 0));
		//m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle - 90)) * 50, -cos(DEG_TO_RAD * (m_angle - 90)) * 50), sf::Color::White, 1, 0, -m_angle, 400, 0));


	}
	else
	{
		SetAccel(0.0f);


	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		SetAccel(400.0f);
		m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle + 90)) * 50, -cos(DEG_TO_RAD * (m_angle + 90)) * 50), sf::Color::White, 1, 0, -m_angle, 100, 0));
		m_owner->AddObject(new ParticleSystem(1, m_pos + sf::Vector2f(sin(DEG_TO_RAD * (m_angle - 90)) * 50, -cos(DEG_TO_RAD * (m_angle - 90)) * 50), sf::Color::White, 1, 0, -m_angle, 100, 0));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		m_firing = true;	
	}

	if (m_firing && m_fireCooldown <= 0.0f)
	{
		bool tripleShot = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
		m_fireCooldown = tripleShot ? 0.9f : m_owner->m_upgradeROF;
		m_owner->PlaySound(SoundType::SHOOT);
		if (tripleShot)
		{
			for (int i = 0; i < m_owner->m_upgradeAOB; i++)
			{
				Bullet* bullet = new Bullet(m_pos);
				bullet->SetAngle(m_angle - 15 + i * 15);
				bullet->SetVelocity(800);
				m_owner->AddObject(bullet);
			}
			
			if ( m_backwardsfire >= 0.0f)
			{
				for (int i = 0; i < m_owner->m_upgradeAOB; i++)
				{
					BulletTwo* bulletTwo = new BulletTwo(m_pos);
					bulletTwo->SetAngle(m_angle - 195 + i * 15);
					bulletTwo->SetVelocity(300);
					m_owner->AddObject(bulletTwo);
					m_backwardsfire -= dt;
				}
			}
			if (m_laser >= 0.0f)
			{
				for (int i = 0; i < m_owner->m_upgradeAOB; i++)
				{
					BulletThree* bulletThree = new BulletThree(m_pos);
					bulletThree->SetAngle(m_angle - 15 + i * 15);
					bulletThree->SetVelocity(750);
					m_owner->AddObject(bulletThree);
					m_laser -= dt;
				}

			}
			

		}
		else
		{
			Bullet* bullet = new Bullet(m_pos);
			bullet->SetAngle(m_angle);
			bullet->SetVelocity(500);
			m_owner->AddObject(bullet);
			
				if (m_backwardsfire >= 0.0f)
				{

					BulletTwo* bulletTwo = new BulletTwo(m_pos);
					bulletTwo->SetAngle(m_angle - 180);
					bulletTwo->SetVelocity(300);
					m_owner->AddObject(bulletTwo);
					m_backwardsfire -= dt;

				}
				if (m_laser >= 0.0f)
				{
					
						BulletThree* bulletThree = new BulletThree(m_pos);
						bulletThree->SetAngle(m_angle );
						bulletThree->SetVelocity(750);
						m_owner->AddObject(bulletThree);
						m_laser -= dt;					
				}
				if (m_bomb >= 0.0f)
				{
					Bomb* bomb = new Bomb(m_pos);
					bomb->SetAngle(m_angle);
					bomb->SetVelocity(50);
					m_owner->AddObject(bomb);
					
					m_bomb = 0;

				}
		}
	}
}

void Player::CollidedWith(GameObject * other)
{
	Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
	if (asteroid && m_invulnerableTimeLeft <= 0.0f)
	{
		Destroy();
		m_owner->Died();
		m_owner->PlaySound(SoundType::DEATH);
	}

	GreenPowerUp* PowerUpG = dynamic_cast<GreenPowerUp*>(other);
	if (PowerUpG )
	{
		m_bomb = 10.0f;
	}
	RedSheild* PowerUpRS = dynamic_cast<RedSheild*>(other);
	if (PowerUpRS )
	{
		m_owner->m_livesRemaining += 1;
		MakeInvulnerable();
	}
	PowerUpStar* PowerUpS = dynamic_cast<PowerUpStar*>(other);
	if (PowerUpS)
	{		
		m_laser = 6.0f;	
	}
	RedPowerUpStar* RPowerUpS = dynamic_cast<RedPowerUpStar*>(other);
	if (RPowerUpS)
	{
		m_backwardsfire = 6.0f;
	}

}

void PowerUp::CollidedWith(GameObject* other)
{
	Player* player = dynamic_cast<Player*>(other);
	if (player && m_invulnerableTimeLeft <= 0.0f)
	{
		Destroy();
		m_owner->PlaySound(SoundType::POWERUP);
		m_owner->AddPoints(500 );
		/*m_owner->Died();
		m_owner->PlaySound(SoundType::DEATH);*/
	}


}

void Player::MakeInvulnerable()
{
	m_invulnerableTimeLeft = 3.0f;
}

void PowerUp::MakeInvulnerable()
{
	m_invulnerableTimeLeft = 0.0f;
}


Asteroid::Asteroid(std::string texturePath, const sf::Vector2f& pos)
	: GameObject(texturePath, pos)
{
	m_rotationRate = rand()% 45 + 45;
	m_rotationRate *= rand() % 2 == 0 ? -1 : 1;
	SetCollisionRadius(50);
}

void Asteroid::Update(sf::RenderWindow * window, float dt)
{
	m_angle += dt * m_rotationRate;
	GameObject::Update(window, dt);
}

void Asteroid::Destroy()
{
	GameObject::Destroy();
	m_owner->PlaySound(SoundType::EXPLOSION);	
}




SmallAsteroid::SmallAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_small1.png", pos)
{
}

void SmallAsteroid::Destroy()
{
	Asteroid::Destroy();
	m_owner->AddPoints(10 * m_owner->m_multiplier);
	m_owner->AddObject(new ParticleSystem(15, m_pos, sf::Color::White, 3, 360, 0, 25, 100));
}

MediumAsteroid::MediumAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_med1.png", pos)
{
}

void MediumAsteroid::Destroy()
{
	Asteroid::Destroy();
	m_owner->AddPoints(50);
	m_owner->AddObject(new ParticleSystem(30, m_pos, sf::Color::White, 5, 360, 0, 25, 100));
	for (int i = 0; i < 3; i++)
	{
		SmallAsteroid* smallAsteroid = new SmallAsteroid(m_pos);
		smallAsteroid->SetAngle(rand() % 360);
		smallAsteroid->SetVelocity(100);
		m_owner->AddObject(smallAsteroid);
	}
}


LargeAsteroid::LargeAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorBrown_big1.png", pos)
{
}

void LargeAsteroid::Destroy()
{
	Asteroid::Destroy();
	m_owner->AddPoints(100 * m_owner->m_multiplier);
	m_owner->AddObject(new ParticleSystem(69, m_pos, sf::Color::White, 7, 360, 0, 25, 100));
	for (int i = 0; i < 3; i++)
	{
		MediumAsteroid* medAsteroid = new MediumAsteroid(m_pos);
		medAsteroid->SetAngle(rand() % 360);
		medAsteroid->SetVelocity(100);
		m_owner->AddObject(medAsteroid);
	}
}


PowerUp::PowerUp(std::string texturePath, const sf::Vector2f& pos)
	: GameObject(texturePath, pos)
	, m_timeAlive(0.0f)
{
	m_rotationRate = rand() % 45 + 45;
	m_rotationRate *= rand() % 2 == 0 ? -1 : 1;
	SetCollisionRadius(50);
}

void PowerUp::Update(sf::RenderWindow * window, float dt)
{
	m_angle += dt * m_rotationRate;
	GameObject::Update(window, dt);
	m_timeAlive += dt;

	if (m_timeAlive > 7.0f)
	{
		Destroy();
	}
}

void PowerUp::Destroy()
{
	GameObject::Destroy();
	
}

PowerUpStar::PowerUpStar(std::string texturePath, const sf::Vector2f & pos)
	:PowerUp("Sprites/PNG/Power-ups/star_gold.png", pos)
{	
}


void PowerUpStar::Destroy()
{
	
	PowerUp::Destroy();

}

RedPowerUpStar::RedPowerUpStar(std::string texturePath, const sf::Vector2f & pos)
	:PowerUp("Sprites/PNG/Power-ups/powerupRed_star.png", pos)
{
}
void RedPowerUpStar::Destroy()
{

	PowerUp::Destroy();

}
RedSheild::RedSheild(std::string texturePath, const sf::Vector2f & pos)
	:PowerUp("Sprites/PNG/Power-ups/powerupRed_star.png", pos)
{
}
void RedSheild::Destroy()
{

	PowerUp::Destroy();

}
GreenPowerUp::GreenPowerUp(std::string texturePath, const sf::Vector2f & pos)
	:PowerUp("Sprites/PNG/Power-ups/powerupGreen_star.png", pos)
{
}
void GreenPowerUp::Destroy()
{

	PowerUp::Destroy();

}



PUAsteroid::PUAsteroid(const sf::Vector2f & pos)
	: Asteroid("Sprites/PNG/Meteors/MeteorGrey_med1.png", pos)
{
}

void PUAsteroid::Destroy()
{
	
	Asteroid::Destroy();
	m_owner->AddPoints(100 * m_owner->m_multiplier);
	m_owner->SpawnPowerUp();
	m_owner->AddObject(new ParticleSystem(69, m_pos, sf::Color::Cyan, 7, 360, 0, 25, 100));
	
	m_owner->SpawnPowerUp();
	

	
	
}

