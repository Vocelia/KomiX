# **KomiX — A remote control software**
<p align="center">
  <a href="#about">About</a> •
  <a href="#usage">Usage</a> •
  <a href="#features">Features</a> •
  <a href="#development">Development</a> •
  <a href="https://github.com/Vocelia/KomiX/releases/latest">Downloads</a>
</p>
<h4 align="center"><b>Transform your phone into a powerful remote control, effortlessly managing and executing commands on your computer.</b></h4>


https://github.com/Vocelia/KomiX/assets/126959304/1e804d63-c1fa-4383-b5f0-aaaea9dc2786


[![OS](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)](https://github.com/torvalds/linux) [![OS](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/en-us/software-download) [![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&labelColor=01427d&logoColor=6295cb&logo=cplusplus)](https://en.wikipedia.org/wiki/C%2B%2B) [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)

## **About**
Can you imagine a world devoid of remote controls? It's a challenging thought, isn't it? The world has witnessed remarkable advancements, all driven by the concept of communication—the transmission of information and intention among one another. This is how televisions made their grand debut in the entertainment industry, where remote-controls transmitted the intention of users to their televisions. However, traditional TVs are now long-gone, discarded by society nowadays. This is where KomiX comes into play: A simple TV;remote-control relationship software, where commands on the host computer are executed through your phone.

## **Features**
- Server configuration
- Multi-threading support
- Logging of commands output
- Execution of commands remotely
- Layout and commands configuration
-  **To-Do List:**
	- Keep-alive connection 
	- A clean and intuitive interface
	- Logging of dynamic commands output

## **Development**
- Language: C++11
- Compiler: GCC 13.1.1 20230429
- IDE: Visual Studio Codium 1.78.2.23132
- Prerequisites:
	- libssl.so.3 (3.1.1-1)
	- libwebsockets.so.19 (4.3.2-3)

## **Usage**
- **Configuration:**
First of all, navigate the `data` directory to access and modify `config.ini` and `server.ini`. These files contain settings and configurations for command execution and server variables respectively.
	- **config.ini:**
		The declaration of a section is as follows:
		```
		[This is my first section]
		```
		Following that is a key-value pair structure, where the key represents the name of a button, and the corresponding value denotes the associated command:
		```
		Open a browser = firefox
		```
		Here is an example of a typical configuration file:
		```
		[Video]
		Pause = wtype -P space
		Skip Forward = wtype -P right
		Skip Backwards = wtype -P left
		
		[Programs]
		obs = obs
		gedit = gedit
		chromium = chromium
		```
	- **server.ini:**
		On the other hand, `server.ini` serves as the configuration to the server, a.k.a. your computer, where variables are static and set-in-stone. To configure it, you simply have to modify their values only.
		```
		port = 8080
		timeout = 1000
		command_output = 2 ;options: 0: false, 1: true, 2: logs
		command_output_logs_path = /home/user/.local/bin/KomiX/logs
		```
		Make sure to provide a valid logs path, regardless of whether you didn't set your output method to logs, or else the program will fail to run.
		
- **Execution:**
	Run the program by double-clicking it on Windows or executing it in a terminal on Posix-like systems. If everything went well, then your console output should resemble the following:
	```
	Reading from server.ini...
	Initialised server constants successfully!
	Checking the validity of LOGS_PATH directory...
	LOGS_PATH is a valid directory!
	Reading from config.ini...
	Created a User Interface page successfully!
	Initialising context creation information...
	[2023/07/03 17:07:49:5602] N: lws_create_context: LWS: 4.3.2-unknown, NET CLI SRV H1 H2 WS ConMon IPV6-on
	[2023/07/03 17:07:49:5603] N: __lws_lc_tag:  ++ [wsi|0|pipe] (1)
	[2023/07/03 17:07:49:5604] N: __lws_lc_tag:  ++ [vh|0|netlink] (1)
	[2023/07/03 17:07:49:5604] N: __lws_lc_tag:  ++ [vh|1|default||8080] (2)
	[2023/07/03 17:07:49:5605] N: [vh|1|default||8080]: lws_socket_bind: source ads 0.0.0.0
	[2023/07/03 17:07:49:5605] N: __lws_lc_tag:  ++ [wsi|1|listen|default||8080] (2)
	[2023/07/03 17:07:49:5605] N: [vh|1|default||8080]: lws_socket_bind: source ads ::
	[2023/07/03 17:07:49:5605] N: __lws_lc_tag:  ++ [wsi|2|listen|default||8080] (3)
	Initialised context creation information successfully!
	-------------------------------------------------
	Your address is: http://192.168.0.100:8080
	-------------------------------------------------
	```
	Head to the given address on your phone's browser. Make sure it is of `http` protocol, and not `https`.  You will see an interface specifically generated based on your configurations. Clicking on one of these buttons will execute the corresponding command associated with it. Congratulations, you just executed a command on your desktop through your phone!
	**Note: In the case of no WiFi connection, the software will fail to display a valid address, as this software depends solely on local network—any interface supporting inet.**

