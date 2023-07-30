#pragma once

//컴파일된 쉐이더 헤더 모아놓은 헤더
#ifdef _DEBUG
	
#include "CompiledShaderHeader/VS_Basic3D_Debug.h"
#include "CompiledShaderHeader/PS_Basic3D_Debug.h"

#include "CompiledShaderHeader/VS_Debug_Debug.h"
#include "CompiledShaderHeader/PS_Debug_Debug.h"

#include "CompiledShaderHeader/VS_Grid_Debug.h"
#include "CompiledShaderHeader/PS_Grid_Debug.h"

#include "CompiledShaderHeader/VS_Particle_Debug.h"
#include "CompiledShaderHeader/GS_Particle_Debug.h"
#include "CompiledShaderHeader/PS_Particle_Debug.h"
#include "CompiledShaderHeader/CS_Particle_Debug.h"

#include "CompiledShaderHeader/VS_PostProcess_Debug.h"
#include "CompiledShaderHeader/PS_PostProcess_Debug.h"

#include "CompiledShaderHeader/VS_Sprite_Debug.h"
#include "CompiledShaderHeader/PS_Sprite_Debug.h"

#include "CompiledShaderHeader/VS_Triangle_Debug.h"
#include "CompiledShaderHeader/PS_Triangle_Debug.h"

#include "CompiledShaderHeader/VS_UserInterface_Debug.h"
#include "CompiledShaderHeader/PS_UserInterface_Debug.h"

#include "CompiledShaderHeader/CS_Paint_Debug.h"

#include "CompiledShaderHeader/VS_Deffered_Debug.h"
#include "CompiledShaderHeader/PS_Deffered_Debug.h"

#include "CompiledShaderHeader/VS_Merge_Debug.h"
#include "CompiledShaderHeader/PS_Merge_Debug.h"

#else

#include "CompiledShaderHeader/VS_Basic3D_Release.h"
#include "CompiledShaderHeader/PS_Basic3D_Release.h"

#include "CompiledShaderHeader/VS_Debug_Release.h"
#include "CompiledShaderHeader/PS_Debug_Release.h"

#include "CompiledShaderHeader/VS_Grid_Release.h"
#include "CompiledShaderHeader/PS_Grid_Release.h"

#include "CompiledShaderHeader/VS_Particle_Release.h"
#include "CompiledShaderHeader/GS_Particle_Release.h"
#include "CompiledShaderHeader/PS_Particle_Release.h"
#include "CompiledShaderHeader/CS_Particle_Release.h"

#include "CompiledShaderHeader/VS_PostProcess_Release.h"
#include "CompiledShaderHeader/PS_PostProcess_Release.h"

#include "CompiledShaderHeader/VS_Sprite_Release.h"
#include "CompiledShaderHeader/PS_Sprite_Release.h"

#include "CompiledShaderHeader/VS_Triangle_Release.h"
#include "CompiledShaderHeader/PS_Triangle_Release.h"

#include "CompiledShaderHeader/VS_UserInterface_Release.h"
#include "CompiledShaderHeader/PS_UserInterface_Release.h"

#include "CompiledShaderHeader/CS_Paint_Release.h"

#include "CompiledShaderHeader/VS_Deffered_Release.h"
#include "CompiledShaderHeader/PS_Deffered_Release.h"

#include "CompiledShaderHeader/VS_Merge_Release.h"
#include "CompiledShaderHeader/PS_Merge_Release.h"

#endif
