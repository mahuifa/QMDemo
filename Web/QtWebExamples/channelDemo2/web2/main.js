/**
 * 程序启动立即初始化
 */
window.onload = function()
{
    if(typeof qt != "undefined")
    {
        window.channel = new QWebChannel(qt.webChannelTransport, function(channel)
        {
            // 获取Qt注册的对象，Qt中registerObject注册的字符串
            window.core = channel.objects.CoreId;

            // 将函数showText和Qt信号toWeb***()绑定
            core.toWebBool.connect(function(msg)
            {
                showText(msg);
            });
            core.toWebDouble.connect(function(msg)
            {
                showText(msg);
            });
            core.toWebString.connect(function(msg)
            {
                showText(msg);
            });
            core.toWebJsonArray.connect(function(msg)
            {
                showText(msg);
            });
            core.toWebJsonObject.connect(function(msg)
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

/**
 * 显示Qt发给Web的信息
 * @param {*} msg 
 */
function showText(msg)
{
    var textEdit = document.getElementById("textAreaId");
    if(textEdit)
    {
        // 由于typeof 不能分辨出数组和对象，所以这里使用构造器来判断
        if(msg.constructor === Array)         // 数组类型
        {
            textEdit.value = textEdit.value + "Array：" + msg+ '\n';                    // 追加信息
        }
        else if(msg.constructor === Object)  // 对象类型
        {
            var str = msg.key1 + " " + msg.key2+ " " + msg.key3;
            textEdit.value = textEdit.value + "Object：" + str+ '\n';                   // 追加信息
        }
        else
        {
            textEdit.value = textEdit.value + typeof msg  + "：" + msg+ '\n';           // 追加信息
        }
        
        textEdit.scrollTop = textEdit.scrollHeight;             // 滚动条一直再最下方
    }
}

/**
 * html中按键点击时调用这个函数将信号发送给Qt
 */
function butClick_bool()
{
    core.on_toQtBool(true);    
}
function butClick_double()
{
    core.on_toQtDouble(123.321);    
}
function butClick_str()
{
    core.on_toQtString("Web 按键点击");    
}
function butClick_arr()
{
    var arr = [1, 2, "123"];
    core.on_toQtJsonArray(arr);   
}
function butClick_obj()
{
    var obj = {
        key1 : 123,
        key2 : 321.1,
        key3 : "abc"
    };
    core.on_toQtJsonObject(obj);   
}

/**
 * 点击按键后直接读取Qt中注册对象Core中使用Q_PROPERTY定义的属性值并修改
 */
function butClick_Property()
{
    var textEdit = document.getElementById("textAreaId");
    if(textEdit)
    {
        textEdit.value = textEdit.value + "Qt对象属性：" + core.value + '\n';              // 读取Qt中使用Q_PROPERTY定义的属性值
        textEdit.scrollTop = textEdit.scrollHeight;             // 滚动条一直再最下方
        core.value++;                                                                     // 修改Qt中使用Q_PROPERTY定义的属性值
    }
}

/**
 * 直接访问Qt中使用Q_ENUM标记的枚举
 */
function butClick_enum()
{
    var textEdit = document.getElementById("textAreaId");
    if(textEdit)
    {
        textEdit.value = textEdit.value + "Qt枚举值：" + core.CoreEnum.Value1 + '\n';  
        textEdit.scrollTop = textEdit.scrollHeight;             // 滚动条一直再最下方
    }
}

/**
 * 在javascript中调用Qt函数后【获取返回值】
 */
function butClick_return()
{
    core.on_returnValue(123, function(returnValue)  // 这里使用回调函数获取返回值
    {
        var textEdit = document.getElementById("textAreaId");
        if(textEdit)
        {
            textEdit.value = textEdit.value + "Qt函数返回值：" + returnValue + '\n';  
            textEdit.scrollTop = textEdit.scrollHeight;             // 滚动条一直再最下方
        }
    });
}