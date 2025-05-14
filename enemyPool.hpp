#pragma once
#include "enemy.hpp"
#include <vector>
#include <memory>
#include <iostream>

//made using : https://www.youtube.com/watch?v=GcybPjVZWzE&ab_channel=CppNuts

class objectPool 
{
public:
	objectPool(int size) : maxSize(size)
	{
		for (int i = 0; i < maxSize; i++)
		{
			pool.push_back(std::make_shared<Enemy>(Tmpl8::vec2(0,0), idleEnemy));
		}
	}

	std::shared_ptr<Enemy> getEnemy()
	{
		if (pool.empty())
		{
			return std::make_shared<Enemy>(Tmpl8::vec2(0, 0), idleEnemy);
		}
		else
		{
			std::shared_ptr<Enemy> enemy = pool.back();
			pool.pop_back();
			return enemy;
		}
	}

	void returnEnemy(std::shared_ptr<Enemy> enemy)
	{
		if (pool.size() < maxSize)
		{
			pool.push_back(enemy);
		}
		else
		{
			std::cout << "pool full" << std::endl;
		}
	}

	std::vector<std::shared_ptr<Enemy>> returnVector()
	{
		return pool;
	}

private:
	std::vector<std::shared_ptr<Enemy>> pool;
	std::shared_ptr<Tmpl8::Sprite> idleEnemy = std::make_shared<Tmpl8::Sprite>(new Tmpl8::Surface("assets/enemy1.png"), 4);
	int maxSize;

};