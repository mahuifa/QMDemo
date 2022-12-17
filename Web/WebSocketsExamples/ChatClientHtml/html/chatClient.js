// 关于WebSocket的用法可以看【https://developer.mozilla.org/zh-CN/docs/Web/API/WebSocket】
var g_client = null;     // WebSocket客户端对象

/**
 * 控制WebSocket连接或者断开
 */
function socketConnectOrDis()
{
    var text = document.getElementById("but_connect").innerHTML;
    if(text === "连接")
    {
        connectWebSocket();
    }
    else
    {
        disConnectWebSocket();
    }
}

/**
 * WebSocket连接
 */
function connectWebSocket() 
{
    try
    {
        if(g_client && g_client.readyState === WebSocket.OPEN)  // 如果g_client不为空，并且为连接状态
        {
            g_client.close();  // 关闭连接
        }
        var url = document.getElementById("url").value;
        g_client = new WebSocket(url);      // 创建一个WebSocket客户端，并连接
        /**
         * 连接成功触发事件执行的回调函数
         */
        g_client.onopen = function(evt)   
        {
            showMessage("连接成功！");
            document.getElementById("but_connect").innerHTML = "断开连接";
            document.getElementById("but_send").disabled = false;        // 按键使能
        }
        /**
         * 关闭连接触发事件执行的回调函数
         */
        g_client.onclose = function(evt)
        {
            showMessage("关闭连接");
            document.getElementById("but_connect").innerHTML = "连接";
            document.getElementById("but_send").disabled = true;
        }
        /**
         * 接收到信息的回调函数
         */
        g_client.onmessage = function(evt)
        {
            showMessage("收到：" + evt.data);
        }
        /**
         * 连接失败后的回调函数
         */
        g_client.onerror = function(evt)
        {
            showMessage("Error：" + evt.data);
        }
    }
    catch(exception)   // 捕获异常信息
    {
        showMessage("Error：" + exception);
    }
}

/**
 * 断开连接
 */
function disConnectWebSocket()
{
    if(g_client)
    {
        g_client.close();
    }
}

function getState()
{
    if(g_client)
    {
        var state;
        switch(g_client.readyState)
        {
            case WebSocket.CONNECTING:
            {
                state = "连接中";
                break;
            }
            case WebSocket.OPEN:
            {
                state = "已连接";
                break;
            }
            case WebSocket.CLOSING:
            {
                state = "正在关闭连接";
                break;
            }
            case WebSocket.CLOSED:
            {
                state = "未连接";
                break;
            }
            default:
            {
                state = "未知";
                break;
            }
        }
        showMessage("客户端当前状态：" + state);
    }
    else
    {
        showMessage("客户端未初始化");
    }
}

/**
 * 显示信息
 */
function showMessage(message) 
{
    var textShow = document.getElementById("textShowId");
    textShow.value += message + "\n";              // 追加信息
    textShow.scrollTop = textShow.scrollHeight;    // 滚动条显示最下方
}

/**
 * 发送信息
 */
function sendMessage()
{
    if(g_client != null)
    {
        var nick = document.getElementById("nick").value;       // 获取昵称
        var msg = document.getElementById("inputText").value;   // 获取发送的数据
        var strToSend = nick + ":" + msg;
        g_client.send(strToSend);
        showMessage("发送：" + strToSend);
    }
}
