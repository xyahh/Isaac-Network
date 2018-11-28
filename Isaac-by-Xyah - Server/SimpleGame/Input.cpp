#include "stdafx.h"
#include "Input.h"
#include "Gamepad.h"
#include "CyanEngine.h"

STD map<id_type, STD set<int>> Input::PushedKeysData;

void Input::AddKeyboardInput(int value, const id_type & CommandID)
{
	KeyboardInput.emplace(value, CommandID);
}

void Input::AddPushedKey(int Key)
{
	m_PushedKeys.push(Key);
}

void Input::AddReleasedKey(int Key)
{
	m_ReleasedKeys.push(Key);
}

void Input::HandleInput(const id_type& ActorID)
{
	while(!m_PushedKeys.empty())
	{
		auto& p = KeyboardInput.equal_range(m_PushedKeys.top());
		for (auto Iter = p.first; Iter != p.second; ++Iter)
			Engine.GetCommand(Iter->second)->execute(ActorID);
		m_PushedKeys.pop();
	}
	while (!m_ReleasedKeys.empty())
	{
		auto& p = KeyboardInput.equal_range(m_ReleasedKeys.top());
		for(auto Iter = p.first; Iter!=p.second; ++Iter)
			Engine.GetCommand(Iter->second)->release(ActorID);
		m_ReleasedKeys.pop();
	}
	
}
