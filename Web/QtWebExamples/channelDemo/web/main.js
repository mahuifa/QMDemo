// 程序启动立即初始化
window.onload = function()
{
    if(typeof qt != "undefined")
    {
        window.channel = new QWebChannel(qt.webChannelTransport, function(channel)
        {
            // 获取Qt注册的对象，Qt中registerObject注册的字符串
            window.core = channel.objects.CoreId;
            // 将函数showText和Qt信号qtButClicked绑定
            core.qtButClicked.connect(function(msg)
            {
                showText(msg);
            });
        });
    }
    else
    {
        alert("qt对象未获取到！");
    }
}

// 显示信息
function showText(msg)
{
    var textEdit = document.getElementById("textAreaId");
    if(textEdit)
    {
        textEdit.value = textEdit.value + msg + '\n';           // 追加信息
        textEdit.scrollTop = textEdit.scrollHeight;             // 滚动条一直再最下方
    }
}

// html中按键点击时调用这个函数
function butClick()
{
    core.on_webButClicked("Web 按键点击");   // 调用Qt信号将js按键事件发送给Qt
}
