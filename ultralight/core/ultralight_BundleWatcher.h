


//==============================================================================
// Helper class which watches JS bundle files for changes and triggers
// a user supplied callback in the event of a bundle file change.
// BundleWatcher is able to handle multiple bundles to support loading
// of multiple bundle files by EcmaScriptEngine in the future.
class BundleWatcher : private juce::Timer
{
    struct WatchedBundle
    {
        juce::File bundle;
        juce::Time bundleLastModifiedTime;
    };

public:
    using BundleChangedCallback = std::function<void(const juce::File&)>;

    explicit BundleWatcher(BundleChangedCallback onBundleChangedCallback)
            : onBundleChanged(std::move(onBundleChangedCallback))
    {
        startTimer(50);
    }

    void watch(const juce::File& bundle)
    {
        watchedBundles.push_back({ bundle, bundle.getLastModificationTime() });
    }

    bool watching(const juce::File &bundle) const
    {
        return std::find_if(  watchedBundles.cbegin()
                , watchedBundles.cend()
                , [=](const WatchedBundle& wb)
                              {
                                  return wb.bundle.getFullPathName() == bundle.getFullPathName();
                              }) != watchedBundles.cend();
    }

private:
    void timerCallback() override
    {
        std::for_each(watchedBundles.begin(), watchedBundles.end(), [=] (WatchedBundle& wb)
        {
            // Sanity check
            jassert(wb.bundle.existsAsFile());

            if (wb.bundle.existsAsFile())
            {
                const auto lmt = wb.bundle.getLastModificationTime();

                // In some instances webpack rebuilds temporarily result in an empty
                // bundle file. We do not want to trigger evaluation callbacks when the
                // bundle file is empty as this is likely to cause errors when users attempt
                // to call javascript functions from C/C++. Calling eval/call in duktape with
                // an empty js file/string does not result in an error and simply pushes undefined
                // to the top of the duktape stack.
                if (lmt > wb.bundleLastModifiedTime && wb.bundle.loadFileAsString().isNotEmpty())
                {
                    onBundleChanged(wb.bundle);
                    wb.bundleLastModifiedTime = lmt;
                }
            }
        });
    }

    std::vector<WatchedBundle> watchedBundles;
    BundleChangedCallback      onBundleChanged;
};

