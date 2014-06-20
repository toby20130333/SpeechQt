SpeechQt
========

一个基于Qt的简单的语音识别项目
      首先，语音识别要做三件事情
      1.记录用户的语音文件到本地
      2.将用户语音编码 使用flac或者speex进行编码
      3.使用第三方语音识别API或者SDK进行分析识别语音
      目前做的比较简单就是使用flac文件对wav音频文件进行编码
      基于Mac OSX和Win 7平台的
      win 7下使用flac.exe，具体exe帮助，读者可以使用flac.exe --help > help.txt 重定向到一个help文件中，方便查阅.
      mac osx下面安装flac.dmg的安装包即可使用flac命令
      我们先看音频的录入
      Qt集成了音频模块

