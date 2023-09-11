# 我超，电脑在自己下载原神

顾名思义，这个下载器可以给你下载原神，不过下载的不是启动器，是本体。在枫丹更新前的晚上，肝了几个小时肝出来了一个非常难用的版本。不排除4.0更新后这个下载器就不能用了的可能性。我倒要看看是我先原神弃坑还是米忽悠把原神这傻了吧唧没有```config.ini```就不知道游戏在哪安装时非要给你下个好几个分卷压缩包然后一起解压的启动器塞回娘胎里重新出生一遍。

我又更新了一下，没有那么难用了，但还是很难用

## 功能

下载原神

### 优点

不需要双倍解压空间。

### 缺点

有点难用  
现在不需要```python3```了，但还是需要```curl```，貌似Windows现在自带```curl```，需要能在环境变量中看到。  
下载完成后不会生成能让傻了吧唧没有```config.ini```就不知道游戏在哪安装时非要给你下个好几个分卷压缩包然后一起解压的启动器发现游戏本体的```config.ini```。

资源消耗非常。下载用```curl```非常吃资源

## 用法

本体下载链接好像不变，所以照着样例的```config.json```写一个就行。这个链接怎么获取的我不会，NGA看到有人发了个链接我就拿来用了，后面不知道链接还会不会变，变了我就没办法了。理论上崩坏星穹铁道也能用这个下载器，只要替换这个链接，但是我没有这个资源链接（摆手

```json
{
    "url_json": "https://sdk-static.mihoyo.com/hk4e_cn/mdk/launcher/api/resource?key=eYd89JmJ&launcher_id=18",
    "pkg": [
        "pkg_version",
        "Audio_Chinese_pkg_version",
        "Audio_English(US)_pkg_version",
        "Audio_Japanese_pkg_version",
        "Audio_Korean_pkg_version"
    ],
    "thread": 16
}
```

```url_json```的链接是个```json```，内含原神当前版本的所有下载链接。

找到下载器的```config.json```配置，把上面得到的链接填到```url```里。```pkg```里改成你想要的包，```pkg_version```是本体，后面都是语音包，不需要就删掉。```thread```是验证文件和下载的时候要同时开多少线程。

验证游戏文件并下载

```Shell
downloader.out verify
```

啥也不管直接下载

```Shell
downloader.out download
```

清除文件无关，注意会清楚游戏目录下的任何不在文件列表中的文件，包括游戏截图和能让傻了吧唧没有```config.ini```就不知道游戏在哪安装时非要给你下个好几个分卷压缩包然后一起解压的启动器发现游戏本体的```config.ini```

```Shell
downloader.out clean
```

下载完成后需要手动生成能让傻了吧唧没有```config.ini```就不知道游戏在哪安装时非要给你下个好几个分卷压缩包然后一起解压的启动器发现游戏本体的```config.ini```。不知道怎么写的可以照着我这个配置改一下，我也不知道哪些是必须的，总之它能用

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

Q: 为啥又全更新成C++了？  
A: 因为我会用C++处理json和md5的hash计算了

Q: 我需要这个或者那个功能  
A: 又不是不能用，摆了.jpg。真想要就提issue，或者看看[Scighost/Starward](https://github.com/Scighost/Starward)  

## Notes

本项目使用了以下第三方库  

* [nlohmann/json](https://github.com/nlohmann/json)解析json
* [OpenSSL](https://www.openssl.org/)计算MD5 hash

虽然我管不着，但还是要做一个免责声明，不管什么人拿这代码干什么跟我没关系。电脑自动下载原神不要找我，找我没用
