#include "stdafx.h"
#include "Input.h"
#include "Gamepad.h"
#include "CyanEngine.h"

STD map<id_type, STD set<int>> Input::PushedKeysData;

void Input::AddKeyboardInput(int value, const id_type & CommandID)
{
	KeyboardInput[value] = CommandID;
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
		Engine.GetCommand(KeyboardInput[m_PushedKeys.top()])->execute(ActorID);
		m_PushedKeys.pop();
	}
	while (!m_ReleasedKeys.empty())
	{
		Engine.GetCommand(KeyboardInput[m_ReleasedKeys.top()])->release(ActorID);
		m_ReleasedKeys.pop();
	}
	
}
