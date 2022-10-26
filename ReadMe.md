# 何为简洁式，何为囊括式

```xml
<!--简洁式-->
<Text name="header1" value="USER" size="4"/>
<!--囊括式-->
<Text name="header1"  size="4">User<Text>
```

# 作用域

>  在主容器与次容器中，所谓的数据域即在某一个容器内不可以同名的标签存在

一下是案例：

```xml
<!--错误在于声明了两个相同的名字Block-->
<DataModel name="USERNAME"> 
    <Block name="Header">
        <Text name="header1" value="USER" size="4"/>
        <Text name="header2" value=" " size="1"/>
        <Text name="header3" value="jack" size="4"/>
    <Block/>
    <Block name="Header">
        <Text name="header1" value="USER" size="4"/>
        <Text name="header2" value=" " size="1"/>
        <Text name="header3" value="jack" size="4"/>
    <Block/>
</DataModel>
```

```xml
<!--这里是正确的-->
<!--虽然两个Block均含有header1,但是在不同的次容器下，不冲突，属于非同一作用域-->
<DataModel name="USERNAME"> 
    <Block name="Header">
        <Text name="header1" value="USER" size="4"/>
        <Text name="header2" value=" " size="1"/>
        <Text name="header3" value="jack" size="4"/>
    <Block/>
    <Block name="Header2">
        <Text name="header1" value="USER" size="4"/>
        <Text name="header2" value=" " size="1"/>
        <Text name="header3" value="jack" size="4"/>
    <Block/>
</DataModel>
```

```xml
<!--这里是错误的-->
<!--虽然是两个不同的标签，但是在同一次容器下，不可以同名-->
<DataModel name="USERNAME"> 
    <Block name="Header">
        <Text name="header1" value="USER" size="4"/>
        <INT name="header1" value=" " size="1"/>
        <Text name="header3" value="jack" size="4"/>
    <Block/>
</DataModel>
```

# 一切容器皆有名

> 不管任何的容器标签他的功能如何的简便，他都必须包含名字。并且在同一作用域下。必须是不同的。
> 
> 基本数据类也必须不同名，但是可以无名。

# 容器类

容器类主要是指数据容器，它用于包囊数据，是数据的载体

# DataModel 主数据容器

```
<DataModel> </DataModel>
```

> DataModel是主要的数据容器，Fuzzer将会每次以它为单位发送一次数据或者接收一次数据。

> DataModel是可以存在多个的。
> 
> DataModel下的数据不能直接包含<INT> <Text>这种数据。需要用Block包裹才可以

属性：

    name  名字

```xml
<DataModel name="USERNAME"> 
    <Block name="Header">
        <Text name="header1" value="USER" size="4"/>
        <Text name="header2" value=" " size="1"/>
        <Text name="header3" value="jack" size="4"/>
    <Block/>
</DataModel>
```

最终的效果为 USER jack(包含了一个空格)

## 错误的实例

```xml
<!--没有用Block包裹数据-->
<DataModel name="USERNAME"> 
        <Text name="header1" value="USER" size="4"/>
        <Text name="header2" value=" " size="1"/>
        <Text name="header3" value="jack" size="4"/>
</DataModel>
```

```xml
<!--第一在同一个作用域下不可以声明两个name为header的次级容器-->
<!--第二不支持容器均不支持简洁式写法-->
<DataModel name="USERNAME"> 
        <Block name="header"/><!--不支持改写法-->
            <Text name="header1" value="USER" size="4"/>
            <Text name="header2" value=" " size="1"/>
            <Text name="header3" value="jack" size="4"/>
        <Block name="header"/><!--不支持改写法-->
</DataModel>
```

属性：

<div>
    name 容器的名字(不可为空)
</div>

### 可用的子标签

```xml
<Block>
```

## Block 次级数据容器

## Block作用

> Block作为次级容器，他只要是为了包裹数据类型，如下便是可用标签。

> Block不支持简洁式写法

属性:

    name 名字

```xml
<Text>
<INT>
<UINT>
<FLOAT>
<DOUBLE>
<BIT>
<BIT16>
<BIT32>
<BIT64>
<LONGLONG>
```

# 状态类

状态类主要是定义状态的，以及每个状态因该做什么（Action）

## Action 动作器

### Action作用

> 主要用于处理数据的输出与输入，状态转移
> 
> 只支持简洁式写法

属性:

    name 名字

    mode 模式 input output changestate对应输入、输出、状态转移

    changeto 当模式为状态转移时使用，指向状态机

    datamodel 当模式为输入输出时，指向一个DataModel

```xml
<Action name="USER" datamodel="USERNAME" mode="output"/>
```

## State 状态主容器

> 只支持囊括式写法(注册时可以简洁式写)
> 
> 包囊所有Action

```xml
<State name="initState">
    <Action.../>
</State>
```

可用标签

```xml
<Action>
```

属性：

    name 名字

# 数据类

数据类的标签主要用于定义各式各样的数据类型

## Text 文本

```
<Text>...</Text>
```

Text可以用于添加字符串,Hex,等等。

属性：

<div>
name 名字（不可为空）
</div>
<div>
size 文本的大小
</div>
<div>
type 文本的类型，hex,ascii,utf8. 默认 ascii
</div>
<div>
value 用户定义的数据
</div>
<div>
mutable 是否可变异
</div>

可用的子标签：

```
无
```

注意点：

> 1.size的大小必须指定，并且默认是1字节对齐。如果为utf8,请自行乘2以此类

实例方式：

```xml
<Text name="str" mutable="true" type="utf8" size="100" value="2222222" />
```

```xml
<Text name="str" mutable="true" type="utf8" size="100">11111111</Text>
```

```xml
<!--默认使用value属性下的数值-->
<Text name="str" mutable="true" type="utf8" size="100" value="2222222" >
11111111
</Text>
<!--正确的数值为2222222-->
```

# INT文本

```
<INT>...<INT>
```

INT可以用于添加int整型数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的INT类型的长度

实例方式：

```xml
<INT name="str" mutable="true" value="100" />
```

```xml
<INT name="str" mutable="true" >11111111</INT >
```

```xml
<!--默认使用value属性下的数值-->
<INT name="str" mutable="true" value="100" >
200
</INT >
<!--正确的数值为100-->
```

# UINT文本

```
<UINT>...<UINT>
```

UINT可以用于添加无符号int整型数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的无符号INT类型的长度

实例方式：

```xml
<UINT name="str" mutable="true" value="100" />
```

```xml
<UINT name="str" mutable="true" >11111111</UINT >
```

```xml
<!--默认使用value属性下的数值-->
<UINT name="str" mutable="true" value="100" >
200
</UINT >
<!--正确的数值为100-->
```

# FLOAT文本

```
<FLOAT>...<FLOAT>
```

FLOAT可以用于添加float单浮点数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的float单浮点的长度

实例方式：

```xml
<FLOAT name="str" mutable="true" value="100.0" />
```

```xml
<FLOAT name="str" mutable="true" >100.0</FLOAT>
```

```xml
<!--默认使用value属性下的数值-->
<FLOAT name="str" mutable="true" value="100.0" >
200.0
</FLOAT>
<!--正确的数值为100.0-->
```

# DOUBLE文本

```
<DOUBLE>...<DOUBLE>
```

DOUBLE可以用于添加双精度浮点数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的双精度浮点的长度

实例方式：

```xml
<DOUBLE name="str" mutable="true" value="100.00" />
```

```xml
<DOUBLE name="str" mutable="true" >100.00</DOUBLE>
```

```xml
<!--默认使用value属性下的数值-->
<DOUBLE name="str" mutable="true" value="100.00" >
200.00
</DOUBLE>
<!--正确的数值为100.00-->
```

# LONGLONG文本

```
<LONGLONG>...<LONGLONG>
```

LONGLONG可以用于添加64位数据整型数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的long long类型的长度

实例方式：

```xml
<LONGLONG name="str" mutable="true" value="1000" />
```

```xml
<LONGLONG name="str" mutable="true" >1000</LONGLONG >
```

```xml
<!--默认使用value属性下的数值-->
<LONGLONG name="str" mutable="true" value="1000" >
2000
</INT >
<!--正确的数值为1000-->
```

# BIT文本

```
<BIT>...<BIT>
```

BIT可以用于添加8个bit位数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的8个bit位的长度

实例方式：

```xml
<BIT name="str" mutable="true" value="10101010" />
```

```xml
<BIT name="str" mutable="true" >10101010</BIT>
```

```xml
<!--默认使用value属性下的数值-->
<BIT name="str" mutable="true" value="10101010" >
01010101
</BIT>
<!--正确的数值为10101010-->
```

# BIT16文本

```
<BIT16>...<BIT16>
```

BIT可以用于添加16个bit位数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的16个bit位的长度

实例方式：

```xml
<BIT name="str" mutable="true" value="1010101010101010" />
```

```xml
<BIT name="str" mutable="true" >1010101010101010</BIT>
```

```xml
<!--默认使用value属性下的数值-->
<BIT name="str" mutable="true" value="1010101010101010" >
0101010101010101
</BIT>
<!--正确的数值为1010101010101010-->
```

# BIT32文本

```
<BIT32>...<BIT32>
```

BIT可以用于添加32个bit位数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的32个bit位的长度

实例方式：

```xml
<BIT name="str" mutable="true" value="10101010101010101010101010101010" />
```

```xml
<BIT name="str" mutable="true" >10101010101010101010101010101010</BIT>
```

```xml
<!--默认使用value属性下的数值-->
<BIT name="str" mutable="true" value="10101010101010101010101010101010" >
0101010101
</BIT>
<!--正确的数值为10101010101010101010101010101010-->
```

# BIT64文本

```
<BIT64>...<BIT64>
```

BIT可以用于添加64个bit位数据

属性：

> name 名字（不可为空）

> value 用户定义的数据

> mutable 是否可变异

可用的子标签：

```
无
```

注意点：

> 1.size为默认的64个bit位的长度

实例方式：

```xml
<BIT name="str" mutable="true" value="1010101010101010101010101010101010101010101010101010101010101010" />
```

```xml
<BIT name="str" mutable="true" >1010101010101010101010101010101010101010101010101010101010101010</BIT>
```

```xml
<!--默认使用value属性下的数值-->
<BIT name="str" mutable="true" value="1010101010101010101010101010101010101010101010101010101010101010" >
10101010
</BIT>
<!--正确的数值为10101010101010101010101010101010-->
```

# 配置类

配置类的标签比较简单，它不需要拥有独特，独一无二的名字，他主要告诉引擎，我要额外使用那些东西。

## Debugger 调试器

> 用于添加调试器
> 
> 只能使用简洁式写法

```xml
<Debugger name="调试器" value="调试器路径" />
```

属性：

> name 调试器的名字
> 
> value 路径

# 注册机Register

注册所有的信息。

> 用于注册所有的信息，如默认状态机，调试器等等
> 
> 只能使用囊括式写法

```xml
<Register name="FTPTest">
    <Debugger name="..." value="..."/>
    <State name="initState"/>
</Register>
```

属性：

>     name 本次测试的名字

可用标签:

```xml
<State>
<Debugger>
```

注意点：

> 一次fuzzer必须拥有一个State才能运行