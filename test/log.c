#include <jsx/log.h>

#include <assert.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    assert(jsx_log_level() == JsxLogLevelInfo);

    jsx_log_set_level(JsxLogLevelTrace);
    jsx_log_enable_feature(JsxLogFeatureTimestamps);

    // Four messages in different colors should be printed.
    jsx_log_info("This is an info-level message, used for general communication");
    jsx_log_debug("This is a debug message, used to add detail");
    jsx_log_trace("This is a trace message, used for providing granular context");
    jsx_log_error("This is an error message, for when things go wrong");

    jsx_log_disable_feature(JsxLogFeatureHighlighting);

    // Should print with no color now that the feature is disabled.
    jsx_log_trace("This is a also trace message, but should have no color");

    jsx_log_set_level(JsxLogLevelDebug);

    // Should not print now that trace messages are disabled.
    jsx_log_trace("This trace message should be hidden");

    return 0;
}
