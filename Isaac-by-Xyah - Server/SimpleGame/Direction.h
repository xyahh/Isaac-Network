#pragma once
enum Direction
{
	Up = 3,
	Down = 2,
	Left = 1,
	Right = 0,
};

inline DX XMVECTOR GetDirectionVector(u_int dir)
{
	switch (dir)
	{
	case Direction::Up:
		return { 0.f,  1.f, 0.f };
	case Direction::Down:
		return { 0.f, -1.f, 0.f };
	case Direction::Left:
		return { -1.f, 0.f, 0.f };
	case Direction::Right:
		return {  1.f, 0.f, 0.f };
	}
	return DX XMVectorZero();
}