# weather
### 项目描述
这是一个跨平台的桌面应用程序，旨在为用户提供实时天气信息、未来几天的天气预报以及一些额外的功能如空气质量指数、湿度等。该应用通过调用第三方天气API获取数据，并以直观的方式展示给用户。
![Markdown Logo](https://github.com/tsovke/weather/blob/master/Screenshot_20250310_140923.png)
### 功能特性
1. 实时天气信息

    当前温度、湿度、风速等。
    实时天气图标显示（晴天、多云、雨等）。

2. 一周天气预报

    一周天气预报。
    每日最高温度和最低温度，并绘制温度曲线。


### 技术实现
1. 界面设计

    使用Qt Designer创建用户界面。
    主界面包括：
        当前天气显示区域。
        一周天气预报显示。
2. 数据获取与处理

    使用QNetworkAccessManager进行网络请求，获取天气API返回的JSON数据。
    解析 JSON 数据并更新 UI。

### 项目成果
成果展示

    完整的跨平台天气预报应用程序，支持 Windows、Linux 和 macOS。
    直观易用的用户界面，提供丰富的天气信息。



