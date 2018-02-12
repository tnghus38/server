#include "D3DShaderEffect.h"

D3DShaderEffect::D3DShaderEffect(LPDIRECT3DDEVICE9 device, LPCWSTR shaderFileName)
{
	_device = device;
	_shaderFileName = shaderFileName;
}

D3DShaderEffect::~D3DShaderEffect()
{
	techniques.clear();
	parameters.clear();
}

HRESULT D3DShaderEffect::Init()
{
	HRESULT hr;

	// ★셰이더 읽기
	LPD3DXBUFFER pErr = NULL;
	if (FAILED(hr = D3DXCreateEffectFromFile(
		_device, _shaderFileName, NULL, NULL,
		0, NULL, &pEffect, &pErr))) {

		// 셰이더 읽기 실패
		MessageBox(NULL, (LPCTSTR)pErr->GetBufferPointer(), TEXT("ERROR"), MB_OK);
	}

	SAFE_RELEASE(pErr);

	return S_OK;
}

HRESULT D3DShaderEffect::AddTechnique(const char* techniqueName)
{
	D3DXHANDLE technique = pEffect->GetTechniqueByName(techniqueName);

	if (technique != NULL)
	{
		techniques.insert(pair<string, D3DXHANDLE>(techniqueName, technique));
	}
	else return E_FAIL;

	return S_OK;
}

HRESULT D3DShaderEffect::AddParameter(const char * paramName)
{
	D3DXHANDLE param = pEffect->GetTechniqueByName(paramName);
	if (param != NULL)
	{
		parameters.insert(pair<string, D3DXHANDLE>(paramName, param));
	}

	return S_OK;
}

D3DXHANDLE D3DShaderEffect::GetTechnique(const char * techniqueName)
{
	if (techniques.find(techniqueName) == techniques.end())
		return NULL;

	return techniques[techniqueName];
}

D3DXHANDLE D3DShaderEffect::GetParameter(const char * paramNames)
{
	if (parameters.find(paramNames) == techniques.end())
		return NULL;

	return techniques[paramNames];
}
