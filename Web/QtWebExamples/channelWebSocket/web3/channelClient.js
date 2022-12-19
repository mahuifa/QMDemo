// 关于WebSocket的用法可以看【https://developer.mozilla.org/zh-CN/docs/Web/API/WebSocket】

/**
 * 创建一个程序加载时自动执行的函数
 * 注意：由于该函数执行时WebSocket会连接Qt中的WebSocket服务端，WebChannel会获取Qt中注册的对象
 *      所以需要先启动Qt程序，再启动当前web程序，否则无法连接(也可以写成手动调用)
 */
window.onload = function() 
{
    var url = "ws://localhost:1234";          // WebSocket服务端地址
    var client = new WebSocket(url);          

    /**
     * 连接关闭后的回调函数
     */
    client.onclose = function() 
    {
        showMessage("关闭连接");
    }

    /**
     * 连接失败后的回调函数。
     */
    client.onerror = function(err)
    {
        showMessage("连接失败：" + err.data);
    }

    /**
     * 连接成功后的回调函数
     */
    client.onopen = function()
    {
        showMessage("WebSocket连接成功");
        // 将创建的websocket客户端传递给QWebChannel，并绑定通信接口
        window.channel = new QWebChannel(client, function(channel)  
        {
            // 获取Qt注册的对象，Qt中registerObject注册的字符串
            window.core = channel.objects.CoreId;  
            
            // 将Qt注册对象的qtButClicked信号绑定到readMessage函数
            core.qtButClicked.connect(readMessage);
            showMessage("初始化WebChannel完成");
        });
    }
}

/**
 * 显示接收到的信息
 */
function readMessage(message)
{
    showMessage("收到：" + message);
}

/**
 * 点击按键调用Qt注册对象中on_webButClicked槽函数将数据发送给Qt
 */
function sendMessage()
{
    var text = document.getElementById("input").value;
    if(!text)
    {
        return;
    }
    window.core.on_webButClicked(text);
}

/**
 * 将信息显示到文本框
 */
function showMessage(msg)
{
    var textShow = document.getElementById("textShowId");
    textShow.value += msg + "\n";              // 追加信息
    textShow.scrollTop = textShow.scrollHeight;    // 滚动条显示最下方
}
