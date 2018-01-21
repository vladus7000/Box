#pragma once

#include "System\AppMsg.hpp"

class CDXUTDialogResourceManager;
class CDXUTTextHelper;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

namespace box
{
	namespace DXUT
	{
		void Restore(ID3D11Device* device, ID3D11DeviceContext* context);
		void Deinit();
		AppMsg::Status MsgProc(const AppMsg& msg);
		CDXUTDialogResourceManager& GetDialogResourceManager();
		CDXUTTextHelper& GetTextHelper();

		ID3D11ShaderResourceView* LoadShaderResourceView(const char* name);
	}
}