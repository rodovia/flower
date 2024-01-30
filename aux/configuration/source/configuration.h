/* configuration.h
   Purpose: Configuration management */
#pragma once

#include "css.h"
#include "css/css_color.h"
#include <libconfig.h++>

#ifdef __linux__
#include <linux/limits.h>
#endif

#define DEFAULT_CONFIG_FILE "config.conf"
#ifdef CONFIG_DEFINE_GLOBAL_VARIABLE
#define _EXTERN_OR_NOTHING
#else
#define _EXTERN_OR_NOTHING extern
#endif

namespace cfg
{

_EXTERN_OR_NOTHING libconfig::Config g_Configuration;

inline void GetCanonicalPath(std::string sttpath, std::string& canon)
{   
    std::string rel;
    if (!g_Configuration.lookupValue(sttpath, rel))
        return;
    
    char* abs = new char[PATH_MAX];
    realpath(rel.data(), abs);
    canon = abs;
    delete[] abs;
}

inline void GetColor(std::string sttpath, css::css_color& color)
{
    std::string cl;
    if (!g_Configuration.lookupValue(sttpath, cl))
        return;

    css::css_string str = { cl };
    css::EnsureColorCorrect(str, color);
}

inline void ReadConfigurationFile()
{
    g_Configuration.setOptions(libconfig::Config::OptionAutoConvert);
    g_Configuration.readFile(DEFAULT_CONFIG_FILE);
}

}
