#include "./plugin_sdk/plugin_sdk.hpp"

#include "loader/Loader.h"

PLUGIN_NAME("TokyoSz");
PLUGIN_TYPE(plugin_type::champion);

//SUPPORTED_CHAMPIONS({ 
//	champion_id::Lux 
//});

PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good)
{
    DECLARE_GLOBALS(plugin_sdk_good);
	return Loader::Load();
}

PLUGIN_API void on_sdk_unload()
{
    Loader::Unload();
}