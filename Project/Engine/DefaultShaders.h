#pragma once

//컴파일된 쉐이더 헤더 모아놓은 헤더
#ifdef _DEBUG
	
#include "CompiledShaderHeader/_0VS_Basic3D_Debug.h"
#include "CompiledShaderHeader/_4PS_Basic3D_Debug.h"

#include "CompiledShaderHeader/_0VS_Debug_Debug.h"
#include "CompiledShaderHeader/_4PS_Debug_Debug.h"

#include "CompiledShaderHeader/_0VS_Grid_Debug.h"
#include "CompiledShaderHeader/_4PS_Grid_Debug.h"

#include "CompiledShaderHeader/_0VS_Particle_Debug.h"
#include "CompiledShaderHeader/_3GS_Particle_Debug.h"
#include "CompiledShaderHeader/_4PS_Particle_Debug.h"
#include "CompiledShaderHeader/CS_Particle_Debug.h"

#include "CompiledShaderHeader/_0VS_PostProcess_Debug.h"
#include "CompiledShaderHeader/_4PS_PostProcess_Debug.h"

#include "CompiledShaderHeader/_0VS_Sprite_Debug.h"
#include "CompiledShaderHeader/_4PS_Sprite_Debug.h"

#include "CompiledShaderHeader/_0VS_Triangle_Debug.h"
#include "CompiledShaderHeader/_4PS_Triangle_Debug.h"

#include "CompiledShaderHeader/_0VS_UserInterface_Debug.h"
#include "CompiledShaderHeader/_4PS_UserInterface_Debug.h"

#include "CompiledShaderHeader/CS_Paint_Debug.h"

#include "CompiledShaderHeader/_0VS_Deffered_Debug.h"
#include "CompiledShaderHeader/_4PS_Deffered_Debug.h"

//#include "CompiledShaderHeader/_0VS_Light_Debug.h"
//#include "CompiledShaderHeader/_4PS_Light_Debug.h"

#include "CompiledShaderHeader/_0VS_LightDir_Debug.h"
#include "CompiledShaderHeader/_4PS_LightDir_Debug.h"

#include "CompiledShaderHeader/_0VS_LightPoint_Debug.h"
#include "CompiledShaderHeader/_4PS_LightPoint_Debug.h"

#include "CompiledShaderHeader/_0VS_Merge_Debug.h"
#include "CompiledShaderHeader/_4PS_Merge_Debug.h"

#include "CompiledShaderHeader/CS_Animation3D_Debug.h"

//#include "CompiledShaderHeader/_0VS_SkyBox_Debug.h"
//#include "CompiledShaderHeader/_4PS_SkyBox_Debug.h"

#else

#include "CompiledShaderHeader/_0VS_Basic3D_Release.h"
#include "CompiledShaderHeader/_4PS_Basic3D_Release.h"

#include "CompiledShaderHeader/_0VS_Debug_Release.h"
#include "CompiledShaderHeader/_4PS_Debug_Release.h"

#include "CompiledShaderHeader/_0VS_Grid_Release.h"
#include "CompiledShaderHeader/_4PS_Grid_Release.h"

#include "CompiledShaderHeader/_0VS_Particle_Release.h"
#include "CompiledShaderHeader/_3GS_Particle_Release.h"
#include "CompiledShaderHeader/_4PS_Particle_Release.h"
#include "CompiledShaderHeader/CS_Particle_Release.h"

#include "CompiledShaderHeader/_0VS_PostProcess_Release.h"
#include "CompiledShaderHeader/_4PS_PostProcess_Release.h"

#include "CompiledShaderHeader/_0VS_Sprite_Release.h"
#include "CompiledShaderHeader/_4PS_Sprite_Release.h"

#include "CompiledShaderHeader/_0VS_Triangle_Release.h"
#include "CompiledShaderHeader/_4PS_Triangle_Release.h"

#include "CompiledShaderHeader/_0VS_UserInterface_Release.h"
#include "CompiledShaderHeader/_4PS_UserInterface_Release.h"

#include "CompiledShaderHeader/CS_Paint_Release.h"

#include "CompiledShaderHeader/_0VS_Deffered_Release.h"
#include "CompiledShaderHeader/_4PS_Deffered_Release.h"

//#include "CompiledShaderHeader/_0VS_Light_Release.h"
//#include "CompiledShaderHeader/_4PS_Light_Release.h"
#include "CompiledShaderHeader/_0VS_LightDir_Release.h"
#include "CompiledShaderHeader/_4PS_LightDir_Release.h"

#include "CompiledShaderHeader/_0VS_LightPoint_Release.h
#include "CompiledShaderHeader/_4PS_LightPoint_Release.h"

#include "CompiledShaderHeader/_0VS_Merge_Release.h"
#include "CompiledShaderHeader/_4PS_Merge_Release.h"

#include "CompiledShaderHeader/CS_Animation3D_Release.h"

//#include "CompiledShaderHeader/_0VS_SkyBox_Release.h"
//#include "CompiledShaderHeader/_4PS_SkyBox_Release.h"

#endif
