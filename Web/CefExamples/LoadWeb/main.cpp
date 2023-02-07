#include "widget.h"

#include <QApplication>
#include <QCefContext.h>

void initCefConfig(QCefConfig& config)
{
    config.setUserAgent("QCefViewTest");                        // 设置用户代理
    config.setLogLevel(QCefConfig::LOGSEVERITY_DEFAULT);        // 设置日志级别
    config.setBridgeObjectName("CallBridge");                   // 设置网桥对象名称
    config.setRemoteDebuggingPort(9000);                        // 设置远程调试端口
    config.setBackgroundColor(QColor::fromRgba(qRgba(255, 0, 0, 0)));  // 设置网页的背景色

    /* 向用于初始化CEF的命令行参数添加开关
     * 可用命令行可以看
     * https://bitbucket.org/chromiumembedded/cef/src/master/libcef/common/cef_switches.cc
     * https://bitbucket.org/chromiumembedded/cef/src/master/tests/shared/common/client_switches.cc
     * https://peter.sh/experiments/chromium-command-line-switches/
     */
    /*
     * 用于启用媒体流（Media Stream）功能。Media Stream 是 HTML5 中的一项新特性，
     * 它允许浏览器通过 WebRTC（Web Real-Time Communication）技术实现音频、视频、屏幕共享等功能，为实时通信提供了强大的支持。
     * 例如，通过 Media Stream，可以实现浏览器端的视频会议、语音聊天、桌面共享等功能。
     * 默认情况下，Chromium 浏览器已经启用了 Media Stream 功能，因此一般情况下不需要添加 enable-media-stream 参数。
     * 但如果您在使用 Chromium 浏览器时遇到了一些与媒体流相关的问题，例如无法共享屏幕或摄像头等，可能需要检查该参数是否被正确设置。
     */
    config.addCommandLineSwitch("enable-media-stream");
    /*
     * use-mock-keychain 是 macOS 中的一个命令行参数，用于在模拟环境下使用 Keychain，而不是使用真实的 Keychain。Keychain 是 macOS 中的一项安全功能，用于存储和管理用户的密码、证书和其他敏感信息。
     * 应用程序可以通过 Keychain API 访问这些信息，从而实现自动登录、自动填充表单等功能。在模拟环境下，如果不想使用真实的 Keychain，可以通过添加 use-mock-keychain 参数来使用模拟环境中的 Keychain。
     * 在使用该参数时，需要注意以下几点：
     * 仅在模拟环境下使用该参数。在实际的生产环境中，应当使用真实的 Keychain。
     * 使用模拟 Keychain 时，存储的密码和其他敏感信息仅存储在内存中，不会写入到硬盘中，因此不会在多次启动应用程序之间保留。
     * 模拟 Keychain 中存储的信息不会被其他应用程序访问，因此仅适用于单个应用程序的测试和开发场景。
     */
    config.addCommandLineSwitch("use-mock-keychain");
    /*
     * 用于允许浏览器访问本地文件系统。默认情况下，Chromium 浏览器禁止通过 file:// 协议访问本地文件系统中的文件，以防止恶意脚本窃取本地文件信息。但在某些情况下，用户可能需要使用 file:// 协议访问本地文件系统，例如在本地开发网页或调试 JavaScript 代码时。
     * 通过添加 allow-file-access-from-files 参数，可以在 Chromium 浏览器中启用对 file:// 协议的访问权限。需要注意的是，在启用该参数时，浏览器的安全性可能会受到一定程度的降低，因为恶意脚本可能会通过 file:// 协议访问本地文件系统中的敏感信息。
     * 因此建议仅在必要情况下使用该参数，并在使用时加强对本地文件系统的保护和监控。
     */
    config.addCommandLineSwitch("allow-file-access-from-files");
    /*
     * 用于禁用拼写检查功能。拼写检查是浏览器的一项功能，用于检查用户在输入表单、文本框等地方输入的文本是否存在拼写错误，并给出相应的纠正建议。如果用户需要输入一些特殊的术语、专业名词等，拼写检查可能会对输入造成干扰。
     * 因此，如果您使用 Chromium 浏览器时需要输入一些特殊的术语、专业名词等，可能会通过添加 disable-spell-checking 参数来禁用拼写检查功能，以避免输入干扰。但需要注意的是，禁用拼写检查功能可能会导致输入错误的单词无法被及时发现和纠正，因此建议仅在必要情况下使用该参数。
     */
    config.addCommandLineSwitch("disable-spell-checking");
    /*
     * 用于禁用站点隔离试验功能。站点隔离是一项安全功能，它能够将不同站点的网页内容隔离开来，防止恶意网站通过跨站点脚本攻击（XSS）等方式盗取用户的敏感信息。
     * 站点隔离试验是一种用于测试站点隔离功能的实验性功能，它可能会导致某些网页无法正常加载或出现其他问题。
     * 因此，如果您使用 Chromium 浏览器时遇到了一些网页加载问题，可能可以通过添加 disable-site-isolation-trials 参数来禁用站点隔离试验功能，以解决这些问题。
     * 不过需要注意的是，禁用站点隔离试验功能可能会降低浏览器的安全性，因此建议仅在必要情况下使用该参数。
     */
    config.addCommandLineSwitch("disable-site-isolation-trials");
    /*
     * 用于启用 DOM 存储数据的积极刷新。
     * DOM 存储是一种浏览器提供的机制，用于在客户端存储数据，包括 localStorage 和 sessionStorage。
     * 默认情况下，当页面的 DOM 存储发生更改时，浏览器会在适当的时候将这些更改写入到磁盘中。
     * 但是，由于写操作可能会影响性能，因此浏览器不会立即将更改写入磁盘，而是将多个更改缓冲在内存中，直到达到一定的阈值才一次性写入磁盘。
     * 通过添加 enable-aggressive-domstorage-flushing 参数，可以启用 DOM 存储数据的积极刷新，即每次发生更改时立即将更改写入磁盘，
     * 以确保数据的及时保存。需要注意的是，启用该参数可能会影响浏览器的性能，因为频繁的磁盘写入操作会增加系统的负载。因此建议仅在必要情况下使用该参数，并在使用时加强对系统资源的监控和管理。
     */
    config.addCommandLineSwitch("enable-aggressive-domstorage-flushing");
    // 将具有值的开关添加到用于初始化CEF的命令行参数中
    /*
     * 用于限制每个站点（域名）可以使用的渲染进程数量。
     * 在 Chromium 浏览器中，每个站点可以使用多个渲染进程，以提高浏览器的性能和稳定性。
     * 但是，如果某个站点占用了过多的渲染进程，可能会导致其他站点无法正常渲染，甚至导致浏览器崩溃。
     * 通过添加 renderer-process-limit 参数，可以限制每个站点可以使用的渲染进程数量，以确保每个站点都能够合理地使用系统资源。
     * 需要注意的是，该参数的默认值为 0，表示不限制渲染进程数量，因此需要手动设置该参数的值，才能实现对渲染进程数量的限制。
     * 建议在使用该参数时，根据系统资源和浏览器的实际情况，适当地调整渲染进程数量的限制。
     */
    config.addCommandLineSwitchWithValue("renderer-process-limit", "1");
    /*
     * 用于禁用某些浏览器功能。Chromium 浏览器中提供了许多功能和特性，有些用户可能不需要或者不喜欢某些功能，因此可以通过添加 disable-features 参数来禁用这些功能。
     * 该参数的语法为：--disable-features=<feature1>[,<feature2>,...]
     * 其中，<feature> 是要禁用的功能的名称，多个功能名称之间使用逗号分隔。
     * 例如，要禁用 Chromium 浏览器中的 JavaScript，可以使用以下命令：
     * chrome --disable-features=JavaScript
     * 需要注意的是，禁用某些功能可能会影响浏览器的功能和性能，因此建议仅在必要情况下使用该参数，并根据实际情况选择要禁用的功能。
     */
    config.addCommandLineSwitchWithValue("disable-features", "BlinkGenPropertyTrees,TranslateUI,site-per-process");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // 使用config初始化QCefContext实例
    QCefConfig config;
    initCefConfig(config);
    QCefContext cefContext(&a, argc, argv, &config);
    // 获取可执行程序所在的路径，然后添加 bin 目录到库的搜索路径中

    Widget w;
    w.show();
    return a.exec();
}
