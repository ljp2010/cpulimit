cpulimit
========

----------------------

从之前版本修改而来，之前的程序用来限制某一个进程的cpu使用率，这一版本是限制系统内所有程序的cpu使用率的，除了自己和system进程。
程序的用途主要是在虚拟机里，让虚拟机主动放弃使用部分的cpu，从而弥补vmware workstation没有调整虚拟机cpu占用率的不足。

目前在windows xp / windows 10（1909）上测试一切正常，虚拟机内工作正常，不会影响正常的图形功能（显卡并没有被限制），会轻微影响鼠标键盘的反馈速度（有可能会丢失一些操作，但影响不大）。
测试是在ryzen 5 3500u的cpu上，使用windows 10（1909）当主机，windows xp当客户机，cpulimit限制设置在50%（-l 50）时测得的。目前最低可以降低到20%而不显著影响使用，再低鼠标键盘事件的丢失将不可容忍。

[![Build Status][0]][1]

CPU usage limiter for windows (cygwin)

This program allow limit CPU usage by any other program. This can be usefull 
for example for those users, who use distributed calculations clients and 
have problems with CPU overheating.

It's a clone of [***cpulimit for linux***][2].


Build and Installation
-----------------------

This package is compatible with the GNU autotools.

* Windows

    $ `git submodule init`

    $ `git submodule update`

    It's configure to build with "Microsoft Visual C++ 2008":

      - executing `cpulimit.vcproj` file.


* Cygwin

    * Requirements

        * `autotools`
        * `make`

    * Briefly the following commands should build, test and install this package.

        $ `autoreconf -i && ./configure && make && make install`

      See the [`INSTALL`](INSTALL.md) file for more detailed instructions.


Help
----

$ `cpulimit --help`


TODO
----

* Tests suite
* Man
* bash conpletion


Acknowledgments
---------------

The original cpulimit was written by [***Killer***][3]

[0]: https://travis-ci.org/alquerci/cpulimit.png?branch=master
[1]: https://travis-ci.org/alquerci/cpumilit
[2]: https://github.com/opsengine/cpulimit
[3]: http://www.killprog.com
