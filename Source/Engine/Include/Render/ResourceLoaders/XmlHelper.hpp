#pragma once

#include <map>
#include <string>
#include <DXUT11\Core\DXUT.h>

namespace box
{

#define START_LOOP(root) for (auto item = root->FirstChild(); item; item = item->NextSibling()) { bool found = false;
#define END_LOOP }

#define ITEM_TEXT(var, name, map) if (found)continue;do{ auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) {const char* v = el->Attribute("value"); auto p = map.find(v); if (p != map.end()){var = p->second;found = true;} }   }while(0)
#define ITEM_BOOL(var, name, textValue, dxValue) if (found)continue;do{ auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) { if (!strcmp(el->Attribute("value"), textValue)) {var = dxValue; found = true; } }}while(0)
#define ITEM_BOOL(var, name) if (found)continue;do {auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) { if (!strcmp(el->Attribute("value"), "true")) { var = true; found = true; } else { var = false; found = true; } } } while (0)

#define ITEM_INT(var, name) if (found)continue;do {auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) { int val(0); el->QueryIntAttribute("value", &val); var = val;found = true;} } while (0)
#define ITEM_FLOAT(var, name) if (found)continue;do {auto el = item->ToElement(); if (!strcmp(el->Attribute("name"), name)) { float val(0.0f); el->QueryFloatAttribute("value", &val); var = val;found = true;} } while (0)

extern std::map<std::string, D3D11_COMPARISON_FUNC> g_cmpFuncs;
extern std::map<std::string, D3D11_FILL_MODE> g_fillModes;
extern std::map<std::string, D3D11_CULL_MODE> g_cullModes;
extern std::map<std::string, D3D11_DEPTH_WRITE_MASK> g_WriteMasks;
extern std::map<std::string, D3D11_STENCIL_OP> g_stencilOps;

extern std::map<std::string, D3D11_BLEND> g_Blends;
extern std::map<std::string, D3D11_BLEND_OP> g_BlendOps;
extern std::map<std::string, UINT8> g_colorWriteMasks;

extern std::map<std::string, D3D11_FILTER> g_Filters;
extern std::map<std::string, D3D11_TEXTURE_ADDRESS_MODE> g_addressModes;

extern std::map<std::string, D3D11_INPUT_CLASSIFICATION> g_inputClassifications;
extern std::map<std::string, DXGI_FORMAT> g_dxgiFormats;
}