// Free open-source Grom library

#ifndef __Grom_XmlUtils_XmlUtils_h__
#define __Grom_XmlUtils_XmlUtils_h__

#include "../System/Config.h"

namespace Sys
{
class XMLNode;
}

namespace Utl
{

void LoadTreeFromXmlString(Sys::XMLNode* root, const char* xml);
void LoadTreeFromXmlFile(Sys::XMLNode* root, const char* xmlFilePath);
void SaveTreeToXmlFile(Sys::XMLNode* root, const char* xmlFilePath);

} // namespace Utl

#endif //__Grom_XmlUtils_XmlUtils_h__
