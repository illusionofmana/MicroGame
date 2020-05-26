#include "DXCore.h"
//init directX
void DXEngine::InitD3D(HWND hwnd)
{
	//create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;
	//clear the struct of any dirty memory so it can be ready for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	//define the swap chain. NOTE: These variables may be set via the game so be sure you define your 
	//passes appropriately and be sure to describe what each variable pass means.
	//variables with a * at the start of their description indicate objects that are safe to adjust
	scd.BufferCount = 1;									//*one back buffer (this could have a secondary buffer)
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//use 32 bit color (would not suggest changing this)
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		//how the swap chain is going to be used (would not suggest changing this)
	scd.OutputWindow = hwnd;								//the viewport we are aiming to project our output to (would not suggest changing this)
	scd.SampleDesc.Count = 1;								//This is the super sampling variable (dont change this unless you plan on rendering Polygons)
	scd.Windowed = TRUE;									//*Fullscreen or windowed varable (We may or may not handle this on windows side or Graphics API side)
																//further note there is a bit more needed to enable fullscreen mode, extra info will be attached to this.

	//create a device and device context (basically creating a virtual GPU for our render tasks)
	//you can think of this as creating a container or package that our job will be stored in before its sent to the GPU
	//to be processed. A GPU is similar to a CPU in how it operates, it needs to multi task just like the CPU does so jobs or programs
	//have their runtime scheduled by the OS. Think of it like filling out a work order that the OS will hand to the GPU in order to have
	//it fulfilled.
	D3D11CreateDeviceAndSwapChain(
		NULL,						//this is the adapter
		D3D_DRIVER_TYPE_HARDWARE,	//*this is the requested acceleration type of the driver (think of this as telling the driver how and where the job should be sent)
		NULL,
		NULL,						//This flag is binary set and reads each individual bit as its own flag(useful for some debug tricks)
		NULL,
		NULL,
		D3D11_SDK_VERSION,			//this is the DirectX version pass (helps tell the OS which DX version will handle the job)
		&scd,						//this is a pointer to our swap chain settings (set above)
		&swapchain,					//this is our output swap chain
		&GPu,						//this is our GPU thread name
		NULL,
		&GPu_Context);				//this is the Context of our GPU Thread
	/* *Continuting on the "requested acceleration type" there are a number of methods that can be used
	her are a few:
	D3D_DRIVER_TYPE_HARDWARE		Uses hardware GPU acceleration
	D3D_DRIVER_TYPE_REFERENCE		Forced Software rendering of DXversion features that may not be available on your GPU(SLOW)
	D3D_DRIVER_TYPE_SOFTWARE		Uses CPU acceleration and just fills the GPU buffer with a rendered frame and an output request
	MIXED use to be an available option but is since depreciated. You can technically used REFERENCE as a mixed type though it will
	not be as optimal as a traditional mixed acceleration runtime. The Suggestion is to always use Hardware.
	*/

	//get the address of the back buffer
	//you do not need to know how this works, just that the back buffer is basically treated
	//like a 3D plane with a "texture" painted on it. That texture is our output of our games graphics.
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	//use the back buffer address to create the render target
	GPu->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	//after transfering the new address pointer to our GPU back buffer, destroy the virtual back buffer.
	pBackBuffer->Release();
	//set the render target on the GPU as the back buffer
	GPu_Context->OMSetRenderTargets(1, &backbuffer, NULL);

	//set the viewport structure and initialize it
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	//set the dimensions of the viewport (this is usefull in many ways but for our purposes were going to leave some parts of it fixed and
	//other pointing to external variables
	viewport.TopLeftX = 0;		//this defines the start point in which the back buffer (that 3D plane we mentioned earlier) top left corner resides
									//realative to the top left corner of the window
	viewport.TopLeftY = 0;		//this operates the same way as the previous except for its effects are applied to the y position of the back buffer
	viewport.Width = 608;		//this parameter sets the width in pixels of the back buffer
	viewport.Height = 488;		//this parameter sets the height in pixels of the back buffer
	//deliver the parameters to the GPU so it can set up the output
	GPu_Context->RSSetViewports(1, &viewport);
	//keep this section of code in mind as it can be used to generate multiple viewports in one window. Keep in mind it may require multiple instances
	//of back buffers created. If the goal is to use this for a game then you will not need to set up multiple back buffers.

	//initialize the shader pipeline to begin 3D processing.
	InitPipeline();
	//initialize the virtex buffer (input a temp object into the buffer as well)
	InitVertex();												//test this will change in the future

	return;
}
//start the frame compilation process
void DXEngine::StartFrame(void)
{
	//clear the back buffer to a solid color (think of this like a raster CRT television... every time the screen is drawn the phosphor holds the image
	//then the image fades... but before it fades the screen starts drawing the next image repeating the process over and over...
	//We are not driving a CRT though so our output device will hold the image forever until we clear it so it can be ready for the next draw.
	//this is done by this line
	GPu_Context->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	//the most important feature of this line is the D3DXCOLOR line, this is simply a Dword or 00 00 00 00 (4) sets of values put together that
	//represent the 8 bit color standard in full 32 bit mode. The last value in this Dword set is for the Alpha or Transparancy, some may know it
	//as opacity. Only values between 0.00float and 1.00float are allowed but if youre familiar with hex then you can pass those values instead.
	//to keep things simple its best to keep the last value at "FF or 1.0f" and use opacity within sprite objects within the engine instead.
	
	return;
}
//send the compiled frame to the GPU
void DXEngine::RenderFrame(void)
{
	//present the display data to the swapchain so it can be rendered and displayed by the GPU
	swapchain->Present(0, 0);

	return;
}
//shut down directx
void DXEngine::ShutDownD3D(void)
{
	//close and release all existing objects

	//release objects
	pVS->Release();
	pPS->Release();
	//release system
	swapchain->Release();
	backbuffer->Release();
	GPu->Release();
	GPu_Context->Release();
	return;
}


//draw vertex
void DXEngine::DrawVertex(int vertex_count, int start_location)
{
	//set the vertex buffer to be displayed
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	GPu_Context->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	//select the primitive type (this is for 3D instancing, you can learn about primitives and definitions in most 3D
	//design documents and "how-to's " but for now do not worry about how it works
	GPu_Context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
	//draw the vertex buffer onto the back buffer
	GPu_Context->Draw(vertex_count, start_location);
	return;
}

//init a shader pipeline
void DXEngine::InitPipeline()
{
	//load and compile two shaders
	ID3DBlob *VS, *PS;
	D3DX11CompileFromFile(
		"Shader.shader",		//shader source code
		0,						//DX10 Shader Macro define (not used)
		0,						//DX10 Source Include (not used)
		"VShader",				//Name of the start function in our shader source code. (think of this as the "void main()" of a shader code)
		"vs_4_0",				//profile of the shader
		0,						//Special Flag (not used)
		0,						//Special Flag (not used)
		0,						//?Thread Pump? (not used)
		&VS,					//pointer to our "Blob" structure (the ID3DBlob struct)
		0,						//Error message and flag pass for our "Blob" struct (not used)
		0);						//an HRESULT pass for error detection (not used)

	//the same descriptions above apply to this one as well
	D3DX11CompileFromFile("Shader.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	//create shader objects with both the vertex and pixel shaders
	//a shader object is sort of a container to hold the shader itself so it can be properly delivered to other entities for use
	GPu->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	GPu->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	//set the shader objects
	//once our shader object is created and our shader is placed within it, we need to set them so they can be put to use
	GPu_Context->VSSetShader(pVS, 0, 0);
	GPu_Context->PSSetShader(pPS, 0, 0);

	//create the input layout object
	D3D11_INPUT_ELEMENT_DESC DXInputLayout[] =
	{
		//each input layout objects accepts 7 arguments that simply tell the GPU what the data is used for.
		//each object needs to have a name to go along with each data type.
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	//pack the input layout and send it to the GPU
	GPu->CreateInputLayout(
		DXInputLayout,				//input our temp input layout object
		2,							//how many elements that reside in our input layout object
		VS->GetBufferPointer(),		//Get the memory location of the buffer
		VS->GetBufferSize(),		//get the size of the buffer
		&GPu_InputLayout);			//state our input layout object for our data to be stored in
	//let the GPU know the information just received is the Input Layout
	GPu_Context->IASetInputLayout(GPu_InputLayout);

	return;
}
//initialize the vertex renderer
void DXEngine::InitVertex()
{
	int vertex_count = 4;
	//create a two triangle plane object
	VERTEX VTtmp[] =
	{
		{-0.5f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)}, //white

		{0.5f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)}, //red

		{0.5f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)}, //black

		{-0.5f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}, //blue
	};

	//create the vertex buffer object
	D3D11_BUFFER_DESC DXBufferOBJ;
	ZeroMemory(&DXBufferOBJ, sizeof(DXBufferOBJ));
	//define our new vertex buffer object
	DXBufferOBJ.Usage = D3D11_USAGE_DYNAMIC;					//write access by CPU and GPU
	DXBufferOBJ.ByteWidth = sizeof(VERTEX) * vertex_count;					//size is the VERTEXT struct above times the number of entities we stated within it
	DXBufferOBJ.BindFlags = D3D11_BIND_VERTEX_BUFFER;			//confirm use as a vertex buffer (some datasets may not be used within a vertex buffer)
	DXBufferOBJ.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//allow the CPU to write in our buffer
	//set the vertex container
//	D3D11_SUBRESOURCE_DATA vertexData;
//	vertexData.pSysMem = VTtmp;
//	vertexData.SysMemPitch = 0;
//	vertexData.SysMemSlicePitch = 0;

	//create the vertex buffer on our GPU           &vertexData
	GPu->CreateBuffer(&DXBufferOBJ, NULL, &pVBuffer);

	//create an indices object
	unsigned long *indices;
	indices = new unsigned long[vertex_count];
	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top left.
	indices[2] = 2;  // top right.

	indices[3] = 0;  // Bottom left.
	indices[4] = 1;  // Top left.
	indices[5] = 2;  // top right.
	//create the indices buffer object
	D3D11_BUFFER_DESC IndexBufferOBJ;
	ZeroMemory(&IndexBufferOBJ, sizeof(IndexBufferOBJ));
	//define our new indices buffer object
	IndexBufferOBJ.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferOBJ.ByteWidth = sizeof(unsigned long) * vertex_count;
	IndexBufferOBJ.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferOBJ.CPUAccessFlags = 0;
	//set the indices container
//	D3D11_SUBRESOURCE_DATA IndexData;
//	IndexData.pSysMem = indices;
//	IndexData.SysMemPitch = 0;
//	IndexData.SysMemSlicePitch = 0;

	//create the vertex buffer on our GPU      &IndexData
	//GPu->CreateBuffer(&IndexBufferOBJ, NULL, &pIBuffer);

	//copy vertex data into the buffer (note we only stated the size in our definition of our vertex buffer so it could set aside memory for it...
	//we have not yet sent our vertex information to the buffer yet).
	//create a resource container and map it within our GPU
	D3D11_MAPPED_SUBRESOURCE Map_SubResource;
	//park the GPU buffer for CPU write
	GPu_Context->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &Map_SubResource);
	//copy our vertex data into our container
	memcpy(Map_SubResource.pData, VTtmp, sizeof(VTtmp));
	//unpark the GPU buffer for GPU processing
	GPu_Context->Unmap(pVBuffer, NULL);

	return;
}
