//using namespace ultralight;

namespace juce_ultralight
{
      
    inline std::string ToUTF8(const ultralight::String& str) {
      ultralight::String8 utf8 = str.utf8();
      return std::string(utf8.data(), utf8.length());
    }

    inline const char* Stringify(ultralight::MessageSource source) {
      switch(source) {
        case ultralight::MessageSource::kMessageSource_XML: return "XML";
        case ultralight::MessageSource::kMessageSource_JS: return "JS";
        case ultralight::MessageSource::kMessageSource_Network: return "Network";
        case ultralight::MessageSource::kMessageSource_ConsoleAPI: return "ConsoleAPI";
        case ultralight::MessageSource::kMessageSource_Storage: return "Storage";
        case ultralight::MessageSource::kMessageSource_AppCache: return "AppCache";
        case ultralight::MessageSource::kMessageSource_Rendering: return "Rendering";
        case ultralight::MessageSource::kMessageSource_CSS: return "CSS";
        case ultralight::MessageSource::kMessageSource_Security: return "Security";
        case ultralight::MessageSource::kMessageSource_ContentBlocker: return "ContentBlocker";
        case ultralight::MessageSource::kMessageSource_Other: return "Other";
        default: return "";
      }
    }
    
    inline const char* Stringify(ultralight::MessageLevel level) {
      switch(level) {
        case ultralight::MessageLevel::kMessageLevel_Log: return "Log";
        case ultralight::MessageLevel::kMessageLevel_Warning: return "Warning";
        case ultralight::MessageLevel::kMessageLevel_Error: return "Error";
        case ultralight::MessageLevel::kMessageLevel_Debug: return "Debug";
        case ultralight::MessageLevel::kMessageLevel_Info: return "Info";
        default: return "";
      }
    }
     
    void ViewWrapper::OnAddConsoleMessage(ultralight::View* caller,
                                          ultralight::MessageSource source,
                                          ultralight::MessageLevel level,
                                          const ultralight::String& message,
                                          uint32_t line_number,
                                          uint32_t column_number,
                                          const ultralight::String& source_id) {

        //juce::Logger::writeToLog("UltralightComponent::OnAddConsoleMessage");

        std::cout << "[Console]: [" << Stringify(source) << "] ["
                  << Stringify(level) << "] " << ToUTF8(message);

        if (source == ultralight::MessageSource::kMessageSource_JS) {
            std::cout << " (" << ToUTF8(source_id) << " @ line " << line_number
                      << ", col " << column_number << ")";
        }

        std::cout << std::endl;
    }
}
