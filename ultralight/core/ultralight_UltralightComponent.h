#pragma once

#include <codecvt>
#include <optional>

#include "../../dependencies/ultralight-sdk/include/Ultralight/Ultralight.h"
//#include "../../dependencies/ultralight-sdk/include/AppCore/JSHelpers.h"
#include "../../dependencies/ultralight-sdk/include/Ultralight/CAPI.h"
#include "../../dependencies/ultralight-sdk/include/AppCore/AppCore.h"
//#include "../../dependencies/ultralight-sdk/include/JavaScriptCore/JavaScript.h"

// Redefine with ultralight namespace
#define BindJSCallbackWithRetval(fn) (ultralight::JSCallbackWithRetval)std::bind(fn, this, std::placeholders::_1, std::placeholders::_2)
typedef std::function<ultralight::JSObject(const ultralight::JSObject&, const ultralight::JSArgs&)> JSCallbackWithRetobj;
#define BindJSCallbackWithRetobj(fn) (JSCallbackWithRetobj)std::bind(fn, this, std::placeholders::_1, std::placeholders::_2)

namespace juce_ultralight {

    const char *htmlString();

    class UltralightComponent;

    /**
     * A JUCE ImageComponent which wraps an Ultralight view
     */
    class ViewWrapper
            : public juce::ImageComponent,
              public ultralight::ViewListener {
    public:

                  ViewWrapper(ultralight::RefPtr<ultralight::Renderer> renderer,
                              ultralight::RefPtr<ultralight::View> view,
                    bool isInspector) : isInspector(isInspector), view(view) {

            /*
            if (isInspector) {

            } else {
            }
            */

            setWantsKeyboardFocus(true);

            /*
            */

            view->Focus();

            //view->set_load_listener(this);
            view->set_view_listener(this);
        };

        ~ViewWrapper() {};

        void loadURL(ultralight::String url) {

            view->LoadURL(url);
        }

        void resized() override
        {
            auto area = getLocalBounds();

            juce::Logger::writeToLog("UltralightComponent::resized " + juce::String(area.getWidth()) + " " + juce::String(area.getHeight()));
            view->Resize(area.getWidth(), area.getHeight());
            JuceImageSurface *surface = (JuceImageSurface *) view->surface();
            setImage(*(surface->image));
            ImageComponent::resized();
        }

        void mouseDown(const juce::MouseEvent &juceEvent) override {

            ultralight::MouseEvent event;
            event.type = ultralight::MouseEvent::kType_MouseDown;

            mouseButton(juceEvent, event);

            //juce::Logger::writeToLog("UltralightComponent mouseDown " + juce::String(event.x) + " " + juce::String(event.y));

            view->FireMouseEvent(event);
        }

        void mouseUp(const juce::MouseEvent &juceEvent) override {

            ultralight::MouseEvent event;
            event.type = ultralight::MouseEvent::kType_MouseUp;

            mouseButton(juceEvent, event);

            view->FireMouseEvent(event);
        }

        void mouseButton(const juce::MouseEvent &juceEvent, ultralight::MouseEvent &event) {

            juce::Point currentPosition = juceEvent.getPosition();
            event.x = juce::roundToInt(currentPosition.getX() / 2.);
            event.y = juce::roundToInt(currentPosition.getY() / 2.);

            if (juceEvent.mods.isLeftButtonDown()) {
                //juce::Logger::writeToLog("Left");
                event.button = ultralight::MouseEvent::kButton_Left;
            }
            if (juceEvent.mods.isRightButtonDown()) {
                //juce::Logger::writeToLog("Right");
                event.button = ultralight::MouseEvent::kButton_Right;
            }
        }

        void mouseMove(const juce::MouseEvent &juceEvent) override {
            ultralight::MouseEvent event;
            event.type = ultralight::MouseEvent::kType_MouseMoved;
            juce::Point currentPosition = juceEvent.getPosition();
            event.x = juce::roundToInt(currentPosition.getX() / 2.);
            event.y = juce::roundToInt(currentPosition.getY() / 2.);

            view->FireMouseEvent(event);
        }

        void mouseDrag(const juce::MouseEvent &juceEvent) override {
            mouseMove(juceEvent);
        }

        bool keyPressed(const KeyPress &key) override {

            ultralight::KeyEvent evt;
            evt.modifiers = 0;

            switch (key.getKeyCode()) {

                case 13: // Enter key
                    evt.type = ultralight::KeyEvent::kType_RawKeyDown;
                    evt.virtual_key_code = evt.native_key_code = ultralight::KeyCodes::GK_RETURN;
                    break;
                case 127: // Backspace
                    evt.type = ultralight::KeyEvent::kType_RawKeyDown;
                    evt.virtual_key_code = evt.native_key_code = ultralight::KeyCodes::GK_BACK;
                    break;
                case 63232: // Up arrow
                    evt.virtual_key_code = evt.native_key_code = ultralight::KeyCodes::GK_UP;
                case 63233: // Down arrow
                    evt.virtual_key_code = evt.native_key_code = ultralight::KeyCodes::GK_DOWN;
                case 63234: // Left arrow
                    evt.virtual_key_code = evt.native_key_code = ultralight::KeyCodes::GK_LEFT;
                case 63235: // Right arrow
                    evt.virtual_key_code = evt.native_key_code = ultralight::KeyCodes::GK_RIGHT;
                    evt.type = ultralight::KeyEvent::kType_RawKeyDown;
                    break;
                default:
                    juce::String juceChar = juce::String::charToString(key.getTextCharacter());
                    ultralight::String ultralightChar(juceChar.toRawUTF8());
                    evt.text = ultralightChar;
                    evt.unmodified_text = ultralightChar;
                    break;
            }

            GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);

            view->FireKeyEvent(evt);
            return true;
        }

        void OnAddConsoleMessage(ultralight::View *caller,
                                 ultralight::MessageSource source,
                                 ultralight::MessageLevel level,
                                 const ultralight::String &message,
                                 uint32_t line_number,
                                 uint32_t column_number,
                                 const ultralight::String &source_id) override;

        bool isInspector;

        ultralight::RefPtr<ultralight::View> view;
    };

    class UltralightComponent
        : public juce::Component,
          public juce::Timer,
          public ultralight::LoadListener,
          public AudioProcessorValueTreeState::Listener
    {
    public:
        //==============================================================================
        UltralightComponent(AudioProcessorValueTreeState& apvts, bool showInspector=false)
            : initialised(false),
              showInspector(showInspector),
              apvts(apvts)
        {
            JUCE_ASSERT_MESSAGE_THREAD

            initialiseUltralight();
            
            //if (parametersEnabled)

            tick();
            tick();
            /*
            // Skirt one side of the inheritance diamond of death via Component
            juce::MouseListener* listener = static_cast<juce::Component*>(this);
            display.addMouseListener(listener, true);
            inspectorDisplay.addMouseListener(listener, true);
            */
        }

        ~UltralightComponent()
        {
            //app = nullptr;
        }

        void setVisible(bool value) override
        {
            juce::Logger::writeToLog("UltralightComponent::setVisible");
            if (!initialised) {
                initialise();
                initialised = true;
            }
            startTimer(50);
            juce::Component::setVisible(value);
        }

        virtual void initialise()
        {

            juce::Logger::writeToLog("UltralightComponent::initialise");
            //(*globalObject)["getFirebase"] = BindJSCallbackWithRetval(&UltralightComponent::getFirebase);
        }

        void initialiseUltralight()
        {

            // Ultralight
            factory = std::make_unique<JuceImageSurfaceFactory>();
            ultralight::Platform::instance().set_surface_factory(factory.get());

            ultralight::Settings settings;
            ultralight::Config config;
            config.scroll_timer_delay = 1.0 / 90.0;

            config.device_scale = 2.0;
            config.font_family_standard = "Arial";
            config.resource_path = "./resources/";
            config.use_gpu_renderer = false;

            ultralight::Platform::instance().set_config(config);

            ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
            ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
            ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));

            renderer = ultralight::Renderer::Create();

        //    createMainView(1200, 400);
        //    createInspectorView(1200, 400);
        //}

        //void createMainView(int width, int height)
        //{
            ultralight::RefPtr<ultralight::View> main = renderer->CreateView(1200, 400, false, nullptr);
            main->set_load_listener(this);
            //ViewWrapper mainView(renderer, main, false);
            mainView = std::make_unique<ViewWrapper>(renderer, main, false);
            //addAndMakeVisible(*mainView);
        //}

        //void createInspectorView(int width, int height)
        //{
            juce::Logger::writeToLog("UltralightComponent::createInspectorView");

            ultralight::RefPtr<ultralight::View> inspector = main->inspector();
            inspector->Resize(1200, 400);
            inspector->Focus();
            inspectorView = std::make_unique<ViewWrapper>(renderer, inspector, true);
            //addAndMakeVisible(*inspectorView);

            splitComponent = std::make_unique<SplitComponent>(*mainView, *inspectorView, false);
            //splitComponent->SetSplitBarPosition(int(getHeight() / 2.0));

            if (showInspector)
            {
                addAndMakeVisible(*splitComponent);
            } else {

                addAndMakeVisible(*mainView);
            }
        }

        void loadURL(ultralight::String url) {

            mainView->loadURL(url);
            tick();
        }

        void sendToJS()
        {

            mainView->view->EvaluateScript("toJS('testing')");
        }

        void tick()
        {

            renderer->Update();
            renderer->Render();
            inspectorView->repaint();
            mainView->repaint();
            splitComponent->repaint();
        }

        void resized() override
        {
            auto area = getLocalBounds();
            if (showInspector)
            {
                splitComponent->setBounds(area);
                splitComponent->SetSplitBarPosition(int(getHeight() / 2.0));
            } else {

                mainView->setBounds(area);
                //inspectorView->setBounds(area.removeFromBottom (400));
            }
        }

        void timerCallback() override
        {
            tick();
        }

        virtual void OnWindowObjectReady(ultralight::View* caller,
                                         uint64_t frame_id,
                                         bool is_mainView_frame,
                                         const ultralight::String& url) override
        {

            ///
            /// Set our View's JSContext as the one to use in subsequent JSHelper calls
            ///
            ultralight::Ref<ultralight::JSContext> context = caller->LockJSContext();
            SetJSContext(context.get());

            ///
            /// Get the global object (this would be the "window" object in JS)
            ///
            globalObject = std::make_unique<ultralight::JSObject>(ultralight::JSGlobalObject());

            if (checker.shouldBailOut())
                return;

            setupParameters();

            listeners.callChecked (checker, [&] (UltralightComponentListener& l) {
                l.windowObjectReady(caller, frame_id, is_mainView_frame, url);
            });

            if (checker.shouldBailOut())
                return;
        }

        void setupParameters()
        {
            juce::Logger::writeToLog("UltralightComponent::setupParameters");
            (*globalObject)["cpp_getParameter"] = BindJSCallbackWithRetval(&UltralightComponent::cpp_getParameter);
            //(*globalObject)["cpp_getParameter"] = BindJSCallbackWithRetobj(&UltralightComponent::cpp_getParameter);
            //registerNativeMethod ("cpp_getParameter", BindJSCallbackWithRetobj(&UltralightComponent::cpp_getParameter));
            registerNativeMethod ("cpp_setParameterValue", BindJSCallbackWithRetval(&UltralightComponent::cpp_setParameterValue));
            //registerNativeMethod ("cpp_subscribeToParameter", BindJSCallbackWithRetval(&UltralightComponent::cpp_subscribeToParameter));
            
            apvts.addParameterListener("gain", this);
        }

        void setProperty(JSContextRef context, JSObjectRef obj, std::string _key, JSValueRef value)
        {

            JSStringRef key = JSStringCreateWithUTF8CString(_key.c_str());
            //JSValueRef value = JSValueMakeNumber(context, 3);
            JSObjectSetProperty(context, obj, key, value, kJSPropertyAttributeNone, NULL);
            // Doesn't seem like we need to release JSValueRefs (https://github.com/ultralight-ux/WebCore/blob/64ab2718147af72c98de4baebcfc843d7b2e9356/Source/JavaScriptCore/API/tests/testapi.c#L1446)
            JSStringRelease(key);
        }

        ultralight::JSValue cpp_getParameter(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
        {

            
            juce::Logger::writeToLog("UltralightComponent::cpp_getParameter");
            std::string name = convertToString(args[0]);
            AudioParameterFloat* parameter = dynamic_cast<AudioParameterFloat*>(apvts.getParameter(name));

            //ValueType min = parameter->range.start;
            //float value = parameter->get();
            //ValueType max = parameter->range.end;
            Range<float> range = parameter->getNormalisableRange().getRange();
            //float start = range.getStart();
            //float end = range.getEnd();

            JSContextRef context = mainView->view->LockJSContext().get();
            JSObjectRef param = JSObjectMake(context, NULL, NULL);

            setProperty(context, param, "min", JSValueMakeNumber(context, range.getStart()));
            setProperty(context, param, "value", JSValueMakeNumber(context, parameter->get()));
            setProperty(context, param, "max", JSValueMakeNumber(context, range.getEnd()));

            return ultralight::JSValue(param);
        }

        ultralight::JSValue cpp_setParameterValue(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
        {

            juce::Logger::writeToLog("cpp_setRawParameterValue");
            std::string name = convertToString(args[0]);
            juce::Logger::writeToLog(name);

            double value = args[1].ToNumber();
            AudioParameterFloat* parameter = dynamic_cast<AudioParameterFloat*>(apvts.getParameter(name));
            parameter->setValueNotifyingHost(value);
            //parameter->setValue(value);
            //parameter->sendValueChangedMessageToListeners(value);
            //parameter = 0.8f;
            //juce::SliderParameterAttachment
            return ultralight::JSValue(false);
        }

        /*
        ultralight::JSValue cpp_subscribeToParameter(const ultralight::JSObject& thisObject, const ultralight::JSArgs& args)
        {

            std::string name = convertToString(args[0]);
            //AudioParameterFloat* parameter = dynamic_cast<AudioParameterFloat*>(apvts.getParameter(name));
            //apvts.addParameterListener(name, this);

            juce::Logger::writeToLog("cpp_subscribeToParameter");
            juce::Logger::writeToLog(name);

            return ultralight::JSValue("Hello from C++!");
        }
        */

        void parameterChanged(const String& parameterId, float newValue) override
        {
            juce::Logger::writeToLog("parameterChanged");
            using string = std::string;
            //std::ostringstream call = string ("handleCppParameterChange(") << std::to_string (0.1f) << string (")");
            std::ostringstream jsCall;
            jsCall << "Parameters.onCppParameterChange('" << parameterId.toStdString().c_str() << "', " << newValue << ");";
            //jsCall << "Parameters.onCppParameterChange('" << parameterId.toStdString().c_str() << "', " << 0.1f << ");";

            //std::string jsCallString = jsCall.str();
            //juce::Logger::writeToLog(jsCallString);

            mainView->view->EvaluateScript(jsCall.str().c_str());
        }

        void registerNativeMethod(juce::String name, std::function<ultralight::JSValue(ultralight::JSObject const&, ultralight::JSArgs const&)> cb)
        {
            ultralight::String n(name.toStdString().c_str());
            (*globalObject)[n] = cb;
        }

        std::string convertToString(ultralight::JSValue val)
        {

            ultralight::String uId(val.ToString());
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
            std::string id = convert.to_bytes(uId.utf16().data());
            //return std::string(utf8.data(), utf8.length());
            return id;
        }

        void addListener(UltralightComponentListener* listener)
        {
            listeners.add(listener);
        }

    protected:

        bool initialised;
        bool showInspector;

        AudioProcessorValueTreeState& apvts;

        juce::ListenerList<UltralightComponentListener>::DummyBailOutChecker checker;
        juce::ListenerList<UltralightComponentListener> listeners;

        ultralight::RefPtr<ultralight::Renderer> renderer;
        ultralight::RefPtr<ultralight::Window> window;

        std::unique_ptr<ViewWrapper> mainView;
        std::unique_ptr<ultralight::JSObject> globalObject;
        std::unique_ptr<ViewWrapper> inspectorView;

        std::unique_ptr<JuceImageSurfaceFactory> factory;

        std::unique_ptr<SplitComponent> splitComponent;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UltralightComponent)
    };
}
