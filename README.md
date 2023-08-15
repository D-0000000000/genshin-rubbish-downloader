# 比NGA还难用的原神下载器

顾名思义，这个下载器可以给你下载原神，不过下载的不是启动器，是本体。在枫丹更新前的晚上，肝了几个小时肝出来了一个非常难用的版本。不排除4.0更新后这个下载器就不能用了的可能性。我倒要看看是我先原神弃坑还是米忽悠把原神这傻了吧唧没有```config.ini```就不知道游戏在哪安装时非要给你下个好几个分卷压缩包然后一起解压的启动器塞回娘胎里重新出生一遍。

## 功能

下载原神

### 优点

不需要双倍解压空间。

### 缺点

一个词概括就是难用  
首先要有```python3```和```curl```  
下载完成后不会生成能让傻了吧唧没有```config.ini```就不知道游戏在哪安装时非要给你下个好几个分卷压缩包然后一起解压的启动器发现游戏本体的```config.ini```。

资源消耗非常。下载用```curl```非常吃资源

## 用法

首先安装```python3```和```curl```  

找到先找到游戏本体下载链接，这个我不会，NGA看到有人发了个链接，后面不知道链接还会不会变，变了我就没办法了

```text
https://sdk-static.mihoyo.com/hk4e_cn/mdk/launcher/api/resource?key=eYd89JmJ&launcher_id=18
```

这是个```json```，找到```data - game - latest - decompressed_path```，这个链接就是原神本体的单独文件下载链接

找到下载器的```config.json```配置，把上面得到的链接填到```url```里。```pkg```里改成你想要的包，```pkg_version```是本体，```Audio_Chinese_pkg_version```是中文语音，别的我不知道，叫啥应该能在自己的本体目录里找到。```thread```是下载的时候要同时开多少个```curl```

```Shell
python3 genshin-downloader.py
```

下载完成后需要手动生成```config.ini```不知道怎么写的可以照着我这个配置改一下

```ini
[General]
cps=mihoyo
channel=1
game_version=3.8.0
plugin_sdk_version=3.8.0
sub_channel=1
```

## FAQ

Q: 为啥这下载器一部分Python一部分C++？  
A: 因为我Python不会多线程C++不会处理json。  

Q: 好难用啊，能再改改吗？  
A: 又不是不能用，摆烂.jpg  
