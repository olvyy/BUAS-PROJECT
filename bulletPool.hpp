#pragma once
#include "bullet.hpp"
#include <iostream>
#include <memory>
#include <vector>

class BulletPool
{
public:
	BulletPool(int size, std::shared_ptr<Tmpl8::Sprite> c_bulletSprite)
		: maxSize(size), bulletSprite(c_bulletSprite)
	{
		for (int i = 0; i < maxSize; i++)
		{
			bullets.push_back(std::make_shared<bullet>(bulletSprite));
		}
	}

	std::shared_ptr<bullet> GetBullet()
	{
		if (bullets.empty())
		{
			//std::cout << "Pool empty, creating new bullet..." << std::endl;
			return std::make_shared<bullet>(bulletSprite);
		}
		else
		{
			auto bullet = bullets.back();
			bullets.pop_back();
			//std::cout << "Bullet taken from pool" << std::endl;
			return bullet;
		}
	}

	void ReturnBullet(std::shared_ptr<bullet> bullet)
	{
		if (bullets.size() < maxSize)
		{
			bullets.push_back(bullet);
		}
		else
		{
			//std::cout << "Pool full, bullet discarded" << std::endl;
		}
	}

	const std::vector<std::shared_ptr<bullet>>& GetPool() const { return bullets; }

private:
	std::vector<std::shared_ptr<bullet>> bullets;
	int maxSize;
	std::shared_ptr<Tmpl8::Sprite> bulletSprite;
};