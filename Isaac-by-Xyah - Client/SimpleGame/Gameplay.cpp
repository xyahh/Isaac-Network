#include "stdafx.h"
#include "Gameplay.h"
#include "Framework.h"
#include "Renderer.h"
#include "Network.h"

void Gameplay::Init()
{
}

void Gameplay::Exit()
{
}


void Gameplay::Render(float fInterpolation) 
{ 

	int Size = 0;

	NW.retval = recv(NW.clientSock, (char*)&Size, sizeof(int ), 0);
	if (NW.retval == SOCKET_ERROR) {
		NW.err_display(const_cast<char*>("recv()"));
		exit(0);
	}

	NW.Positions.resize(Size);
	NW.retval = NW.recvn(NW.clientSock, (char*)&NW.Positions[0], sizeof(DX XMVECTOR) * NW.Positions.size(), 0);
	if (NW.retval == SOCKET_ERROR) {
		NW.err_display(const_cast<char*>("recv()"));
		exit(0);
	}

	for (size_t i = 0; i < NW.Positions.size(); ++i) 
	{
		NW.RenderDevice.DrawTexRect(
			NW.Positions[i],
			{ 1.25, 1.25 },
			{ 1.f, 1.f, 1.f, 1.f },
			NW.TEX
		);
		/*NW.RenderDevice.DrawSprite(
			NW.Positions[i],
			{ 1.25, 1.25 },
			{ 1.f, 1.f, 1.f, 1.f },
			NW.TEX,
			{ 0, 0 },
			{ 1, 1 }
			
		);*/
	}

}

void Gameplay::Update() 
{ 
	
	// recv
	//Engine.Update();
}
