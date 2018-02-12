#pragma once
#include <d3dx9.h>
#include <map>
#include "D3DObject.h"
using namespace std;

class D3DShaderEffect : public D3DObject
{
private:
	LPCWSTR							_shaderFileName;

public:
	LPD3DXEFFECT					pEffect	= NULL;		// ���̴�
	LPDIRECT3DVERTEXDECLARATION9	_pDecl	= NULL;		// ��������

	map<string, D3DXHANDLE>			techniques;			// hlsl techniques �ڵ鷯 �����̳�
	map<string, D3DXHANDLE>			parameters;			// Shader �Ķ���� �ڵ鷯 �����̳�

public:

	D3DShaderEffect(LPDIRECT3DDEVICE9 device, LPCWSTR shaderFileName);
	~D3DShaderEffect();

	HRESULT Init();

	HRESULT AddTechnique(const char* techniqueName);
	HRESULT AddParameter(const char* paramName);

	D3DXHANDLE GetTechnique(const char* techniqueName);
	D3DXHANDLE GetParameter(const char* techniqueName);
};