#include "stdafx.h"
#include "Sprite.h"

void Sprite::SetTexture(size_t TexIdx)
{
 	TexIndex = TexIdx;
}

void Sprite::SetDirection(u_int _State)
{
	Current.y = _State;
}

void XM_CALLCONV Sprite::SetOffset(DX FXMVECTOR _Offset)
{
	Offset = DX3 Store(_Offset);
}

void Sprite::SetSpriteType(SpriteType _Type)
{
	Type = _Type;
}

void Sprite::SetFrameRate(float _FrameRate)
{
	FrameRate = _FrameRate;
}

void XM_CALLCONV Sprite::SetSize(DX FXMVECTOR _Size)
{
	Size = DX2 Store(_Size);
}

void XM_CALLCONV Sprite::SetTotal(DX FXMVECTOR _Total)
{
	Total = DX2 StoreUINT(_Total);
}

void Sprite::ResetSprite()
{
	Current.x = 0;
	CurrentFrame = 0.f;
}

bool Sprite::Update()
{
	switch (Type)
	{
	case SpriteType::Linear: 
		return LinearUpdate();
	case SpriteType::Grid: 
		return GridUpdate();
	}
	return false;
}

void Sprite::NextFrame()
{
	CurrentFrame += 1.f;
}

float Sprite::GetFrameRate() const
{
	return FrameRate;
}

bool Sprite::LinearUpdate()
{
	CurrentFrame += UPDATE_TIME *  FrameRate;
	Current.x = (u_int)(CurrentFrame) % Total.x;
	return CurrentFrame >= Total.x;
}

bool Sprite::GridUpdate()
{
	CurrentFrame += UPDATE_TIME * FrameRate;
	Current.x = (u_int)(CurrentFrame) % Total.x;
	u_int Vframes = (u_int)(CurrentFrame) / Total.x;
	Current.y = Vframes % Total.y;
	if (Vframes >= Total.y)
	{
		Dispatch(SpriteEvent::LoopEnd);
		CurrentFrame = 0;
		return true;
	}
	return false;
}

size_t Sprite::GetTexture() const
{
	return TexIndex;
}

u_int Sprite::GetDirection() const
{
	return Current.y;
}

DX XMVECTOR XM_CALLCONV Sprite::GetSize() const
{
	return DX2 Load(Size);
}

DX XMVECTOR XM_CALLCONV Sprite::GetCurrent() const
{
	return DX2 LoadUINT(Current);
}

DX XMVECTOR XM_CALLCONV Sprite::GetTotal() const
{
	return DX2 LoadUINT(Total);
}

DX XMVECTOR XM_CALLCONV Sprite::GetOffset() const
{
	return DX3 Load(Offset);
}
