#include "stdafx.h"
#include "Framework.h"
#include "Scene.h"
#include "Logo.h"
#include "Gameplay.h"

int main(int argc, char **argv)
{		
	/* Game Loop */
	Fw.Initialize("The Binding of Isaac", 800, 800, argc, argv);
	Fw.ToScene(new Gameplay);
	Fw.Run();
	Fw.Close();

    return FALSE;
}

