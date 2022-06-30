我们在使用gtest工具进行测试时, 一般结果会直接输出到终端, 对阅读很不友好, 可以利用gtest测试的结果, 做成一个可以展示的html页面, 让gtest测试结果看上起一目了然。

## 1.将gtest执行结果生成为xml文件

`--gtest_output`选项说明：

>  --gtest_output=(json|xml)[:DIRECTORY_PATH/|:FILE_PATH]
>      Generate a JSON or XML report in the given directory or with the given
>      file name. FILE_PATH defaults to test_detail.xml.

只需要在运行可执行程序时，指定输出选项即可，例如：
```shell
./test --gtest_output=xml:./testResult.xml
```

## 2.将gtest生成的xml结果文件转化为html文件

当前目录下的两个文件gtest2html2.xsl和gtest2html.xslt是从git仓库`https://github.com/NeilZhy/gtest2html`下载的，它们使用了不同的配置, 生成的页面效果不一样, 学习了解xslt的语法, 可以按照自己的风格生成多种多样的展示结果。

```shell
├── gtest2html2.xsl
├── gtest2html.xslt
```

借助`xsltproc`工具可以将生成的`testResult.xml`转化为`html`文件,执行命令：

```shell
xsltproc gtest2html2.xsl testResult.xml > testResult.html
```





