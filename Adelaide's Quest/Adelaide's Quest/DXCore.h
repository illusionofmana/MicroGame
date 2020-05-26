#pragma once
#include "Main.h"
//--------------------------------------------------------------------------
//---------Graphics Extensions----------------------------------------------
//DX Macros
#define USING_DX 0
#define INITDX 0
#define SHUTDOWNDX 1
#define DX_DRAWFRAME 2
#define DX_PRESENTFRAME 3
#define DX_SETVERTEX 4
//-------------Direct-X-11--------------------------------------------------
//global declarations


//funcion and Class lookup
class DXEngine
{
public:
	void InitD3D(HWND hwnd);
	void StartFrame(void);
	void RenderFrame(void);
	void ShutDownD3D(void);

	void DrawVertex(int vertex_count, int start_location);
	void InitVertex();

private:

	void InitPipeline();

public:
	//virtual GPU
	ID3D11Device *GPu;
	//Context of said GPU
	ID3D11DeviceContext *GPu_Context;
	//GPU input layout
	ID3D11InputLayout *GPu_InputLayout;
	//shaders
	ID3D11VertexShader *pVS;	//vertex shader
	ID3D11PixelShader *pPS;		//pixel shader
	//objects

private:
	/*defining SwapChain
	A buffer used to store your rendered image and hold onto it until it is ready to be desplayed by the GPU
	Think of this as is it were a Screen Buffer*/
	IDXGISwapChain *swapchain;
	//the backbuffer (basically a pointer to our screen buffer)
	ID3D11RenderTargetView *backbuffer;
	//the vertex buffer (our buffer to store corner points of our 3D objects)
	ID3D11Buffer *pVBuffer;
	//the vertex index buffer
	ID3D11Buffer *pIBuffer;

};


//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//---------Memory Structures------------------------------------------------

//basic sprite structure (may help with objectifying sprites further while providing and easy interface)
struct SPRITE
{
	int x, y;
	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;
	int direction;
	int velx, vely;
	DWORD color;

	SPRITE()
	{
		frame = 0;
		columns = 1;
		width = height = 1;
		scaling = 1.0f;
		rotation = 0.0f;
		startframe = endframe = 0;
		direction = 0;
		starttime = delay = 0;
		velx = vely = 0;
		color = 0xFFFFFFFF;
	}
};
//basic vertex struct
struct VERTEX
{
	FLOAT X, Y, Z; //position
	D3DXCOLOR DXColor;
};
