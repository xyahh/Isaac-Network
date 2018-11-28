#pragma once
#include "Gamepad.h"

/* Devices */
#define KEYBOARD	0x01
#define GAMEPAD		0x02

class Input
{
	friend Cyan;
	friend State;

public:
	Input()  {}
	~Input() {}

	void AddKeyboardInput(int value, const id_type& CommandID);

private:

	void HandleInput(const id_type& ActorID);
	void AddPushedKey(int Key);
	void AddReleasedKey(int Key);

private:
	static STD map<id_type, STD set<int>> PushedKeysData;

	STD stack<int>			m_PushedKeys;
	STD stack<int>			m_ReleasedKeys;
	STD multimap<int, id_type>   KeyboardInput;	// Keyboard Key Input (INT VALUE, ID_TYPE COMMANDID)
};