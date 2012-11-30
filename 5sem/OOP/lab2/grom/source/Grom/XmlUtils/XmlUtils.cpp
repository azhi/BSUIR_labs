// Free open-source Grom library

#include "PrecompiledHeader.h"

#include <stdio.h>
#include "expat.h"

#include "../System/XML/XMLNode.h"
#include "../System/XML/XMLAttribute.h"
#include "../System/XML/XMLComment.h"
#include "../System/XML/XMLElement.h"
#include "../System/XML/XMLInstruction.h"
#include "../System/XML/XMLText.h"
#include "../System/XML/XMLCData.h"
#include "XmlUtils.h"

namespace Utl
{

// Forward

void XMLCALL LoadFromXml_OnStartElement(
  void *userData, const XML_Char *name, const XML_Char **props);
void XMLCALL LoadFromXml_OnEndElement(
  void *userData, const XML_Char *name);
void XMLCALL LoadFromXml_OnCharacterData(
  void *userData, const XML_Char *text, int textLength);
void XMLCALL LoadFromXml_OnCDataCharacters(
  void *userData, const XML_Char *text, int textLength);

void CopyAllProps(Sys::XMLNode* result, const XML_Char **props)
{
  for (size_t i = 0; props[i] != NULL; i += 2)
  {
    Sys::XMLAttribute* prop = Sys::Create<Sys::XMLAttribute>(result);
    prop->SetParent(result);
    prop->SetName(props[i]);
    prop->SetValue(props[i + 1]);
  }
}

void XMLCALL LoadFromXml_OnStartRootElement(
  void *userData, const XML_Char *name, const XML_Char **props)
{
  XML_Parser parser = (XML_Parser)userData;
  Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
  result->SetName(name);
  CopyAllProps(result, props);
  XML_SetElementHandler(parser,
    LoadFromXml_OnStartElement,
    LoadFromXml_OnEndElement);
}

void XMLCALL LoadFromXml_OnStartCDataElement(void *userData)
{
  XML_Parser parser = (XML_Parser)userData;
  Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
  Sys::XMLCData* cdata_node = Sys::Create<Sys::XMLCData>(result);
  cdata_node->SetParent(result);
  cdata_node->SetName("cdata");
  XML_SetCharacterDataHandler(parser, LoadFromXml_OnCDataCharacters);
  XML_SetUserData(parser, cdata_node);
}

void XMLCALL LoadFromXml_OnEndCDataElement(void *userData)
{
  XML_Parser parser = (XML_Parser)userData;
  Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
  result = result->Parent();
  XML_SetCharacterDataHandler(parser, LoadFromXml_OnCharacterData);
  XML_SetUserData(parser, result);
}

void XMLCALL LoadFromXml_OnEndRootElement(
  void *userData, const XML_Char *name)
{
  XML_Parser parser = (XML_Parser)userData;
  Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
  // std::string s = Sys::TrimSpaces(result->Text());
  // result->SetText(s.c_str());
}

void XMLCALL LoadFromXml_OnStartElement(
  void *userData, const XML_Char *name, const XML_Char **props)
{
  XML_Parser parser = (XML_Parser)userData;
  Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
  Sys::XMLElement* child_element = Sys::Create<Sys::XMLElement>(result);
  child_element->SetParent(result);
  child_element->SetName(name);
  CopyAllProps(child_element, props);
  XML_SetUserData(parser, child_element);
}

void XMLCALL LoadFromXml_OnEndElement(
  void *userData, const XML_Char *name)
{
  XML_Parser parser = (XML_Parser)userData;
  Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
  result = result->Parent();
  XML_SetUserData(parser, result);
  if (result == NULL)
  {
    // Expat reads first root tag only, so we do not need
    // protection from multiple root tags.
  }
}

void XMLCALL LoadFromXml_OnCharacterData(
  void *userData, const XML_Char *text, int textLength)
{
  if ( !Sys::IsBlank(text, textLength) )
  {
    XML_Parser parser = (XML_Parser)userData;
    Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
    Sys::XMLText* text_node = Sys::Create<Sys::XMLText>(result);
    text_node->SetParent(result);
    text_node->SetValue(text, textLength);
  }
}

void XMLCALL LoadFromXml_OnCDataCharacters(
  void *userData, const XML_Char *text, int textLength)
{
  if ( !Sys::IsBlank(text, textLength) )
  {
    XML_Parser parser = (XML_Parser)userData;
    Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
    std::string s = result->Value();
    s.append(text, textLength);
    result->SetValue(s.c_str());
  }
}

void XMLCALL LoadFromXml_OnComment(
  void *userData, const XML_Char *text)
{
  if ( !Sys::IsBlank(text) )
  {
    XML_Parser parser = (XML_Parser)userData;
    Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
    Sys::XMLComment* comment_node = Sys::Create<Sys::XMLComment>(result);
    comment_node->SetParent(result);
    comment_node->SetValue(Sys::TrimSpaces(text).c_str());
  }
}

void XMLCALL LoadFromXml_OnInstruction(
  void *userData, const XML_Char *target, const XML_Char *data)
{
  XML_Parser parser = (XML_Parser)userData;
  Sys::XMLNode* result = (Sys::XMLNode*)XML_GetUserData(parser);
  Sys::XMLInstruction* instruction_node = Sys::Create<Sys::XMLInstruction>(result);
  instruction_node->SetParent(result);
  instruction_node->SetName(Sys::TrimSpaces(target).c_str());
  instruction_node->SetValue(Sys::TrimSpaces(data).c_str());
}

void LoadTreeFromXmlString(Sys::XMLNode* root, const char* xml)
{
  if (xml != NULL)
  {
    XML_Parser parser = XML_ParserCreate("UTF-8");
    if (parser != NULL)
    {
      try
      {
        // Prepare parser
        XML_SetElementHandler(parser,
          LoadFromXml_OnStartRootElement,
          LoadFromXml_OnEndRootElement);
        XML_SetCharacterDataHandler(parser, LoadFromXml_OnCharacterData);
        XML_SetCommentHandler(parser, LoadFromXml_OnComment);
        XML_SetProcessingInstructionHandler(parser, LoadFromXml_OnInstruction);
        XML_SetCdataSectionHandler(parser,
            LoadFromXml_OnStartCDataElement,
            LoadFromXml_OnEndCDataElement);
        XML_UseParserAsHandlerArg(parser);
        XML_SetUserData(parser, root);

        // Parse
        int n = (int)strlen(xml);
        if (XML_Parse(parser, xml, (int)n, true) == XML_STATUS_ERROR)
          throw new Sys::Exception("cannot parse XML string");
      }
      catch (...)
      {
        // Protect from possible exceptions in callbacks (e.g. XML_SetElementHandler)
        XML_ParserFree(parser);
        throw;
      }
      XML_ParserFree(parser);
    }
    else
      throw new Sys::Exception("cannot create XML parser");
  }
  else
    throw Sys::CreateArgumentNullException();
}

void LoadTreeFromXmlFile(Sys::XMLNode* root, const char* xmlFilePath)
{
  if (xmlFilePath != NULL)
  {
    FILE* f = fopen(xmlFilePath, "r");
    if (f != NULL)
    {
      try
      {
        XML_Parser parser = XML_ParserCreate("UTF-8");
        if (parser != NULL)
        {
          char *buf = NULL;
          try
          {
            // Prepare parser
            XML_SetElementHandler(parser,
              LoadFromXml_OnStartRootElement,
              LoadFromXml_OnEndRootElement);
            XML_SetCharacterDataHandler(parser, LoadFromXml_OnCharacterData);
            XML_SetCommentHandler(parser, LoadFromXml_OnComment);
            XML_SetProcessingInstructionHandler(parser, LoadFromXml_OnInstruction);
            XML_SetCdataSectionHandler(parser,
                LoadFromXml_OnStartCDataElement,
                LoadFromXml_OnEndCDataElement);
            XML_UseParserAsHandlerArg(parser);
            XML_SetUserData(parser, root);

            // Prepare buffer
            const size_t BUF_SIZE = 16 * 1024;
            buf = new char[BUF_SIZE];
            if (buf != NULL)
            {
              root->Clear();
              // Read & parse
              size_t n = fread(buf, 1, BUF_SIZE, f);
              while (n != 0)
              {
                if (XML_Parse(parser, buf, (int)n, n != BUF_SIZE) == XML_STATUS_ERROR)
                  throw new Sys::Exception(Sys::FormattedString(
                    "cannot parse XML file '%s'", xmlFilePath));
                n = fread(buf, 1, BUF_SIZE, f);
              }
            }
            else
              throw new Sys::SystemException("out of memory");
          }
          catch (...)
          {
            // Protect from possible exceptions in callbacks (e.g. XML_SetElementHandler)
            delete[] buf;
            XML_ParserFree(parser);
            throw;
          }
          delete[] buf;
          XML_ParserFree(parser);
        }
        else
          throw new Sys::Exception("cannot create XML parser");
      }
      catch (...)
      {
        // Protect from possible exceptions in callbacks (e.g. XML_SetElementHandler)
        fclose(f);
        throw;
      }
      fclose(f);
    }
    else
      throw new Sys::IOException(Sys::FormattedString(
        "cannot open file '%s' for reading", xmlFilePath));
  }
  else
    throw Sys::CreateArgumentNullException();
}

void XmlQuote(const char* s, std::string* result)
{
  result->clear();
  if (s != NULL)
  {
    while (*s != 0)
    {
      switch (*s)
      {
        case '&':
          result->append("&amp;");
          break;
        case '<':
          result->append("&lt;");
          break;
        case '"':
          result->append("&quot;");
          break;
        case '>':
          result->append("&gt;");
          break;
        case 9:
          result->append("&#9;");
          break;
        case 10:
          result->append("&#10;");
          break;
        case 13:
          result->append("&#13;");
          break;
        default:
          result->append(s, 1);
          break;
      }
      ++s;
    }
  }
}

std::string SerializeTreeToString(Sys::XMLNode* node, const char* tab)
{
  std::string head = tab;
  head += "<";
  head += node->Name();

  std::string body;

  std::string buf;
  std::string t = tab;
  t += "  ";

  if (node->ChildCount() > 0)
  {
    for (int i = 0; i < node->ChildCount(); ++i)
    {
      Sys::XMLNode* child = node->Child(i);
      switch (child->Type())
      {
        case Sys::XMLNode::XML_ATTRIBUTE:
          XmlQuote(child->Value(), &buf);
          head += "\n";
          head += t;
          head += child->Name();
          head += "=\"";
          head += buf;
          head += "\"";
          break;
        case Sys::XMLNode::XML_TEXT:
          body += child->Value();
          body += "\n";
          break;
        case Sys::XMLNode::XML_COMMENT:
          body += t;
          body += "<!-- ";
          body += child->Value();
          body += " -->";
          body += "\n";
          break;
        case Sys::XMLNode::XML_INSTRUCTION:
          body += t;
          body += "<?";
          body += child->Name();
          body += " ";
          body += child->Value();
          body += ">";
          body += "\n";
          break;
        case Sys::XMLNode::XML_CDATA:
          body += t;
          body += "<![CDATA[";
          body += child->Value();
          body += "]]>";
          body += "\n";
          break;
        case Sys::XMLNode::XML_ELEMENT:
          body += SerializeTreeToString(child, t.c_str());
          break;
      }
    }
    head += ">\n";
    body += tab;
    body += "</";
    body += node->Name();
    body += ">\n";
  }
  else
    body += "/>\n";

  head.append(body);    
  return head;
}

void SaveTreeToXmlFile(Sys::XMLNode* root, const char* xmlFilePath)
{
  if (xmlFilePath != NULL)
  {
    FILE* f = fopen(xmlFilePath, "w");
    if (f != 0)
    {
      try
      {
        std::string xml_str = SerializeTreeToString(root, "");
        fprintf(f, "%s", xml_str.c_str());
      }
      catch (...)
      {
        fclose(f);
        throw;
      }
      fclose(f);
    }
    else
      throw new Sys::IOException(Sys::FormattedString(
        "cannot open file '%s' for writing", xmlFilePath));
  }
  else
    throw Sys::CreateArgumentNullException();
}

} // namespace Utl
